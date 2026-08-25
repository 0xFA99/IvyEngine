#include "platform_internal.h"
#include <time.h>
#include <stdio.h>

IvyPlatform g_Platform = {0};

#ifndef _WIN32

static bool IsWaylandAvailable(void)
{
    struct wl_display *display = wl_display_connect(NULL);
    if (display) {
        wl_display_disconnect(display);
        return true;
    }
    return false;
}
#endif

bool Ivy_Platform_Init(const char *title, int width, int height)
{
#ifdef _WIN32
    g_Platform.type = IVY_PLATFORM_WIN32;
    printf("[Platform] Initializing Win32 backend...\n");
    return Ivy_Platform_Init_Win32(title, width, height);
#else
    if (IsWaylandAvailable()) {
        g_Platform.type = IVY_PLATFORM_WAYLAND;
        printf("[Platform] Initializing Wayland backend...\n");
        if (Ivy_Platform_Init_Wayland(title, width, height)) {
            return true;
        }
    }
    g_Platform.type = IVY_PLATFORM_X11;
    printf("[Platform] Initializing X11 backend...\n");
    return Ivy_Platform_Init_X11(title, width, height);
#endif
}

void Ivy_Platform_Shutdown(void)
{
    switch (g_Platform.type) {
#ifdef _WIN32
        case IVY_PLATFORM_WIN32:   Ivy_Platform_Shutdown_Win32(); break;
#else
        case IVY_PLATFORM_WAYLAND: Ivy_Platform_Shutdown_Wayland(); break;
        case IVY_PLATFORM_X11:     Ivy_Platform_Shutdown_X11(); break;
#endif
    }
}

void Ivy_Platform_PollEvents(void)
{
    switch (g_Platform.type) {
#ifdef _WIN32
        case IVY_PLATFORM_WIN32:   Ivy_Platform_PollEvents_Win32(); break;
#else
        case IVY_PLATFORM_WAYLAND: Ivy_Platform_PollEvents_Wayland(); break;
        case IVY_PLATFORM_X11:     Ivy_Platform_PollEvents_X11(); break;
#endif
    }
}

void Ivy_Platform_SetWindowPosition(int x, int y)
{
    switch (g_Platform.type) {
#ifdef _WIN32
        case IVY_PLATFORM_WIN32:   Ivy_Platform_SetWindowPosition_Win32(x, y); break;
#else
        case IVY_PLATFORM_WAYLAND: Ivy_Platform_SetWindowPosition_Wayland(x, y); break;
        case IVY_PLATFORM_X11:     Ivy_Platform_SetWindowPosition_X11(x, y); break;
#endif
    }
}

void Ivy_Platform_SetWindowState(u32 flags)
{
    switch (g_Platform.type) {
#ifdef _WIN32
        case IVY_PLATFORM_WIN32:   Ivy_Platform_SetWindowState_Win32(flags); break;
#else
        case IVY_PLATFORM_WAYLAND: Ivy_Platform_SetWindowState_Wayland(flags); break;
        case IVY_PLATFORM_X11:     Ivy_Platform_SetWindowState_X11(flags); break;
#endif
    }
}

void Ivy_Platform_ToggleFullscreen(void)
{
    switch (g_Platform.type) {
#ifdef _WIN32
        case IVY_PLATFORM_WIN32:   Ivy_Platform_ToggleFullscreen_Win32(); break;
#else
        case IVY_PLATFORM_WAYLAND: Ivy_Platform_ToggleFullscreen_Wayland(); break;
        case IVY_PLATFORM_X11:     Ivy_Platform_ToggleFullscreen_X11(); break;
#endif
    }
}

void Ivy_Platform_ToggleBorderlessWindow(void)
{
    switch (g_Platform.type) {
#ifdef _WIN32
        case IVY_PLATFORM_WIN32:   Ivy_Platform_ToggleBorderlessWindow_Win32(); break;
#else
        case IVY_PLATFORM_WAYLAND: Ivy_Platform_ToggleBorderlessWindow_Wayland(); break;
        case IVY_PLATFORM_X11:     Ivy_Platform_ToggleBorderlessWindow_X11(); break;
#endif
    }
}

