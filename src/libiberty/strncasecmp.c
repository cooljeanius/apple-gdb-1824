/* strncasecmp.c
 * Copyright (c) 1987 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and that due credit is given
 * to the University of California at Berkeley. The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific written prior permission. This software
 * is provided ``as is'' without express or implied warranty.
 */

/*

@deftypefn Supplemental int strncasecmp (const char *@var{s1}, const char *@var{s2})

A case-insensitive @code{strncmp}.

@end deftypefn

*/

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)strcasecmp.c	5.5 (Berkeley) 11/24/87";
#endif /* LIBC_SCCS and not lint */

#include <ansidecl.h>
#include <stddef.h>

/*
 * This array is designed for mapping upper and lower case letter
 * together for a case independent comparison.  The mappings are
 * based upon ascii character sequences.
 */
static const unsigned char charmap[] = {
  '\000', '\001', '\002', '\003', '\004', '\005', '\006', '\007',
  '\010', '\011', '\012', '\013', '\014', '\015', '\016', '\017',
  '\020', '\021', '\022', '\023', '\024', '\025', '\026', '\027',
  '\030', '\031', '\032', '\033', '\034', '\035', '\036', '\037',
  '\040', '\041', '\042', '\043', '\044', '\045', '\046', '\047',
  '\050', '\051', '\052', '\053', '\054', '\055', '\056', '\057',
  '\060', '\061', '\062', '\063', '\064', '\065', '\066', '\067',
  '\070', '\071', '\072', '\073', '\074', '\075', '\076', '\077',
  '\100', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
  '\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
  '\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
  '\170', '\171', '\172', '\133', '\134', '\135', '\136', '\137',
  '\140', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
  '\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
  '\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
  '\170', '\171', '\172', '\173', '\174', '\175', '\176', '\177',
  (unsigned char)'\200', (unsigned char)'\201', (unsigned char)'\202',
  (unsigned char)'\203', (unsigned char)'\204', (unsigned char)'\205',
  (unsigned char)'\206', (unsigned char)'\207', (unsigned char)'\210',
  (unsigned char)'\211', (unsigned char)'\212', (unsigned char)'\213',
  (unsigned char)'\214', (unsigned char)'\215', (unsigned char)'\216',
  (unsigned char)'\217', (unsigned char)'\220', (unsigned char)'\221',
  (unsigned char)'\222', (unsigned char)'\223', (unsigned char)'\224',
  (unsigned char)'\225', (unsigned char)'\226', (unsigned char)'\227',
  (unsigned char)'\230', (unsigned char)'\231', (unsigned char)'\232',
  (unsigned char)'\233', (unsigned char)'\234', (unsigned char)'\235',
  (unsigned char)'\236', (unsigned char)'\237', (unsigned char)'\240',
  (unsigned char)'\241', (unsigned char)'\242', (unsigned char)'\243',
  (unsigned char)'\244', (unsigned char)'\245', (unsigned char)'\246',
  (unsigned char)'\247', (unsigned char)'\250', (unsigned char)'\251',
  (unsigned char)'\252', (unsigned char)'\253', (unsigned char)'\254',
  (unsigned char)'\255', (unsigned char)'\256', (unsigned char)'\257',
  (unsigned char)'\260', (unsigned char)'\261', (unsigned char)'\262',
  (unsigned char)'\263', (unsigned char)'\264', (unsigned char)'\265',
  (unsigned char)'\266', (unsigned char)'\267', (unsigned char)'\270',
  (unsigned char)'\271', (unsigned char)'\272', (unsigned char)'\273',
  (unsigned char)'\274', (unsigned char)'\275', (unsigned char)'\276',
  (unsigned char)'\277', (unsigned char)'\300', (unsigned char)'\341',
  (unsigned char)'\342', (unsigned char)'\343', (unsigned char)'\344',
  (unsigned char)'\345', (unsigned char)'\346', (unsigned char)'\347',
  (unsigned char)'\350', (unsigned char)'\351', (unsigned char)'\352',
  (unsigned char)'\353', (unsigned char)'\354', (unsigned char)'\355',
  (unsigned char)'\356', (unsigned char)'\357', (unsigned char)'\360',
  (unsigned char)'\361', (unsigned char)'\362', (unsigned char)'\363',
  (unsigned char)'\364', (unsigned char)'\365', (unsigned char)'\366',
  (unsigned char)'\367', (unsigned char)'\370', (unsigned char)'\371',
  (unsigned char)'\372', (unsigned char)'\333', (unsigned char)'\334',
  (unsigned char)'\335', (unsigned char)'\336', (unsigned char)'\337',
  (unsigned char)'\340', (unsigned char)'\341', (unsigned char)'\342',
  (unsigned char)'\343', (unsigned char)'\344', (unsigned char)'\345',
  (unsigned char)'\346', (unsigned char)'\347', (unsigned char)'\350',
  (unsigned char)'\351', (unsigned char)'\352', (unsigned char)'\353',
  (unsigned char)'\354', (unsigned char)'\355', (unsigned char)'\356',
  (unsigned char)'\357', (unsigned char)'\360', (unsigned char)'\361',
  (unsigned char)'\362', (unsigned char)'\363', (unsigned char)'\364',
  (unsigned char)'\365', (unsigned char)'\366', (unsigned char)'\367',
  (unsigned char)'\370', (unsigned char)'\371', (unsigned char)'\372',
  (unsigned char)'\373', (unsigned char)'\374', (unsigned char)'\375',
  (unsigned char)'\376', (unsigned char)'\377',
};

int
strncasecmp(const char *s1, const char *s2, register size_t n)
{
    register unsigned char u1, u2;

    for (; n != 0; --n) {
	u1 = (unsigned char) *s1++;
	u2 = (unsigned char) *s2++;
	if (charmap[u1] != charmap[u2]) {
	    return charmap[u1] - charmap[u2];
	}
	if (u1 == '\0') {
	    return 0;
	}
    }
    return 0;
}

/* EOF */
