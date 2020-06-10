#include"buf.h"
#include"fs.h"
#include<stdio.h>
#include"fs.h"
#include"stat.h"
#include"file.h"

int main()
{
    init_cache();
    mkfs();
    icache_init();
    struct inode *p,*q;
    p = ialloc(T_FILE);
    p->size = 14*512*8+100;

    printf("num:%d,type:%d,ref:%d\n",p->inum,p->type,p->ref);

    printf("%d\n",bmap(p,5));
    printf("%d\n",bmap(p,9));

    char a[100];
    char b[100];
    int i;
    for (i=0; i<99; i++)
        a[i]='1';
    a[99] = '\0';

    assert(writei(p,a,0,100) == 100);
    assert(readi(p,b,0,100) == 100);

    printf("b:%s\n",b);

    return 0;
}
