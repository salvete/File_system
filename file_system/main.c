#include"buf.h"
#include"fs.h"
#include<stdio.h>
#include"fs.h"

int main()
{
    init_cache();
    mkfs();
    icache_init();
    return 0;
}
