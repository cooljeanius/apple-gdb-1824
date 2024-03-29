#! /bin/sh

# Copyright (C) 2011-2014 Free Software Foundation, Inc.
#
# This file is part of GDB.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Usage: update-gnulib.sh <path-to-gnulib-repository>
# Update our import of gnulib in the GDB source tree.
#
# This script assumes that it is being called from the gdb/gnulib
# subdirectory, and will verify this before proceeding.
#
# This script will also make a number of other verifications:
#   . The gnulib version (it should match $GNULIB_COMMIT_SHA1).
#   . The correct versions of the auto-tools that are used to
#     regenerate the various scripts and Makefiles are on the PATH.

# The list of gnulib modules we are importing in GDB.
IMPORTED_GNULIB_MODULES="\
    absolute-header alignof alloca alloca-opt ansi-c++-opt assert assert-h \
    assure autobuild \
    bcopy bison-i18n bitset btowc builtin-expect \
    c-ctype c-strcase c-strcasestr c99 calloc-posix chdir-long configmake \
    connect closedir crc ctime \
    dirent dirent-safer dirfd double-slash-root dup2 \
    environ errno error euidaccess exitfail extensions extern-inline \
    faccessat fclose fcntl fcntl-h fcntl-safer fflush fileblocks filename \
    flexmember float fnmatch fnmatch-gnu fnmatch-h fopen fpieee fprintf-posix \
    fpucw frexp frexpl fstatat fts \
    getcwd getcwd-lgpl getpagesize gettext-h gettimeofday git-version-gen \
    gitlog-to-changelog gnu-make gpl-2.0 \
    havelib host-cpu-c-abi host-os \
    ignore-value include_next inline intprops inttypes inttypes-incomplete \
    isnand-nolibm isnanl-nolibm iswctype \
    largefile ldd ldexpl limits-h localcharset locale localtime \
    localtime longlong lstat \
    malloc-gnu malloc-posix manywarnings math mbrtowc mbschr mbsinit mbsrchr \
    mbsrtowcs mbsspn memchr memcmp memmem memmem-simple \
    mempcpy memrchr mkdir mkdtemp multiarch \
    nextafter no-c++ nocrash noreturn \
    obstack opendirat openmp \
    pathmax pclose perror pipe-posix popen posixcheck putenv \
    readdir readlink realloc-gnu realloc-posix recv regex regex-quote \
    regexprops-generic rmdir \
    secure_getenv send setenv sh-filename sig2str sigaction signal signal-h \
    sigpipe sigpipe-die sigprocmask snippet/_Noreturn snippet/arg-nonnull \
    snippet/c++defs snippet/link-warning snippet/warn-on-use snprintf \
    sprintf-posix ssize_t stat stat-macros stat-size stat-time stdbool stddef \
    stdint stdlib streq strerror strerror_r-posix strerror-override \
    strftime-fixes string strncat strndup strnlen strnlen1 strstr \
    strstr-simple strtok_r sys_select sys_stat sys_time sys_types sys_wait \
    tempname time time_r time_rz timegm tzset \
    unistd unistd-safer unlink unlink-busy unsetenv update-copyright usleep \
    vc-list-files verify vfprintf-posix \
    warnings wchar wcscat wcslen wcsncasecmp wctype wctype-h winsz-ioctl \
    winsz-termios wmemchr wmemcpy \
    xalloc xalloc-die xalloc-oversized xmalloca xmemdup0 xstrndup"
# (might want to check to see if any of the libiberty/gettext duplicates cause
# any conflicts...)

# Obsolete modules I am tempted to add, but am going to omit for now:
# - atexit
# - dup2-obsolete
# - memchr-obsolete
# - memmove
# - strdup (or strdup-posix)
# (even though I cannot add them, there is no need to explicitly ignore them
# below, though, so they may still get dragged in as dependencies)
# Reasons for explicitly ignoring some:
# - setlocale: depends on localename, which depends on lock, which is
#   explicitly ignored.

