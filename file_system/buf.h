#ifndef BUF_H_INCLUDED
#define BUF_H_INCLUDED

#include"types.h"
#include"fs.h"

#define NBUF 30

struct buf{
    uint blockno;
    uint refcnt;
    struct buf *prev;
    struct buf *next;
    uchar data[BSIZE];
};

void init_cache();
struct buf* bread(uint blockno);
void bwrite(struct buf *b);
void brelse(struct buf *b);

#endif // BUF_H_INCLUDED
