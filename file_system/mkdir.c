#include"headers.h"

int mkdir(char *path)
{
    int res = sys_mkdir(path);
    if (res == -1)

        printf("Error:mkdir failed.\n");
    return res;
}
