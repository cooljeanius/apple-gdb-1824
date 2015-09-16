/*
 * FCntl.h -- faccess(), fcntl(), and open() mode flags
 *
 * Portions copyright American Telephone & Telegraph
 * Used with permission, Apple Computer Inc. (1985,1988,1990,1992)
 * All rights reserved.
 */

#ifndef __FCNTL__
#define __FCNTL__

#ifndef __TYPES__
# include <Types.h>
#endif /* !__TYPES__ */

/*
 *	For use by lseek():
 */

#ifndef __STDIO__			/* these defns exactly paralled in StdIO.h for fseek() */
# define SEEK_CUR	1
# define SEEK_END	2
# define SEEK_SET	0
#endif /* !__STDIO__ */

/*
 * faccess() commands; for general use
 */
 					/* 'd' => "directory" ops */
#define F_DELETE		(('d'<<8)|0x01)
#define F_RENAME		(('d'<<8)|0x02)

/*
 * more faccess() commands; for use only by MPW tools
 */

#define F_OPEN 			(('d'<<8)|0x00)		/* reserved for operating system use */
					/* 'e' => "editor" ops */
#define F_GTABINFO 		(('e'<<8)|0x00)		/* get tab offset for file */
#define F_STABINFO 		(('e'<<8)|0x01)		/* set 	"	"		"	"  */
#define F_GFONTINFO		(('e'<<8)|0x02)		/* get font number and size for file */
#define F_SFONTINFO		(('e'<<8)|0x03)		/* set 	"		"	"	"	"	" 	 */
#define F_GPRINTREC		(('e'<<8)|0x04)		/* get print record for file */
#define F_SPRINTREC		(('e'<<8)|0x05)		/* set 	"		"	"	" 	 */
#define F_GSELINFO 		(('e'<<8)|0x06)		/* get selection information for file */
#define F_SSELINFO 		(('e'<<8)|0x07)		/* set		"		"		"		" */
#define F_GWININFO 		(('e'<<8)|0x08)		/* get current window position */
#define F_SWININFO 		(('e'<<8)|0x09)		/* set	"		"		" 	   */
#define F_GSCROLLINFO	(('e'<<8)|0x0A)		/* get scroll information */
#define F_SSCROLLINFO	(('e'<<8)|0x0B)		/* set    "   		"  	  */
#define F_GMARKER		(('e'<<8)|0x0D)		/* Get Marker */
#define F_SMARKER		(('e'<<8)|0x0C)		/* Set   " 	  */
#define F_GSAVEONCLOSE	(('e'<<8)|0x0F)		/* Get Save on close */
#define F_SSAVEONCLOSE	(('e'<<8)|0x0E)		/* Set   "	 "	 " 	 */

/*
 *	argument structures used by various faccess() commands
 */

struct MarkElement {
	int				start;			/* start position of mark */
	int				end;			/* end position */
	unsigned char	charCount;		/* number of chars in mark name */
	char			name[64];		/* mark name */
} ;									/* note: marker names may be up to 64 characters long */

#ifndef __cplusplus
typedef struct MarkElement MarkElement;
#endif /* !__cplusplus */

struct SelectionRecord {
	long	startingPos;
	long	endingPos;
	long	displayTop;
};

#ifndef __cplusplus
typedef struct SelectionRecord SelectionRecord;
#endif /* !__cplusplus */


/*
 * Mode values accessible to open()
 */
#ifndef O_RDONLY
# define O_RDONLY		 0 		/* Bits 0 and 1 are used internally */
#endif /* !O_RDONLY */
#ifndef O_WRONLY
# define O_WRONLY		 1 		/* Values 0..2 are historical */
#endif /* !O_WRONLY */
#ifndef O_RDWR
# define O_RDWR 		 2		/* NOTE: it goes 0, 1, 2, *!* 8, 16, 32, ... */
#endif /* !O_RDWR */
#ifndef O_APPEND
# define O_APPEND	(1<< 3)		/* append (writes guaranteed at the end) */
#endif /* !O_APPEND */
#define O_RSRC 		(1<< 4)		/* Open the resource fork */
#define O_ALIAS		(1<< 5)		/* Open alias file */
#ifndef O_CREAT
# define O_CREAT	(1<< 8)		/* Open with file create */
#endif /* !O_CREAT */
#ifndef O_TRUNC
# define O_TRUNC	(1<< 9)		/* Open with truncation */
#endif /* !O_TRUNC */
#ifndef O_EXCL
# define O_EXCL 	(1<<10) 	/* w/ O_CREAT:  Exclusive "create-only" */
#endif /* !O_EXCL */
#define O_BINARY	(1<<11) 	/* Open as a binary stream */
#define O_NRESOLVE	(1<<14)		/* Do NOT resolve any aliases */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * function prototypes:
 */
int  close(int);
int	 dup(int filedes); /* OBSOLETE: fcntl(filedes, F_DUPFD, 0) is preferred */
int	 faccess(char*, unsigned int, long*);
int  open(const char*, int, ...);

#ifndef _SYS_FCNTL_H_
int  creat(const char*);
int  fcntl(int, unsigned int, int);
long lseek(int, long, int);
int  read(int, char*, unsigned);
int  unlink(char*);
int  write(int, const char*, unsigned);
#endif /* !_SYS_FCNTL_H_ */

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*
 * fcntl() commands
 */
#define F_DUPFD 0	   /* Duplicate files (file descriptor) */

#endif /* !__FCNTL__ */

/* EOF */
