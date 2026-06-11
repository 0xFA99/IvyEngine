#ifndef IVY_INPUT_H
#define IVY_INPUT_H

#include "ivy/math.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IVY_MAX_KEYBOARD_KEYS 512
#define IVY_MAX_MOUSE_BUTTONS 8

#define MAX_KEY_PRESSED_QUEUE 16
#define MAX_CHAR_PRESSED_QUEUE 16

typedef enum {
    IVY_MOUSE_CURSOR_DEFAULT       = 0,
    IVY_MOUSE_CURSOR_ARROW         = 1,
    IVY_MOUSE_CURSOR_IBEAM         = 2,
    IVY_MOUSE_CURSOR_CROSSHAIR     = 3,
    IVY_MOUSE_CURSOR_POINTING_HAND = 4,
    IVY_MOUSE_CURSOR_RESIZE_EW     = 5,
    IVY_MOUSE_CURSOR_RESIZE_NS     = 6,
    IVY_MOUSE_CURSOR_RESIZE_NWSE   = 7,
    IVY_MOUSE_CURSOR_RESIZE_NESW   = 8,
    IVY_MOUSE_CURSOR_RESIZE_ALL    = 9,
    IVY_MOUSE_CURSOR_NOT_ALLOWED   = 10,
} IvyMouseCursor;

typedef struct {
    int  exitKey;
    char currentKeyState[IVY_MAX_KEYBOARD_KEYS];
    char prevKeyState[IVY_MAX_KEYBOARD_KEYS];
    char keyRepeatInFrame[IVY_MAX_KEYBOARD_KEYS];

    int keyPressedQueue[MAX_KEY_PRESSED_QUEUE];
    int keyPressedQueueCount;

    int charPressedQueue[MAX_CHAR_PRESSED_QUEUE];
    int charPressedQueueCount;
} IvyKeyboard;

typedef struct {
    IvyVector2    scale;
    IvyVector2    currentPosition;
    IvyVector2      prevPosition;
    IvyMouseCursor cursor;
    char          currentButtonState[IVY_MAX_MOUSE_BUTTONS];
    char          prevButtonState[IVY_MAX_MOUSE_BUTTONS];
    IvyVector2 currentWheelMove;
    IvyVector2 prevWheelMove;
} IvyMouse;

typedef struct {
    IvyKeyboard keyboard;
    IvyMouse    mouse;
} IvyInput;

void Ivy_Input_KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void Ivy_Input_MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
void Ivy_Input_CursorPosCallback(GLFWwindow *window, double x, double y);

void Ivy_Input_PoolEvents(void);

#ifdef __cplusplus
}
#endif

#endif
