
#ifndef LOG_H
#define LOG_H

#include "time.h"

// Enumeration for log levels
typedef enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

// Function prototypes
void setLogOutput(const char *filename);
void setSeverityLevel(LogLevel level);
void log_Write(LogLevel level, const char *format, ...);
void clearLogs();

#endif
