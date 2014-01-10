
/**
 * \file char-mapper.c
 *
 *  This is the main routine for char-mapper.
 *
 *  This file is part of char-mapper.
 *  char-mapper Copyright (C) 2003-2013 by Bruce Korb - all rights reserved
 *
 * char-mapper is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * char-mapper is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "char-mapper.h"

int
main(int argc, char ** argv)
{
    program = strrchr(argv[0], '/');
    if (program++ == NULL)
        program = argv[0];

    if (argc > 1) {
        char * pz = argv[1];
        if (*pz == '-')
            parse_help(pz);
        if (freopen(argv[1], "r", stdin) != stdin) {
            fprintf(stderr, "fs error %d (%s) reopening '%s' as stdin\n",
                    errno, strerror(errno), pz);
            exit(EXIT_FAILURE);
        }

    } else if (isatty(STDIN_FILENO))
        parse_help(NULL);

    else argv[1] = "stdin";

    bit_count = read_data();
    init_names();

    emit_leader(argv[1]);
    emit_macros(bit_count);
    emit_table(bit_count);
    if (optimize_code)
        emit_opt_functions();
    else
        emit_functions();
    return EXIT_SUCCESS;
}

void
die(char const * fmt, ...)
{
    va_list ap;
    fputs("char-mapper error:  ", stderr);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(EXIT_FAILURE);
}

static void
emit_leader(char * input)
{
    if (add_test_code) {
        if (out_file_nm != NULL)
            printf(test_script_fmt, base_fn_nm, base_ucase, out_file_nm);
    }

    {
        time_t tm = time(NULL);
        struct tm * tmp = localtime(&tm);
        char const * mask_type =
            (bit_count <= 8) ? type_8bits
            : ((bit_count <= 16) ? type_16bits
               : ((bit_count <= 32) ? type_32bits
                  : type_64bits ));

        strftime(buffer, BUF_SIZE, "%x at %X", tmp);
        printf(leader_fmt, bit_count, total_map_ct, buffer,
               commentary, file_guard, mask_type, program);
    }

    if (fseek(stdin, 0, SEEK_SET) == 0)
        copy_input_text(input);

    {
        int width = (bit_count+3)/4;
        if (width < 2) width = 2;
        sprintf(mask_fmt, mask_fmt_fmt, width);
        if (width > 8)
            strcat(mask_fmt, "ULL");
    }
}

static void
init_names(void)
{
    if (table_name == NULL) {

        /*
         * If the guard is unchanged from the initial default,
         */
        if (file_guard == char_map_gd)
            table_name = "char_type_table";

        else {
            static char const grd[] = "_GUARD";
            char * p = NULL;
            char const * e = file_guard + strlen(file_guard)
                - (sizeof(grd) - 1);

            /*
             * If the guard ends with "_GUARD", then set "e" back to the end.
             * Otherwise, if it ends with "_H_GUARD", then trim the "_H" too.
             */
            if (strcmp(e, grd) != 0)
                e += sizeof(grd) - 1;
            else if ((e[-2] == '_') && (e[-1] == 'H'))
                e -= 2;

            /*
             * Make the table name be the downcased, trimmed guard.
             */
            size_t copy_len = e - file_guard;
            table_name = p = malloc(copy_len + table_sfx_LEN + 1);
            memcpy(p, file_guard, copy_len);
            memcpy(p + copy_len, table_sfx, table_sfx_LEN + 1);
            for (; p < table_name + copy_len; p++)
                *p = tolower((int)*p);
        }
    }

    /*
     *  If "%guard" has not been specified (as either empty or with a name),
     *  then create an ifdef name to guard the data tables.
     */
    if (! table_is_static && (data_guard == NULL)) {
        char * p = malloc(strlen(table_name) + define_fmt_LEN);
        sprintf(p, define_fmt, table_name);
        make_define_name(p);
        data_guard = p;
    }

    {
        static char const msk[] = "_mask_t";
        size_t base_name_len = strlen(table_name); ///< base name length

        /*
         * If the table name already has a "_table" suffix, don't count it in
         * the base name length.  Otherwise, "table_name" is our base name.
         */
        if (strcmp(table_name + base_name_len - table_sfx_LEN, table_sfx) == 0)
            base_name_len -= table_sfx_LEN;

        char * p = malloc(base_name_len + table_sfx_LEN + 1);
        memcpy(p, table_name, base_name_len);
        memcpy(p + base_name_len, msk, sizeof(msk));
        mask_name = p;

        p = malloc(base_name_len + 1);
        memcpy(p, table_name, base_name_len);
        p[base_name_len]  = NUL;
        base_fn_nm = p; // base name of most everything

        p = malloc(base_name_len + 1);
        memcpy(p, table_name, base_name_len);
        p[base_name_len]  = NUL;
        make_define_name(p);
        base_ucase = p; // upper case flavor of our base name
    }
}

