#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <libcraft/log.h>

int current_level = LOG_INFO;

void log_header(int level);

void log_write(int level, const char *message) {
    if (level >= LOG_MINIMUM && level <= LOG_MAXIMUM && message) {
        if (level <= current_level) {
            log_header(level);
            puts(message);
        }
    } else {
        errno = EINVAL;
    }
}

void log_fmt(int level, const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    log_vfmt(level, format, arg);
    va_end(arg);
}

void log_vfmt(int level, const char *format, va_list arg) {
    if (level >= LOG_MINIMUM && level <= LOG_MAXIMUM && format) {
        if (level <= current_level) {
            log_header(level);
            vprintf(format, arg);
        }
    } else {
        errno = EINVAL;
    }
}

void log_header(int level) {
    const char *level_name;
    switch (level) {
        case LOG_DEBUG:
            level_name = "DEBUG";
            break;
        case LOG_INFO:
            level_name = "INFO ";
            break;
        case LOG_WARN:
            level_name = "WARN ";
            break;
        case LOG_ERROR:
            level_name = "ERROR";
            break;
        case LOG_CRASH:
            level_name = "CRASH";
            break;
        default:
            level_name = " ??? ";
            break;
    }
    time_t raw = time(NULL);
    struct tm *tm = localtime(&raw);
    printf("[%02d:%02d:%02d] [%s] ", tm->tm_hour, tm->tm_min, tm->tm_sec, level_name);
}

int log_level(int level) {
    if (level != LOG_NO_SET) {
        if (level < LOG_MINIMUM || level > LOG_MAXIMUM) {
            return ~(errno = EINVAL);
        } else {
            return current_level = level;
        }
    } else {
        return current_level;
    }
}

#ifdef LOG_ERRNO_SET
int log_errno(int no, const char *message) {
    log_header(LOG_ERROR);
    puts(message);
    return ~(errno = no);
}
#endif
