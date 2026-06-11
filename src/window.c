#include "ivy/window.h"
#include "ivy/core.h"
#include "ivy/math.h"

extern IvyCoreData    IVY_CORE;
extern IvyPlatformData IVY_PLATFORM;

bool Ivy_Window_ShouldClose(void)
{
    if (IVY_CORE.window.ready) return IVY_CORE.window.shouldClose;
    return true;
}

IvyVector2 Ivy_Window_GetWindowScaleDPI(void)
{
    IvyVector2 scale = { 1.0f, 1.0f };

    if (FLAG_IS_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_HIGHDPI))
        glfwGetWindowContentScale(IVY_PLATFORM.handle, &scale.x, &scale.y);

    return scale;
}

int Ivy_Window_GetCurrentMonitor(void)
{
    int monitorCount = 0;
    GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);
    int index = 0;

    if (monitorCount < 1) return 0;

    if (FLAG_IS_SET(IVY_CORE.window.flags, IVY_FLAG_FULLSCREEN_MODE)) {
        GLFWmonitor *monitor = glfwGetWindowMonitor(IVY_PLATFORM.handle);
        for (int i = 0; i < monitorCount; i++) {
            if (monitors[i] == monitor) { index = i; break; }
        }
        return index;
    }

    int wcx = 0, wcy = 0;
    glfwGetWindowPos(IVY_PLATFORM.handle, &wcx, &wcy);
    wcx += (int)IVY_CORE.window.screen.width  / 2;
    wcy += (int)IVY_CORE.window.screen.height / 2;

    unsigned int closestDist = 0xFFFFFFFFu;

    for (int i = 0; i < monitorCount; i++) {
        int mx = 0, my = 0;
        glfwGetMonitorPos(monitors[i], &mx, &my);
        const GLFWvidmode *mode = glfwGetVideoMode(monitors[i]);
        if (!mode) continue;

        const int right  = mx + mode->width  - 1;
        const int bottom = my + mode->height - 1;

        if (wcx >= mx && wcx <= right && wcy >= my && wcy <= bottom) {
            return i;
        }

        const int xclosest = (wcx < mx) ? mx : (wcx > right  ? right  : wcx);
        const int yclosest = (wcy < my) ? my : (wcy > bottom ? bottom : wcy);
        const int dx = wcx - xclosest;
        const int dy = wcy - yclosest;
        const u32 dist = (u32)(dx*dx + dy*dy);

        if (dist < closestDist) { closestDist = dist; index = i; }
    }

    return index;
}

void Ivy_Window_SetWindowPosition(const int x, const int y)
{
    IVY_CORE.window.position.x = x;
    IVY_CORE.window.position.y = y;
    glfwSetWindowPos(IVY_PLATFORM.handle, x, y);
}

void Ivy_Window_MinimizeWindow(void)
{
    glfwIconifyWindow(IVY_PLATFORM.handle);
}

void Ivy_Window_MaximizeWindow(void)
{
    if (glfwGetWindowAttrib(IVY_PLATFORM.handle, GLFW_RESIZABLE) == GLFW_TRUE) {
        glfwMaximizeWindow(IVY_PLATFORM.handle);
        FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_MAXIMIZED);
    }
}

void Ivy_Window_ToggleFullscreen(void)
{
    if (!FLAG_IS_SET(IVY_CORE.window.flags, IVY_FLAG_FULLSCREEN_MODE)) {
        IVY_CORE.window.prevPosition = IVY_CORE.window.position;
        IVY_CORE.window.prevScreen   = IVY_CORE.window.screen;

        int monitorCount = 0;
        const int monitorIdx = Ivy_Window_GetCurrentMonitor();
        GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);
        GLFWmonitor  *monitor  = (monitorIdx < monitorCount) ? monitors[monitorIdx] : NULL;

        if (monitor) {
            const GLFWvidmode *mode = glfwGetVideoMode(monitor);
            IVY_CORE.window.display.width  = mode->width;
            IVY_CORE.window.display.height = mode->height;
            IVY_CORE.window.position = (IvyPoint){ 0, 0 };
            IVY_CORE.window.screen   = IVY_CORE.window.display;
            FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_FULLSCREEN_MODE);

#if defined(_GLFW_X11) || defined(_GLFW_WAYLAND)
            glfwSetWindowAttrib(IVY_PLATFORM.handle, GLFW_DECORATED, GLFW_FALSE);
            FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_UNDECORATED);
