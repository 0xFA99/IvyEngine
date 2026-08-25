#ifndef IVY_PLATFORM_INTERNAL_H
#define IVY_PLATFORM_INTERNAL_H

#include "ivy/core.h"
#include "ivy/platform.h"

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <X11/keysym.h>
    #include <wayland-client.h>
    #include <wayland-egl.h>
    #include <xkbcommon/xkbcommon.h>
    #include <EGL/egl.h>
    #include <EGL/eglext.h>
#endif

#include "ivy/gl.h"

typedef struct {
    IvyPlatformType type;

#ifndef _WIN32

    EGLDisplay eglDisplay;
    EGLContext eglContext;
    EGLSurface eglSurface;
    EGLConfig  eglConfig;
#endif

    bool shouldClose;

    union {
        struct {
            Display *display;
            Window window;
            Atom wmDeleteMessage;
            int screen;
            bool isFullscreen;
        } x11;
        struct {
            struct wl_display *display;
            struct wl_registry *registry;
            struct wl_compositor *compositor;
            struct wl_shm *shm;
            struct wl_surface *surface;
            struct xdg_wm_base *xdg_wm_base;
            struct xdg_surface *xdg_surface;
            struct xdg_toplevel *xdg_toplevel;
            struct wl_seat *seat;
            struct wl_keyboard *keyboard;
            struct wl_pointer *pointer;
            struct wl_egl_window *egl_window;
            struct xkb_context *xkb_context;
            struct xkb_keymap *xkb_keymap;
            struct xkb_state *xkb_state;
            int width;
            int height;
            bool configured;
            bool isFullscreen;
        } wayland;
#ifdef _WIN32
        struct {
            HWND hWnd;
            HDC hDC;
            HGLRC hRC;
            HINSTANCE hInstance;
            bool isFullscreen;
        } win32;
#endif
    };
} IvyPlatform;

extern IvyPlatform g_Platform;
extern IvyCoreData IVY_CORE;

#ifndef _WIN32

bool Ivy_EGL_Init(EGLNativeDisplayType native_display);
bool Ivy_EGL_CreateWindowSurface(EGLNativeWindowType native_window);
void Ivy_EGL_Shutdown(void);
#endif

bool Ivy_Platform_Init_Wayland(const char *title, int width, int height);
void Ivy_Platform_Shutdown_Wayland(void);
void Ivy_Platform_PollEvents_Wayland(void);
void Ivy_Platform_SetWindowPosition_Wayland(int x, int y);
void Ivy_Platform_SetWindowState_Wayland(u32 flags);
void Ivy_Platform_ToggleFullscreen_Wayland(void);
void Ivy_Platform_ToggleBorderlessWindow_Wayland(void);
void Ivy_Platform_SwapBuffers_Wayland(void);
int  Ivy_Platform_GetCurrentMonitor_Wayland(void);
IvyVector2 Ivy_Platform_GetWindowScaleDPI_Wayland(void);
void *Ivy_Platform_GetProcAddress_Wayland(const char *name);

bool Ivy_Platform_Init_X11(const char *title, int width, int height);
void Ivy_Platform_Shutdown_X11(void);
void Ivy_Platform_PollEvents_X11(void);
void Ivy_Platform_SetWindowPosition_X11(int x, int y);
void Ivy_Platform_SetWindowState_X11(u32 flags);
void Ivy_Platform_ToggleFullscreen_X11(void);
void Ivy_Platform_ToggleBorderlessWindow_X11(void);
void Ivy_Platform_SwapBuffers_X11(void);
int  Ivy_Platform_GetCurrentMonitor_X11(void);
IvyVector2 Ivy_Platform_GetWindowScaleDPI_X11(void);
void *Ivy_Platform_GetProcAddress_X11(const char *name);

bool Ivy_Platform_Init_Win32(const char *title, int width, int height);
void Ivy_Platform_Shutdown_Win32(void);
void Ivy_Platform_PollEvents_Win32(void);
void Ivy_Platform_SetWindowPosition_Win32(int x, int y);
void Ivy_Platform_SetWindowState_Win32(u32 flags);
void Ivy_Platform_ToggleFullscreen_Win32(void);
void Ivy_Platform_ToggleBorderlessWindow_Win32(void);
void Ivy_Platform_SwapBuffers_Win32(void);
int  Ivy_Platform_GetCurrentMonitor_Win32(void);
IvyVector2 Ivy_Platform_GetWindowScaleDPI_Win32(void);
void *Ivy_Platform_GetProcAddress_Win32(const char *name);

#endif
