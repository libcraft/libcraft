#!/bin/bash
set -e

libtoolize
aclocal
autoconf
automake --add-missing
./configure $*