/**
 * Copy out the input to the output as a comment.
 * The input file was successfully rewound, so we simply re-read it.
 *
 * @param name the name of the input file.
 */
static void
copy_input_text(char const * name)
{
    bool skip_comment = false;

    printf(copy_input_start, name);
    while (fgets(buffer, BUF_SIZE, stdin) != NULL) {
        if (strstr(buffer, "%comment") != NULL) {
            skip_comment = true;
            fputs("// %comment -- see above\n", stdout);
            continue;
        }
        if (strstr(buffer, "%emit") != NULL) {
            skip_comment = true;
            fputs("// %emit code -- see below\n", stdout);
            continue;
        }
        if (skip_comment) {
            int bfix = 0;
            while (isspace(buffer[bfix]))  bfix++;
            if (buffer[bfix] != '%') continue;
            skip_comment = false;
        }
        printf("// %s", buffer);
    }

    fwrite(copy_input_end, copy_input_end_LEN, 1, stdout);
}

/**
 * See of the command argument matches a help request.
 * We accept "-h" and "--h[e[l[p]]]".
 *
 * @param name the name of the input file.
 */
void
parse_help(char const * opt_pz)
{
    int exit_code = EXIT_SUCCESS;
    FILE * fp = stdout;
    char const * pz = opt_pz;

    if (opt_pz == NULL) {
        fp = stderr;
        fprintf(stderr, opterrmsg, input_is_tty, must_be_file);
        fwrite(usage_text, usage_text_LEN, 1, fp);
        exit_code = EXIT_FAILURE;
    } else

    switch (pz[1]) {
    case '-':
        pz += 2;
        static char const help[] = "help";
        char const * p = help;
        for (;;) {
            if (*pz != *p)
                goto bad_opt;
            if (*(p++) == NUL)
                break;
            if (*++pz == NUL)
                break;
        }
        /* FALLTHROUGH */

    case 'h':
        fwrite(usage_text, usage_text_LEN, 1, fp);
        break;

    default:
    bad_opt:
        exit_code = EXIT_FAILURE;
        fp = stderr;
        fprintf(stderr, opterrmsg, "unknown option", opt_pz);
        pz = usage_text;
        do putc(*pz, stderr);
        while (*(pz++) != '\n');
    }

    exit(exit_code);
}

/**
 * Emit all the macros for doing the char classification tests.
 * "NBBY" is "Number of Bits per BYte".
 *
 * @param bit_count  the number of bits in use
 */
void
emit_macros(int bit_count)
{
    char fill[CLASS_NAME_LIMIT+3];
    int  map_ix = 0;

    /*
     * The type to use for the mask depends on the number of bits
     * in the output.  Make the selection and emit the mask typedef.
     */
    {
        char const * mask_type;
        switch (bit_count) {
        case  1 ...  8: mask_type = "uint8_t";  break;
        case  9 ... 16: mask_type = "uint16_t"; break;
        case 17 ... 32: mask_type = "uint32_t"; break;
        case 33 ... 64: mask_type = "uint64_t"; break;

        default:
            die("too many char types (%u max)\n", sizeof(long long) * NBBY);
        }
        printf(typedef_mask, mask_type, mask_name);
    }

    /*
     *  Make a padding string so our output looks pretty.
     */
    if (max_name_len < CLASS_NAME_LIMIT-2)
        max_name_len += 2;
    memset(fill, ' ', max_name_len);
    fill[max_name_len - 1] = NUL;

    for (value_map_t * map = all_map.next; map != NULL; map = map->next) {
        size_t ln = strlen(map->vname);
        char * pz = fill + ln; // padding pointer
        char z[24], y[24]; // 24 >= ceil(log10(1 << 63)) + 1
        snprintf(z, sizeof(z), mask_fmt, map->mask);

        if (optimize_code)
            sprintf(y, "%u", map_ix++);
        else
            strcpy(y, z);

        printf(macro_def_fmt, map->vname, pz, base_fn_nm, z, y);
        if (add_backup_code)
            printf(backup_def_fmt, map->vname, pz, base_fn_nm, y);
    }

    putc('\n', stdout);
}

