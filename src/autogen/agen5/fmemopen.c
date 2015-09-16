#if defined(ENABLE_FMEMOPEN)
#include <sys/ioctl.h>

typedef enum {
    FMEMC_INVALID       = 0,
    FMEMC_GET_BUF_ADDR
} fmemctl_t;

typedef struct {
    enum { FMEMC_GBUF_LEAVE_OWNERSHIP,
           FMEMC_GBUF_TAKE_OWNERSHIP
    }          own;
    char *     buffer;
    size_t     buf_size;
    size_t     eof;
} fmemc_get_buf_addr_t;

#ifdef HURD
#define _IOT__IOTBASE_fmemc_get_buf_addr_t sizeof(fmemc_get_buf_addr_t)
#endif

#define IOCTL_FMEMC_GET_BUF_ADDR \
    _IOWR('m', FMEMC_GET_BUF_ADDR, fmemc_get_buf_addr_t)

/**
 * @file /old-home/bkorb/tools/mine/lib/fmemopen/fmemopen.c
 *
 * Copyright (c) 2004-2012 by Bruce Korb.  All rights reserved.
 *
 * This code was inspired from software written by
 *   Hanno Mueller, kontakt@hanno.de
 * and completely rewritten by Bruce Korb, bkorb@gnu.org
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#if defined(HAVE_FOPENCOOKIE)
#  if defined(HAVE_LIBIO_H)
#    include <libio.h>
#  endif

   typedef off64_t * seek_off_t;
   typedef int       seek_ret_t;

#elif defined(HAVE_FUNOPEN)
   typedef fpos_t  seek_off_t;
   typedef fpos_t  seek_ret_t;

#  ifdef NEED_COOKIE_FUNCTION_TYPEDEFS
     typedef int     (cookie_read_function_t )(void *, char *, int);
     typedef int     (cookie_write_function_t)(void *, char const *, int);
     typedef fpos_t  (cookie_seek_function_t )(void *, fpos_t, int);
     typedef int     (cookie_close_function_t)(void *);
#  endif /* NEED_COOKIE_FUNCTION_TYPEDEFS */
#endif

#define PROP_TABLE \
_Prop_( read,       "Read from buffer"        ) \
_Prop_( write,      "Write to buffer"         ) \
_Prop_( append,     "Append to buffer okay"   ) \
_Prop_( binary,     "byte data - not string"  ) \
_Prop_( create,     "allocate the string"     ) \
_Prop_( truncate,   "start writing at start"  ) \
_Prop_( allocated,  "we allocated the buffer" ) \
_Prop_( fixed_size, "writes do not append"    )

#define _Prop_(n,s)   BIT_ID_ ## n,
typedef enum { PROP_TABLE BIT_CT } fmem_flags_e;
#undef  _Prop_

#define FLAG_BIT(n)  (1 << BIT_ID_ ## n)

typedef unsigned long mode_bits_t;
typedef unsigned char buf_bytes_t;

typedef struct fmem_cookie_s fmem_cookie_t;
struct fmem_cookie_s {
    mode_bits_t    mode;
    buf_bytes_t *  buffer;
    size_t         buf_size;    /* Full size of buffer */
    size_t         next_ix;     /* Current position */
    size_t         eof;         /* End Of File */
    size_t         pg_size;     /* size of a memory page.
                                   Future architectures allow it to vary
                                   by memory region. */
};

typedef struct {
    FILE *          fp;
    fmem_cookie_t * cookie;
} cookie_fp_map_t;

#ifndef AUTOGEN_BUILD
/**
 * invent our own boolean rather than try to work around all the ways
 * different systems spell it.
 */
typedef enum { AG_FALSE = 0, AG_TRUE = 1 } ag_bool;
#endif

static cookie_fp_map_t const * map    = NULL;
static unsigned int            map_ct = 0;
static unsigned int            map_alloc_ct = 0;

/* = = = START-STATIC-FORWARD = = = */
static int
fmem_getmode(char const * mode, mode_bits_t * pRes);

static int
fmem_extend(fmem_cookie_t *pFMC, size_t new_size);

static ssize_t
fmem_read(void *cookie, void *pBuf, size_t sz);

static ssize_t
fmem_write(void *cookie, const void *pBuf, size_t sz);

static seek_ret_t
fmem_seek(void * cookie, seek_off_t offset, int dir);

static int
fmem_close(void * cookie);

