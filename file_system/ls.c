#include"headers.h"
#include<stdio.h>

void ls(struct inode *cur_inode)
{
    if (cur_inode->type != T_DIR)
        return;
    struct dirent de;
    uint off;
    for (off = 0; off<cur_inode->size; off += sizeof(de))
    {
        if (readi(cur_inode,(char*)&de,off,sizeof(de)) != sizeof(de))
            return;
        printf("%s\n",de.name);
    }
}
