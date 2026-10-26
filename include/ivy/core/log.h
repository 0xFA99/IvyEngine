#ifndef IVY_LOG_H
#define IVY_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

#if defined(__GNUC__) || defined(__clang__)
    #define IVY_PRINTF_ATTR(fmt_idx, var_idx) __attribute__((format(printf, fmt_idx, var_idx)))
    #define IVY_PRINTF_FMT_STR
#elif defined(_MSC_VER)
    #define IVY_PRINTF_ATTR(fmt_idx, var_idx)
    #define IVY_PRINTF_FMT_STR _Printf_format_string_
#else
    #define IVY_PRINTF_ATTR(fmt_idx, var_idx)
    #define IVY_PRINTF_FMT_STR
#endif

#ifndef IVY_COLOR_RESET
    #define IVY_COLOR_RESET    "\033[0m"
    #define IVY_COLOR_GREEN    "\033[32m"
    #define IVY_COLOR_YELLOW   "\033[33m"
    #define IVY_COLOR_RED      "\033[31m"
    #define IVY_COLOR_CYAN     "\033[36m"
    #define IVY_COLOR_GRAY     "\033[90m"
    #define IVY_COLOR_BOLD_RED "\033[1;31m"
#endif


typedef enum IvyLogLevel {
    IVY_LOG_TRACE = 0,
    IVY_LOG_DEBUG,
    IVY_LOG_INFO,
    IVY_LOG_WARN,
    IVY_LOG_ERROR,
    IVY_LOG_FATAL,
    IVY_LOG_LEVEL_COUNT,
    IVY_LOG_NONE = IVY_LOG_LEVEL_COUNT
} IvyLogLevel;

typedef enum IvyLogMode {
    IVY_LOG_MODE_SYNC = 0,
    IVY_LOG_MODE_ASYNC
} IvyLogMode;

typedef enum IvyLogPolicy {
    IVY_LOG_POLICY_DROP = 0,
    IVY_LOG_POLICY_BLOCK
} IvyLogPolicy;

typedef struct IvyLogConfig {
    IvyLogMode    mode;
    IvyLogLevel   min_level;
    IvyLogPolicy  policy;
    size_t        queue_capacity;
    size_t        batch_size;
    uint32_t      flush_interval_ms;
    bool          use_color;
    bool          show_timestamp;
    FILE         *out;
} IvyLogConfig;

typedef void (*IvyLogCallback)(IvyLogLevel level, const char *file, int line, const char *message);


#ifdef IVY_LOG_DISABLE

static inline bool Ivy_Log_Init(const IvyLogConfig *cfg)
{
    (void)cfg;
    return true;
}
static inline void Ivy_Log_Terminate(void) {}
static inline void Ivy_Log_Flush(void) {}
static inline void Ivy_Log_SetLevel(IvyLogLevel level) { (void)level; }
static inline IvyLogLevel Ivy_Log_GetLevel(void) { return IVY_LOG_NONE; }
static inline void Ivy_Log_SetCallback(IvyLogCallback callback) { (void)callback; }
static inline void Ivy_Log_Write(IvyLogLevel level, const char *file, int line, const char *fmt, ...)
{
    (void)level; (void)file; (void)line; (void)fmt;
}

#undef IVY_LOG_TRACE
#undef IVY_LOG_DEBUG
#undef IVY_LOG_INFO
#undef IVY_LOG_WARN
#undef IVY_LOG_ERROR
#undef IVY_LOG_FATAL

#define IVY_LOG_TRACE(fmt, ...) ((void)0)
#define IVY_LOG_DEBUG(fmt, ...) ((void)0)
#define IVY_LOG_INFO(fmt, ...)  ((void)0)
#define IVY_LOG_WARN(fmt, ...)  ((void)0)
#define IVY_LOG_ERROR(fmt, ...) ((void)0)
#define IVY_LOG_FATAL(fmt, ...) ((void)0)

