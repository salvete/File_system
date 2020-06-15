#include"headers.h"

int mkdir(char *path,short type)
{
    int res = sys_mkdir(path,type);
    if (res == -1)

        printf("Error:mkdir failed.\n");
    return res;
}
