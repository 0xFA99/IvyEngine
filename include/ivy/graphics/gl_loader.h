#ifndef IVY_GL_LOADER_H
#define IVY_GL_LOADER_H

#include "ivy/core/types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GL_COLOR_BUFFER_BIT  0x00004000
#define GL_DEPTH_BUFFER_BIT  0x00000100

typedef unsigned int GLbitfield;
typedef float        GLclampf;
typedef int          GLint;
typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef int          GLsizei;

#define IVY_GL_FUNCTIONS(X) \
    X(void, glClear, GLbitfield mask) \
    X(void, glClearColor, GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) \
    X(void, glViewport, GLint x, GLint y, GLsizei width, GLsizei height) \
    X(void, glGenFramebuffers, GLsizei n, GLuint *framebuffers) \
    X(void, glDeleteFramebuffers, GLsizei n, const GLuint *framebuffers) \
    X(void, glBindFramebuffer, GLenum target, GLuint framebuffer)
    // adjust as needed

#define X(ret, name, ...) typedef ret (*PFN_##name)(__VA_ARGS__);
IVY_GL_FUNCTIONS(X)
#undef X

#define X(ret, name, ...) extern PFN_##name name;
IVY_GL_FUNCTIONS(X)
#undef X

IvyStatus Ivy_GL_Load(void);
void Ivy_GL_Unload(void);

#ifdef __cplusplus
}
#endif

#ifdef IVY_GL_LOADER_IMPLEMENTATION

#include "ivy/core/log.h"
#include "ivy/platform/glfw_loader.h"

#define X(ret, name, ...) PFN_##name name = NULL;
IVY_GL_FUNCTIONS(X)
#undef X

// NOLINTNEXTLINE(misc-definitions-in-headers)
IvyStatus Ivy_GL_Load(void)
{
    IvyStatus status = IVY_SUCCESS;

#define X(ret, name, ...) \
    name = (PFN_##name) glfwGetProcAddress(#name); \
    if (IVY_UNLIKELY(name == NULL)) { \
        IVY_LOG_ERROR("Failed to load GL function: %s", #name); \
        status = IVY_ERROR_LOAD_FAILED; \
    }
    IVY_GL_FUNCTIONS(X)
#undef X

    return status;
}

// NOLINTNEXTLINE(misc-definitions-in-headers)
void Ivy_GL_Unload(void) {
    #define X(ret, name, ...) name = NULL;
    IVY_GL_FUNCTIONS(X)
    #undef X
}

#endif

#endif