#ifdef IVY_LOG_IMPLEMENTATION
    #undef IVY_LOG_IMPLEMENTATION
#endif

#else

bool Ivy_Log_Init(const IvyLogConfig *cfg);
void Ivy_Log_Terminate(void);
void Ivy_Log_Flush(void);
void Ivy_Log_SetLevel(IvyLogLevel level);
IvyLogLevel Ivy_Log_GetLevel(void);
void Ivy_Log_SetCallback(IvyLogCallback callback);

IVY_PRINTF_ATTR(4, 5)
void Ivy_Log_Write(IvyLogLevel level, const char *file, int line, IVY_PRINTF_FMT_STR const char *fmt, ...);

#ifdef IVY_DEBUG
    #define IVY_LOG_TRACE(fmt, ...) Ivy_Log_Write(IVY_LOG_TRACE, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #define IVY_LOG_DEBUG(fmt, ...) Ivy_Log_Write(IVY_LOG_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
    #define IVY_LOG_TRACE(fmt, ...) do { if (0) Ivy_Log_Write(IVY_LOG_TRACE, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } while (0)
    #define IVY_LOG_DEBUG(fmt, ...) do { if (0) Ivy_Log_Write(IVY_LOG_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } while (0)
#endif

#define IVY_LOG_INFO(fmt, ...) Ivy_Log_Write(IVY_LOG_INFO,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define IVY_LOG_WARN(fmt, ...) Ivy_Log_Write(IVY_LOG_WARN,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define IVY_LOG_ERROR(fmt, ...) Ivy_Log_Write(IVY_LOG_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define IVY_LOG_FATAL(fmt, ...) Ivy_Log_Write(IVY_LOG_FATAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#endif

#ifdef __cplusplus
}
#endif

#if defined(IVY_LOG_IMPLEMENTATION) && !defined(IVY_LOG_DISABLE)

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>

#ifndef IVY_LOG_MSG_MAX
    #define IVY_LOG_MSG_MAX 1024
#endif

typedef struct IvyLogMsg {
    IvyLogLevel level;
    char        file[64];
    int         line;
    char        raw_msg[IVY_LOG_MSG_MAX];
    char        formatted[IVY_LOG_MSG_MAX + 128];
} IvyLogMsg;

typedef struct IvyLogRing {
    IvyLogMsg       *slots;
    size_t           capacity;
    size_t           head;
    size_t           tail;
    size_t           count;
    pthread_mutex_t  mtx;
    pthread_cond_t   not_empty;
    pthread_cond_t   not_full;
} IvyLogRing;

static struct {
    IvyLogConfig     cfg;
    IvyLogRing       ring;
    pthread_t        worker;
    volatile bool    running;
    volatile bool    initialized;
    pthread_mutex_t  state_mtx;
    IvyLogCallback   callback;
} gIvyLogger = {
    .cfg = {
        .mode = IVY_LOG_MODE_SYNC,
        .min_level = IVY_LOG_TRACE,
        .policy = IVY_LOG_POLICY_DROP,
        .queue_capacity = 4096,
        .batch_size = 128,
        .flush_interval_ms = 50,
        .use_color = true,
        .show_timestamp = true,
        .out = NULL
    },
    .running = false,
    .initialized = false,
    .callback = NULL
};

