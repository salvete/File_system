#include"headers.h"
#include<stdio.h>


int main()
{
    init_cache();
    mkfs();
    icache_init();
    init_ofile();
    init_global();

    int fd;

    assert(sys_mkdir("/dir1/") == 0);
    assert(sys_mkdir("/dir2/") == 0);
    assert(sys_mkdir("/dir3/") == 0);
    assert((fd =sys_open("/dir1/dir1.txt",OP_C)) > 2);
    assert(sys_link("/dir1_link","/dir1/dir1.txt") == 0);
    assert(sys_close(get_file_by_fd(fd)) == 0);

    struct file *f;
    char info[50] = {"This is just for testing...\n"};
    char res[50];

    assert((fd =sys_open("/dir1/dir1.txt",OP_W)) > 2);
    assert(sys_write(get_file_by_fd(fd),info,50) == 50);
    assert(sys_close(get_file_by_fd(fd)) == 0);

    assert((fd =sys_open("/dir1/dir1.txt",OP_R)) > 2);
    f = get_file_by_fd(fd);
    assert(sys_read(f,res,50) != -1);
    assert(sys_close(f) == 0);

    //查看目录内容
    printf("根目录 / 内容为：\n");
    ls(cur_inode);
    printf("\n目录 /dir1 内容为：\n");
    f = get_file_by_fd(sys_open("/dir1",OP_R));
    ls(f->ip);

    return 0;
}
