
/**
 * @file scmStrings.c
 *
 *  Temporary SCM strings.
 *
 * Time-stamp:        "2012-01-29 09:48:03 bkorb"
 *
 * This file is part of AutoGen.
 * AutoGen Copyright (c) 1992-2012 by Bruce Korb - all rights reserved
 *
 * AutoGen is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AutoGen is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define ROUND_SCRIBBLE(_v, _sz)    (((_v) + ((_sz) - 1)) & ~((_sz) - 1))

typedef struct string_buf_s string_buf_t;

struct string_buf_s {
    string_buf_t *  next_p;
    ssize_t  const  sb_size;
    ssize_t         sb_off;
    unsigned char   sb_buf[1];
};

static string_buf_t *  ag_strbufs  = NULL;
static string_buf_t ** next_strbuf = &ag_strbufs;
static size_t const    str_buf_hdr_sz =
    (&(((string_buf_t *)NULL)->sb_buf[0])) - ((unsigned char *)NULL);

/**
 * Initialize the scribble string library
 */
LOCAL void
ag_scribble_init(void)
{
    ag_strbufs = NULL;
    next_strbuf = &ag_strbufs;
}

/**
 * Free up the scribble strings memory pool.
 */
LOCAL void
ag_scribble_deinit(void)
{
    string_buf_t* sb = ag_strbufs;
    ag_scribble_init();

    while (sb != NULL) {
        string_buf_t* sb_next_p = sb->next_p;
        free(sb);
        sb = sb_next_p;
    }
}

/**
 *  Free up the scribble strings used during the processing of one macro.
 */
LOCAL void
ag_scribble_free(void)
{
    string_buf_t* sb = ag_strbufs;

    while (sb != NULL) {
        sb->sb_off = 0;
        sb = sb->next_p;
    }
}

/**
 * allocate a new scribble block.  Multiple of 8K bytes.
 */
static string_buf_t *
new_scribble_block(size_t min_size)
{
    string_buf_t * res = NULL;

    /*
     * allow space for allocation header and round up
     */
    min_size    += str_buf_hdr_sz;
    min_size     = ROUND_SCRIBBLE(min_size, 0x2000);

    /*
     * Allocate and link into list.  Advance pointer to next entry.
     */
    *next_strbuf = res = AGALOC(min_size, "scribble buf");
    next_strbuf  = &(res->next_p);
    res->next_p  = NULL;
    res->sb_off  = 0;
    /*
     *  The "sb_size" field is read-only.  Override this.
     */
    {
        size_t * psz = (void *)&(res->sb_size);
        *psz = min_size - str_buf_hdr_sz;
    }

    return res;
}

/**
 *  Allocate a scribble string.  It will be deallocated when a macro finishes.
 *  Therefore, *DO NOT* use these at the start of a block macro expecting
 *  the space to still be usable at the end of the block macro.
 *  These allocations are intended for temporary space needs that cannot
 *  be kept on the stack.  Expression processing.
 *
 *  @param  size  the number of bytes needed
 *  @return the memory as a pointer to character
 */
LOCAL char *
ag_scribble(ssize_t size)
{
    string_buf_t* sb = ag_strbufs;
    char* buf;

    size += 1;  // allow for NUL byte & round to word boundary
    size  = ROUND_SCRIBBLE(size, sizeof(void *));

    for (;;) {
        if (sb == NULL) {
            sb = new_scribble_block(size);
            break;
        }

        if ((sb->sb_size - sb->sb_off) >= size)
            break;

        sb   = sb->next_p;
    }

    buf = (char*)(sb->sb_buf + sb->sb_off);
    sb->sb_off += size;
    return buf;
}

/**
 *  As of Guile 1.7.x, access to the NUL terminated string referenced by
 *  an SCM is no longer guaranteed.  Therefore, we must extract the string
 *  into one of our "scribble" buffers.
 *
 * @param  s     the string to convert
 * @param  type  a string describing the string
 * @return a NUL terminated string, or it aborts.
 */
LOCAL char *
ag_scm2zchars(SCM s, const char * type)
{
#if GUILE_VERSION < 107000  /* pre-Guile 1.7.x */
#   error Your Guile is too old.
    choke me.

#else
    size_t len;
    char * buf;

    if (! AG_SCM_STRING_P(s))
        AG_ABEND(aprf(NOT_STR_FMT, type));

    len = scm_c_string_length(s);
    if (len == 0) {
        static char z = NUL;
        return &z;
    }

    buf = ag_scribble(len);

    {
        size_t buflen = scm_to_locale_stringbuf(s, buf, len);
        if (buflen != len)
            AG_ABEND(aprf(SCM2ZCHARS_BAD_VAL, buflen, len));
    }

    buf[len] = NUL;
    return buf;
#endif
}

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of agen5/scmStrings.c */