/**
 * produce a test main procedure that shows the classsifications.
 * Iterate character values from 0 through 0x7F.
 */
static void
emit_test_code(void)
{
    int ix = 0;
    value_map_t * map;
    printf(testit_fmt, base_ucase);
    for (map = all_map.next; map != NULL; map = map->next) {
        printf(testit_class_names, ix++, map->vname);
    }
    fputs(testit_class_hdrs, stdout);
    for (map = all_map.next, ix=0;
         map != NULL;
         map = map->next) {
        printf(" %02X", ix++);
    }
    fputs(test_loop, stdout);
    for (map = all_map.next, ix=0;
         map != NULL;
         map = map->next) {
        char z[24]; // 24 >= ceil(log10(1 << 63)) + 1
        snprintf(z, sizeof(z), mask_fmt, map->mask);
        printf(each_test, base_fn_nm, z);
        ix++;
    }
    fputs("        putchar('\\n');\n", stdout);
    printf(endtest_fmt, base_ucase);
}

void
emit_functions(void)
{
    if (! table_is_static)
        printf(declare_tbl, mask_name, table_name, TABLE_SIZE);

    printf(inline_functions,
           base_fn_nm, mask_name, TABLE_SIZE, table_name);

    if (add_backup_code)
        printf(inline_backup, base_fn_nm, mask_name);

    if (add_on_text != NULL)
        printf(emit_text_fmt, add_on_text);

    if (add_test_code)
        emit_test_code();

    printf(endif_fmt, file_guard);
}

void
emit_opt_functions(void)
{
    if (! table_is_static)
        printf(declare_opt_tbl, mask_name, base_fn_nm,
               TABLE_SIZE, base_ucase, total_map_ct);

    printf(inline_opt_functions, base_fn_nm, mask_name, TABLE_SIZE);

    if (add_backup_code)
        printf(inline_opt_backup, base_fn_nm);

    if (add_on_text != NULL)
        printf(emit_text_fmt, add_on_text);

    if (add_test_code)
        emit_test_code();

    printf(endif_fmt, file_guard);
}

void
emit_table(int bit_count)
{
    char const * type_pz;
    char * fmt;

    int  ix = 0;

    /**
     * How many entries can be initialized on one line?  We can do four
     * uint8_t's, uint16_t's or even uint32_t's.  Only two uint64_t's tho.
     */
    int  init_ct  = (bit_count > 32) ? 2 : 4;
    int  entry_ct = init_ct;

    if (table_is_static)
        printf(static_table_fmt, mask_name, table_name, TABLE_SIZE);
    else
        printf(table_fmt, data_guard, mask_name, table_name, TABLE_SIZE);

    for (;;) {
        if (isprint(ix))
            printf(" /* %c */ ", (char)ix);

        else switch (ix) {
            case NUL:  fputs(" /*NUL*/ ", stdout); break;
            case '\a': fputs(" /*BEL*/ ", stdout); break;
            case '\b': fputs(" /* BS*/ ", stdout); break;
            case '\t': fputs(" /* HT*/ ", stdout); break;
            case '\n': fputs(" /* NL*/ ", stdout); break;
            case '\v': fputs(" /* VT*/ ", stdout); break;
            case '\f': fputs(" /* FF*/ ", stdout); break;
            case '\r': fputs(" /* CR*/ ", stdout); break;
            case 0x1B: fputs(" /*ESC*/ ", stdout); break;
            default:  printf(" /*x%02X*/ ", ix);   break;
            }

        printf(mask_fmt, all_map.values[ix]);

        if (++ix >= TABLE_SIZE)
            break;

        putc(',', stdout);

        if (--entry_ct <= 0) {
            putc('\n', stdout);
            putc(' ', stdout);
            entry_ct = init_ct;
        }
    }

    fputs(end_table, stdout);

    if (optimize_code) {
        static char const fmtfmt[] = "    %%s, /* %%-%us*/\n";
        char const * static_pfx  = table_is_static ? "static " : "";
        char const * locking_fmt = pthread_code ? pthread_locking : no_locking;
        char fmt[256];

        printf(optimize_fmt, static_pfx, base_fn_nm, total_map_ct);
        snprintf(fmt, sizeof(fmt), fmtfmt, (unsigned int)max_name_len);

        for (value_map_t * map = all_map.next; map != NULL; map = map->next) {
            char z[24]; // 24 >= ceil(log10(1 << 63)) + 1
            snprintf(z, sizeof(z), mask_fmt, map->mask);
            printf(fmt, z, map->vname);
        }
        printf(locking_fmt, base_fn_nm);
        printf(fill_opt_fmt, base_fn_nm, static_pfx);
    }

    if (! table_is_static)
        printf(endif_fmt, data_guard);
}

