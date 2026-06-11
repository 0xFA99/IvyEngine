#include "ivy/core.h"
#include "ivy/input.h"
#include "ivy/glfw_loader.h"

extern IvyPlatformData IVY_PLATFORM;
extern IvyCoreData IVY_CORE;

void Ivy_Input_KeyCallback(GLFWwindow *window, const int key, const int scancode, const int action, const int mods)
{
    (void)window; (void)scancode; (void)mods;

    if (key < 0 || key >= IVY_MAX_KEYBOARD_KEYS) return;

    if      (action == GLFW_PRESS)   IVY_CORE.input.keyboard.currentKeyState[key] = 1;
    else if (action == GLFW_RELEASE) IVY_CORE.input.keyboard.currentKeyState[key] = 0;
}

void Ivy_Input_MouseButtonCallback(GLFWwindow *window, const int button, const int action, const int mods)
{
    (void)window; (void)mods;

    if (button < 0 || button >= IVY_MAX_MOUSE_BUTTONS) return;

    if      (action == GLFW_PRESS)   IVY_CORE.input.mouse.currentButtonState[button] = 1;
    else if (action == GLFW_RELEASE) IVY_CORE.input.mouse.currentButtonState[button] = 0;
}

void Ivy_Input_CursorPosCallback(GLFWwindow *window, const double x, const double y)
{
    (void)window;

    IVY_CORE.input.mouse.currentPosition.x = (float)x;
    IVY_CORE.input.mouse.currentPosition.y = (float)y;
}

void Ivy_Input_PoolEvents(void)
{
    IVY_CORE.input.keyboard.keyPressedQueueCount = 0;
    IVY_CORE.input.keyboard.charPressedQueueCount = 0;

    for (int i = 0; i < IVY_MAX_KEYBOARD_KEYS; i++) {
        IVY_CORE.input.keyboard.prevKeyState[i] = IVY_CORE.input.keyboard.currentKeyState[i];
        IVY_CORE.input.keyboard.keyRepeatInFrame[i] = 0;
    }

    for (int i = 0; i < IVY_MAX_MOUSE_BUTTONS; i++) {
        IVY_CORE.input.mouse.prevButtonState[i] = IVY_CORE.input.mouse.currentButtonState[i];
    }

    IVY_CORE.input.mouse.prevWheelMove = IVY_CORE.input.mouse.currentWheelMove;
    IVY_CORE.input.mouse.currentWheelMove = (IvyVector2){ 0.0f };
    IVY_CORE.input.mouse.prevPosition = IVY_CORE.input.mouse.currentPosition;

    IVY_CORE.window.resizedLastFrame = false;

    if ((IVY_CORE.window.eventWaiting) || (FLAG_IS_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_MINIMIZED) && !FLAG_IS_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_ALWAYS_RUN))) {
        glfwWaitEvents();
        IVY_CORE.time.prev = Ivy_Core_GetTime();
    }
    else {
        glfwPollEvents();
    }

    IVY_CORE.window.shouldClose = glfwWindowShouldClose(IVY_PLATFORM.handle);
    glfwSetWindowShouldClose(IVY_PLATFORM.handle, GLFW_FALSE);
}