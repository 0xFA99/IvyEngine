#ifndef IVY_WINDOW_H
#define IVY_WINDOW_H

#include "ivy/core/fwd.h"
#include "ivy/core/signal.h"
#include "ivy/core/types.h"
#include "ivy/platform/glfw_loader.h"

#include <stdbool.h>

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
        IvySignal key;
        IvySignal mouse_button;
        IvySignal mouse_move;
        IvySignal mouse_scroll;
    } events;
};

IvyStatus Ivy_Window_Init(IvyWindow *window, const char *title, int width, int height);
void Ivy_Window_SetVSync(IvyWindow *window, bool status);
void Ivy_Window_Terminate(IvyWindow *window);

#ifdef __cplusplus
}
#endif

#endif