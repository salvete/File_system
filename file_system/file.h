#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include"fs.h"

#define NINODE 50
#define NFILE 100
#define MAXOPBLOCKS 10

#define NFD 100

#define OP_C 1
#define OP_R 2
#define OP_W 4
#define OP_RW 8


struct file
{
    enum{FD_NONE,FD_INODE } type;
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
