#include"headers.h"

int login(char *name)
{
    set_cur_name(name);
    return 0;
}

int logout()
{
    set_cur_name("Default");
    return 0;
}

int whoami()
{
    printf("%s\n",cur_name);
    return 0;
}