int
read_data(void)
{
    int bit_num = 0;

    while (fgets(buffer, BUF_SIZE, stdin) != NULL) {
        char nm_buf[CLASS_NAME_LIMIT+1];
        char * scan = trim(buffer);
        value_map_t * map = NULL;
        if (scan == NULL)  continue;

        scan = get_name(scan, nm_buf, sizeof(nm_buf));
        if (curr_name_len > max_name_len)
            max_name_len = curr_name_len;

        while (scan != NULL) {
            switch (*scan) {
            case NUL: goto end_while;
            case '"':
                if (map == NULL)
                    map  = new_map(nm_buf);
                if (map->bit_no == ~0) {
                    map->mask  |= 1 << bit_num;
                    map->bit_no = bit_num;
                    bit_num++;
                }
                scan = scan_quoted_value(scan+1, map, VAL_ADD);
                break;

            case ' ': case '\t':
                while (isspace(*scan))  scan++;
                break;

            case '+':
            case '-':
                if (map == NULL)
                    map  = new_map(nm_buf);
                if (scan[1] == '"')
                     scan = scan_quoted_value(scan+2, map, VAL_REMOVE);
                else scan = copy_another_value(scan, map);
                break;

            case '%':
                scan = parse_directive(scan);
                break;

            default:
                die("value must start with quote or '+' or '-'\n\t%s\n", scan);
            }
        } end_while:;
    }

    return bit_num;
}

char *
eat_a_byte(char * scan, int * byte_val)
{
    if (*scan != '\\') {
        *byte_val = (unsigned char)*(scan++);

    } else {
        int chval = '\\';

        switch (*++scan) {
        case NUL: break;
        case '\\':               scan++; break;
        case 'a':  chval = '\a'; scan++; break;
        case 'b':  chval = '\b'; scan++; break;
        case 't':  chval = '\t'; scan++; break;
        case 'n':  chval = '\n'; scan++; break;
        case 'v':  chval = '\v'; scan++; break;
        case 'f':  chval = '\f'; scan++; break;
        case 'r':  chval = '\r'; scan++; break;
        case '"':  chval = '"';  scan++; break;
        case '0' ... '7':
            {
                char octbuf[4];
                octbuf[0] = *(scan++);
                if ((*scan < '0') || (*scan > '7'))
                    octbuf[1] = NUL;
                else {
                    octbuf[1] = *(scan++);
                    if ((*scan < '0') || (*scan > '7'))
                        octbuf[2] = NUL;
                    else {
                        octbuf[2] = *(scan++);
                        octbuf[3] = NUL;
                    }
                }
                chval = (int)strtoul(octbuf, NULL, 8);
                if (chval > 0xFF) {
                    scan -= 2;
                    goto invalid_escape;
                }
                break;
            }

        case 'x': case 'X':
        {
            char hexbuf[4];
            if (! isxdigit(*++scan)) goto invalid_escape;
            hexbuf[0] = *(scan++);
            if (! isxdigit(*scan))
                hexbuf[1] = NUL;
            else {
                hexbuf[1] = *(scan++);
                hexbuf[2] = NUL;
            }
            chval = (int)strtoul(hexbuf, NULL, 16);
            break;
        }

        default:
        invalid_escape:
            die("invalid escape sequence:  %s\n", scan-1);
        }

        *byte_val = chval;
    }

    return scan;
}


