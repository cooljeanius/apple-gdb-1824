#!/bin/sh -ev
mkdir build
cd build
guile18-build ../configure --enable-snprintfv-install -C
make
