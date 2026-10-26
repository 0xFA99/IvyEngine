#ifndef IVY_TYPES_H
#define IVY_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

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

typedef enum IvyStatus {
    IVY_SUCCESS           =  0,
    IVY_ERROR_NO_MEMORY   = -1,
    IVY_ERROR_NULL_PTR    = -2,
    IVY_ERROR_LOAD_FAILED = -3
} IvyStatus;

#if defined(_MSC_VER)
    #define IVY_INLINE      static __forceinline
    #define IVY_NOINLINE    __declspec(noinline)
    #define IVY_LIKELY(x)   (x)
    #define IVY_UNLIKELY(x) (x)
    #define IVY_UNREACHABLE __assume(0)
#else
    #define IVY_INLINE      static inline __attribute__((always_inline))
    #define IVY_NOINLINE    __attribute__((noinline))
    #define IVY_LIKELY(x)   __builtin_expect(!!(x), 1)
    #define IVY_UNLIKELY(x) __builtin_expect(!!(x), 0)
    #define IVY_UNREACHABLE __builtin_unreachable()
#endif

#define IVY_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)

typedef struct IvyAllocator {
    void *(*alloc)(usize size, const void *data);
    void *(*realloc)(void *restrict block, usize size, const void *restrict data);
    void  (*dealloc)(void *restrict block, const void *restrict data);
    void *data;
} IvyAllocator;

#ifndef IVY_ASSERT
#ifdef IVY_DEBUG
#define IVY_ASSERT(cond, ...) \
    do { \
        if (IVY_UNLIKELY(!(cond))) { \
            fprintf(stderr, "\n[ASSERT FAILED] %s:%d\n  Condition: %s\n  Message: " __VA_ARGS__ "\n\n", \
                __FILE__, __LINE__, #cond); \
            abort(); \
        } \
    } while (0)
#else
    #define IVY_ASSERT(cond, ...) ((void)0)
#endif
#endif

#ifndef IVY_CHECK
#define IVY_CHECK(cond, ...) \
    do { \
        if (IVY_UNLIKELY(!(cond))) { \
            fprintf(stderr, "\n[FATAL CHECK] %s:%d\n  Condition: %s\n  Message: " __VA_ARGS__ "\n\n", \
            __FILE__, __LINE__, #cond); \
            abort(); \
        } \
    } while (0)
#endif

#ifndef IVY_ENSURE
#ifdef IVY_DEBUG
    #define IVY_ENSURE(cond) IVY_ASSERT(cond, "Critical condition failed")
#else
    #define IVY_ENSURE(cond) do { if (!(cond)) IVY_UNREACHABLE; } while (0)
#endif
#endif

IVY_INLINE void *Ivy_DefaultAllocator_Alloc(const usize size, const void *user)
{
    (void)user;
    return malloc(size);
}

IVY_INLINE void *Ivy_DefaultAllocator_Realloc(void *restrict block, const usize size, const void *restrict user)
{
    (void)user;
    return realloc(block, size);
}

IVY_INLINE void Ivy_DefaultAllocator_Free(void *block, const void *user)
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

IVY_INLINE void Ivy_DefaultAllocator_Set(const IvyAllocator *allocator)
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