#include"headers.h"
#include<stdio.h>

int ls(struct inode *cur_inode)
{
    printf("Type          Name\n");
    if (cur_inode->type != T_DIR)
        return -1;
    struct dirent de;
    struct inode *ip;
    uint off;
    for (off = 0; off<cur_inode->size; off += sizeof(de))
    {
        if (readi(cur_inode,(char*)&de,off,sizeof(de)) != sizeof(de))
            return -1;

        ip = dirlookup(cur_inode,de.name,0);

        if (ip->type == T_DIR)
            printf("Directory     ");
        else
            printf("File          ");

        printf("%s\n",de.name);
    }
    return 0;
}
