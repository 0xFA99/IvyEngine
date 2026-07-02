#include "ivy/window.h"
#include "ivy/core.h"
#include "ivy/math.h"
#include "ivy/platform.h"

#include <string.h>

extern IvyCoreData IVY_CORE;

bool Ivy_Window_ShouldClose(void)
{
    if (IVY_CORE.window.ready) return Ivy_Platform_ShouldClose();
    return true;
}

IvyVector2 Ivy_Window_GetWindowScaleDPI(void)
{
    if (IVY_CORE.window.ready) return Ivy_Platform_GetWindowScaleDPI();
    return (IvyVector2){ 1.0f, 1.0f };
}

int Ivy_Window_GetCurrentMonitor(void)
{
    if (IVY_CORE.window.ready) return Ivy_Platform_GetCurrentMonitor();
    return 0;
}

void Ivy_Window_SetWindowPosition(const int x, const int y)
{
    IVY_CORE.window.position.x = x;
    IVY_CORE.window.position.y = y;
    if (IVY_CORE.window.ready) {
        Ivy_Platform_SetWindowPosition(x, y);
    }
}

void Ivy_Window_MinimizeWindow(void)
{
    IVY_FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_MINIMIZED);
    if (IVY_CORE.window.ready) {
        Ivy_Platform_SetWindowState(IVY_CORE.window.flags);
    }
}

void Ivy_Window_MaximizeWindow(void)
{
    IVY_FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_MAXIMIZED);
    if (IVY_CORE.window.ready) {
        Ivy_Platform_SetWindowState(IVY_CORE.window.flags);
    }
}

void Ivy_Window_ToggleFullscreen(void)
{
    if (IVY_CORE.window.ready) {
        Ivy_Platform_ToggleFullscreen();
    }
}

void Ivy_Window_ToggleBorderlessWindow(void)
{
    if (IVY_CORE.window.ready) {
        Ivy_Platform_ToggleBorderlessWindow();
    }
}

void Ivy_Window_SetWindowState(const u32 flags)
{
    IVY_CORE.window.flags = flags;
    if (IVY_CORE.window.ready) {
        Ivy_Platform_SetWindowState(flags);
    }
}

bool Ivy_Window_IsResized(void)
{
    return IVY_CORE.window.resizedLastFrame;
}
