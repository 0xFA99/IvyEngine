#ifndef IVY_WINDOW_H
#define IVY_WINDOW_H

#include "ivy/types.h"
#include "ivy/math.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char *title;
    u32         flags;

    bool        ready;
    bool        shouldClose;
    bool        resizedLastFrame;
    bool        eventWaiting;
    bool        usingFBO;

    IvySize     display;
    IvySize     screen;
    IvySize     render;
    IvySize     currentFBO;

    IvyPoint    renderOffset;

    IvyPoint    position;
    IvySize     prevScreen;
    IvyPoint    prevPosition;

    IvyMatrix   screenScale;

    u32         msaaResolveFBO;
} IvyWindow;

bool        Ivy_Window_ShouldClose(void);
void        Ivy_Window_SetWindowState(u32 flags);
void        Ivy_Window_SetWindowPosition(int x, int y);
void        Ivy_Window_MinimizeWindow(void);
void        Ivy_Window_MaximizeWindow(void);
void        Ivy_Window_ToggleFullscreen(void);
void        Ivy_Window_ToggleBorderlessWindow(void);
IvyVector2  Ivy_Window_GetWindowScaleDPI(void);
int         Ivy_Window_GetCurrentMonitor(void);
bool        Ivy_Window_IsResized(void);

#ifdef __cplusplus
}
#endif

#endif
