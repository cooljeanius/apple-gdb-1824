#!/bin/ksh

export CFLAGS='-Os -pipe -fPIC -DUSE_MMAP -dead_strip -DVEC_OPTIMIZE'
export ZLIB_SRCDIR=../..

set -ex

gcc-4.2 -arch i386 -arch x86_64 ${CFLAGS} -c -o adler32.o ${ZLIB_SRCDIR}/adler32.c
gcc-4.2 -arch i386 -arch x86_64 ${CFLAGS} -c -o compress.o ${ZLIB_SRCDIR}/compress.c
gcc-4.2 -arch i386 -arch x86_64 ${CFLAGS} -c -o crc32.o ${ZLIB_SRCDIR}/crc32.c
gcc-4.2 -arch i386 -arch x86_64 ${CFLAGS} -c -o deflate.o ${ZLIB_SRCDIR}/deflate.c
gcc-4.2 -arch i386 -arch x86_64 ${CFLAGS} -c -o gzclose.o ${ZLIB_SRCDIR}/gzclose.c
gcc-4.2 -arch i386 -arch x86_64 ${CFLAGS} -c -o gzlib.o ${ZLIB_SRCDIR}/gzlib.c
gcc-4.2 -arch i386 -arch x86_64 ${CFLAGS} -c -o gzread.o ${ZLIB_SRCDIR}/gzread.c
gcc-4.2 -arch i386 -arch x86_64 ${CFLAGS} -c -o gzwrite.o ${ZLIB_SRCDIR}/gzwrite.c
gcc-4.2 -arch i386 -arch x86_64 ${CFLAGS} -c -o infback.o ${ZLIB_SRCDIR}/infback.c
if test -z "${__DoAppleInternalLogging__}"; then
  gcc-4.2 -arch i386 -arch x86_64 ${CFLAGS} -c -o inffast.o ${ZLIB_SRCDIR}/inffast.c
fi
gcc-4.2 -arch i386 -arch x86_64 ${CFLAGS} -c -o inflate.o ${ZLIB_SRCDIR}/inflate.c
gcc-4.2 -arch i386 -arch x86_64 ${CFLAGS} -c -o inftrees.o ${ZLIB_SRCDIR}/inftrees.c
gcc-4.2 -arch i386 -arch x86_64 ${CFLAGS} -c -o trees.o ${ZLIB_SRCDIR}/trees.c
gcc-4.2 -arch i386 -arch x86_64 ${CFLAGS} -c -o uncompr.o ${ZLIB_SRCDIR}/uncompr.c
gcc-4.2 -arch i386 -arch x86_64 ${CFLAGS} -c -o zutil.o ${ZLIB_SRCDIR}/zutil.c

gcc-4.2 -arch i386 -arch x86_64 ${CFLAGS} -c -o adler32vec.o ${ZLIB_SRCDIR}/intel/adler32vec.s
gcc-4.2 -arch i386 -arch x86_64 ${CFLAGS} -c -o inffast.o ${ZLIB_SRCDIR}/intel/inffast.s

gcc-4.2 -Wmost -pedantic -MMD  -O3 -g -std=c99   -arch x86_64 -msse3  -c -o ClockServices.64.o ClockServices.c
gcc-4.2 -Wmost -pedantic -MMD  -O3 -g -std=c99   -arch i386 -msse3  -c -o ClockServices.32.o ClockServices.c

lipo -create -o ClockServices.o ClockServices.64.o ClockServices.32.o 

rm ClockServices.64.o ClockServices.32.o ClockServices.64.d ClockServices.32.d

gcc-4.2 -arch i386 -arch x86_64 ${CFLAGS} compress.c ./*.o -o compress
gcc-4.2 -arch i386 -arch x86_64 ${CFLAGS} decompress.c ./*.o -o decompress 

rm -f ./*.o




