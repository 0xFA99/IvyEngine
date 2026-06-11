#include "ivy/glfw_loader.h"

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    static HMODULE glfwLibrary = NULL;
#else
    #include <dlfcn.h>
    static void* glfwLibrary = NULL;
#endif

GLFWINIT                          glfwInit = NULL;
GLFWTERMINATE                     glfwTerminate = NULL;
GLFWDEFAULTWINDOWHINTS            glfwDefaultWindowHints = NULL;
GLFWWINDOWHINT                    glfwWindowHint = NULL;
GLFWCREATEWINDOW                  glfwCreateWindow = NULL;
GLFWGETPRIMARYMONITOR             glfwGetPrimaryMonitor = NULL;
GLFWGETVIDEOMODE                  glfwGetVideoMode = NULL;
GLFWMAKECONTEXTCURRENT            glfwMakeContextCurrent = NULL;
GLFWGETERROR                      glfwGetError = NULL;
GLFWSWAPINTERVAL                  glfwSwapInterval = NULL;
GLFWGETMONITORS                   glfwGetMonitors = NULL;
GLFWGETMONITORWORKAREA            glfwGetMonitorWorkarea = NULL;
GLFWSETWINDOWSIZECALLBACK         glfwSetWindowSizeCallback = NULL;
GLFWSETFRAMEBUFFERSIZECALLBACK    glfwSetFramebufferSizeCallback = NULL;
GLFWSETWINDOWPOSCALLBACK          glfwSetWindowPosCallback = NULL;
GLFWSETWINDOWICONIFYCALLBACK      glfwSetWindowIconifyCallback = NULL;
GLFWSETWINDOWMAXIMIZECALLBACK     glfwSetWindowMaximizeCallback = NULL;
GLFWSETWINDOWFOCUSCALLBACK        glfwSetWindowFocusCallback = NULL;
GLFWSETWINDOWCONTENTSCALECALLBACK glfwSetWindowContentScaleCallback = NULL;
GLFWSETKEYCALLBACK                glfwSetKeyCallback = NULL;
GLFWSETMOUSEBUTTONCALLBACK        glfwSetMouseButtonCallback = NULL;
GLFWSETCURSORPOSCALLBACK          glfwSetCursorPosCallback = NULL;
GLFWSETINPUTMODE                  glfwSetInputMode = NULL;
GLFWDESTROYWINDOW                 glfwDestroyWindow = NULL;
GLFWGETTIME                       glfwGetTime = NULL;
GLFWINITALLOCATOR                 glfwInitAllocator = NULL;
GLFWSETERRORCALLBACK              glfwSetErrorCallback = NULL;
GLFWGETPROCADDRESS                glfwGetProcAddress = NULL;
GLFWSWAPBUFFERS                   glfwSwapBuffers = NULL;
GLFWPOLLEVENTS                    glfwPollEvents = NULL;
GLFWWAITEVENTS                    glfwWaitEvents = NULL;
GLFWWINDOWSHOULDCLOSE             glfwWindowShouldClose = NULL;
GLFWSETWINDOWSHOULDCLOSE          glfwSetWindowShouldClose = NULL;
GLFWGETWINDOWCONTENTSCALE         glfwGetWindowContentScale = NULL;
GLFWGETWINDOWMONITOR              glfwGetWindowMonitor = NULL;
GLFWGETWINDOWPOS                  glfwGetWindowPos = NULL;
GLFWGETMONITORPOS                 glfwGetMonitorPos = NULL;
GLFWSETWINDOWPOS                  glfwSetWindowPos = NULL;
GLFWICONIFYWINDOW                 glfwIconifyWindow = NULL;
GLFWGETWINDOWATTRIB               glfwGetWindowAttrib = NULL;
GLFWMAXIMIZEWINDOW                glfwMaximizeWindow = NULL;
GLFWSETWINDOWMONITOR              glfwSetWindowMonitor = NULL;
GLFWSETWINDOWATTRIB               glfwSetWindowAttrib = NULL;
GLFWFOCUSWINDOW                   glfwFocusWindow = NULL;
GLFWHIDEWINDOW                    glfwHideWindow = NULL;

