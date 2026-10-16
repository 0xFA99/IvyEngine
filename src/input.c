#include "input.h"
#include "glfw.h"

#include <string.h>

#include "events.h"
#include "window.h"

static void IvyInput_HandleKeyEvent(IvyListener *listener, void *data);

IvyStatus Ivy_Input_Init(IvyInput *restrict input, const IvyWindow *restrict window)
{
    IVY_ASSERT(input != NULL, "(IvyInput) is NULL.");

    memset(input->current_keys, 0, sizeof(input->current_keys));
    memset(input->previous_keys, 0, sizeof(input->previous_keys));

    Ivy_Listener_Init(&input->key_listener, IvyInput_HandleKeyEvent);
    Ivy_Signal_Add(&window->events.key, &input->key_listener);

    return IVY_SUCCESS;
}

void Ivy_Input_Update(IvyInput *input)
{
    IVY_ASSERT(input != NULL, "(IvyInput) is NULL");

    memcpy(input->previous_keys, input->current_keys, sizeof(input->current_keys));
}

bool Ivy_Input_IsKeyDown(const IvyInput *input, const int key)
{
    if (IVY_UNLIKELY(key < 0 || key >= IVY_MAX_KEYS)) return false;
    return input->current_keys[key];
}

bool Ivy_Input_IsKeyPressed(const IvyInput *input, const int key)
{
    if (IVY_UNLIKELY(key < 0 || key >= IVY_MAX_KEYS)) return false;
    return input->current_keys[key] && !input->previous_keys[key];
}

bool Ivy_Input_IsKeyReleased(const IvyInput *input, const int key)
{
    if (IVY_UNLIKELY(key < 0 || key >= IVY_MAX_KEYS)) return false;
    return !input->current_keys[key] && input->previous_keys[key];
}

static void IvyInput_HandleKeyEvent(IvyListener *listener, void *data)
{
    IvyInput *input = IVY_CONTAINER_OF(listener, IvyInput, key_listener);
    const IvyEventKey *event = data;

    if (event->key >= 0 && event->key < IVY_MAX_KEYS)
    {
        if (event->action == GLFW_PRESS) input->current_keys[event->key] = true;
        else if (event->action == GLFW_RELEASE) input->current_keys[event->key] = false;
    }
}
