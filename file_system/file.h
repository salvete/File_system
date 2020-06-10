#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#define NINODE 50

struct file
{
    enum{FD_NONE,FD_PIPE,FD_INODE } type;
    int ref;
    char readable;
    char writeable;
    struct inode *ip;
    uint off;
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