int Ivy_GLFW_LoadFunctions(void)
{
    if (glfwLibrary != NULL)
        return 0;

#ifdef _WIN32
    glfwLibrary = LoadLibraryA("lib/glfw3.dll");
    if (glfwLibrary == NULL)
        return -1;

    #define LOAD_FUNC(name) \
        *(void**)&name = (void*)GetProcAddress(glfwLibrary, #name); \
        if (name == NULL) { \
            FreeLibrary(glfwLibrary); \
            glfwLibrary = NULL; \
            return -1; \
        }
#else
    glfwLibrary = dlopen("libglfw.so", RTLD_NOW);
    if (glfwLibrary == NULL)
        return -1;

    #define LOAD_FUNC(name) \
        *(void**)&name = dlsym(glfwLibrary, #name); \
        if (name == NULL) { \
            dlclose(glfwLibrary); \
            glfwLibrary = NULL; \
            return -1; \
        }
#endif

    LOAD_FUNC(glfwInit);
    LOAD_FUNC(glfwTerminate);
    LOAD_FUNC(glfwDefaultWindowHints);
    LOAD_FUNC(glfwWindowHint);
    LOAD_FUNC(glfwCreateWindow);
    LOAD_FUNC(glfwGetPrimaryMonitor);
    LOAD_FUNC(glfwGetVideoMode);
    LOAD_FUNC(glfwMakeContextCurrent);
    LOAD_FUNC(glfwGetError);
    LOAD_FUNC(glfwSwapInterval);
    LOAD_FUNC(glfwGetMonitors);
    LOAD_FUNC(glfwGetMonitorWorkarea);
    LOAD_FUNC(glfwSetWindowSizeCallback);
    LOAD_FUNC(glfwSetFramebufferSizeCallback);
    LOAD_FUNC(glfwSetWindowPosCallback);
    LOAD_FUNC(glfwSetWindowIconifyCallback);
    LOAD_FUNC(glfwSetWindowMaximizeCallback);
    LOAD_FUNC(glfwSetWindowFocusCallback);
    LOAD_FUNC(glfwSetWindowContentScaleCallback);
    LOAD_FUNC(glfwSetKeyCallback);
    LOAD_FUNC(glfwSetMouseButtonCallback);
    LOAD_FUNC(glfwSetCursorPosCallback);
    LOAD_FUNC(glfwSetInputMode);
    LOAD_FUNC(glfwDestroyWindow);
    LOAD_FUNC(glfwGetTime);
    LOAD_FUNC(glfwInitAllocator);
    LOAD_FUNC(glfwSetErrorCallback);
    LOAD_FUNC(glfwGetProcAddress);
    LOAD_FUNC(glfwSwapBuffers);
    LOAD_FUNC(glfwPollEvents);
    LOAD_FUNC(glfwWaitEvents);
    LOAD_FUNC(glfwWindowShouldClose);
    LOAD_FUNC(glfwSetWindowShouldClose);
    LOAD_FUNC(glfwGetWindowContentScale);
    LOAD_FUNC(glfwGetWindowMonitor);
    LOAD_FUNC(glfwGetWindowPos);
    LOAD_FUNC(glfwGetMonitorPos);
    LOAD_FUNC(glfwSetWindowPos);
    LOAD_FUNC(glfwIconifyWindow);
    LOAD_FUNC(glfwGetWindowAttrib);
    LOAD_FUNC(glfwMaximizeWindow);
    LOAD_FUNC(glfwSetWindowMonitor);
    LOAD_FUNC(glfwSetWindowAttrib);
    LOAD_FUNC(glfwFocusWindow);
    LOAD_FUNC(glfwHideWindow);

#undef LOAD_FUNC

    return 0;
}

void Ivy_GLFW_UnloadFunctions(void)
{
    if (glfwLibrary == NULL)
        return;

#ifdef _WIN32
    FreeLibrary(glfwLibrary);
#else
    dlclose(glfwLibrary);
#endif
    glfwLibrary = NULL;
}