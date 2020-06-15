#include"headers.h"

int echo(char *info, char *path)
{
    int fd;
    int res;

    fd = sys_open(path,OP_C|OP_RW);

    if (fd == -1)
    {
        printf("Error:open %s\n",path);
        return -1;
    }
    res = sys_write(get_file_by_fd(fd),info,strlen(info));

    if (res == -1)
    {
        printf("Error:write %s\n",path);
        return -1;
    }

    close(get_file_by_fd(fd));
    return 0;
}
