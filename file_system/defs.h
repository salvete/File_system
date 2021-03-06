#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

//bio.c
void init_cache();
struct buf*bread(uint);
void bwrite(struct buf*);
void brelse(struct buf *);

//fs.c
void detect();
void init_with_clean();
void init_without_clean();
void mkfs();
void init_global();
void set_cur_name(char *);
void readsb(struct superblock *);
void icache_init();
struct inode* ialloc(short);
void iupdate(struct inode*);
void save_info();
struct inode* idup(struct inode*);
void iput(struct inode *);
void stati(struct inode*, struct stat *);
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


//sysfile.c
void init_ofile();
int sys_dup(struct file*);
int sys_read(struct file *f, char *dst, int n);
int sys_write(struct file *f, char *src, int n);
int sys_close(struct file *f);
int sys_fstat(struct file *f, struct stat *st);
int sys_link(char *new_name, char *old);
int sys_unlink(char *path);
int sys_open(char *path, int o_mode);
int sys_mkdir(char *path,short type);
struct file* get_file_by_fd(int fd);


//ls.c
int ls(struct inode*);

//cd.c
int cd(char *path);

//mkdir.c
int mkdir(char *path,short type);

//pwd.c
int pwd(struct inode *now);

//echo.c
int echo(char* info, char *path);

//cat.c
int cat(char *path);

//ln.c
int ln(char *from, char *to);

//del.c
int del(char *path);

//login.c
int login(char *name);
int logout();
int whoami();

//orders.c
void Read_TXT();

//format.c
int format();

//shell.c
void shell();


#endif // DEFS_H_INCLUDED
