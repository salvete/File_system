#include"types.h"
#include"fs.h"
#include"buf.h"
#include"file.h"
#include<stdio.h>
#include<string.h>
#include"stat.h"
#include"defs.h"

#define min(a,b) ( (a) < (b)? (a):(b))

struct superblock sb;

static struct inode* iget(uint inum);

void mkfs()
{
    struct superblock tmp;
    tmp.size = 1+NDSKINODE+NDBITMAP+ NDBITMAP * BPB;
    tmp.nblocks = NDBITMAP * BPB;
    tmp.ninodes = NDSKINODE * IPB;
    tmp.inodestart = 2;
    tmp.bmapstart = 1+NDSKINODE;

    struct buf *bp;
    bp = bread(1);
    memset(bp->data,0,BSIZE);
    memmove(bp->data,&tmp,sizeof(tmp));
    memmove(&tmp,bp->data,sizeof(tmp));
    bwrite(bp);
    brelse(bp);

    //将dinode清空
    int i;
    for (i=2; i<2+NDSKINODE; i++)
    {
        bp = bread(i);
        memset(bp->data,0,BSIZE);
        bwrite(bp);
        brelse(bp);
    }


    //将bitmap从重新清空
    for (i = tmp.bmapstart; i<tmp.bmapstart+NDBITMAP; i++)
    {
        bp = bread(i);
        memset(bp->data,0,BSIZE);
        bwrite(bp);
        brelse(bp);
    }

    //将17块前面标记为占用
    bp = bread(tmp.bmapstart);
    int m;
    for (i =0; i<1+1+NDSKINODE+NDBITMAP; i++)
    {
        m = 1 <<(i%8);
        bp->data[i/8] |= m;
    }
    bwrite(bp);
    brelse(bp);

    //将inode的第一块设置为/目录
    bp = bread(IBLOCK(ROOTINO,tmp));
    struct dinode *dip;
    dip = (struct dinode*)bp->data + ROOTINO%IPB;
    dip->type = T_DIR;
    dip->nlink = 0;
    dip->size = 0;
    for (i=0; i<NDIRECT + 1; i++)
        dip->addrs[i] = 0;
    bwrite(bp);
    brelse(bp);
}


void readsb(struct superblock *sb)
{

    struct buf *bp;
    bp = bread(1);
    memmove(sb,bp->data,sizeof(*sb));
    brelse(bp);
}

static void b_zero(uint bno)
{
    struct buf *bp;
    bp = bread(bno);
    memset(bp->data,0,BSIZE);
    bwrite(bp);
    brelse(bp);
}



static uint balloc()
{
    int b,bi,m;
    struct buf *bp;
    bp = 0;
    for (b = 0; b < sb.size; b += BPB)
    {
        bp = bread(BBLOCK(b,sb));
        for (bi = 0; bi < BPB && b + bi < sb.size; bi++)
        {
            m = 1 << (bi%8);
            if ((bp->data[bi/8] & m) == 0)
            {
                bp->data[bi/8] |= m;
                bwrite(bp);
                brelse(bp);
                b_zero(bi+b);
                return b + bi;
            }

         brelse(bp);
        }
    }

    assert(0);
}


static void bfree(uint b)
{
    struct buf *bp;
    int bi,m;

    readsb(&sb);
    bp = bread(BBLOCK(b,sb));
    bi = b % BPB;
    m = 1<<(bi%8);
    if ((bp->data[bi/8] & m) == 0)
        printf("It is already free!\n");
    bp->data[bi/8] &= ~m;
    bwrite(bp);
    brelse(bp);
}

///////////////////////////////////
//Inode.
struct{
    struct inode inode[NINODE];
}icache;

void icache_init()
{
    readsb(&sb);
    printf("sb: size %d nblocks %d ninodes %d inodestart %d bmap start %d\n",\
           sb.size,sb.nblocks,sb.ninodes,sb.inodestart,sb.bmapstart\
           );
}


