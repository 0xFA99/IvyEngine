#ifndef IVY_TYPES_H
#define IVY_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>


typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;

typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;

typedef size_t    usize;
typedef ptrdiff_t isize;

typedef float     f32;
typedef double    f64;


#if defined(_WIN32)
    #if defined(IVY_BUILD_DLL)
        #define IVY_API __declspec(dllexport)
    #elif defined(IVY_SHARED)
        #define IVY_API __declspec(dllimport)
    #else
        #define IVY_API
    #endif
#else // Linux (GCC/Clang)
    #if defined(IVY_BUILD_DLL)
        #define IVY_API __attribute__((visibility("default")))
    #else
        #define IVY_API
    #endif
#endif


#if defined(_MSC_VER)
    #define IVY_INLINE      static __forceinline
    #define IVY_NOINLINE    __declspec(noinline)
    #define IVY_LIKELY(x)   (x)
    #define IVY_UNLIKELY(x) (x)
    #define IVY_UNREACHABLE __assume(0)
#else // GCC & Clang
    #define IVY_INLINE      static inline __attribute__((always_inline))
    #define IVY_NOINLINE    __attribute__((noinline))
    #define IVY_LIKELY(x)   __builtin_expect(!!(x), 1)
    #define IVY_UNLIKELY(x) __builtin_expect(!!(x), 0)
    #define IVY_UNREACHABLE __builtin_unreachable()
#endif

#define IVY_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)


#define IVY_COLOR_RESET  "\033[0m"
#define IVY_COLOR_GREEN  "\033[32m"
#define IVY_COLOR_YELLOW "\033[33m"
#define IVY_COLOR_RED    "\033[31m"
#define IVY_COLOR_CYAN   "\033[36m"

#define IVY_LOG_INFO(fmt, ...) \
    fprintf(stdout, IVY_COLOR_GREEN "[INFO] " IVY_COLOR_RESET fmt "\n", ##__VA_ARGS__)

#define IVY_LOG_WARN(fmt, ...) \
    fprintf(stderr, IVY_COLOR_YELLOW "[WARN] " IVY_COLOR_RESET fmt "\n", ##__VA_ARGS__)

#define IVY_LOG_ERROR(fmt, ...) \
    fprintf(stderr, IVY_COLOR_RED "[ERROR] (%s:%d) " IVY_COLOR_RESET fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#ifdef IVY_DEBUG
    #define IVY_LOG_DEBUG(fmt, ...) \
        fprintf(stdout, IVY_COLOR_CYAN "[DEBUG] (%s:%d) " IVY_COLOR_RESET fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define IVY_LOG_DEBUG(fmt, ...) ((void)0)
#endif


#define IVY_CHECK(cond, fmt, ...) \
    do { \
        if (IVY_UNLIKELY(!(cond))) { \
            fprintf(stderr, "\n" IVY_COLOR_RED "[FATAL CHECK FAILED] %s:%d\n  Condition: %s\n  Message: " fmt IVY_COLOR_RESET "\n\n", \
                __FILE__, __LINE__, #cond, ##__VA_ARGS__); \
            abort(); \
        } \
    } while (0)

#ifdef IVY_DEBUG
    #define IVY_ASSERT(cond, fmt, ...) \
        do { \
            if (IVY_UNLIKELY(!(cond))) { \
                fprintf(stderr, "\n" IVY_COLOR_RED "[ASSERT FAILED] %s:%d\n  Condition: %s\n  Message: " fmt IVY_COLOR_RESET "\n\n", \
                        __FILE__, __LINE__, #cond, ##__VA_ARGS__); \
                abort(); \
            } \
        } while (0)

    #define IVY_ENSURE(cond) IVY_ASSERT(cond, "Critical condition failed")
#else
    #define IVY_ASSERT(cond, ...) ((void)0)
    #define IVY_ENSURE(cond)      do { if (!(cond)) IVY_UNREACHABLE; } while (0)
#endif


typedef struct IvyAllocator {
    void *(*alloc)(usize size, void *data);
    void *(*realloc)(void *block, usize size, void *data);
    void  (*dealloc)(void *block, void *data);
    void *data;
} IvyAllocator;

IVY_INLINE void *Ivy_DefaultAllocator_Alloc(usize size, void *user)
{
    (void)user;
    return malloc(size);
}

IVY_INLINE void *Ivy_DefaultAllocator_Realloc(void* block, usize size, void* user)
{
    (void)user;
    return realloc(block, size);
}

IVY_INLINE void Ivy_DefaultAllocator_Free(void* block, void* user)
{
    (void)user;
    free(block);
}

static IvyAllocator IvyDefaultAllocator = {
    .alloc   = Ivy_DefaultAllocator_Alloc,
    .realloc = Ivy_DefaultAllocator_Realloc,
    .dealloc = Ivy_DefaultAllocator_Free,
    .data    = NULL
};

IVY_INLINE void Ivy_DefaultAllocator_Set(const IvyAllocator* allocator)
{
    if (allocator) {
        IvyDefaultAllocator = *allocator;
        return;
    }

    IvyDefaultAllocator.alloc   = Ivy_DefaultAllocator_Alloc;
    IvyDefaultAllocator.realloc = Ivy_DefaultAllocator_Realloc;
    IvyDefaultAllocator.dealloc = Ivy_DefaultAllocator_Free;
    IvyDefaultAllocator.data    = NULL;
}

#define IVY_ALLOC(type, count) \
    ((type*)IvyDefaultAllocator.alloc(sizeof(type) * (count), IvyDefaultAllocator.data))

#define IVY_REALLOC(ptr, type, count) \
    ((type*)IvyDefaultAllocator.realloc((void*)(ptr), sizeof(type) * (count), IvyDefaultAllocator.data))

#define IVY_FREE(ptr) \
    do { \
        if (ptr) { \
            IvyDefaultAllocator.dealloc((void*)(ptr), IvyDefaultAllocator.data); \
            (ptr) = NULL; \
        } \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif