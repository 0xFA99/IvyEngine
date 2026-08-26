#ifndef IVY_WINDOW_H
#define IVY_WINDOW_H

#include "fwd.h"
#include "types.h"
#include "glfw.h"

#include <stdbool.h>

#include "signal.h"

#ifdef __cplusplus
extern "C" {
#endif

struct IvyWindow
{
    GLFWwindow *handle;
    const char *title;
    int width, height;
    bool vsync;

    struct {
        IvySignal resize;
    } events;
};

IvyStatus Ivy_Window_Init(IvyWindow *window, const char *title, int width, int height);
void Ivy_Window_SetVSync(IvyWindow *window, bool status);
void Ivy_Window_Terminate(IvyWindow *window);

#ifdef __cplusplus
}
#endif

#endif