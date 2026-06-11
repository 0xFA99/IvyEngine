#ifndef IVY_GLFW_LOADER_H
#define IVY_GLFW_LOADER_H

#include "ivy/types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GLFW_CONTEXT_VERSION_MAJOR  0x00022002
#define GLFW_CONTEXT_VERSION_MINOR  0x00022003
#define GLFW_OPENGL_PROFILE         0x00022008
#define GLFW_OPENGL_FORWARD_COMPAT  0x00022006
#define GLFW_OPENGL_CORE_PROFILE    0x00032001
#define GLFW_TRUE                   1
#define GLFW_FALSE                  0
#define GLFW_RESIZABLE              0x00020003
#define GLFW_VISIBLE                0x00020004
#define GLFW_DECORATED              0x00020005
#define GLFW_SAMPLES                0x0002100D
#define GLFW_NO_WINDOW_CONTEXT      0x0001000A
#define GLFW_DONT_CARE              (-1)
#define GLFW_RELEASE                0
#define GLFW_PRESS                  1
#define GLFW_FOCUS_ON_SHOW          0x0002000C
#define GLFW_FLOATING               0x00020007
#define GLFW_MOUSE_PASSTHROUGH      0x0002000D
#define GLFW_PLATFORM_ERROR         0x00010008
#define GLFW_LOCK_KEY_MODS          0x00033004

typedef struct GLFWwindow GLFWwindow;
typedef struct GLFWmonitor GLFWmonitor;

typedef struct GLFWvidmode {
    int width;
    int height;
    int redBits;
    int greenBits;
    int blueBits;
    int refreshRate;
} GLFWvidmode;

typedef void (*GLFWwindowsizefun)(GLFWwindow*, int, int);
typedef void (*GLFWframebuffersizefun)(GLFWwindow*, int, int);
typedef void (*GLFWwindowposfun)(GLFWwindow*, int, int);
typedef void (*GLFWwindowiconifyfun)(GLFWwindow*, int);
typedef void (*GLFWwindowmaximizefun)(GLFWwindow*, int);
typedef void (*GLFWwindowfocusfun)(GLFWwindow*, int);
typedef void (*GLFWwindowcontentscalefun)(GLFWwindow*, float, float);
typedef void (*GLFWkeyfun)(GLFWwindow*, int, int, int, int);
typedef void (*GLFWmousebuttonfun)(GLFWwindow*, int, int, int);
typedef void (*GLFWcursorposfun)(GLFWwindow*, double, double);

// Allocator
typedef void* (*GLFWallocatefun)(usize size, void* user);
typedef void* (*GLFWreallocatefun)(void* block, size_t size, void* user);
typedef void  (*GLFWdeallocatefun)(void* block, void* user);
typedef struct GLFWallocator {
    GLFWallocatefun   allocate;
    GLFWreallocatefun reallocate;
    GLFWdeallocatefun deallocate;
    void*             user;
} GLFWallocator;

typedef void (*GLFWerrorfun)(int error_code, const char* description);
typedef void (*GLFWglproc)(void);

// GLFW API function pointer types
typedef int          (*GLFWINIT)(void);
typedef void         (*GLFWTERMINATE)(void);
typedef void         (*GLFWDEFAULTWINDOWHINTS)(void);
typedef void         (*GLFWWINDOWHINT)(int hint, int value);
typedef GLFWwindow*  (*GLFWCREATEWINDOW)(int width, int height, const char* title,
                                         GLFWmonitor* monitor, GLFWwindow* share);
typedef GLFWmonitor* (*GLFWGETPRIMARYMONITOR)(void);
typedef GLFWvidmode* (*GLFWGETVIDEOMODE)(GLFWmonitor* monitor);
typedef void         (*GLFWMAKECONTEXTCURRENT)(GLFWwindow* window);
typedef int          (*GLFWGETERROR)(const char** description);
typedef void         (*GLFWSWAPINTERVAL)(int interval);
typedef GLFWmonitor**(*GLFWGETMONITORS)(int* count);
typedef void         (*GLFWGETMONITORWORKAREA)(GLFWmonitor* monitor,
                                               int* xpos, int* ypos,
                                               int* width, int* height);
