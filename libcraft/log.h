#ifndef LIBCRAFT_LOG_H
#define LIBCRAFT_LOG_H
#include <errno.h>
#include <stdarg.h>
#include <libcraft/ids.h>

#define LOG_NO_SET 0
#define LOG_DEBUG 5
#define LOG_INFO 4
#define LOG_WARN 3
#define LOG_ERROR 2
#define LOG_CRASH 1
#define LOG_MINIMUM LOG_CRASH
#define LOG_MAXIMUM LOG_DEBUG

void log_write(int level, const char *message);

void log_fmt(int level, const char *format, ...);

void log_vfmt(int level, const char *format, va_list arg);

int log_level(int level);

#define log_debug(message) log_write(LOG_DEBUG, message)
#define log_debug_fmt(format, ...) log_fmt(LOG_DEBUG, format, __VA_ARGS__)
#define log_debug_vfmt(level, format, arg) log_vfmt(LOG_DEBUG, level, format, arg)

#define log_info(message) log_write(LOG_INFO, message)
#define log_info_fmt(format, ...) log_fmt(LOG_INFO, format, __VA_ARGS__)
#define log_info_vfmt(level, format, arg) log_vfmt(LOG_INFO, level, format, arg)

#define log_warn(message) log_write(LOG_WARN, message)
#define log_warn_fmt(format, ...) log_fmt(LOG_WARN, format, __VA_ARGS__)
#define log_warn_vfmt(level, format, arg) log_vfmt(LOG_WARN, level, format, arg)

#define log_error(message) log_write(LOG_ERROR, message)
#define log_error_fmt(format, ...) log_fmt(LOG_ERROR, format, __VA_ARGS__)
#define log_error_vfmt(level, format, arg) log_vfmt(LOG_ERROR, level, format, arg)

#define log_crash(message) log_write(LOG_CRASH, message)
#define log_crash_fmt(format, ...) log_fmt(LOG_CRASH, format, __VA_ARGS__)
#define log_crash_vfmt(level, format, arg) log_vfmt(LOG_CRASH, level, format, arg)

#ifdef LOG_ERRNO_SET
int log_errno(int no, const char *message);

#define ERROR(no) log_errno(no, "errno = " # no " (" # __FILE__ ":" # __LINE__ ")")
#else
#define ERROR(no) (~(errno = no))
#endif

#define NOT_IMPLEMENTED(func) \
    log_error("Not implemented: " # func); \
    ERROR(ENOTSUP);

#define NOT_IMPLEMENTED_INT(func) \
    NOT_IMPLEMENTED(func) \
    return ~ENOTSUP;

#define NOT_IMPLEMENTED_PTR(func) \
    NOT_IMPLEMENTED(func) \
    return NULL;

#define NOT_IMPLEMENTED_ID(func) \
    NOT_IMPLEMENTED(func) \
    return CRAFT_ID_NULL;

#endif