static const char *sIvyLevelNames[IVY_LOG_LEVEL_COUNT] = {
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

static const char *sIvyLevelColors[IVY_LOG_LEVEL_COUNT] = {
    IVY_COLOR_GRAY,
    IVY_COLOR_CYAN,
    IVY_COLOR_GREEN,
    IVY_COLOR_YELLOW,
    IVY_COLOR_RED,
    IVY_COLOR_BOLD_RED
};

static void Ivy_Log_FormatMessage(const IvyLogConfig *cfg, IvyLogMsg *msg)
{
    int off = 0;
    size_t cap = sizeof(msg->formatted);

    if (cfg->show_timestamp) {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        struct tm tmv;
        localtime_r(&tv.tv_sec, &tmv);
        off += snprintf(msg->formatted + off, cap - off,
                        "%02d:%02d:%02d.%03d ",
                        tmv.tm_hour, tmv.tm_min, tmv.tm_sec, (int)(tv.tv_usec / 1000));
    }

    if (cfg->use_color && msg->level < IVY_LOG_LEVEL_COUNT) {
        off += snprintf(msg->formatted + off, cap - off, "%s%-5s" IVY_COLOR_RESET " ",
                        sIvyLevelColors[msg->level], sIvyLevelNames[msg->level]);
    }
    else if (msg->level < IVY_LOG_LEVEL_COUNT) {
        off += snprintf(msg->formatted + off, cap - off, "%-5s ", sIvyLevelNames[msg->level]);
    }

    if (msg->file[0] != '\0') {
        off += snprintf(msg->formatted + off, cap - off, "[%s:%d] ", msg->file, msg->line);
    }

    off += snprintf(msg->formatted + off, (off < (int)cap) ? cap - off : 0, "%s\n", msg->raw_msg);

    if (off < 0) off = 0;
    if ((size_t)off >= cap) off = (int)cap - 1;

    msg->formatted[off] = '\0';
}

static bool Ivy_Ring_Init(IvyLogRing *r, size_t cap)
{
    r->slots = (IvyLogMsg *)malloc(sizeof(IvyLogMsg) * cap);
    if (!r->slots) return false;

    r->capacity = cap;
    r->head = r->tail = r->count = 0;

    pthread_mutex_init(&r->mtx, NULL);
    pthread_cond_init(&r->not_empty, NULL);
    pthread_cond_init(&r->not_full, NULL);

    return true;
}

static void Ivy_Ring_Destroy(IvyLogRing *r)
{
    if (r->slots) {
        free(r->slots);
        r->slots = NULL;
    }

    pthread_mutex_destroy(&r->mtx);
    pthread_cond_destroy(&r->not_empty);
    pthread_cond_destroy(&r->not_full);
}

static bool Ivy_Ring_Push(IvyLogRing *r, const IvyLogMsg *m, IvyLogPolicy policy)
{
    pthread_mutex_lock(&r->mtx);

    if (r->count == r->capacity)
    {
        if (policy == IVY_LOG_POLICY_DROP) {
            pthread_mutex_unlock(&r->mtx);
            return false;
        }

        while (r->count == r->capacity && gIvyLogger.running) {
            pthread_cond_wait(&r->not_full, &r->mtx);
        }

        if (!gIvyLogger.running) {
            pthread_mutex_unlock(&r->mtx);
            return false;
        }
    }

    r->slots[r->head] = *m;
    r->head = (r->head + 1) % r->capacity;
    r->count++;

    pthread_cond_signal(&r->not_empty);
    pthread_mutex_unlock(&r->mtx);

    return true;
}

static size_t Ivy_Ring_PopBatch(IvyLogRing *r, IvyLogMsg *out, size_t max, uint32_t timeout_ms)
{
    pthread_mutex_lock(&r->mtx);

    if (r->count == 0 && timeout_ms > 0)
    {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);

        ts.tv_sec  += timeout_ms / 1000;
        ts.tv_nsec += (long)(timeout_ms % 1000) * 1000000L;

        if (ts.tv_nsec >= 1000000000L) {
            ts.tv_sec++;
            ts.tv_nsec -= 1000000000L;
        }

        while (r->count == 0 && gIvyLogger.running) {
            int rc = pthread_cond_timedwait(&r->not_empty, &r->mtx, &ts);
            if (rc != 0) break;
        }
    }

    size_t n = 0;
    while (n < max && r->count > 0)
    {
        out[n++] = r->slots[r->tail];
        r->tail = (r->tail + 1) % r->capacity;
        r->count--;
    }

    if (n > 0) pthread_cond_broadcast(&r->not_full);

    pthread_mutex_unlock(&r->mtx);
    return n;
}