void Ivy_Platform_SwapBuffers(void)
{
    switch (g_Platform.type) {
#ifdef _WIN32
        case IVY_PLATFORM_WIN32:   Ivy_Platform_SwapBuffers_Win32(); break;
#else
        case IVY_PLATFORM_WAYLAND: Ivy_Platform_SwapBuffers_Wayland(); break;
        case IVY_PLATFORM_X11:     Ivy_Platform_SwapBuffers_X11(); break;
#endif
    }
}

int Ivy_Platform_GetCurrentMonitor(void)
{
    switch (g_Platform.type) {
#ifdef _WIN32
        case IVY_PLATFORM_WIN32:   return Ivy_Platform_GetCurrentMonitor_Win32();
#else
        case IVY_PLATFORM_WAYLAND: return Ivy_Platform_GetCurrentMonitor_Wayland();
        case IVY_PLATFORM_X11:     return Ivy_Platform_GetCurrentMonitor_X11();
#endif
    }
    return 0;
}

IvyVector2 Ivy_Platform_GetWindowScaleDPI(void)
{
    switch (g_Platform.type) {
#ifdef _WIN32
        case IVY_PLATFORM_WIN32:   return Ivy_Platform_GetWindowScaleDPI_Win32();
#else
        case IVY_PLATFORM_WAYLAND: return Ivy_Platform_GetWindowScaleDPI_Wayland();
        case IVY_PLATFORM_X11:     return Ivy_Platform_GetWindowScaleDPI_X11();
#endif
    }
    return (IvyVector2){1.0f, 1.0f};
}

bool Ivy_Platform_ShouldClose(void)
{
    return g_Platform.shouldClose;
}

void *Ivy_Platform_GetProcAddress(const char *name)
{
    switch (g_Platform.type) {
#ifdef _WIN32
        case IVY_PLATFORM_WIN32:   return Ivy_Platform_GetProcAddress_Win32(name);
#else
        case IVY_PLATFORM_WAYLAND: return Ivy_Platform_GetProcAddress_Wayland(name);
        case IVY_PLATFORM_X11:     return Ivy_Platform_GetProcAddress_X11(name);
#endif
    }
    return NULL;
}

double Ivy_Platform_GetTime(void)
{
#ifdef _WIN32
    static LARGE_INTEGER frequency;
    static BOOL frequencyInitialized = FALSE;
    if (!frequencyInitialized) {
        QueryPerformanceFrequency(&frequency);
        frequencyInitialized = TRUE;
    }
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)frequency.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
#endif
}

#ifndef _WIN32

#ifndef EGL_OPENGL_API
#define EGL_OPENGL_API 0x30A2
#endif
#ifndef EGL_OPENGL_BIT
#define EGL_OPENGL_BIT 0x0008
#endif
#ifndef EGL_CONTEXT_MAJOR_VERSION
#define EGL_CONTEXT_MAJOR_VERSION 0x3098
#endif
#ifndef EGL_CONTEXT_MINOR_VERSION
#define EGL_CONTEXT_MINOR_VERSION 0x30FB
#endif
#ifndef EGL_CONTEXT_OPENGL_PROFILE_MASK
#define EGL_CONTEXT_OPENGL_PROFILE_MASK 0x30FD
#endif
#ifndef EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT
#define EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT 0x00000001
#endif

