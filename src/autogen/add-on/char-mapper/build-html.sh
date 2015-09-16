#! /bin/bash

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

cd ${addon_dir}/${dir}

case "${1}" in
( description )
    title='Create customized character classifications'
    text=$(cat <<- _EOF_
        This program will take a list of character classes and convert them
	into bit map tests.
	_EOF_
        )
    printf '<h3><a href="#%s">%s</a></h3><p>%s</p>' ${dir} "${title}" "${text}"
    return 0
    ;;

( files )
    primary_source=$(
        test -f .sdir && cd $(<.sdir) >/dev/null
        git ls-files . | \
        grep -E -v '^(.gitignore|test.sh|README|build-html.sh)$')
    example_source=test.sh
    {
        make
        KEEP_TEST_RESULTS=true make check
    } >/dev/null 2>&1

    example_output=$(echo test-cmap.map test-cmap.h)
    text=$(sed '1,/^PURPOSE:/d;/^[A-Z ]\+:/Q' README)
    ;;
esac
