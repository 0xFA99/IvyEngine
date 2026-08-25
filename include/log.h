#ifndef IVY_LOG_H
#define IVY_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef IVY_COLOR_RESET
    #define IVY_COLOR_RESET  "\033[0m"
    #define IVY_COLOR_GREEN  "\033[32m"
    #define IVY_COLOR_YELLOW "\033[33m"
    #define IVY_COLOR_RED    "\033[31m"
    #define IVY_COLOR_CYAN   "\033[36m"
    #define IVY_COLOR_GRAY   "\033[90m"
#endif

#if defined(__GNUC__) || defined(__clang__)
    #define IVY_PRINTF_ATTR(fmt_idx, var_idx) \
        __attribute__((format(printf, fmt_idx, var_idx)))
    #define IVY_PRINTF_FMT_STR
#elif defined(_MSC_VER)
    #define IVY_PRINTF_ATTR(fmt_idx, var_idx)
    #define IVY_PRINTF_FMT_STR _Printf_format_string_
#else
    #define IVY_PRINTF_ATTR(fmt_idx, var_idx)
    #define IVY_PRINTF_FMT_STR
#endif

typedef enum IvyLogLevel {
    IVY_LOG_TRACE = 0,
    IVY_LOG_DEBUG,
    IVY_LOG_INFO,
    IVY_LOG_WARN,
    IVY_LOG_ERROR,
    IVY_LOG_FATAL,
    IVY_LOG_NONE
} IvyLogLevel;

typedef void (*IvyLogCallback)(IvyLogLevel level, const char *file, int line, const char *fmt, va_list args);

void Ivy_Log_SetLevel(IvyLogLevel level);
void Ivy_Log_SetCallback(IvyLogCallback callback);

IVY_PRINTF_ATTR(4, 5)
void Ivy_Log_Write(IvyLogLevel level, const char *file, int line, IVY_PRINTF_FMT_STR const char *fmt, ...);

#ifdef IVY_DEBUG
    #define IVY_LOG_TRACE(fmt, ...) \
        Ivy_Log_Write(IVY_LOG_TRACE, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #define IVY_LOG_DEBUG(fmt, ...) \
        Ivy_Log_Write(IVY_LOG_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #define IVY_LOG_INFO(fmt, ...) \
        Ivy_Log_Write(IVY_LOG_INFO,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
    #define IVY_LOG_TRACE(fmt, ...) \
        do { if (0) Ivy_Log_Write(IVY_LOG_TRACE, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } while (0)
    #define IVY_LOG_DEBUG(fmt, ...) \
        do { if (0) Ivy_Log_Write(IVY_LOG_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } while (0)
    #define IVY_LOG_INFO(fmt, ...) \
        do { if (0) Ivy_Log_Write(IVY_LOG_INFO,  __FILE__, __LINE__, fmt, ##__VA_ARGS__); } while (0)
#endif

#define IVY_LOG_WARN(fmt, ...) \
    Ivy_Log_Write(IVY_LOG_WARN,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define IVY_LOG_ERROR(fmt, ...) \
    Ivy_Log_Write(IVY_LOG_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define IVY_LOG_FATAL(fmt, ...) \
    Ivy_Log_Write(IVY_LOG_FATAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#ifdef IVY_LOG_IMPLEMENTATION

static IvyLogLevel    g_ivy_log_level = IVY_LOG_INFO;
static IvyLogCallback g_ivy_log_callback = NULL;

static void Ivy_DefaultLogHandler(IvyLogLevel level, const char *file, int line, const char *fmt, va_list args) {
    FILE *target = (level >= IVY_LOG_WARN) ? stderr : stdout;
    switch (level) {
        case IVY_LOG_TRACE: fprintf(target, IVY_COLOR_GRAY "[TRACE] (%s:%d) " IVY_COLOR_RESET, file, line); break;
        case IVY_LOG_DEBUG: fprintf(target, IVY_COLOR_CYAN "[DEBUG] (%s:%d) " IVY_COLOR_RESET, file, line); break;
        case IVY_LOG_INFO:  fprintf(target, IVY_COLOR_GREEN "[INFO]  " IVY_COLOR_RESET); break;
        case IVY_LOG_WARN:  fprintf(target, IVY_COLOR_YELLOW "[WARN]  " IVY_COLOR_RESET); break;
        case IVY_LOG_ERROR: fprintf(target, IVY_COLOR_RED "[ERROR] (%s:%d) " IVY_COLOR_RESET, file, line); break;
        case IVY_LOG_FATAL: fprintf(target, IVY_COLOR_RED "[FATAL] (%s:%d) " IVY_COLOR_RESET, file, line); break;
        default: break;
    }
    vfprintf(target, fmt, args);
    fputc('\n', target);
    fflush(target);
}

void Ivy_Log_SetLevel(IvyLogLevel level) { g_ivy_log_level = level; }
void Ivy_Log_SetCallback(IvyLogCallback callback) { g_ivy_log_callback = callback; }

void Ivy_Log_Write(IvyLogLevel level, const char *file, int line, const char *fmt, ...) {
    if (level < g_ivy_log_level || g_ivy_log_level == IVY_LOG_NONE) return;
    va_list args;
    va_start(args, fmt);
    if (g_ivy_log_callback) {
        g_ivy_log_callback(level, file, line, fmt, args);
    } else {
        Ivy_DefaultLogHandler(level, file, line, fmt, args);
    }
    va_end(args);
    if (level == IVY_LOG_FATAL) abort();
}

#endif

#endif