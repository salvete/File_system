#include"headers.h"
#include<stdio.h>


int main()
{
    init_cache();
    mkfs();
    icache_init();
    init_ofile();
    init_global();

    shell();

    return 0;
}
