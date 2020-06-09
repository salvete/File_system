#include<stdio.h>
#include<stdlib.h>
#include<sys/fcntl.h>
#include<fcntl.h>
#include<string.h>
int main()
{
    int fd = open("disk.vhd",O_RDWR);
    printf("fd:%d\n",fd);
    char buf[1000]={"hello,world!"};
    int x = write(fd,buf,sizeof(buf));
    printf("x:%d\n",x);
    close(fd);
    fd = open("disk.vhd",O_RDWR);
    char aaa[100];
    int size = read(fd,aaa,100);
    printf("size:%d\n",size);
    printf("%s\n",aaa);
    return 0;
}
