#include"buf.h"
#include"fs.h"
#include<stdio.h>
#include"fs.h"
#include"stat.h"
#include"file.h"
#include"defs.h"

int main()
{
    init_cache();
    mkfs();
    icache_init();

    struct file *f;
    f = new_file(T_FILE);


    char a[20000];
    char b[20000]={0};
    char c[20000];
    char d[20000]={0};
    int i;
    for ( i=0; i<19999; i++)
    {
        a[i] = 'a';
        c[i] = 'c';
    }
    a[19999] = '\0';
    c[19999] = '\0';

    assert(file_write(f,a,20000) != -1);

    assert(file_read(f,b,20000) != -1);

    assert(file_write(f,c,20000) != -1);

    assert(file_read(f,d,20000) != -1);

    printf("b:%s\n",b);
    printf("d:%s\n",d);

//    struct inode *ip;
//    ip = ialloc(T_FILE);
//    printf("inum:%d\n",ip->inum);
//
//    assert(writei(ip,a,0,10) != -1);
//    assert(readi(ip,b,0,10) != -1);
//    printf("size:%d\n",ip->size);
//    assert(writei(ip,c,10,10) != -1);
//    assert(readi(ip,d,10,10) != -1);
//
//
    printf("fielsize:%d,ipsize:%d\n",f->off,f->ip->size);
    return 0;
}