char *
scan_quoted_value(char * scan, value_map_t * map, quoted_val_action_t act)
{
    value_bits_t mask = 1 << map->bit_no;
    if (act == VAL_REMOVE)
        mask = ~mask;

    for (;;) {
        int lo_ix, hi_ix;

        switch (*scan) {
            case NUL: return scan;
            case '"': return scan + 1;
            default:  break;
        }
        scan = eat_a_byte(scan, &lo_ix);
        if ((*scan == '-') && (scan[1] != NUL) && (scan[1] != '"')) {
            scan = eat_a_byte(scan + 1, &hi_ix);
        } else {
            hi_ix = lo_ix;
        }

        switch (act) {
        case VAL_ADD:
            do {
                all_map.values[lo_ix] |= (map->values[lo_ix] = mask);
            } while (++lo_ix <= hi_ix);
            break;

        case VAL_REMOVE:
            map->values[lo_ix] = 0;
            do {
                all_map.values[lo_ix] &= mask;
            } while (++lo_ix <= hi_ix);
        }
    }
}

char *
trim(char * buf)
{
    while (isspace(*buf))  buf++;

    {
        char * pe = buf + strlen(buf);
        while ((pe > buf) && isspace(pe[-1]))  pe--;
        *pe = NUL;
    }

    switch (*buf) {
    case NUL:
    case '#':
        return NULL;
    default:
        return buf;
    }
}

char *
get_name(char * scan, char * nm_buf, size_t buf_size)
{
    char * buf_end = nm_buf + buf_size - 2;
    size_t nm_len = 0;

    while (isalnum(*scan) || (*scan == '_') || (*scan == '-')) {
        int ch = *(scan++);
        if (ch == '-')
            ch = '_';
        else if (islower(ch))
            ch = toupper(ch);

        *(nm_buf++) = ch;
        nm_len++;

        if (nm_buf >= buf_end)
            nm_buf--;
    }
    if (scan == nm_buf)
        die("input line does not start with a name:\n\t%s\n", nm_buf);

    while (isspace(*scan)) scan++;

    *nm_buf = NUL;
    curr_name_len = nm_len;

    return scan;
}

value_map_t *
new_map(char * name)
{
    value_map_t * map = malloc(sizeof(*map));
    memset(map, 0, sizeof(*map));
    *end_map = map;
    end_map  = &(map->next);
    strcpy(map->vname, name);
    map->bit_no = ~0;
    total_map_ct++;
    return map;
}

char *
copy_another_value(char * scan, value_map_t * map)
{
    int  add_in = (*scan == '+');
    char z[CLASS_NAME_LIMIT+1];
    value_map_t * mp = all_map.next;
    scan = get_name(scan+1, z, sizeof(z));

    for (;;mp = mp->next) {
        if (mp == NULL)
            die("No entry named %s\n", z);

        if (strcmp(z, mp->vname) == 0)
            break;
    }

    if (add_in) {
        map->mask |= mp->mask;
    }

    else if (map->bit_no == ~0)
        die("You cannot remove bits that are not there.\n");

    else {
        value_bits_t   mask = ~(1 << map->bit_no);
        value_bits_t   skip = mp->mask;
        value_bits_t * this = map->values;
        value_bits_t * all  = all_map.values;

        int ct = sizeof(mp->values) / sizeof(mp->values[0]);

        do  {
            /*
             *  We are negating, so if the named value has this bit set,
             *  turn off the current bit in the global mask and turn off
             *  all bits in the for-this-named-value array of masks.
             */
            if ((*all & skip) != 0) {
                *all &= mask;
                *this = 0;
            }

            this++; all++;
        } while (--ct > 0);
    }

    return scan;
}

static void
make_define_name(char * ptr)
{
    while (*ptr) {
        if (islower(*ptr))
            *ptr = toupper(*ptr);
        else if (! isalnum(*ptr))
            *ptr = '_';

        ptr++;
    }
}

char *
parse_directive(char * scan)
{
    while (isspace(*++scan))
        /* skip leading '%' and white space */;

    if (! isalpha(*scan))
        die("directives must begin with an alphabetic character:  %s\n", scan);

    return handle_cm_opt_disp(scan);
}

