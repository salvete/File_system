#include"headers.h"
#include<string.h>
#include<stdio.h>
//全局描述符表
static struct file* ofile[NFD];

void init_ofile()
{
    memset(ofile,0,sizeof(ofile));
}


static int fdalloc(struct file *f)
{
    int fd;
    //0,1,2留给stdin,stdout,stderr
    for (fd = 3; fd<NFD; fd++)
    {
        if (ofile[fd] == 0)
        {
            ofile[fd] = f;
            return fd;
        }
    }
    return -1;
}

int sys_dup(struct file *f)
{
    int fd;
    assert((fd=fdalloc(f)) >= 0);
    file_dup(f);
    return fd;
}

int sys_read(struct file *f, char *dst, int n)
{
    return file_read(f,dst,n);
}

int sys_write(struct file *f, char *src, int n)
{
    return file_write(f,src,n);
}

int sys_close(struct file *f)
{
    int i;
    for (i=3; i<NFD; i++)
    {
        if (ofile[i] == f)
        {
            ofile[i] = 0;
            file_close(f);
            return 0;
        }
    }
    return -1;
}

int sys_fstat(struct file *f, struct stat *st)
{
    return file_stat(f,st);
}

static void undo(struct inode *ip)
{
    ip->nlink --;
    iupdate(ip);
}

int sys_link(char *new_name, char *old)
{
    char name[DIRSIZE];
    struct inode *dp,*ip;
    assert((ip=namei(old)) != 0);
    //链接的目标必须为文件
    assert(ip->type != T_DIR);
    ip->nlink ++;

    iupdate(ip);

    if ((dp = nameiparent(new_name,name)) == 0)
    {
        undo(ip);
        return -1;
    }

    if (dirlink(dp,name,ip->inum) < 0)
    {
        undo(ip);
        return -1;
    }

    iput(ip);

    return 0;
}

static int is_dir_empty(struct inode *dp)
{
    assert(dp->type == T_DIR);

    int off;
    struct dirent de;
    for (off = 2*sizeof(de); off <dp->size; off += sizeof(de))
    {
        assert(readi(dp,(char*)&de,off,sizeof(de)) == sizeof(de));

        if (de.inum != 0)
            return 0;
    }

    return 1;
}

int sys_unlink(char *path)
{
    struct inode *ip,*dp;
    struct dirent de;
    char name[DIRSIZE];
    uint off;

    if ((dp = nameiparent(path,name)) == 0)
        return -1;

    //不能unlink . 或者 ..
    if (namecmp(name,".") == 0 || namecmp(name,"..") == 0)
        return -1;

    if((ip = dirlookup(dp,name,&off)) == 0)
        return -1;

    assert(ip->nlink >= 1);

    if (ip->type == T_DIR && !is_dir_empty(ip))
        return -1;

    memset(&de,0,sizeof(de));

    assert(writei(dp,(char*)&de,off,sizeof(de)) == sizeof(de));

    if (ip->type == T_DIR)
    {
        //..指向它
        dp->nlink  --;
        iupdate(dp);
    }

    ip->nlink --;
    iupdate(ip);

    return 0;
}

static struct inode * create_dir_file(char *path, short type)
{
    uint off;
    struct inode *ip,*dp;
    char name[DIRSIZE];


    if ((dp = nameiparent(path,name)) == 0)
    {
        printf("NULL in create\n");
        return 0;
    }

    if ((ip = dirlookup(dp,name,&off)) != 0)
    {
        if (type == T_FILE && ip->type == T_FILE)
            return ip;
        printf("NULL in create\n");
        return 0;
    }

    assert((ip = ialloc(type)) != 0);

    ip->nlink = 1;
    iupdate(ip);

    if (type == T_DIR)
    {
        //..
        dp->nlink ++;
        iupdate(dp);

        assert(dirlink(ip,".",ip->inum) >= 0);
        assert(dirlink(ip,"..",dp->inum) >= 0);
    }

    assert(dirlink(dp,name,ip->inum) >= 0);
    return ip;
}

int sys_open(char *path, int o_mode)
{
    struct file *f;
    struct inode *ip;
    int fd;

    if ( o_mode & OP_C)
    {
        ip = create_dir_file(path,T_FILE);
        if (ip == 0)
            return -1;
    }
    else
    {
        if ((ip = namei(path)) == 0)
            return -1;

        //目录只可以读
        if (ip->type == T_DIR && o_mode != OP_R)
            return -1;

    }

    if ((f = file_alloc()) == 0 || (fd = fdalloc(f)) < 0)
    {
        if (f)
            file_close(f);

    }

    f->type = FD_INODE;
    f->ip = ip;
    f->off = 0;
    f->off_r = 0;
    //有写就不能够读

    f->readable = !(o_mode & OP_W);
    //有写就可写
    f->writeable = (o_mode & OP_W) || (o_mode & OP_RW);
    return fd;
}

int sys_mkdir(char *path,short type)
{
    struct inode *ip;
    if ((ip = create_dir_file(path,type)) == 0)
        return -1;
    return 0;
}

struct file *get_file_by_fd(int fd)
{
    return ofile[fd];
}
