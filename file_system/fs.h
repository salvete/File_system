#ifndef FS_H_INCLUDED
#define FS_H_INCLUDED

#include"types.h"


#define ROOTINO 0
#define BSIZE 512


struct superblock{
    //共有多少块
    uint size;
    //数据块的数量
    uint nblocks;
    //inode数量
    uint ninodes;
    //inode起始位置
    uint inodestart;
    //bitmap起始位置
    uint bmapstart;
};

#define NDIRECT 12
#define NINDIRECT (BSIZE/sizeof(uint))
#define MAXFILE (NDIRECT + NINDIRECT)

struct dinode{
    short type;
    short nlink;
    uint size;//bytes
    uint addrs[NDIRECT+1];
};

//每块包含多少个dinode
#define IPB (BSIZE / sizeof(struct dinode))
//第i个dinode在何处（块偏移量）
#define IBLOCK(i,sb) ((i)/(IPB) + sb.inodestart)
//一个块包含的位数
#define BPB (BSIZE*8)
//第b个块所在的bmap距离bmapstart的块数
#define BBLOCK(b,sb) (b/BPB + sb.bmapstart)


//dinode所占的块数
#define NDSKINODE 5
//bitmap所占用块数
#define NDBITMAP 10
//目录名字长度
#define DIRSIZE 20

struct dirent{
    ushort inum;
    char name[DIRSIZE];
};


#endif // FS_H_INCLUDED
