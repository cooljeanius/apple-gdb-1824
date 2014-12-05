
#include <stdio.h>
#include <stdlib.h>
#include "scribble.h"

#ifndef _
#define _(_s) _s
#endif

#define ROUND_SCRIBBLE(_v, _sz)    (((_v) + ((_sz) - 1)) & ~((_sz) - 1))

typedef struct scrib_buf_s scrib_buf_t;

struct scrib_buf_s {
    scrib_buf_t *   sb_next;
    ssize_t  const  sb_size;
    ssize_t         sb_off;
    unsigned char   sb_buf[1];
};

static scrib_buf_t *  bufs     = NULL;
static scrib_buf_t ** next_buf = &bufs;
static size_t const   hdr_sz   =
    (&(((scrib_buf_t *)NULL)->sb_buf[0])) - ((unsigned char *)NULL);

/**
 * Initialize scribble space.  This ensures that the library module globals
 * are ready to start.
 */
void
scribble_init(void)
{
    bufs = NULL;
    next_buf = &bufs;
}

/**
 * De-initialize scribble space.  Frees all the allocated space.
 */
void
scribble_deinit(void)
{
    scrib_buf_t * sb = bufs;
    scribble_init();

    while (sb != NULL) {
        scrib_buf_t * nxt = sb->sb_next;
        free(sb);
        sb = nxt;
    }
}

/**
 * Free the space in the scribble buffers.
 * The scribble buffer allocations are *not* freed.
 */
void
scribble_free(void)
{
    scrib_buf_t * sb = bufs;

    while (sb != NULL) {
        sb->sb_off = 0;
        sb = sb->sb_next;
    }
}

/**
 * allocate a new scribble block.  Multiple of 8K bytes, but has at least
 * \a min_size bytes of data space.
 *
 * @param min_size  minimum size required for current allocation
 * @returns a pointer to the new buffer
 */
static scrib_buf_t *
new_scribble_block(size_t min_size)
{
    scrib_buf_t * res = NULL;

    min_size = ROUND_SCRIBBLE(min_size + hdr_sz, 0x2000U);

    /*
     * Allocate and link into list.  Advance pointer to next entry.
     */
    *next_buf    = res = malloc(min_size);
    if (*next_buf == NULL)
        return NULL;

    next_buf     = &(res->sb_next);
    res->sb_next = NULL;
    res->sb_off  = 0;
    /*
     *  The "sb_size" field is read-only.  Override this during allocation.
     */
    {
        size_t * psz = (void *)&(res->sb_size);
        *psz = min_size - hdr_sz;
    }

    return res;
}

/**
 * Get some scribble space.  Allocates a new scribble buffer, if needed.
 * The allocation gets incremented by one and then rounded to a multiple
 * of sizeof(void*).
 *
 * @param size thesize required.
 * @returns the allocated chunk.
 */
void *
scribble_get(ssize_t size)
{
    scrib_buf_t * sb = bufs;
    char * buf;

     // allow for NUL byte & round to word boundary
    size  = ROUND_SCRIBBLE(size+1, (ssize_t)sizeof(void *));

    for (;;) {
        if (sb == NULL) {
            sb = new_scribble_block((size_t)size);
            if (sb == NULL)
                return NULL;
            break;
        }

        if ((sb->sb_size - sb->sb_off) >= size)
            break;

        sb   = sb->sb_next;
    }

    buf = (char*)(sb->sb_buf + sb->sb_off);
    sb->sb_off += size;
    return buf;
}

/**
 * get some scribble space, but exit on failure.
 *
 * @param size thesize required.
 * @returns the allocated chunk.
 */
void *
xscribble_get(ssize_t sz)
{
    void * p = scribble_get(sz);
    if (p == NULL) {
        fprintf(stderr, _("could not allocate %u bytes of scribble space"), sz);
        exit(EXIT_FAILURE);
    }
    return p;
}
