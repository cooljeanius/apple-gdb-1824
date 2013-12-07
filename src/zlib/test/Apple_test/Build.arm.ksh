#!/bin/ksh

export CFLAGS='-std=gnu99 -Os -g -pipe -fPIC -DUSE_MMAP -DVEC_OPTIMIZE'
export CCC='xcrun -sdk iphoneos.internal -run gcc -isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS5.0.Internal.sdk'
export ZLIB_SRCDIR=../..
if test ! -z "$(xcodebuild -showsdks 2>/dev/null | grep internal)"; then
  export IPHONEOS_INTERNAL_SDK_PATH="$(xcodebuild -version -sdk iphoneos.internal Path)"
else
  # feel free to change the "6.0" here to whichever version you have
  export IPHONEOS_INTERNAL_SDK_PATH="$(xcodebuild -version -sdk iphoneos6.0 Path)"
fi

set -ex 

xcrun -sdk iphoneos6.0 -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -arch armv7 ${CFLAGS}  -o decompress decompress.c ${ZLIB_SRCDIR}/adler32.c ${ZLIB_SRCDIR}/crc32.c ${ZLIB_SRCDIR}/gzclose.c ${ZLIB_SRCDIR}/gzlib.c ${ZLIB_SRCDIR}/gzread.c ${ZLIB_SRCDIR}/gzwrite.c ${ZLIB_SRCDIR}/deflate.c ${ZLIB_SRCDIR}/compress.c ${ZLIB_SRCDIR}/infback.c ${ZLIB_SRCDIR}/inflate.c ${ZLIB_SRCDIR}/inftrees.c ${ZLIB_SRCDIR}/trees.c ${ZLIB_SRCDIR}/uncompr.c ${ZLIB_SRCDIR}/zutil.c ${ZLIB_SRCDIR}/arm/inffast.s ${ZLIB_SRCDIR}/arm/adler32vec.s ClockServices.c

xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -arch armv6 -arch armv7 ${CFLAGS} -c -o adler32.o ${ZLIB_SRCDIR}/adler32.c
xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -arch armv6 -arch armv7 ${CFLAGS} -c -o compress.o ${ZLIB_SRCDIR}/compress.c
xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -arch armv6 -arch armv7 ${CFLAGS} -c -o crc32.o ${ZLIB_SRCDIR}/crc32.c
xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -arch armv6 -arch armv7 ${CFLAGS} -c -o deflate.o ${ZLIB_SRCDIR}/deflate.c
xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -arch armv6 -arch armv7 ${CFLAGS} -c -o gzclose.o ${ZLIB_SRCDIR}/gzclose.c
xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -arch armv6 -arch armv7 ${CFLAGS} -c -o gzlib.o ${ZLIB_SRCDIR}/gzlib.c
xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -arch armv6 -arch armv7 ${CFLAGS} -c -o gzread.o ${ZLIB_SRCDIR}/gzread.c
xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -arch armv6 -arch armv7 ${CFLAGS} -c -o gzwrite.o ${ZLIB_SRCDIR}/gzwrite.c
xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -arch armv6 -arch armv7 ${CFLAGS} -c -o infback.o ${ZLIB_SRCDIR}/infback.c
xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -arch armv6 -arch armv7 ${CFLAGS} -c -o inflate.o ${ZLIB_SRCDIR}/inflate.c
xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -arch armv6 -arch armv7 ${CFLAGS} -c -o inftrees.o ${ZLIB_SRCDIR}/inftrees.c
xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -arch armv6 -arch armv7 ${CFLAGS} -c -o trees.o ${ZLIB_SRCDIR}/trees.c
xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -arch armv6 -arch armv7 ${CFLAGS} -c -o uncompr.o ${ZLIB_SRCDIR}/uncompr.c
xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -arch armv6 -arch armv7 ${CFLAGS} -c -o zutil.o ${ZLIB_SRCDIR}/zutil.c

if test -z "${__DoAppleInternalLogging__}"; then
  xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -arch armv6 -arch armv7 ${CFLAGS} -c -o inflateS.o ${ZLIB_SRCDIR}/arm/inflateS.s
fi
xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -arch armv6 -arch armv7 ${CFLAGS} -c -o adler32vec.o ${ZLIB_SRCDIR}/arm/adler32vec.s
xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -arch armv6 -arch armv7 ${CFLAGS} -c -o inffast.o ${ZLIB_SRCDIR}/arm/inffast.s

xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -Wmost -pedantic -MMD  -O3 -g -std=c99 -mfpu=neon  -arch armv7 -c -o ClockServices.64.o ClockServices.c
xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -Wmost -pedantic -MMD  -O3 -g -std=c99   -arch armv6 -c -o ClockServices.32.o ClockServices.c

lipo -create -o ClockServices.o ClockServices.64.o ClockServices.32.o 

rm ClockServices.64.o ClockServices.32.o ClockServices.64.d ClockServices.32.d

xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -arch armv6 -arch armv7 ${CFLAGS} compress.c ./*.o -o compress
if test -z "${__DoAppleInternalLogging__}"; then
  xcrun -sdk iphoneos.internal -run gcc -isysroot "${IPHONEOS_INTERNAL_SDK_PATH}" -arch armv6 -arch armv7 ${CFLAGS} -g decompress.c ./*.o -o decompress
fi

rm -f ./*.o
