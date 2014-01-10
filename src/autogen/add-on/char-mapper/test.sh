#! /bin/sh

## \file map-text.def
##
##  This file is part of char-mapper.
##  Copyright (C) 2005-2013 Bruce Korb - all rights reserved
##
## char-mapper is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by the
## Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## char-mapper is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
## See the GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License along
## with this program.  If not, see <http://www.gnu.org/licenses/>.

case "${VERBOSE}" in
[YytT1-9]* ) set -x ;;
esac
base_name=test-cmap
rm -f ${base_name}*

case "${1}" in
-c | --cl* )
    echo "rm -f ${base_name}*"
    exit 0
    ;;
esac

rm -f test[-_]*
set -e

cat > ${base_name}.map <<- _EOF_
	%file ${base_name}.h
	%test
	%comment
	   Copyright (C) 2000, 2001 Free Software Foundation, Inc.

	This file is part of some library.
	%

	# Basic types:
	#
	cntrl       "\x00-\x1F\x7F"
	digit       "0-9"
	lower       "a-z"
	horiz-space " \t"
	line-end    "\r\n"
        vert-space  "\f\v"
	upper       "A-Z"

	# Compound char types.  Only these may have subtracted names.
	#
	xdigit      "a-fA-F"    +digit
	token-end   "\x00"      +horiz-space +line-end
	name-start  "_"         +upper +lower
	punctuation "\x20-\x7E" -upper -lower -horiz-space

	# pure combined char types:
	#
	space       +horiz-space +line-end +vert-space

	%emit
	#define HOST_CHARSET_UNKNOWN 0
	#define HOST_CHARSET_ASCII   1
	#define HOST_CHARSET_EBCDIC  2

	#if  '\n' == 0x0A && ' ' == 0x20 && '0' == 0x30 \\
	   && 'A' == 0x41 && 'a' == 0x61 && '!' == 0x21
	#  define HOST_CHARSET HOST_CHARSET_ASCII
	#else
	# if '\n' == 0x15 && ' ' == 0x40 && '0' == 0xF0 \\
	   && 'A' == 0xC1 && 'a' == 0x81 && '!' == 0x5A
	#  define HOST_CHARSET HOST_CHARSET_EBCDIC
	# else
	#  define HOST_CHARSET HOST_CHARSET_UNKNOWN
	# endif
	#endif
	%
	_EOF_

./char-mapper ${base_name}.map
bash ./${base_name}.h
sed -i $'/%test/{\na\\\n%pthread\\\n%optimize\n\n}' ${base_name}.map
./char-mapper ${base_name}.map
bash ./${base_name}.h
test "X${KEEP_TEST_RESULTS}" = Xtrue || \
    rm -f ${base_name}*
