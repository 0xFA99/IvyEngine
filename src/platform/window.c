#include "ivy/platform/window.h"
#include "ivy/core/fwd.h"
#include "ivy/core/log.h"
#include "ivy/core/signal.h"
#include "ivy/platform/events.h"
#include "ivy/platform/glfw_loader.h"

static void Ivy_GLFW_FramebufferSize_Callback(GLFWwindow *glfwWindow, int width, int height);
static void Ivy_GLFW_Key_Callback(GLFWwindow *glfwWindow, int key, int scancode, int action, int mods);
static void Ivy_GLFW_MouseButton_Callback(GLFWwindow *glfwWindow, int button, int action, int mods);
static void Ivy_GLFW_MouseMove_Callback(GLFWwindow *glfwWindow, double xpos, double ypos);
static void Ivy_GLFW_MouseScroll_Callback(GLFWwindow *glfwWindow, double xoffset, double yoffset);

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
    Ivy_Signal_Init(&window->events.key);
    Ivy_Signal_Init(&window->events.mouse_button);
    Ivy_Signal_Init(&window->events.mouse_move);
    Ivy_Signal_Init(&window->events.mouse_scroll);

    glfwSetWindowUserPointer(glfwWindow, window);
    glfwSetFramebufferSizeCallback(glfwWindow, Ivy_GLFW_FramebufferSize_Callback);
    glfwSetKeyCallback(glfwWindow, Ivy_GLFW_Key_Callback);
    glfwSetMouseButtonCallback(glfwWindow, Ivy_GLFW_MouseButton_Callback);
    glfwSetCursorPosCallback(glfwWindow, Ivy_GLFW_MouseMove_Callback);
    glfwSetScrollCallback(glfwWindow, Ivy_GLFW_MouseScroll_Callback);

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

static void Ivy_GLFW_Key_Callback(GLFWwindow *glfwWindow, int key, int scancode, int action, int mods)
{
    IvyWindow *window = glfwGetWindowUserPointer(glfwWindow);
    if (IVY_UNLIKELY(window == NULL)) return;

    IvyEventKey event = {
        .key = key,
        .scancode = scancode,
        .action = action,
        .mods = mods
    };

    Ivy_Signal_Emit(&window->events.key, &event);
}

static void Ivy_GLFW_MouseButton_Callback(GLFWwindow *glfwWindow, int button, int action, int mods)
{
    IvyWindow *window = glfwGetWindowUserPointer(glfwWindow);
    if (!window) return;

    IvyEventMouseButton event = {
        .button = button,
        .action = action,
        .mods = mods
    };

    Ivy_Signal_Emit(&window->events.mouse_button, &event);
}

static void Ivy_GLFW_MouseMove_Callback(GLFWwindow *glfwWindow, double xpos, double ypos)
{
    IvyWindow *window = glfwGetWindowUserPointer(glfwWindow);
    if (!window) return;

    IvyEventMouseMove event = {
        .x = xpos,
        .y = ypos
    };

    Ivy_Signal_Emit(&window->events.mouse_move, &event);
}

static void Ivy_GLFW_MouseScroll_Callback(GLFWwindow *glfwWindow, double xoffset, double yoffset)
{
    IvyWindow *window = glfwGetWindowUserPointer(glfwWindow);
    if (!window) return;

    IvyEventMouseScroll event = {
        .x_offset = xoffset,
        .y_offset = yoffset
    };

    Ivy_Signal_Emit(&window->events.mouse_scroll, &event);
}
