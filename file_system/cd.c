#include"headers.h"

int cd(char *path)
{
    struct inode *ip;
    ip = namei(path);
    if (ip == 0)
       {
            printf("Error:path is not right\n");
            return -1;
       }
    else
       {
            cur_inode = ip;
            return 0;
       }
}
