#include <stdio.h>
#include <stdarg.h>
#include "logger.h"

static LogLevel log_level = INFO;

LogLevel set_log_level(LogLevel level) {
    LogLevel old_log_level = log_level;
    log_level = level;
    return old_log_level;
}

void log(LogLevel level, const char *format, ...) {
    if (level <= log_level) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}

void error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log(ERROR, format, args);
    va_end(args);
}

void warning(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log(WARNING, format, args);
    va_end(args);
}

void info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log(INFO, format, args);
    va_end(args);
}

void trace(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log(TRACE, format, args);
    va_end(args);
}

void debug(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log(DEBUG, format, args);
    va_end(args);
}
