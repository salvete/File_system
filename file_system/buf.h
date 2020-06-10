#ifndef BUF_H_INCLUDED
#define BUF_H_INCLUDED

#include"types.h"
#include"fs.h"

#define NBUF 3000

struct buf{
    uint blockno;
    uint refcnt;
    struct buf *prev;
    struct buf *next;
    uchar data[BSIZE];
};
#endif // BUF_H_INCLUDED
