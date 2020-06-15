#include"headers.h"

static int nums[100];
static int len;



void recur_path(struct inode *now)
{
    if (now->inum == ROOTINO)
        return;
    else
    {
        struct inode *ip;
        ip = dirlookup(now,"..",0);
        nums[len++] = now->inum;
        recur_path(ip);
        iput(ip);
    }
}

static struct inode* get_root(struct inode *now)
{
    while (now->inum != ROOTINO)
    {
        now = dirlookup(now,"..",0);
    }
    return now;
}

int pwd(struct inode *now)
{

    if (now->inum == ROOTINO)
    {
        printf("/");
        return 0;
    }

    recur_path(now);
    struct inode *cur;
    struct dirent de;
    int off;
    int flag = 0;


    int i;


    cur = get_root(now);
    while (len > 0)
    {
        flag = 0;
        for (off =0; off<cur->size; off += sizeof(de))
        {
            assert(readi(cur,(char*)&de,off,sizeof(de)) == sizeof(de));
            if (de.inum == nums[len-1])
            {
                printf("/%s",de.name);
                cur = dirlookup(cur,de.name,0);
                flag = 1;
                break;
            }
        }

        assert(flag == 1);
        len = len-1;
    }
    return 0;
}