static void Ivy_Log_WriteBatch(IvyLogMsg *batch, size_t n)
{
    if (n == 0) return;
    FILE *last_stream = NULL;

    for (size_t i = 0; i < n; i++)
    {
        FILE *stream = gIvyLogger.cfg.out ? gIvyLogger.cfg.out : (batch[i].level >= IVY_LOG_WARN ? stderr : stdout);
        fputs(batch[i].formatted, stream);
        last_stream = stream;

        if (gIvyLogger.callback) {
            gIvyLogger.callback(batch[i].level, batch[i].file, batch[i].line, batch[i].raw_msg);
        }

        if (i + 1 < n) {
            FILE *next = gIvyLogger.cfg.out ? gIvyLogger.cfg.out : (batch[i+1].level >= IVY_LOG_WARN ? stderr : stdout);
            if (next != stream) fflush(stream);
        }
    }

    if (last_stream) fflush(last_stream);
}

static void *Ivy_Log_WorkerMain(void *arg)
{
    (void)arg;
    IvyLogMsg *batch = (IvyLogMsg *)malloc(sizeof(IvyLogMsg) * gIvyLogger.cfg.batch_size);
    if (!batch) return NULL;

    while (gIvyLogger.running) {
        size_t n = Ivy_Ring_PopBatch(&gIvyLogger.ring, batch, gIvyLogger.cfg.batch_size, gIvyLogger.cfg.flush_interval_ms);
        Ivy_Log_WriteBatch(batch, n);
    }

    size_t n;
    while ((n = Ivy_Ring_PopBatch(&gIvyLogger.ring, batch, gIvyLogger.cfg.batch_size, 0)) > 0) {
        Ivy_Log_WriteBatch(batch, n);
    }

    free(batch);
    return NULL;
}


bool Ivy_Log_Init(const IvyLogConfig *cfg)
{
    if (gIvyLogger.initialized) return true;

    IvyLogConfig config = cfg ? *cfg : (IvyLogConfig){0};

    if (config.queue_capacity == 0)    config.queue_capacity = 4096;
    if (config.batch_size == 0)        config.batch_size = 128;
    if (config.flush_interval_ms == 0) config.flush_interval_ms = 50;

    if (cfg == NULL) {
        config.mode = IVY_LOG_MODE_SYNC;
        config.min_level = IVY_LOG_TRACE;
        config.use_color = true;
        config.show_timestamp = true;
    }

    pthread_mutex_init(&gIvyLogger.state_mtx, NULL);
    gIvyLogger.cfg = config;

    if (config.mode == IVY_LOG_MODE_ASYNC)
    {
        if (!Ivy_Ring_Init(&gIvyLogger.ring, config.queue_capacity)) {
            pthread_mutex_destroy(&gIvyLogger.state_mtx);
            return false;
        }

        gIvyLogger.running = true;
        if (pthread_create(&gIvyLogger.worker, NULL, Ivy_Log_WorkerMain, NULL) != 0)
        {
            Ivy_Ring_Destroy(&gIvyLogger.ring);
            gIvyLogger.running = false;
            pthread_mutex_destroy(&gIvyLogger.state_mtx);
            return false;
        }
    }

    gIvyLogger.initialized = true;
    return true;
}

void Ivy_Log_Flush(void)
{
    if (!gIvyLogger.initialized || gIvyLogger.cfg.mode != IVY_LOG_MODE_ASYNC)
    {
        if (gIvyLogger.cfg.out) fflush(gIvyLogger.cfg.out);
        else { fflush(stdout); fflush(stderr); }
        return;
    }

    for (;;) {
        pthread_mutex_lock(&gIvyLogger.ring.mtx);
        size_t n = gIvyLogger.ring.count;
        pthread_mutex_unlock(&gIvyLogger.ring.mtx);

        if (n == 0) break;

        struct timespec ts = { 0, 1 * 1000000L };
        nanosleep(&ts, NULL);
    }
}