#endif
            glfwSetWindowMonitor(IVY_PLATFORM.handle, monitor, 0, 0,
                                 IVY_CORE.window.screen.width, IVY_CORE.window.screen.height,
                                 GLFW_DONT_CARE);
        }
    } else {
        IVY_CORE.window.position = IVY_CORE.window.prevPosition;
        IVY_CORE.window.screen   = IVY_CORE.window.prevScreen;
        FLAG_CLEAR(IVY_CORE.window.flags, IVY_FLAG_FULLSCREEN_MODE);

        glfwSetWindowMonitor(IVY_PLATFORM.handle, NULL,
                             IVY_CORE.window.position.x, IVY_CORE.window.position.y,
                             IVY_CORE.window.screen.width, IVY_CORE.window.screen.height,
                             GLFW_DONT_CARE);

#if defined(_GLFW_X11) || defined(_GLFW_WAYLAND)
        glfwSetWindowAttrib(IVY_PLATFORM.handle, GLFW_DECORATED, GLFW_TRUE);
        FLAG_CLEAR(IVY_CORE.window.flags, IVY_FLAG_WINDOW_UNDECORATED);
#endif
    }

    if (FLAG_IS_SET(IVY_CORE.window.flags, IVY_FLAG_VSYNC_HINT))
        glfwSwapInterval(1);
}

void Ivy_Window_ToggleBorderlessWindow(void)
{
    if (FLAG_IS_SET(IVY_CORE.window.flags, IVY_FLAG_FULLSCREEN_MODE))
        Ivy_Window_ToggleFullscreen();

    int monitorCount = 0;
    GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);
    const int monitorIdx = Ivy_Window_GetCurrentMonitor();

    if (monitorIdx < 0 || monitorIdx >= monitorCount) return;

    const GLFWvidmode *mode = glfwGetVideoMode(monitors[monitorIdx]);
    if (!mode) return;

    if (!FLAG_IS_SET(IVY_CORE.window.flags, IVY_FLAG_BORDERLESS_WINDOWED_MODE)) {
        IVY_CORE.window.prevPosition = IVY_CORE.window.position;
        IVY_CORE.window.prevScreen   = IVY_CORE.window.screen;

        glfwSetWindowAttrib(IVY_PLATFORM.handle, GLFW_DECORATED, GLFW_FALSE);
        FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_UNDECORATED);

        glfwGetMonitorPos(monitors[monitorIdx],
                          &IVY_CORE.window.position.x, &IVY_CORE.window.position.y);
        IVY_CORE.window.screen.width  = mode->width;
        IVY_CORE.window.screen.height = mode->height;

        glfwSetWindowMonitor(IVY_PLATFORM.handle, monitors[monitorIdx],
                             IVY_CORE.window.position.x, IVY_CORE.window.position.y,
                             IVY_CORE.window.screen.width, IVY_CORE.window.screen.height,
                             mode->refreshRate);
        glfwFocusWindow(IVY_PLATFORM.handle);
        FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_BORDERLESS_WINDOWED_MODE);
    } else {
        IVY_CORE.window.position = IVY_CORE.window.prevPosition;
        IVY_CORE.window.screen   = IVY_CORE.window.prevScreen;

        glfwSetWindowAttrib(IVY_PLATFORM.handle, GLFW_DECORATED, GLFW_TRUE);
        FLAG_CLEAR(IVY_CORE.window.flags, IVY_FLAG_WINDOW_UNDECORATED);

        glfwSetWindowMonitor(IVY_PLATFORM.handle, NULL,
                             IVY_CORE.window.position.x, IVY_CORE.window.position.y,
                             IVY_CORE.window.screen.width, IVY_CORE.window.screen.height,
                             mode->refreshRate);
        glfwFocusWindow(IVY_PLATFORM.handle);
        FLAG_CLEAR(IVY_CORE.window.flags, IVY_FLAG_BORDERLESS_WINDOWED_MODE);
    }
}

