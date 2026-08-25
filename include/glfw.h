#ifndef IVY_GLFW_LOADER_H
#define IVY_GLFW_LOADER_H

#include "types.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GLFW_CONTEXT_VERSION_MAJOR   0x00022002
#define GLFW_CONTEXT_VERSION_MINOR   0x00022003
#define GLFW_OPENGL_PROFILE          0x00022008
#define GLFW_OPENGL_CORE_PROFILE     0x00032001
#define GLFW_RESIZABLE               0x00020003
#define GLFW_VISIBLE                 0x00020004
#define GLFW_DECORATED               0x00020005
#define GLFW_FOCUSED                 0x00020007
#define GLFW_MAXIMIZED               0x00020008
#define GLFW_FLOATING                0x00020009
#define GLFW_SAMPLES                 0x0002100D
#define GLFW_SRGB_CAPABLE            0x0002100E
#define GLFW_DOUBLEBUFFER            0x00021010
#define GLFW_STEREO                  0x00021011
#define GLFW_REFRESH_RATE            0x00021012

typedef struct GLFWwindow  GLFWwindow;
typedef struct GLFWmonitor GLFWmonitor;
typedef void (*GLFWglproc)(void);
typedef void (* GLFWframebuffersizefun)(GLFWwindow* window, int width, int height);

#define IVY_GLFW_FUNCTIONS(X) \
    X(int, glfwInit, void) \
    X(void, glfwTerminate, void) \
    X(GLFWwindow*, glfwCreateWindow, int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share) \
    X(void, glfwWindowHint, int hint, int value) \
    X(void, glfwMakeContextCurrent, GLFWwindow *window) \
    X(void, glfwDestroyWindow, GLFWwindow *window) \
    X(void, glfwSwapInterval, int internal) \
    X(void, glfwSwapBuffers, GLFWwindow *window) \
    X(void, glfwPollEvents, void) \
    X(int, glfwWindowShouldClose, GLFWwindow *window) \
    X(GLFWglproc, glfwGetProcAddress, const char *procname) \
    X(void*, glfwGetWindowUserPointer, GLFWwindow *window) \
    X(void, glfwSetWindowUserPointer, GLFWwindow *window, void *pointer) \
    X(GLFWframebuffersizefun, glfwSetFramebufferSizeCallback, GLFWwindow *window, GLFWframebuffersizefun callback)
    // adjust as needed

#define X(ret, name, ...) typedef ret (*PFN_##name)(__VA_ARGS__);
IVY_GLFW_FUNCTIONS(X)
#undef X

#define X(ret, name, ...) extern PFN_##name name;
IVY_GLFW_FUNCTIONS(X)
#undef X

IvyStatus Ivy_GLFW_Load(void);
void Ivy_GLFW_Unload(void);

#ifdef __cplusplus
}
#endif

#ifdef IVY_GLFW_LOADER_IMPLEMENTATION

#if defined(_WIN32)
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0600
    #endif
    #include <windows.h>
    typedef HMODULE IvyLibraryHandler;
    #define LIB_OPEN(path)          LoadLibraryA(path)
    #define LIB_CLOSE(handle)       FreeLibrary(handle)
    #define LIB_SYM(handle, name)   GetProcAddress(handle, name)
    #define DEFAULT_LIB_NAME        "glfw3.dll"
#else // linux
    #include <dlfcn.h>
    typedef void *IvyLibraryHandler;
    #define LIB_OPEN(path)          dlopen(path, RTLD_NOW | RTLD_LOCAL)
    #define LIB_CLOSE(handle)       dlclose(handle)
    #define LIB_SYM(handle, name)   dlsym(handle, name)
    #define DEFAULT_LIB_NAME        "libglfw.so.3"
#endif

#ifndef IVY_GLFW_LIBRARY_NAME
    #define IVY_GLFW_LIBRARY_NAME DEFAULT_LIB_NAME
#endif

static IvyLibraryHandler g_library_handler = NULL;

#define X(ret, name, ...) PFN_##name name = NULL;
IVY_GLFW_FUNCTIONS(X)
#undef X

// NOLINTNEXTLINE(misc-definitions-in-headers)
IvyStatus Ivy_GLFW_Load(void)
{
    if (g_library_handler)
        return IVY_SUCCESS;

    g_library_handler = LIB_OPEN(IVY_GLFW_LIBRARY_NAME);

#if defined(__linux__)
    if (!g_library_handler) {
        g_library_handler = LIB_OPEN("libglfw.so");
    }
#endif

    if (g_library_handler == NULL) {
        IVY_LOG_ERROR("Failed to load GLFW library (not found or invalid)");
        return IVY_ERROR_LOAD_FAILED;
    }

    bool success = true;

#define X(ret, name, ...) \
    name = (PFN_##name) LIB_SYM(g_library_handler, #name); \
    if (!name) { \
        IVY_LOG_ERROR("Missing symbol: %s", #name); \
        success = false; \
    }
IVY_GLFW_FUNCTIONS(X)
#undef X

    if (!success) {
        Ivy_GLFW_Unload();
        return IVY_ERROR_NULL_PTR;
    }

    return IVY_SUCCESS;
}

// NOLINTNEXTLINE(misc-definitions-in-headers)
void Ivy_GLFW_Unload(void)
{
    if (g_library_handler) {
        LIB_CLOSE(g_library_handler);
        g_library_handler = NULL;
    }

    #define X(ret, name, ...) name = NULL;
    IVY_GLFW_FUNCTIONS(X)
    #undef X
}

#endif

#endif