
typedef enum {
    ERROR = 0,
    WARNING,
    INFO,
    TRACE,
    DEBUG
} LogLevel;

LogLevel set_log_level(LogLevel level);
void logger(LogLevel level, const char *format, ...);
void error(const char *format, ...);
void warning(const char *format, ...);
void info(const char *format, ...);
void trace(const char *format, ...);
void debug(const char *format, ...);

#pragma once