static ag_bool
fmem_config_user_buf(fmem_cookie_t * pFMC, void * buf, ssize_t len);

static ag_bool
fmem_alloc_buf(fmem_cookie_t * pFMC, ssize_t len);
/* = = = END-STATIC-FORWARD = = = */

#ifdef TEST_FMEMOPEN
  static fmem_cookie_t* saved_cookie = NULL;
#endif

/**
 * Convert a mode string into mode bits.
 */
static int
fmem_getmode(char const * mode, mode_bits_t * pRes)
{
    if (mode == NULL)
        return 1;

    switch (*mode) {
    case 'a': *pRes = FLAG_BIT(write) | FLAG_BIT(append);
              break;
    case 'w': *pRes = FLAG_BIT(write) | FLAG_BIT(truncate);
              break;
    case 'r': *pRes = FLAG_BIT(read);
              break;
    default:  return EINVAL;
    }

    /*
     *  If someone wants to supply a "wxxbxbbxbb+" mode string, I don't care.
     */
    for (;;) {
        switch (*++mode) {
        case '+': *pRes |= FLAG_BIT(read) | FLAG_BIT(write);
                  if (mode[1] != NUL)
                      return EINVAL;
                  break;
        case NUL: break;
        case 'b': *pRes |= FLAG_BIT(binary); continue;
        case 'x': continue;
        default:  return EINVAL;
        }
        break;
    }

    return 0;
}

/**
 * Extend the space associated with an fmem file.
 */
static int
fmem_extend(fmem_cookie_t *pFMC, size_t new_size)
{
    size_t ns = (new_size + (pFMC->pg_size - 1)) & (~(pFMC->pg_size - 1));

    /*
     *  We can expand the buffer only if we are in append mode.
     */
    if (pFMC->mode & FLAG_BIT(fixed_size))
        goto no_space;

    if ((pFMC->mode & FLAG_BIT(allocated)) == 0) {
        /*
         *  Previously, this was a user supplied buffer.  We now move to one
         *  of our own.  The user is responsible for the earlier memory.
         */
        void* bf = malloc(ns);
        if (bf == NULL)
            goto no_space;

        memcpy(bf, pFMC->buffer, pFMC->buf_size);
        pFMC->buffer = bf;
        pFMC->mode  |= FLAG_BIT(allocated);
    }
    else {
        void* bf = realloc(pFMC->buffer, ns);
        if (bf == NULL)
            goto no_space;

        pFMC->buffer = bf;
    }

    /*
     *  Unallocated file space is set to zeros.  Emulate that.
     */
    memset(pFMC->buffer + pFMC->buf_size, 0, ns - pFMC->buf_size);
    pFMC->buf_size = ns;
    return 0;

 no_space:
    errno = ENOSPC;
    return -1;
}

/**
 * Handle file system callback to read data from our string.
 */
static ssize_t
fmem_read(void *cookie, void *pBuf, size_t sz)
{
    fmem_cookie_t *pFMC = cookie;

    if (pFMC->next_ix + sz > pFMC->eof) {
        if (pFMC->next_ix >= pFMC->eof)
            return (sz > 0) ? -1 : 0;
        sz = pFMC->eof - pFMC->next_ix;
    }

    memcpy(pBuf, pFMC->buffer + pFMC->next_ix, sz);

    pFMC->next_ix += sz;

    return sz;
}

/**
 * Handle file system callback to write data to our string
 */
