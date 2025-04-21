#include <stdarg.h>

enum LogLevel { ERROR, INFO, TRACE, DEBUG };
extern enum LogLevel log_level;

void logger(enum LogLevel level, const char *format, ...);
enum LogLevel set_log_level(enum LogLevel level);

#pragma once