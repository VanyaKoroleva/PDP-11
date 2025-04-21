#include <stdio.h>
#include "logger.h"
#include <stdarg.h>

enum LogLevel log_level = DEBUG;

void logger(enum LogLevel level, const char *format, ...) {

    if (level <= log_level) {

        va_list ap;
        va_start(ap, format);
        vprintf(format, ap);
        va_end(ap);
    }
}

enum LogLevel set_log_level(enum LogLevel level) {
  enum LogLevel old_log_level = log_level;
  log_level = level;
  return old_log_level;
}

