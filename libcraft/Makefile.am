AM_CFLAGS = -I..
lib_LTLIBRARIES = libcraft.la
libcraft_la_LIBADD = -lm
libcraft_la_SOURCES = \
    collections/hashmap.c \
    collections/linkedlist.c \
    net/server/platform/posix/server.c \
    threading/platform/platform.h \
    threading/platform/posix.c \
    threading/loop.c \
    threading/task.c \
    ids.c \
    log.c
include_HEADERS = \
    collections/hashmap.h \
    collections/linkedlist.h \
    net/server/server.h \
    threading/loop.h \
    threading/task.h \
    ids.h \
    log.h