void Ivy_Log_Terminate(void)
{
    if (!gIvyLogger.initialized) return;

    if (gIvyLogger.cfg.mode == IVY_LOG_MODE_ASYNC)
    {
        Ivy_Log_Flush();
        gIvyLogger.running = false;

        pthread_mutex_lock(&gIvyLogger.ring.mtx);
        pthread_cond_broadcast(&gIvyLogger.ring.not_empty);
        pthread_cond_broadcast(&gIvyLogger.ring.not_full);
        pthread_mutex_unlock(&gIvyLogger.ring.mtx);

        pthread_join(gIvyLogger.worker, NULL);

        Ivy_Ring_Destroy(&gIvyLogger.ring);
    }

    pthread_mutex_destroy(&gIvyLogger.state_mtx);
    gIvyLogger.initialized = false;
}

void Ivy_Log_SetLevel(IvyLogLevel level)
{
    pthread_mutex_lock(&gIvyLogger.state_mtx);
    gIvyLogger.cfg.min_level = level;
    pthread_mutex_unlock(&gIvyLogger.state_mtx);
}

IvyLogLevel Ivy_Log_GetLevel(void)
{
    pthread_mutex_lock(&gIvyLogger.state_mtx);
    IvyLogLevel lvl = gIvyLogger.cfg.min_level;
    pthread_mutex_unlock(&gIvyLogger.state_mtx);

    return lvl;
}

void Ivy_Log_SetCallback(IvyLogCallback callback)
{
    pthread_mutex_lock(&gIvyLogger.state_mtx);
    gIvyLogger.callback = callback;
    pthread_mutex_unlock(&gIvyLogger.state_mtx);
}

void Ivy_Log_Write(IvyLogLevel level, const char *file, int line, const char *fmt, ...)
{
    if (level < gIvyLogger.cfg.min_level || gIvyLogger.cfg.min_level == IVY_LOG_NONE)
        return;

    IvyLogMsg msg;
    msg.level = level;
    msg.line = line;

    if (file) {
        const char *base = strrchr(file, '/');
#if defined(_WIN32)
        const char *base_win = strrchr(file, '\\');
        if (base_win && (!base || base_win > base)) base = base_win;
#endif
        const char *short_file = base ? base + 1 : file;
        snprintf(msg.file, sizeof(msg.file), "%s", short_file);
    }
    else {
        msg.file[0] = '\0';
    }

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(msg.raw_msg, sizeof(msg.raw_msg), fmt, ap);
    va_end(ap);

    Ivy_Log_FormatMessage(&gIvyLogger.cfg, &msg);

    if (level == IVY_LOG_FATAL)
    {
        Ivy_Log_Flush();
        fputs(msg.formatted, stderr);
        fflush(stderr);

        if (gIvyLogger.callback) {
            gIvyLogger.callback(level, msg.file, line, msg.raw_msg);
        }

        abort();
    }

    if (gIvyLogger.initialized && gIvyLogger.cfg.mode == IVY_LOG_MODE_ASYNC)
    {
        bool pushed = Ivy_Ring_Push(&gIvyLogger.ring, &msg, gIvyLogger.cfg.policy);
        if (!pushed) {
            fputs("[IvyLog] Warning: queue full, log message dropped.\n", stderr);
        }

        return;
    }

    // sync mode
    FILE *stream = gIvyLogger.cfg.out ? gIvyLogger.cfg.out : (level >= IVY_LOG_WARN ? stderr : stdout);
    fputs(msg.formatted, stream);
    fflush(stream);

    if (gIvyLogger.callback) {
        gIvyLogger.callback(level, msg.file, line, msg.raw_msg);
    }
}

#endif

#endif