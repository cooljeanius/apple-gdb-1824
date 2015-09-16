#! /bin/bash
#  mk-opt-table.sh
#
#  This file is part of char-mapper.
#  char-mapper Copyright (C) 2003-2013 Bruce Korb - all rights reserved
#
# char-mapper is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# char-mapper is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program.  If not, see <http://www.gnu.org/licenses/>.

declare -r progdir=$(\cd $(dirname $0) && pwd -P)
declare -r prognam=$(basename $0)
declare -r program=${progdir}/${prognam}

die() {
    echo "mk-opt-table error:  $*"
    rm -f ${temp_stamp}
    exit 1
} >&2

init() {
    base_name=${1%%.*}
    exe_name=char-mapper
    set -e

    declare -g hdl_list=$(
        egrep '^handle_[a-z_]+\(' ${exe_name}.c | \
            sed '/^handle_invalid(/d;s/^handle_//;s/(.*//' | \
            sort)

    declare -g new_source=false
}

is_up_to_date() {
    declare svnew=${new_source}
    declare base=$1 ; shift
    new_source=true

    test -f ${base}.c -a -f ${base}.h || return 1

    for f
    do
        test ${base}.c -nt ${f} -a ${base}.h -nt ${f} || return 1
    done

    new_source=${svnew}
    return 0
}

mk_enum() {
    autogen -L ../../autoopts/tpl <<- _EOF_
	AutoGen Definitions str2enum;

	base-name = ${base_name};
	length    = none;
	no-case; no-name;
	dispatch = {
	    d-ret = 'char *';
	    d-nam = 'handle_%s';
	};
	cmd    = $(echo $hdl_list | sed 's/ /, /g');
	_EOF_
}

assemble_usage() {
    cat <<- _EOF_
	str = <<_EOUsage_
	USAGE:  ${exe_name} [ -h | --help | <input-file> ]
	If the '<input-file>' is not specified, it is read from standard input.
	Input may not be from a TTY device.  Various directives affecting the
	output are embedded in the input text.  These are:

	_EOF_

    for d in $hdl_list
    do
        printf '  %s' "%$d"
        x="\\* *handle $d directive"
        sed -n "/$x/,/@param/{
		/$x/d"$'
		/@param/d
		s/^ *\* *//
		s/^/\t/
		p
	}' ${exe_name}.c | \
            fmt
    done

    cat <<- _EOF_
	Otherwise, input lines that are blank or start with a hash ('#') are
	ignored.  All other lines must conform to the following syntax:

	    <name> { <literal-set> | <existing-name> } \\
	        [-]{<literal-set> | <existing-name>}...

	<name> must begin with an alphabetic character and consist of
	alphanumerics, plus the hyphen and underscore.

	<literal-set> is a double quoted string (not apostrophe quoted) and
	it gets cooked first.  "Cooking" consists of escape processing
	(hex "\x00", plus the traditional "\n\f\v\t\b" escapes) and
	hyphenated range expansion.  New fangled POSIX character class names
	are not expanded.

	A hyphen will cause the characters selected to be removed from any
	literal sets applied previously.  So:
	    foo " -~" -alphanumeric
	represents all characters from space through tilde, except for
	alphanumerics.

	If the input file can be rewound and re-read, then the input text will
	also be inserted into the output as a comment.

	_EOUsage_;
	_EOF_
}

mk_text_def() {
    is_up_to_date map-text ${program} && return 0

    echo "updating map-text.[ch]"
    declare ag=$(command -v autogen)
    test -x "$ag" || {
        test -f map-text.c -a -f map-text.h || \
            die "map-text.c and .h are missing and cannot be rebuilt"
        echo "warning: map-text.c and .h are out of date and cannot be rebuilt"
        touch map-text.c map-text.h
        return 0
    } 1>&2

    # pull the name of the included file from the def file and then
    # put our assembled text into that file.
    #
    declare tmp_text=$(awk '/^#include/{print $2}' map-text.def)
    assemble_usage > $tmp_text
    ${ag} map-text.def
    rm -f $tmp_text
}

trap "die failure" EXIT
stamp_file=${1}
temp_stamp=${1}-temp
date > ${temp_stamp}

init "$1"
mk_enum
mk_text_def

if ${new_source}
then mv -f ${temp_stamp} ${stamp_file}
else rm -f ${temp_stamp}
     echo no changes to generated source
fi

trap '' EXIT
exit 0
