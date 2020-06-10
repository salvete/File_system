#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include"fs.h"

#define NINODE 50
#define NFILE 100
#define MAXOPBLOCKS 10


struct file
{
    enum{FD_NONE,FD_PIPE,FD_INODE } type;
    int ref;
    char readable;
    char writeable;
    struct inode *ip;
    uint off;
    uint off_r;
};

struct inode{
    uint inum;
    int ref;

    short type;
    short nlink;
    uint size;//bytes
    uint addrs[NDIRECT+1];
};

#endif // FILE_H_INCLUDED
