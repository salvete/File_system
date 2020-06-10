#include"types.h"
#include"file.h"
#include"stat.h"
#include"fs.h"
#include"defs.h"

struct{
    struct file file[NFILE];
}ftable;

struct file* file_alloc()
{
    struct file *f;
    for (f = ftable.file; f<ftable.file + NFILE; f++)
    {
        if (f->ref == 0)
        {
            f->ref = 1;

            return f;
        }
    }
    return 0;
};

struct file* file_dup(struct file *f)
{
    assert(f->ref >= 1);
    f->ref ++;
    return f;
}

void file_close(struct file *f)
{

    struct file ff;
    assert(f->ref >= 1);
    if (--f->ref > 0)
        return;
    ff = *f;
    f->ref = 0;
    iput(ff.ip);
    f->type = FD_NONE;
}

int file_stat(struct file *f, struct stat *st)
{
    assert(f->type == FD_INODE);
    stati(f->ip,st);
    return 0;
}

int file_read(struct file *f, char *addr, int n)
{
    int r=-1;
    if (f->readable == 0)
        return -1;
    assert(f->type = FD_INODE);
    if ((r = readi(f->ip,addr,f->off_r,n)) > 0)
        f->off_r += r;
    return r;
}

int file_write(struct file *f, char *addr, int n)
{
    int r;
    if (f->writeable == 0)
        return -1;

    assert(f->type == FD_INODE);


    int max = ((MAXOPBLOCKS-1-1-2)/2) * 512;
    int i=0;
    while (i<n)
    {

        int n1= n-i;
        if (n1 > max)
            n1 = max;

        if ((r=writei(f->ip,addr+i,f->off,n1)) > 0)
            f->off += r;
        if (r < 0)
            break;
        assert(r == n1);
        i += r;
    }

    return i==n?n:-1;
}

struct file* new_file(short type)
{
    struct file *f = file_alloc();
    f->type = FD_INODE;
    f->ip = ialloc(type);
    f->readable = 1;
    f->writeable = 1;
    f->off = 0;
    f->off_r = 0;
    return f;
};