struct inode* ialloc(short type)
{
    int inum;
    struct buf *bp;
    struct dinode *dip;
    struct inode *x;
    for (inum = 1; inum < sb.ninodes; inum++)
    {

        bp = bread(IBLOCK(inum,sb));
        dip = (struct dinode*)bp->data + inum%IPB;


        if (dip->type == 0)
        {
            memset(dip,0,sizeof(*dip));
            dip->type = type;
            bwrite(bp);
            brelse(bp);
            x = iget(inum);
            x->type = type;
            return x;
        }
    }

    assert(0);
};

void iupdate(struct inode *ip)
{
    struct buf *bp;
    struct dinode *dip;

    bp = bread(IBLOCK(ip->inum,sb));
    dip = (struct dinode*)bp->data + ip->inum % IPB;
    dip->type = ip->type;
    dip->nlink = ip->nlink;
    dip->size = ip->size;
    memmove(dip->addrs, ip->addrs, sizeof(ip->addrs));
    bwrite(bp);
    brelse(bp);
}

static struct inode* iget(uint inum)
{

    struct inode *ip,*empty;

    empty = 0;
    for (ip=&icache.inode[0]; ip < &icache.inode[NINODE]; ip++)
    {
        if (ip->ref > 0 && ip->inum == inum)
        {
            ip->ref ++;
            return ip;
        }
        if (empty == 0 && ip->ref == 0)
                empty = ip;//后面不要加break,因为可以符合条件的在后面
    }

    if (empty == 0)
    {
        printf("There is no free inode!\n");
        assert(0);
    }

    struct buf *bp;
    struct dinode *dip;
    ip = empty;
    ip->inum = inum;
    ip->ref = 1;

    bp = bread(IBLOCK(inum,sb));
    dip = (struct dinode*)bp->data + inum % IPB;
    ip->type = dip->type;
    ip->nlink = dip->nlink;
    ip->size = dip->size;
    memmove(ip->addrs,dip->addrs,sizeof(dip->addrs));
    brelse(bp);

    return ip;
}

struct inode* idup(struct inode *ip)
{
    ip->ref ++;
    return ip;
}

static void itrunc(struct inode *);

void iput(struct inode *ip)
{
    if (ip->nlink == 0)
    {

        int r = ip->ref;
        if (r == 1)
        {
            itrunc(ip);
            ip->type = 0;
            iupdate(ip);
        }
    }

    ip->ref --;
}

//Inode content
static uint bmap(struct inode *ip, uint bn)
{
    uint addr,*a;
    struct buf *bp;

    if (bn < NDIRECT)
    {
        if ((addr=ip->addrs[bn]) == 0)
            ip->addrs[bn] = addr = balloc();
        return addr;
    }

    bn -= NDIRECT;

    if (bn < NINDIRECT)
    {
        if ((addr = ip->addrs[NDIRECT]) == 0)
            ip->addrs[NDIRECT] = addr = balloc();
        bp = bread(addr);
        a = (uint*)bp->data;

        if ((addr = a[bn]) == 0)
        {
            a[bn] = addr = balloc();
            bwrite(bp);
        }
        brelse(bp);
        return addr;

    }

    printf("Error in bmap!\n");
    assert(0);
}

static void itrunc(struct inode *ip)
{
    int i,j;
    struct buf *bp;
    uint *a;

    for (i = 0; i<NDIRECT; i++)
    {
        if (ip->addrs[i])
        {
            bread(ip->addrs[i]);
            ip->addrs[i] = 0;
        }
    }

    if (ip->addrs[NDIRECT])
    {
        bp = bread(ip->addrs[NDIRECT]);
        a = (uint*)bp->data;
        for (j=0; j<NINDIRECT; j++)
        {
            if (a[j])
                    bfree(a[j]);
        }
        brelse(bp);
        bfree(ip->addrs[NDIRECT]);
        ip->addrs[NDIRECT] = 0;
    }
    ip->size = 0;
    iupdate(ip);
}


//赋值ip的信息至st中
void stati(struct inode *ip, struct stat *st)
{

    st->ino = ip->inum;
    st->type = ip->type;
    st->nlink = ip->nlink;
    st->size = ip->size;
}

