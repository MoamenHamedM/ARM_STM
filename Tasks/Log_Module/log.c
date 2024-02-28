/* logging.c */

#include "log.h"
#include <stdio.h>
#include <stdarg.h>

static FILE *logFile = NULL;
static LogLevel Severity_Level = LOG_INFO;

// Function to choose log output to file, console, or both
void setLogOutput(const char *filename) {
    if (filename) {
        if (logFile) {
            fclose(logFile);
        }
        logFile = fopen(filename, "a");
    } else {
        logFile = stderr;
    }
}

// Function to set the log level
void setSeverityLevel(LogLevel level) {
    Severity_Level = level;
}

// Function to write to the log
void log_Write(LogLevel level, const char *format, ...) {
    if (level >= Severity_Level) {
        // Get current time
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        // Format time as string
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);

        // Write log message with timestamp
        va_list args;
        va_start(args, format);
        if (logFile) {
            fprintf(logFile, "[%s] [%d] ", timeStr, level);
            vfprintf(logFile, format, args);
        }
        va_end(args);
    }
}

// Function to clear logs from the file
void clearLogs() {
    if (logFile) {
        fclose(logFile);
        logFile = fopen("log.txt", "w"); // Open file in write mode, clearing its contents
        fclose(logFile);
        logFile = fopen("log.txt", "a"); // Reopen file in append mode
    }
}