static ssize_t
fmem_write(void *cookie, const void *pBuf, size_t sz)
{
    fmem_cookie_t *pFMC = cookie;
    int add_nul_char;

    /*
     *  In append mode, always seek to the end before writing.
     */
    if (pFMC->mode & FLAG_BIT(append))
        pFMC->next_ix = pFMC->eof;

    /*
     *  Only add a NUL character if:
     *
     *  * we are not in binary mode
     *  * there are data to write
     *  * the last character to write is not already NUL
     */
    add_nul_char =
           ((pFMC->mode & FLAG_BIT(binary)) != 0)
        && (sz > 0)
        && (((char*)pBuf)[sz - 1] != NUL);

    {
        size_t next_pos = pFMC->next_ix + sz + add_nul_char;
        if (next_pos > pFMC->buf_size) {
            if (fmem_extend(pFMC, next_pos) != 0) {
                /*
                 *  We could not extend the memory.  Try to write some data.
                 *  Fail if we are either at the end or not writing data.
                 */
                if ((pFMC->next_ix >= pFMC->buf_size) || (sz == 0))
                    return -1; /* no space at all.  errno is set. */

                /*
                 *  Never add the NUL for a truncated write.  "sz" may be
                 *  unchanged or limited here.
                 */
                add_nul_char = 0;
                sz = pFMC->buf_size - pFMC->next_ix;
            }
        }
    }

    memcpy(pFMC->buffer + pFMC->next_ix, pBuf, sz);

    pFMC->next_ix += sz;

    /*
     *  Check for new high water mark and remember it.  Add a NUL if
     *  we do that and if we have a new high water mark.
     */
    if (pFMC->next_ix > pFMC->eof) {
        pFMC->eof = pFMC->next_ix;
        if (add_nul_char)
            /*
             *  There is space for this NUL.  The "add_nul_char" is not part of
             *  the "sz" that was added to "next_ix".
             */
            pFMC->buffer[ pFMC->eof ] = NUL;
    }

    return sz;
}

/**
 * Handle file system callback to set a new current position
 */
static seek_ret_t
fmem_seek(void * cookie, seek_off_t offset, int dir)
{
    size_t new_pos;
    fmem_cookie_t *pFMC = cookie;

#ifdef HAVE_FOPENCOOKIE
    /*
     *  GNU interface:  offset passed and returned by address.
     */
    switch (dir) {
    case SEEK_SET: new_pos = *offset;  break;
    case SEEK_CUR: new_pos = pFMC->next_ix  + *offset;  break;
    case SEEK_END: new_pos = pFMC->eof - *offset;  break;

    default:
        goto seek_oops;
    }
#else
    /*
     *  BSD interface:  offset passed by value, returned as retval.
     */
    switch (dir) {
    case SEEK_SET: new_pos = offset;  break;
    case SEEK_CUR: new_pos = pFMC->next_ix  + offset;  break;
    case SEEK_END: new_pos = pFMC->eof - offset;  break;

    default:
        goto seek_oops;
    }
#endif

    if ((signed)new_pos < 0)
        goto seek_oops;

    if (new_pos > pFMC->buf_size) {
        if (fmem_extend(pFMC, new_pos))
            return -1; /* errno is set */
    }

    pFMC->next_ix = new_pos;

#ifdef HAVE_FOPENCOOKIE
    *offset = (off64_t)new_pos;
    return 0;
#else
    return new_pos;
#endif

 seek_oops:
    errno = EINVAL;
    return -1;
}

/**
 * Free up the memory associated with an fmem file.
 * If the user is managing the space, then the allocated bit is set.
 */
static int
fmem_close(void * cookie)
{
    fmem_cookie_t   * pFMC = cookie;
    cookie_fp_map_t * pmap = (void *)map;
    unsigned int      mct  = map_ct;

    while (mct-- != 0) {
        if (pmap->cookie == cookie) {
            *pmap = map[--map_ct];
            break;
        }
        pmap++;
    }

    if (mct > map_ct)
        errno = EINVAL;

    if (pFMC->mode & FLAG_BIT(allocated))
        free(pFMC->buffer);
    free(pFMC);

    return 0;
}

/**
 * Configure the user supplied buffer.
 */
static ag_bool
fmem_config_user_buf(fmem_cookie_t * pFMC, void * buf, ssize_t len)
{
    /*
     *  User allocated buffer.  User responsible for disposal.
     */
    if (len == 0) {
        free(pFMC);
        errno = EINVAL;
        return AG_FALSE;
    }

    pFMC->buffer = (buf_bytes_t*)buf;

    /*  Figure out where our "next byte" and EOF are.
     *  Truncated files start at the beginning.
     */
    if (pFMC->mode & FLAG_BIT(truncate)) {
        /*
         *  "write" mode
         */
        pFMC->eof = \
            pFMC->next_ix = 0;
    }

    else if (pFMC->mode & FLAG_BIT(binary)) {
        pFMC->eof = len;
        pFMC->next_ix    = (pFMC->mode & FLAG_BIT(append)) ? len : 0;

    } else {
        /*
         * append or read text mode -- find the end of the buffer
         * (the first NUL character)
         */
        buf_bytes_t *p = (buf_bytes_t*)buf;

        pFMC->eof = 0;
        while ((*p != NUL) && (++(pFMC->eof) < len))  p++;
        pFMC->next_ix =
            (pFMC->mode & FLAG_BIT(append)) ? pFMC->eof : 0;
    }

    /*
     *  text mode - NUL terminate buffer, if it fits.
     */
    if (  ((pFMC->mode & FLAG_BIT(binary)) == 0)
       && (pFMC->next_ix < len)) {
        pFMC->buffer[pFMC->next_ix] = NUL;
    }

    pFMC->buf_size = len;
    return AG_TRUE;
}

