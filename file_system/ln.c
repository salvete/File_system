#include"headers.h"

int ln(char *from, char* to)
{
    int res = sys_link(from, to);

    if (res == -1)
    {
        printf("Error:link\n");
        return -1;
    }
    return 0;
}