# The gnulib commit ID to use for the update.
GNULIB_COMMIT_SHA1="4ec032e7c67deccd194f0a230cfaffd25c706b08"
# (feel free to update if you know that your version works and is newer)
# (last updated November 14, 2023)

# The expected version number for the various auto tools we will
# use after the import.
AUTOCONF_VERSION="2.64"
AUTOMAKE_VERSION="1.11.6"
ACLOCAL_VERSION="$AUTOMAKE_VERSION"

if [ $# -ne 1 ]; then
   echo "Error: Path to gnulib repository missing. Aborting."
   echo "Usage: update-gnulib.sh <path-to-gnulib-repository>"
   exit 1
fi
gnulib_prefix=$1

gnulib_tool="${gnulib_prefix}/gnulib-tool"

# Verify that the gnulib directory does exist...
if [ ! -f "${gnulib_tool}" ]; then
   echo "Error: Invalid gnulib directory. Cannot find gnulib tool"
   echo "       (${gnulib_tool})."
   echo "Aborting."
   exit 1
else
   echo "found ${gnulib_tool}"
fi

# Verify that we have the right version of gnulib...
gnulib_head_sha1=$(cd "${gnulib_prefix}" && git rev-parse HEAD)
if [ "${gnulib_head_sha1}" != "${GNULIB_COMMIT_SHA1}" ]; then
   echo "Error: Wrong version of gnulib: ${gnulib_head_sha1}"
   echo "       (we expected it to be ${GNULIB_COMMIT_SHA1})"
   echo "Aborting."
   exit 1
else
   echo "using gnulib commit ${gnulib_head_sha1}"
fi

# Verify that we are in the gdb/gnulib/ subdirectory.
if [ ! -f ../main.c -o ! -d import ]; then
   echo "Error: This script should be called from the gdb/gnulib subdirectory."
   echo "Aborting."
   exit 1
fi

# (autotools checks moved to after the import; auto-regenerating with this
# script is not strictly necessary)

echo "actually importing now; this may take a while..."
# Update our gnulib import.
"${gnulib_prefix}"/gnulib-tool --import --dir=. --lib=libgnu \
  --source-base=import --m4-base=import/m4 --doc-base=doc \
  --tests-base=tests --aux-dir=import/extra \
  --avoid=localename --avoid=lock --avoid=msvc-nothrow --avoid=setlocale \
  --avoid=threadlib --no-conditional-dependencies --no-libtool \
  --macro-prefix=gl --no-vc-files --with-obsolete \
  "${IMPORTED_GNULIB_MODULES}"
if [ $? -ne 0 ]; then
   echo "Error: gnulib import failed.  Aborting."
   exit 1
fi

# Verify that we have the correct version of autoconf.
ver=$(autoconf --version 2>&1 | head -1 | sed 's/.*) //')
if [ "${ver}" != "${AUTOCONF_VERSION}" ]; then
   echo "Error: Wrong autoconf version: ${ver}. Aborting."
   exit 1
fi

# Verify that we have the correct version of automake.
ver=$(automake --version 2>&1 | head -1 | sed 's/.*) //')
if [ "${ver}" != "${AUTOMAKE_VERSION}" ]; then
   echo "Error: Wrong automake version (${ver}), we need ${AUTOMAKE_VERSION}."
   echo "Aborting."
   exit 1
fi

# Verify that we have the correct version of aclocal.
ver=$(aclocal --version 2>&1 | head -1 | sed 's/.*) //')
if [ "${ver}" != "${ACLOCAL_VERSION}" ]; then
   echo "Error: Wrong aclocal version: ${ver}. Aborting."
   exit 1
fi

# Regenerate all necessary files...
aclocal -Iimport/m4 -Im4 --install --warnings=all &&
autoconf --warnings=all &&
autoheader --warnings=all &&
automake --add-missing --copy
if [ $? -ne 0 ]; then
   echo "Error: Failed to regenerate Makefiles and configure scripts."
   exit 1
fi

# EOF
