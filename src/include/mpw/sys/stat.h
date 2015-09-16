/* Imitation of sys/stat.h. */

#ifndef __SYS_STAT_H__
#define __SYS_STAT_H__

#include  <sys/types.h>
#include  <time.h>

struct stat {
  dev_t   st_dev;
  ino_t   st_ino;
  mode_t  st_mode;
  short   st_nlink;
  uid_t   st_uid;
  gid_t   st_gid;
  dev_t   st_rdev;
  off_t   st_size;
  off_t   st_rsize;
  time_t  st_atime;
  int     st_spare1;
  time_t  st_mtime;
  int     st_spare2;
  time_t  st_ctime;
  int     st_spare3;
  long    st_blksize;
  long    st_blocks;
  long    st_spare4[2];
};

#ifndef S_IFMT
# define S_IFMT		0170000L
#endif /* !S_IFMT */
#ifndef S_IFDIR
# define S_IFDIR	0040000L
#endif /* !S_IFDIR */
#ifndef S_IFREG
# define S_IFREG	0100000L
#endif /* !S_IFREG */
#ifndef S_IREAD
# define S_IREAD    0400
#endif /* !S_IREAD */
#ifndef S_IWRITE
# define S_IWRITE   0200
#endif /* !S_IWRITE */
#ifndef S_IEXEC
# define S_IEXEC    0100
#endif /* !S_IEXEC */

#ifndef S_IFIFO
# define S_IFIFO	010000  /* FIFO special */
#endif /* !S_IFIFO */
#ifndef S_IFCHR
# define S_IFCHR	020000  /* character special */
#endif /* !S_IFCHR */
#ifndef S_IFBLK
# define S_IFBLK	030000  /* block special */
#endif /* !S_IFBLK */

int stat(char *path, struct stat *buf);
int fstat(int fd, struct stat *buf);

#endif /* __SYS_STAT_H___ */

/* EOF */
