AC_INIT([libcraft], [1.0])
AM_INIT_AUTOMAKE([subdir-objects])
LT_INIT

AC_CONFIG_MACRO_DIRS([m4])

AC_CHECK_TYPE([unsigned __int128], [AC_DEFINE(HAVE_UINT_128)], [])
AC_CHECK_TYPE([uint64], [AC_DEFINE(HAVE_UINT_64)], [], [#include <stdint.h>])
AC_CHECK_LIB([pthread], [pthread_create])

AC_DEFINE([LIBCRAFT_SOURCE])

AC_PROG_CC

AC_OUTPUT([Makefile libcraft/Makefile server/Makefile])
