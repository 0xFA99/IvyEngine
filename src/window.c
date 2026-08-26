#include "window.h"
#include "events.h"
#include "fwd.h"
#include "glfw.h"
#include "log.h"
#include "signal.h"

static void Ivy_GLFW_FramebufferSize_Callback(GLFWwindow *glfwWindow, int width, int height)
{
    IvyWindow *window = (IvyWindow *)glfwGetWindowUserPointer(glfwWindow);
    if (IVY_UNLIKELY(window == NULL)) return;

    window->width = width;
    window->height = height;

    IvyEventWindowResize event = {
        .window = window,
        .width = width,
        .height = height
    };

    Ivy_Signal_Emit(&window->events.resize, &event);
}

IvyStatus Ivy_Window_Init(IvyWindow *window, const char *title, int width, int height)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
    if (IVY_UNLIKELY(glfwWindow == NULL)) {
        IVY_LOG_ERROR("(GLFWwindow) Failed to created.");
        return IVY_ERROR_NULL_PTR;
    }

    glfwMakeContextCurrent(glfwWindow);

    window->handle = glfwWindow;
    window->title = title;
    window->width = width;
    window->height = height;

    Ivy_Signal_Init(&window->events.resize);

    glfwSetWindowUserPointer(glfwWindow, window);
    glfwSetFramebufferSizeCallback(glfwWindow, Ivy_GLFW_FramebufferSize_Callback);

    Ivy_Window_SetVSync(window, 1);

    IVY_LOG_INFO("(IvyWindow) Created successfully.");

    return IVY_SUCCESS;
}

void Ivy_Window_SetVSync(IvyWindow *window, const bool status)
{
    IVY_ASSERT(window, "(IvyWindow) is NULL.");

    if (window->vsync == status) return;

    window->vsync = status;
    glfwSwapInterval(status);

    IVY_LOG_INFO("(IvyWindow) VSync is %s.", (status) ? "Enabled" : "Disabled");
}

void Ivy_Window_Terminate(IvyWindow *window)
{
    IVY_ASSERT(window != NULL, "(IvyWindow) is NULL.");

    if (IVY_UNLIKELY(window->handle == NULL))
        return;

    glfwDestroyWindow(window->handle);
    window->handle = NULL;

    IVY_LOG_INFO("(IvyWindow) Terminated.");
}

