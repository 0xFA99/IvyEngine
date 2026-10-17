#ifndef IVY_INPUT_H
#define IVY_INPUT_H

#include "ivy/core/fwd.h"
#include "ivy/core/signal.h"
#include "ivy/core/types.h"

#include <stdbool.h>

#define IVY_MAX_KEYS 512
#define IVY_MAX_MOUSE_BUTTONS 3

#ifdef __cplusplus
extern "C" {
#endif

struct IvyInput
{
    struct {
        bool current_keys[IVY_MAX_KEYS];
        bool previous_keys[IVY_MAX_KEYS];
        IvyListener listener;
    } Keyboard;

    struct {
        bool current_buttons[IVY_MAX_MOUSE_BUTTONS];
        bool previous_buttons[IVY_MAX_MOUSE_BUTTONS];
        IvyListener listener;
    } MouseButton;

    struct {
        double x, y;
        double delta_x, delta_y;
        IvyListener listener;
    } MousePosition;

    struct {
        double x, y;
        IvyListener listener;
    } MouseScroll;
};

IvyStatus Ivy_Input_Init(IvyInput *restrict input, const IvyWindow *restrict window);
void Ivy_Input_Update(IvyInput *input);

IVY_INLINE bool Ivy_Input_IsKeyDown(const IvyInput *input, const int key)
{
    if (IVY_UNLIKELY(key < 0 || key >= IVY_MAX_KEYS)) return false;
    return input->Keyboard.current_keys[key];
}

IVY_INLINE bool Ivy_Input_IsKeyPressed(const IvyInput *input, const int key)
{
    if (IVY_UNLIKELY(key < 0 || key >= IVY_MAX_KEYS)) return false;
    return input->Keyboard.current_keys[key] && !input->Keyboard.previous_keys[key];
}

IVY_INLINE bool Ivy_Input_IsKeyReleased(const IvyInput *input, const int key)
{
    if (IVY_UNLIKELY(key < 0 || key >= IVY_MAX_KEYS)) return false;
    return !input->Keyboard.current_keys[key] && input->Keyboard.previous_keys[key];
}

#ifdef __cplusplus
}
#endif

#endif