/**
 * Allocate an initial buffer for fmem.
 */
static ag_bool
fmem_alloc_buf(fmem_cookie_t * pFMC, ssize_t len)
{
    /*
     *  We must allocate the buffer.  If "len" is zero, set it to page size.
     */
    pFMC->mode |= FLAG_BIT(allocated);
    if (len == 0)
        len = pFMC->pg_size;

    /*
     *  Unallocated file space is set to NULs.  Emulate that.
     */
    pFMC->buffer = calloc((size_t)1, (size_t)len);
    if (pFMC->buffer == NULL) {
        errno = ENOMEM;
        free(pFMC);
        return AG_FALSE;
    }

    /*
     *  We've allocated the buffer.  The end of file and next entry
     *  are both zero.
     */
    pFMC->next_ix  = 0;
    pFMC->eof      = 0;
    pFMC->buf_size = len;
    return AG_TRUE;
}

/*=export_func ag_fmemopen
 *
 *  what:  Open a stream to a string
 *
 *  arg: + void*   + buf  + buffer to use for i/o +
 *  arg: + ssize_t + len  + size of the buffer +
 *  arg: + char*   + mode + mode string, a la fopen(3C) +
 *
 *  ret-type:  FILE*
 *  ret-desc:  a stdio FILE* pointer
 *
 *  err:  NULL is returned and errno is set to @code{EINVAL} or @code{ENOSPC}.
 *
 *  doc:
 *
 *  This function requires underlying @var{libc} functionality:
 *  either @code{fopencookie(3GNU)} or @code{funopen(3BSD)}.
 *
 *  If @var{buf} is @code{NULL}, then a buffer is allocated.  The initial
 *  allocation is @var{len} bytes.  If @var{len} is less than zero, then the
 *  buffer will be reallocated as more space is needed.  Any allocated
 *  memory is @code{free()}-ed when @code{fclose(3C)} is called.
 *
 *  If @code{buf} is not @code{NULL}, then @code{len} must not be zero.
 *  It may still be less than zero to indicate that the buffer may
 *  be reallocated.
 *
 *  The mode string is interpreted as follows.  If the first character of
 *  the mode is:
 *
 *  @table @code
 *  @item a
 *  Then the string is opened in "append" mode.  In binary mode, "appending"
 *  will begin from the end of the initial buffer.  Otherwise, appending will
 *  start at the first NUL character in the initial buffer (or the end of the
 *  buffer if there is no NUL character).  Do not use fixed size buffers
 *  (negative @var{len} lengths) in append mode.
 *
 *  @item w
 *  Then the string is opened in "write" mode.  Any initial buffer is presumed
 *  to be empty.
 *
 *  @item r
 *  Then the string is opened in "read" mode.
 *  @end table
 *
 *  @noindent
 *  If it is not one of these three, the open fails and @code{errno} is
 *  set to @code{EINVAL}.  These initial characters may be followed by:
 *
 *  @table @code
 *  @item +
 *  The buffer is marked as updatable and both reading and writing is enabled.
 *
 *  @item b
 *  The I/O is marked as "binary" and a trailing NUL will not be inserted
 *  into the buffer.  Without this mode flag, one will be inserted after the
 *  @code{EOF}, if it fits.  It will fit if the buffer is extensible (the
 *  provided @var{len} was negative).  This mode flag has no effect if
 *  the buffer is opened in read-only mode.
 *
 *  @item x
 *  This is ignored.
 *  @end table
 *
 *  @noindent
 *  Any other letters following the inital 'a', 'w' or 'r' will cause an error.
=*/
FILE *
ag_fmemopen(void * buf, ssize_t len, char const * mode)
{
    fmem_cookie_t *pFMC;

    {
        mode_bits_t bits;

        if (fmem_getmode(mode, &bits) != 0) {
            return NULL;
        }

        pFMC = malloc(sizeof(fmem_cookie_t));
        if (pFMC == NULL) {
            errno = ENOMEM;
            return NULL;
        }

        pFMC->mode = bits;
    }

    /*
     *  Two more mode bits that do not come from the mode string:
     *  a negative size implies fixed size buffer and a NULL
     *  buffer pointer means we must allocate (and free) it.
     */
    if (len <= 0) {
        /*
         *  We only need page size if we might extend an allocation.
         */
        len = -len;
        pFMC->pg_size = getpagesize();
    }

    else {
        pFMC->mode |= FLAG_BIT(fixed_size);
    }

    if (buf != NULL) {
        if (! fmem_config_user_buf(pFMC, buf, len))
            return NULL;

    } else if ((pFMC->mode & (FLAG_BIT(append) | FLAG_BIT(truncate))) == 0) {
        /*
         *  Not appending and not truncating.  We must be reading.
         *  We also have no user supplied buffer.  Nonsense.
         */
        errno = EINVAL;
        free(pFMC);
        return NULL;
    }

    else if (! fmem_alloc_buf(pFMC, len))
        return NULL;

#ifdef TEST_FMEMOPEN
    saved_cookie = pFMC;
#endif

    {
        FILE * res;

        cookie_read_function_t* pRd = (pFMC->mode & FLAG_BIT(read))
            ?  (cookie_read_function_t*)fmem_read  : NULL;
        cookie_write_function_t* pWr = (pFMC->mode & FLAG_BIT(write))
            ? (cookie_write_function_t*)fmem_write : NULL;

#ifdef HAVE_FOPENCOOKIE
        cookie_io_functions_t iof;
        iof.read  = pRd;
        iof.write = pWr;
        iof.seek  = fmem_seek;
        iof.close = fmem_close;

        res = fopencookie(pFMC, mode, iof);
#else
        res = funopen(pFMC, pRd, pWr, fmem_seek, fmem_close);
#endif
        if (res == NULL)
            return res;

        if (++map_ct >= map_alloc_ct) {
            void * p = (map_alloc_ct > 0)
                ? realloc((void *)map, (map_alloc_ct += 4) * sizeof(*map))
                : malloc((map_alloc_ct = 4) * sizeof(*map));

            if (p == NULL) {
                fclose(res);
                errno = ENOMEM; /* "fclose" set it to "EINVAL". */
                return NULL;
            }

            map = p;
        }

        {
            cookie_fp_map_t * p = (void *)(map + map_ct - 1);
            p->fp = res;
            p->cookie = pFMC;
        }

        return res;
    }
}