void Ivy_Window_SetWindowState(const u32 flags)
{
    IVY_CHECK(IVY_CORE.window.ready, "[ERROR] SetWindowState called before window is ready!");

#define CHANGED_AND_SET(f) \
    (FLAG_IS_SET(IVY_CORE.window.flags, f) != FLAG_IS_SET(flags, f)) && FLAG_IS_SET(flags, f)

    if (CHANGED_AND_SET(IVY_FLAG_VSYNC_HINT)) {
        glfwSwapInterval(1);
        FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_VSYNC_HINT);
    }
    if (CHANGED_AND_SET(IVY_FLAG_BORDERLESS_WINDOWED_MODE))
        Ivy_Window_ToggleBorderlessWindow();

    if (CHANGED_AND_SET(IVY_FLAG_FULLSCREEN_MODE))
        Ivy_Window_ToggleFullscreen();

    if (CHANGED_AND_SET(IVY_FLAG_WINDOW_RESIZABLE)) {
        glfwSetWindowAttrib(IVY_PLATFORM.handle, GLFW_RESIZABLE, GLFW_TRUE);
        FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_RESIZABLE);
    }
    if (CHANGED_AND_SET(IVY_FLAG_WINDOW_UNDECORATED)) {
        glfwSetWindowAttrib(IVY_PLATFORM.handle, GLFW_DECORATED, GLFW_FALSE);
        FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_UNDECORATED);
    }
    if (CHANGED_AND_SET(IVY_FLAG_WINDOW_HIDDEN)) {
        glfwHideWindow(IVY_PLATFORM.handle);
        FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_HIDDEN);
    }
    if (CHANGED_AND_SET(IVY_FLAG_WINDOW_MINIMIZED))
        Ivy_Window_MinimizeWindow();

    if (CHANGED_AND_SET(IVY_FLAG_WINDOW_MAXIMIZED))
        Ivy_Window_MaximizeWindow();

    if (CHANGED_AND_SET(IVY_FLAG_WINDOW_UNFOCUSED)) {
        glfwSetWindowAttrib(IVY_PLATFORM.handle, GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
        FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_UNFOCUSED);
    }
    if (CHANGED_AND_SET(IVY_FLAG_WINDOW_TOPMOST)) {
        glfwSetWindowAttrib(IVY_PLATFORM.handle, GLFW_FLOATING, GLFW_TRUE);
        FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_TOPMOST);
    }
    if (CHANGED_AND_SET(IVY_FLAG_WINDOW_ALWAYS_RUN))
        FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_ALWAYS_RUN);

    if (CHANGED_AND_SET(IVY_FLAG_WINDOW_MOUSE_PASSTHROUGH)) {
        glfwSetWindowAttrib(IVY_PLATFORM.handle, GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
        FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_MOUSE_PASSTHROUGH);
    }

    if (CHANGED_AND_SET(IVY_FLAG_WINDOW_TRANSPARENT))
        IVY_CHECK(false, "[WARNING] Framebuffer transparency can only be set before window init!");
    if (CHANGED_AND_SET(IVY_FLAG_WINDOW_HIGHDPI))
        IVY_CHECK(false, "[WARNING] High DPI can only be set before window init!");
    if (CHANGED_AND_SET(IVY_FLAG_MSAA_4X_HINT))
        IVY_CHECK(false, "[WARNING] MSAA can only be set before window init!");
    if (CHANGED_AND_SET(IVY_FLAG_INTERLACED_HINT))
        IVY_CHECK(false, "[WARNING] Interlaced mode can only be set before window init!");

#undef CHANGED_AND_SET
}

void Ivy_Window_SizeCallback(GLFWwindow *window, int width, int height)
{
    (void)window; (void)width; (void)height;
}

void Ivy_Window_FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    (void)window;
    if (width == 0 || height == 0) return;

    IVY_CORE.window.render.width  = width;
    IVY_CORE.window.render.height = height;
    IVY_CORE.window.currentFBO    = IVY_CORE.window.render;
    IVY_CORE.window.resizedLastFrame = true;

    if (FLAG_IS_SET(IVY_CORE.window.flags, IVY_FLAG_FULLSCREEN_MODE)) {
        IVY_CORE.window.screen.width  = width;
        IVY_CORE.window.screen.height = height;
        IVY_CORE.window.screenScale   = Ivy_Math_MatrixScale(1.0f, 1.0f, 1.0f);
    }
}

void Ivy_Window_ContentScaleCallback(GLFWwindow *window, float scaleX, float scaleY)
{
    (void)window;
    IVY_CORE.window.render.width  = (u32)(IVY_CORE.window.screen.width  * scaleX);
    IVY_CORE.window.render.height = (u32)(IVY_CORE.window.screen.height * scaleY);
    IVY_CORE.window.currentFBO    = IVY_CORE.window.render;
    IVY_CORE.window.screenScale   = Ivy_Math_MatrixScale(scaleX, scaleY, 1.0f);
}

void Ivy_Window_PosCallback(GLFWwindow *window, int x, int y)
{
    (void)window;
    IVY_CORE.window.position.x = x;
    IVY_CORE.window.position.y = y;
}

void Ivy_Window_IconifyCallback(GLFWwindow *window, int iconified)
{
    (void)window;
    if (iconified) FLAG_SET(IVY_CORE.window.flags,   IVY_FLAG_WINDOW_MINIMIZED);
    else           FLAG_CLEAR(IVY_CORE.window.flags, IVY_FLAG_WINDOW_MINIMIZED);
}

void Ivy_Window_MaximizeCallback(GLFWwindow *window, int maximized)
{
    (void)window;
    if (maximized) FLAG_SET(IVY_CORE.window.flags,   IVY_FLAG_WINDOW_MAXIMIZED);
    else           FLAG_CLEAR(IVY_CORE.window.flags, IVY_FLAG_WINDOW_MAXIMIZED);
}

void Ivy_Window_FocusCallback(GLFWwindow *window, int focused)
{
    (void)window;
    if (focused) FLAG_CLEAR(IVY_CORE.window.flags, IVY_FLAG_WINDOW_UNFOCUSED);
    else         FLAG_SET(IVY_CORE.window.flags,   IVY_FLAG_WINDOW_UNFOCUSED);
}
