#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

//bio.c
void init_cache();
struct buf*bread(uint);
void bwrite(struct buf*);
void brelse(struct buf *);

//fs.c
void mkfs();
void readsb(struct superblock *);
void icache_init();
struct inode* ialloc(short);
void iupdate(struct inode*);
struct inode* idup(struct inode*);

void iput(struct inode *);
void stati(struct inode*, struct stat *st);
int readi(struct inode*, char *, uint, uint);
int writei(struct inode*, char*, uint,uint);
int namecmp(const char*, const char*);
struct inode *dirlookup(struct inode*, char *, uint*);
int dirlink(struct inode*, char*, uint);
struct inode* namei(char*);
struct inode*nameiparent(char *, char *);

//file.c
struct file *file_alloc();
struct file *file_dup(struct file *);
void file_close(struct file*);
int file_stat(struct file*, struct stat *);
int file_read(struct file *, char *, int );
int file_write(struct file*, char *, int);
struct file *new_file(short);


#endif // DEFS_H_INCLUDED
