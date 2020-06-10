#include"buf.h"
#include"fs.h"
#include"types.h"
#include<assert.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>

struct {
    struct buf buf[NBUF];
    struct buf head;
}bcache;


void init_cache()
{

    struct buf *b;
    bcache.head.prev = &bcache.head;
    bcache.head.next = &bcache.head;

    //建立循环双向链表
    for (b = bcache.buf; b< bcache.buf+NBUF; b++)
    {
        b->next = bcache.head.next;
        b->prev = &bcache.head;
        bcache.head.next->prev = b;
        bcache.head.next = b;
    }
}

static struct buf* bget(uint blockno)
{
    struct buf *b;

    for (b = bcache.head.next; b != &bcache.head; b = b->next)
    {
        //已经找到
        if (b->blockno == blockno)
        {
            b->refcnt++;
            return b;
        }
    }

    for (b = bcache.head.prev; b != &bcache.head; b = b->next)
    {
        if (b->refcnt == 0)
        {
            b->blockno = blockno;
            b->refcnt = 1;
            return b;
        }
    }

    assert(0);

}


struct buf *bread(uint blockno)
{
    struct buf *b;
    b = bget(blockno);
    assert(b != 0);
    int offset = blockno*BSIZE;
    int fd = open("disk.vhd",O_RDONLY);
    assert(fd != -1);
    assert(lseek(fd,offset,SEEK_SET) != -1);
    assert(read(fd,b->data,BSIZE) != -1);
    close(fd);
    return b;
};

void bwrite(struct buf *b)
{
    assert(b);
    int offset = BSIZE*(b->blockno);
    int fd = open("disk.vhd",O_WRONLY);
    assert(fd != -1);
    assert(lseek(fd,offset,SEEK_SET) != -1);
    assert(write(fd,b->data,BSIZE) != -1);
    close(fd);
}

void brelse(struct buf *b)
{
    b->refcnt --;
    if (b->refcnt == 0)
    {
        b->next->prev = b->prev;
        b->prev->next = b->next;
        b->next = bcache.head.next;
        b->prev = &bcache.head;
        bcache.head.next->prev = b;
        bcache.head.next = b;
    }
}


