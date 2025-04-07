#include <stdio.h>
#include <stdarg.h>
#include "logger.h"

static LogLevel log_level = INFO;

LogLevel set_log_level(LogLevel level) {
    LogLevel old_log_level = log_level;
    log_level = level;
    return old_log_level;
}

void log_message(LogLevel level, const char *format, ...) {
    if (level <= log_level) {
        va_list args;
        va_start(args, format);
        
        switch (level) {
            case ERROR:   printf("ERROR: "); break;
            case WARNING: printf("WARNING: "); break;
            case INFO:    printf("INFO: "); break;
            case TRACE:   printf("TRACE: "); break;
            case DEBUG:   printf("DEBUG: "); break;
            default:      printf("UNKNOWN: "); break;
        }

        vprintf(format, args);
        printf("\n");

        va_end(args);
    }
}

void logger(LogLevel level, const char *format, ...) {
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
    logger(ERROR, format, args);
    va_end(args);
}

void warning(const char *format, ...) {
    va_list args;
    va_start(args, format);
    logger(WARNING, format, args);
    va_end(args);
}

void info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    logger(INFO, format, args);
    va_end(args);
}

void trace(const char *format, ...) {
    va_list args;
    va_start(args, format);
    logger(TRACE, format, args);
    va_end(args);
}

void debug(const char *format, ...) {
    va_list args;
    va_start(args, format);
    logger(DEBUG, format, args);
    va_end(args);
}
