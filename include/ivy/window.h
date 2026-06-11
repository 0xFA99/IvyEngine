#ifndef IVY_WINDOW_H
#define IVY_WINDOW_H

#include "ivy/types.h"
#include "ivy/math.h"

#define GLFW_INCLUDE_NONE
#include "external/glfw3.h"

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

    IvySize     display;
    IvySize     screen;
    IvySize     render;
    IvySize     currentFBO;

    IvyPoint    position;
    IvySize     prevScreen;
    IvyPoint    prevPosition;

    IvyMatrix   screenScale;
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

void Ivy_Window_SizeCallback(GLFWwindow *window, int width, int height);
void Ivy_Window_FramebufferSizeCallback(GLFWwindow *window, int width, int height);
void Ivy_Window_ContentScaleCallback(GLFWwindow *window, float scaleX, float scaleY);
void Ivy_Window_PosCallback(GLFWwindow *window, int x, int y);
void Ivy_Window_IconifyCallback(GLFWwindow *window, int iconified);
void Ivy_Window_MaximizeCallback(GLFWwindow *window, int maximized);
void Ivy_Window_FocusCallback(GLFWwindow *window, int focused);

#ifdef __cplusplus
}
#endif

#endif
