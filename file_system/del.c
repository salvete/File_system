#include"headers.h"

int del(char *path)
{
    int res = sys_unlink(path);
    if (res == -1)
    {
        printf("Error:del %s\n",path);
        return -1;
    }
    return 0;
}