int readi(struct inode *ip, char *dst, uint off, uint n)
{
    uint tot,m;
    struct buf *bp;

    if (off > ip->size || off + n < off)
        return -1;
    if (off + n > ip->size)
        n = ip->size - off;
    for (tot = 0; tot<n; tot +=m, off+=m,dst+=m)
    {
        bp = bread(bmap(ip,off/BSIZE));
        m = min(n-tot, BSIZE-off%BSIZE);
        memmove(dst,bp->data + off%BSIZE,m);
        brelse(bp);
    }
    return n;
}

int writei(struct inode *ip, char *src, uint off, uint n)
{
    uint tot,m;
    struct buf *bp;
    if (off > ip->size || off + n < off)
        return -1;

    if (off + n > MAXFILE * BSIZE)
        return -1;

    for (tot=0; tot<n; tot+=m, off+=m, src+=m)
    {
        bp = bread(bmap(ip,off/BSIZE));
        m = min(n-tot, BSIZE - off%BSIZE);
        memmove(bp->data+off%BSIZE,src,m);
        bwrite(bp);
        brelse(bp);
    }



    if (n > 0 && off > ip->size)
    {
        ip->size = off;
        iupdate(ip);
    }
    return n;
}

/////////////////////////////////////
//Directories

int namecmp(const char *s,const char *t)
{
    return strncmp(s,t,DIRSIZE);
}

struct inode *dirlookup(struct inode *dp, char *name, uint *poff)
{
    uint off,inum;
    struct dirent de;

    if (dp->type != T_DIR)
    {
        printf("Error in dirlookup!\n");
        assert(0);
    }

    for (off = 0; off < dp->size; off += sizeof(de))
    {
        if (readi(dp,(char*)&de,off,sizeof(de)) != sizeof(de))
        {
            printf("Error in dirlook up:the len is not right.");
            assert(0);
        }

        if (de.inum == 0)
            continue;
        if (namecmp(name,de.name) == 0)
        {
            if (poff)
                *poff = off;
            inum = de.inum;
            return iget(inum);
        }

    }
    return 0;
}

int dirlink(struct inode *dp, char *name, uint inum)
{
    int off;
    struct dirent de;
    struct inode *ip;

    if ((ip = dirlookup(dp,name,0)) != 0)
    {
        iput(ip);
        return -1;
    }

    for (off = 0; off<dp->size; off+=sizeof(de))
    {
        if (readi(dp,(char*)&de, off, sizeof(de)) != sizeof(de))
        {
            printf("Error in dirlink:len is not right");
            assert(0);
        }

        if (de.inum == 0)
            break;
    }

    strncpy(de.name,name,DIRSIZE);
    de.inum = inum;
    if (writei(dp,(char*)&de,off,sizeof(de)) != sizeof(de))
    {
        printf("Error: write.");
        assert(0);
    }

    return 0;
}

///////////////////////////
//paths

static char *skipelem(char *path, char*name)
{
    char *s;
    int len;

    while (*path == '/')
            path ++;
    if (*path == 0)
        return 0;
    s = path;
    while (*path !='/' && *path != 0)
        path++;
    len = path -s;
    if (len >= DIRSIZE)
        memmove(name,s,DIRSIZE);
    else
    {
        memmove(name,s,len);
        name[len]=0;
    }
    while (*path == '/')
        path++;
    return path;
}

struct inode *get_current()
{
    return iget(ROOTINO);
}

static struct inode *namex(char *path, int nameiparent, char *name)
{
    struct inode  *ip,*next;

    if (*path == '/')
        ip = iget(ROOTINO);
    else
        ip = get_current();

    while ((path = skipelem(path,name)) != 0)
    {

        if (ip->type != T_DIR)
            return 0;
        if (nameiparent && *path == '\0')
            return ip;
        if ((next = dirlookup(ip,name,0)) == 0)
            return 0;
        ip = next;
    }

    if (nameiparent)
    {
        iput(ip);
        return 0;
    }

    return ip;
}

struct inode* namei(char *path)
{
    char name[DIRSIZE];
    return namex(path,0,name);
};

struct inode* nameiparent(char *path, char *name)
{
    return namex(path,1,name);
};

