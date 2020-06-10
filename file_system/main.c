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
    //mkfs();
    icache_init();

//    struct inode *p,*q,*m,*t;
//    p = iget(ROOTINO);
//    char b[10]={"axxxxxxx\n"};
//    char c[10];
//    q = ialloc(T_DIR);
//    m = ialloc(T_FILE);
//    assert(writei(m,b,0,10) == 10);
//    assert(dirlink(p,"b",q->inum) == 0);
//    assert(dirlink(q,"test.txt",m->inum) == 0);
//    t = namei("/b/test.txt");
//    assert(readi(t,c,0,10) == 10);
//    printf("res:%s\n",c);
//    printf("%d,%d\n",t->inum,m->inum);

    struct inode *t;
    char c[10];
    t = namei("/b/test.txt");
    assert(readi(t,c,0,10) == 10);
    printf("res:%s\n",c);

    return 0;
}
