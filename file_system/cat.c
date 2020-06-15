#include"headers.h"

int cat(char *path)
{
    char info[100];

    int fd = sys_open(path,OP_R);

    if (fd == -1)
    {
        printf("Error:open %s\n",path);
        return -1;
    }

    struct file *f = get_file_by_fd(fd);

    if (f == 0)
    {
        printf("Error:file is null\n");
        return -1;
    }

    while (sys_read(f,info,99)>0)
    {
        info[99] = '\0';
        printf("%s",info);
    }
    printf("\n");
    sys_close(f);
    return 0;
}
