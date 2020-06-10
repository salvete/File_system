#ifndef STAT_H_INCLUDED
#define STAT_H_INCLUDED

#include"types.h"

#define T_DIR 1
#define T_FILE 2

struct stat
{
    short type;
    uint ino;
    short nlink;
    uint size;
};


#endif // STAT_H_INCLUDED