/*=export_func ag_fmemioctl
 *
 *  what:  perform an ioctl on a FILE* descriptor
 *
 *  arg: + FILE* + fp      + file pointer  +
 *  arg: + int   + req     + ioctl command +
 *  arg: + ...   + varargs + arguments for command +
 *
 *  ret-type:  int
 *  ret-desc:  zero on success, otherwise error in errno
 *
 *  err:  errno is set to @code{EINVAL} or @code{ENOSPC}.
 *
 *  doc:
 *
 *  The file pointer passed in must have been returned by ag_fmemopen.
=*/
int
ag_fmemioctl(FILE * fp, int req, ...)
{
    fmem_cookie_t * cookie;
    fmemc_get_buf_addr_t * gba;

    if (req != IOCTL_FMEMC_GET_BUF_ADDR) {
        /*
         *  It is not any of the IOCTL commands we know about.
         */
        errno = EINVAL;
        return -1;
    }

    {
        cookie_fp_map_t const * pmap = map;
        unsigned int mct  = map_ct;

        for (;;) {
            if (mct-- == 0) {
                /*
                 *  fmemopen didn't create this FILE*, so it is invalid.
                 */
                errno = EINVAL;
                return -1;
            }
            if (pmap->fp == fp)
                break;
            pmap++;
        }

        cookie = pmap->cookie;
    }

    {
        va_list ap;
        va_start(ap, req);
        gba = va_arg(ap, fmemc_get_buf_addr_t *);
        va_end(ap);
    }

    gba->buffer   = (char *)(cookie->buffer);
    gba->buf_size = cookie->buf_size;
    gba->eof      = cookie->eof;
    if (gba->own != FMEMC_GBUF_LEAVE_OWNERSHIP)
        cookie->mode &= ~FLAG_BIT(allocated);
    return 0;
}

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of fmemopen.c */
#endif /* ENABLE_FMEMOPEN */