static char *
add_text(char ** buff, size_t * sz, char * curr, char * add, int pfx)
{
    size_t len = (add == NULL) ? 0 : strlen(add);
    char * e = *buff + *sz - 4;

    if (*buff + len >= e) {
        size_t o = curr - *buff;
        *sz += BUF_SIZE;
        *buff = realloc(*buff, *sz);
        curr  = *buff + o;
    }

    if (pfx)
        *(curr++) = ' ', *(curr++) = '*';

    if (len == 0) {
        *(curr++) = '\n';

    } else {
        if (pfx)
            *(curr++) = ' ', *(curr++) = ' ';

        memcpy(curr, add, len);
        curr += len;
    }

    return curr;
}

static char *
load_text(int pfx)
{
    char * scan_in;
    char * com_buf      = malloc(BUF_SIZE);
    char * scan_out     = com_buf;
    size_t com_buf_size = BUF_SIZE;
    int    line_ct      = 0;
    int    blank_ct     = 0;

    for (;;) {
        if (fgets(buffer, BUF_SIZE, stdin) == NULL)
            die("incomplete comment section");

        scan_in = buffer;
        while (isspace(*scan_in)) scan_in++;

        /*
         *  if scan is NULL, we've got a comment
         */
        if (*scan_in == NUL) {
            blank_ct++;
            continue;
        }

        if (*scan_in == '%')
            break;

        if (line_ct++ == 0) {
            blank_ct = 0;

        } else while (blank_ct > 0) {
                scan_out =
                    add_text(&com_buf, &com_buf_size, scan_out, NULL, pfx);
                blank_ct--;
                line_ct++;
            }

        scan_out = add_text(&com_buf, &com_buf_size, scan_out, scan_in, pfx);
    }

    *scan_out  = NUL;
    return com_buf;
}

/* * * * * * * * * * * * * * * *

   The following handler functions document and define the embedded
   directives.  The first line of the comment must be exactly
   "handle" + one space + directive name + one space + "directive".
   Dispatch tables and documentation is derived from this.

 * * * * * * * * * * * * * * * */

/**
 * handle file directive.
 *
 * Specifies the output file name.  The multi-inclusion guard is derived
 * from this name.  If %file is not specified, that guard defaults to
 * CHAR_MAPPER_H_GUARD.  The default output is to stdout.
 *
 * @param scan current scan point
 * @returns    end of guard scan
 */
char *
handle_file(cm_opt_enum_t id, char const * txt)
{
    char * scan = (char *)(unsigned long)txt;
    char * pz;
    size_t fname_len;

    if (! isspace(*scan)) die(bad_directive, scan-4);

    while (isspace(*scan))   scan++;

    if (freopen(scan, "w", stdout) != stdout)
        die("fs error %d (%s) reopening %s as stdout\n", errno,
            strerror(errno), scan);

    fname_len = strlen(scan);
    file_guard = pz = malloc(fname_len + guard_fmt_LEN);
    sprintf(pz, guard_fmt, scan);
    make_define_name(pz);

    out_file_nm = pz = malloc(fname_len + 1);
    strcpy(out_file_nm, scan);

    *scan = NUL;
    return scan;
}

/**
 * handle comment directive.
 *
 * Specifies the text to insert in a comment block at the head of the output
 * file.  The comment text starts on the next line and ends with the first
 * input line with a percent ('%') in the first column.  The default is:
 * char-mapper Character Classifications
 *
 * @param scan current scan point
 * @returns    end of guard scan
 */
char *
handle_comment(cm_opt_enum_t id, char const * txt)
{
    char * scan = (char *)(unsigned long)txt;
    commentary = load_text(1);
    *scan = NUL;
    return scan;
}

/**
 * handle emit directive.
 *
 * Collect text to be emitted at the end of the header.  Like "%comment",
 * the block of text also ends with a line with a '%' character in column 1.
 *
 * @param scan current scan point
 * @returns    The '%' starting the next directive.
 */
char *
handle_emit(cm_opt_enum_t id, char const * txt)
{
    char * scan = (char *)(unsigned long)txt;
    add_on_text = load_text(0);
    *scan = NUL;
    return scan;
}