bool Ivy_EGL_Init(EGLNativeDisplayType native_display)
{
    g_Platform.eglDisplay = eglGetDisplay(native_display);
    if (g_Platform.eglDisplay == EGL_NO_DISPLAY) {
        fprintf(stderr, "[EGL] Failed to get display\n");
        return false;
    }

    EGLint major, minor;
    if (!eglInitialize(g_Platform.eglDisplay, &major, &minor)) {
        fprintf(stderr, "[EGL] Failed to initialize EGL\n");
        return false;
    }
    printf("[EGL] Initialized EGL %d.%d\n", major, minor);

    eglBindAPI(EGL_OPENGL_API);

    #ifndef EGL_SAMPLE_BUFFERS
    #define EGL_SAMPLE_BUFFERS 0x30E2
    #endif
    #ifndef EGL_SAMPLES
    #define EGL_SAMPLES 0x30E4
    #endif

    EGLint configAttribs[24] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_STENCIL_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    };
    int attribIndex = 16;
    if (IVY_CORE.window.flags & IVY_FLAG_MSAA_4X_HINT) {
        configAttribs[attribIndex++] = EGL_SAMPLE_BUFFERS;
        configAttribs[attribIndex++] = 1;
        configAttribs[attribIndex++] = EGL_SAMPLES;
        configAttribs[attribIndex++] = 4;
    }
    configAttribs[attribIndex] = EGL_NONE;

    EGLint numConfigs;
    if (!eglChooseConfig(g_Platform.eglDisplay, configAttribs, &g_Platform.eglConfig, 1, &numConfigs) || numConfigs < 1) {
        if (IVY_CORE.window.flags & IVY_FLAG_MSAA_4X_HINT) {
            printf("[EGL WARNING] MSAA 4x not supported, falling back to no MSAA\n");
            configAttribs[16] = EGL_NONE;
            if (eglChooseConfig(g_Platform.eglDisplay, configAttribs, &g_Platform.eglConfig, 1, &numConfigs) && numConfigs >= 1) {
                goto config_chosen;
            }
        }

        configAttribs[9] = 16;
        if (!eglChooseConfig(g_Platform.eglDisplay, configAttribs, &g_Platform.eglConfig, 1, &numConfigs) || numConfigs < 1) {
            fprintf(stderr, "[EGL] Failed to choose EGL config\n");
            return false;
        }
    }
config_chosen:

    EGLint contextAttribs[] = {
        EGL_CONTEXT_MAJOR_VERSION, 3,
        EGL_CONTEXT_MINOR_VERSION, 3,
        EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
        EGL_NONE
    };

    g_Platform.eglContext = eglCreateContext(g_Platform.eglDisplay, g_Platform.eglConfig, EGL_NO_CONTEXT, contextAttribs);
    if (g_Platform.eglContext == EGL_NO_CONTEXT) {
        fprintf(stderr, "[EGL] Failed to create OpenGL 3.3 Core context\n");
        return false;
    }

    return true;
}

bool Ivy_EGL_CreateWindowSurface(EGLNativeWindowType native_window)
{
    g_Platform.eglSurface = eglCreateWindowSurface(g_Platform.eglDisplay, g_Platform.eglConfig, native_window, NULL);
    if (g_Platform.eglSurface == EGL_NO_SURFACE) {
        fprintf(stderr, "[EGL] Failed to create window surface\n");
        return false;
    }

    if (!eglMakeCurrent(g_Platform.eglDisplay, g_Platform.eglSurface, g_Platform.eglSurface, g_Platform.eglContext)) {
        fprintf(stderr, "[EGL] Failed to make EGL context current\n");
        return false;
    }

    return true;
}

void Ivy_EGL_Shutdown(void)
{
    if (g_Platform.eglDisplay != EGL_NO_DISPLAY) {
        eglMakeCurrent(g_Platform.eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (g_Platform.eglSurface != EGL_NO_SURFACE) {
            eglDestroySurface(g_Platform.eglDisplay, g_Platform.eglSurface);
            g_Platform.eglSurface = EGL_NO_SURFACE;
        }
        if (g_Platform.eglContext != EGL_NO_CONTEXT) {
            eglDestroyContext(g_Platform.eglDisplay, g_Platform.eglContext);
            g_Platform.eglContext = EGL_NO_CONTEXT;
        }
        eglTerminate(g_Platform.eglDisplay);
        g_Platform.eglDisplay = EGL_NO_DISPLAY;
    }
}

#endif