typedef GLFWwindowsizefun          (*GLFWSETWINDOWSIZECALLBACK)         (GLFWwindow*, GLFWwindowsizefun);
typedef GLFWframebuffersizefun     (*GLFWSETFRAMEBUFFERSIZECALLBACK)    (GLFWwindow*, GLFWframebuffersizefun);
typedef GLFWwindowposfun           (*GLFWSETWINDOWPOSCALLBACK)          (GLFWwindow*, GLFWwindowposfun);
typedef GLFWwindowiconifyfun       (*GLFWSETWINDOWICONIFYCALLBACK)      (GLFWwindow*, GLFWwindowiconifyfun);
typedef GLFWwindowmaximizefun      (*GLFWSETWINDOWMAXIMIZECALLBACK)     (GLFWwindow*, GLFWwindowmaximizefun);
typedef GLFWwindowfocusfun         (*GLFWSETWINDOWFOCUSCALLBACK)        (GLFWwindow*, GLFWwindowfocusfun);
typedef GLFWwindowcontentscalefun  (*GLFWSETWINDOWCONTENTSCALECALLBACK) (GLFWwindow*, GLFWwindowcontentscalefun);
typedef GLFWkeyfun                 (*GLFWSETKEYCALLBACK)                (GLFWwindow*, GLFWkeyfun);
typedef GLFWmousebuttonfun         (*GLFWSETMOUSEBUTTONCALLBACK)        (GLFWwindow*, GLFWmousebuttonfun);
typedef GLFWcursorposfun           (*GLFWSETCURSORPOSCALLBACK)          (GLFWwindow*, GLFWcursorposfun);
typedef void         (*GLFWSETINPUTMODE)(GLFWwindow* window, int mode, int value);
typedef void         (*GLFWDESTROYWINDOW)(GLFWwindow* window);
typedef double       (*GLFWGETTIME)(void);
typedef void         (*GLFWINITALLOCATOR)(const GLFWallocator* allocator);
typedef GLFWerrorfun (*GLFWSETERRORCALLBACK)(GLFWerrorfun callback);
typedef GLFWglproc   (*GLFWGETPROCADDRESS)(const char* procname);
typedef void         (*GLFWSWAPBUFFERS)(GLFWwindow* window);
typedef void         (*GLFWPOLLEVENTS)(void);
typedef void         (*GLFWWAITEVENTS)(void);
typedef int          (*GLFWWINDOWSHOULDCLOSE)(GLFWwindow* window);
typedef void         (*GLFWSETWINDOWSHOULDCLOSE)(GLFWwindow* window, int value);
typedef void         (*GLFWGETWINDOWCONTENTSCALE)(GLFWwindow* window, float* xscale, float* yscale);
typedef GLFWmonitor  *(*GLFWGETWINDOWMONITOR)(GLFWwindow* window);
typedef void         (*GLFWGETWINDOWPOS)(GLFWwindow* window, int* xpos, int* ypos);
typedef void         (*GLFWGETMONITORPOS)(GLFWmonitor* monitor, int* xpos, int* ypos);
typedef void         (*GLFWSETWINDOWPOS)(GLFWwindow* window, int xpos, int ypos);
typedef void         (*GLFWICONIFYWINDOW)(GLFWwindow* window);
typedef int          (*GLFWGETWINDOWATTRIB)(GLFWwindow* window, int attrib);
typedef void         (*GLFWMAXIMIZEWINDOW)(GLFWwindow* window);
typedef void         (*GLFWSETWINDOWMONITOR)(GLFWwindow* window, GLFWmonitor* monitor, int xpos, int ypos, int width, int height, int refreshRate);
typedef void         (*GLFWSETWINDOWATTRIB)(GLFWwindow* window, int attrib, int value);
typedef void         (*GLFWFOCUSWINDOW)(GLFWwindow* window);
typedef void         (*GLFWHIDEWINDOW)(GLFWwindow* window);

