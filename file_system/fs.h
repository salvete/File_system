#ifndef FS_H_INCLUDED
#define FS_H_INCLUDED

#include"types.h"


#define ROOTINO 1
#define BSIZE 512

#define MAGIC 65535

struct superblock{
    uint magic;
    //���ж��ٿ�
    uint size;
    //���ݿ������
    uint nblocks;
    //inode����
    uint ninodes;
    //inode��ʼλ��
    uint inodestart;
    //bitmap��ʼλ��
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

//ÿ��������ٸ�dinode
#define IPB (BSIZE / sizeof(struct dinode))
//��i��dinode�ںδ�����ƫ������
#define IBLOCK(i,sb) ((i)/(IPB) + sb.inodestart)
//һ���������λ��
#define BPB (BSIZE*8)
//��b�������ڵ�bmap����bmapstart�Ŀ���
#define BBLOCK(b,sb) (b/BPB + sb.bmapstart)


//dinode��ռ�Ŀ���
#define NDSKINODE 5
//bitmap��ռ�ÿ���
#define NDBITMAP 10
//Ŀ¼���ֳ���
#define DIRSIZE 20

struct dirent{
    ushort inum;
    char name[DIRSIZE];
};

#endif // FS_H_INCLUDED
