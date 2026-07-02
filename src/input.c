#include "ivy/core.h"
#include "ivy/input.h"
#include "ivy/platform.h"

extern IvyCoreData IVY_CORE;

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
    IVY_CORE.input.mouse.currentWheelMove = (IvyVector2){ 0.0f, 0.0f };
    IVY_CORE.input.mouse.prevPosition = IVY_CORE.input.mouse.currentPosition;

    IVY_CORE.window.resizedLastFrame = false;

    Ivy_Platform_PollEvents();

    IVY_CORE.window.shouldClose = Ivy_Platform_ShouldClose();
}