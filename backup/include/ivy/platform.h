#ifndef IVY_PLATFORM_H
#define IVY_PLATFORM_H

#include "ivy/types.h"
#include "ivy/math.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    IVY_PLATFORM_X11,
    IVY_PLATFORM_WAYLAND,
    IVY_PLATFORM_WIN32
} IvyPlatformType;

bool   Ivy_Platform_Init(const char *title, int width, int height);
void   Ivy_Platform_Shutdown(void);
void   Ivy_Platform_PollEvents(void);
double Ivy_Platform_GetTime(void);
void   Ivy_Platform_SetWindowPosition(int x, int y);
void   Ivy_Platform_SetWindowState(u32 flags);
void   Ivy_Platform_ToggleFullscreen(void);
void   Ivy_Platform_ToggleBorderlessWindow(void);
void   Ivy_Platform_SwapBuffers(void);
int    Ivy_Platform_GetCurrentMonitor(void);
IvyVector2 Ivy_Platform_GetWindowScaleDPI(void);
bool   Ivy_Platform_ShouldClose(void);
void  *Ivy_Platform_GetProcAddress(const char *name);

#ifdef __cplusplus
}
#endif

#endif