extern GLFWINIT                          glfwInit;
extern GLFWTERMINATE                     glfwTerminate;
extern GLFWDEFAULTWINDOWHINTS            glfwDefaultWindowHints;
extern GLFWWINDOWHINT                    glfwWindowHint;
extern GLFWCREATEWINDOW                  glfwCreateWindow;
extern GLFWGETPRIMARYMONITOR             glfwGetPrimaryMonitor;
extern GLFWGETVIDEOMODE                  glfwGetVideoMode;
extern GLFWMAKECONTEXTCURRENT            glfwMakeContextCurrent;
extern GLFWGETERROR                      glfwGetError;
extern GLFWSWAPINTERVAL                  glfwSwapInterval;
extern GLFWGETMONITORS                   glfwGetMonitors;
extern GLFWGETMONITORWORKAREA            glfwGetMonitorWorkarea;
extern GLFWSETWINDOWSIZECALLBACK         glfwSetWindowSizeCallback;
extern GLFWSETFRAMEBUFFERSIZECALLBACK    glfwSetFramebufferSizeCallback;
extern GLFWSETWINDOWPOSCALLBACK          glfwSetWindowPosCallback;
extern GLFWSETWINDOWICONIFYCALLBACK      glfwSetWindowIconifyCallback;
extern GLFWSETWINDOWMAXIMIZECALLBACK     glfwSetWindowMaximizeCallback;
extern GLFWSETWINDOWFOCUSCALLBACK        glfwSetWindowFocusCallback;
extern GLFWSETWINDOWCONTENTSCALECALLBACK glfwSetWindowContentScaleCallback;
extern GLFWSETKEYCALLBACK                glfwSetKeyCallback;
extern GLFWSETMOUSEBUTTONCALLBACK        glfwSetMouseButtonCallback;
extern GLFWSETCURSORPOSCALLBACK          glfwSetCursorPosCallback;
extern GLFWSETINPUTMODE                  glfwSetInputMode;
extern GLFWDESTROYWINDOW                 glfwDestroyWindow;
extern GLFWGETTIME                       glfwGetTime;
extern GLFWINITALLOCATOR                 glfwInitAllocator;
extern GLFWSETERRORCALLBACK              glfwSetErrorCallback;
extern GLFWGETPROCADDRESS                glfwGetProcAddress;
extern GLFWSWAPBUFFERS                   glfwSwapBuffers;
extern GLFWPOLLEVENTS                    glfwPollEvents;
extern GLFWWAITEVENTS                    glfwWaitEvents;
extern GLFWWINDOWSHOULDCLOSE             glfwWindowShouldClose;
extern GLFWSETWINDOWSHOULDCLOSE          glfwSetWindowShouldClose;
extern GLFWGETWINDOWCONTENTSCALE         glfwGetWindowContentScale;
extern GLFWGETWINDOWMONITOR              glfwGetWindowMonitor;
extern GLFWGETWINDOWPOS                  glfwGetWindowPos;
extern GLFWGETMONITORPOS                 glfwGetMonitorPos;
extern GLFWSETWINDOWPOS                  glfwSetWindowPos;
extern GLFWICONIFYWINDOW                 glfwIconifyWindow;
extern GLFWGETWINDOWATTRIB               glfwGetWindowAttrib;
extern GLFWMAXIMIZEWINDOW                glfwMaximizeWindow;
extern GLFWSETWINDOWMONITOR              glfwSetWindowMonitor;
extern GLFWSETWINDOWATTRIB               glfwSetWindowAttrib;
extern GLFWFOCUSWINDOW                   glfwFocusWindow;
extern GLFWHIDEWINDOW                    glfwHideWindow;

int Ivy_GLFW_LoadFunctions(void);
void Ivy_GLFW_UnloadFunctions(void);

#ifdef __cplusplus
}
#endif

#endif