/**
 * handle table directive.
 *
 * Specifies the name of the output table.  If not specified, it defaults to
 * the base file name, suffixed with "_table" and "char_type_table" if %file
 * is not specified.  Normally, this table is "extern" in scope, but may be
 * made static by specifying an empty %guard.
 *
 * @param scan current scan point
 * @returns    end of guard scan
 */
char *
handle_table(cm_opt_enum_t id, char const * txt)
{
    char * scan = (char *)(unsigned long)txt;
    char * pz;

    if (! isspace(*scan)) die(bad_directive, scan-5);

    while (isspace(*scan))   scan++;
    table_name = pz = strdup(scan);
    for (;*pz; pz++) {
        if (! isalnum((int)*pz))
            *pz = '_';
    }

    return scan + strlen(scan);
}

/**
 * handle optimize directive.
 *
 * Specifies setting up optimization tables for span/break scanning operations
 * on character classes.  A static array of pointers is defined and populated
 * on the first use of a scan/break on each character classification.
 *
 * @param scan current scan point
 * @returns    end of guard scan
 */
char *
handle_optimize(cm_opt_enum_t id, char const * txt)
{
    char * scan = (char *)(unsigned long)txt;
    optimize_code = true;
    return scan + strlen(scan);
}

/**
 * handle pthread directive.
 *
 * Specifies pthread locking on allocation of optimization maps.  This has no
 * effect unless %optimize has been specified.  Pthread-ed multi-threaded
 * applications *must* specify this if optimization maps are emitted.
 * Spanning maps get allocated dynamically when first used.  Only one thread
 * should do the work.
 *
 * @param scan current scan point
 * @returns    end of guard scan
 */
char *
handle_pthread(cm_opt_enum_t id, char const * txt)
{
    char * scan = (char *)(unsigned long)txt;
    pthread_code = true;
    return scan + strlen(scan);
}

/**
 * handle guard directive.
 *
 * Specifies the name of a '#ifdef' guard protecting the compilation of
 * the bit mask table.  One compilation unit must '#define' this name.
 * The default is the table name surrounded by "DEFINE_" and "_TABLE".
 * If empty, the output table is unguarded and made static in scope.
 *
 * @param scan current scan point
 * @returns    end of guard scan
 */
char *
handle_guard(cm_opt_enum_t id, char const * txt)
{
    char * scan = (char *)(unsigned long)txt;
    char * pz;

    if (! isspace(*scan)) {
        table_is_static = true;
        return scan;
    }

    while (isspace(*scan))   scan++;
    data_guard = pz = strdup(scan);
    make_define_name(pz);
    return scan + strlen(scan);
}

/**
 * handle backup directive.
 *
 * Specifies emitting code to backup over matching text at the end of a string.
 * The macro takes two arguments:  a start point and an end point.
 * The "end point" is returned after backing up over skipped characters,
 * but it will be at or after the start pointer.  The second pointer may also
 * be NULL or set to the start pointer.  In that case, it is advanced to the
 * next NUL byte at or after where the start pointer points.
 *
 * @param scan current scan point
 * @returns    end of guard scan
 */
char *
handle_backup(cm_opt_enum_t id, char const * txt)
{
    char * scan = (char *)(unsigned long)txt;
    add_backup_code = true;
    return scan + strlen(scan);
}

/**
 * handle test directive.
 *
 * Specifies that a main procedure is to be emitted under an ifdef guard.
 * The first few lines of the output can also be processed by a Bourne
 * shell to compile and test that main procedure.  That shell script is
 * invisible to the C compiler.  It is under a "#if 0" guard.
 *
 * @param scan current scan point
 * @returns    end of guard scan
 */
char *
handle_test(cm_opt_enum_t id, char const * txt)
{
    char * scan = (char *)(unsigned long)txt;
    add_test_code = true;
    return scan + strlen(scan);
}

/**
 * handle invalid directive.
 *
 * This function gets called when input is messed up.  This function does not
 * return.
 *
 * @param scan current scan point
 * @returns    not
 */
char *
handle_invalid(cm_opt_enum_t id, char const * txt)
{
    char * scan = (char *)(unsigned long)txt;
    die(bad_directive, scan);
}
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * indent-tabs-mode: nil
 * End:
 * end of char-mapper.c */
