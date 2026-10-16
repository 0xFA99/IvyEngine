#include "glfw.h"
#include "window.h"
#include "input.h"
#include "events.h"

#include <string.h>

static void IvyInput_HandleKeyEvent(IvyListener *listener, void *data);
static void IvyInput_HandleMouseButtonEvent(IvyListener *listener, void *data);
static void IvyInput_HandleMouseMoveEvent(IvyListener *listener, void *data);
static void IvyInput_HandleMouseScrollEvent(IvyListener *listener, void *data);

IvyStatus Ivy_Input_Init(IvyInput *restrict input, const IvyWindow *restrict window)
{
    IVY_ASSERT(input != NULL, "(IvyInput) is NULL.");

    memset(input->Keyboard.current_keys, 0, sizeof(input->Keyboard.current_keys));
    memset(input->Keyboard.previous_keys, 0, sizeof(input->Keyboard.previous_keys));

    Ivy_Listener_Init(&input->Keyboard.listener, IvyInput_HandleKeyEvent);
    Ivy_Signal_Add(&window->events.key, &input->Keyboard.listener);

    Ivy_Listener_Init(&input->MouseButton.listener, IvyInput_HandleMouseButtonEvent);
    Ivy_Signal_Add(&window->events.mouse_button, &input->MouseButton.listener);

    Ivy_Listener_Init(&input->MousePosition.listener, IvyInput_HandleMouseMoveEvent);
    Ivy_Signal_Add(&window->events.mouse_move, &input->MousePosition.listener);

    Ivy_Listener_Init(&input->MouseScroll.listener, IvyInput_HandleMouseScrollEvent);
    Ivy_Signal_Add(&window->events.mouse_scroll, &input->MouseScroll.listener);

    return IVY_SUCCESS;
}

void Ivy_Input_Update(IvyInput *input)
{
    IVY_ASSERT(input != NULL, "(IvyInput) is NULL");

    memcpy(input->Keyboard.previous_keys, input->Keyboard.current_keys, sizeof(input->Keyboard.current_keys));
    memcpy(input->MouseButton.previous_buttons, input->MouseButton.current_buttons, sizeof(input->MouseButton.current_buttons));

    input->MousePosition.delta_x = 0.0;
    input->MousePosition.delta_y = 0.0;
    input->MouseScroll.x = 0.0;
    input->MouseScroll.y = 0.0;
}

static void IvyInput_HandleKeyEvent(IvyListener *listener, void *data)
{
    IvyInput *input = IVY_CONTAINER_OF(listener, IvyInput, Keyboard.listener);
    const IvyEventKey *event = data;

    if (event->key >= 0 && event->key < IVY_MAX_KEYS)
    {
        if (event->action == GLFW_PRESS) input->Keyboard.current_keys[event->key] = true;
        else if (event->action == GLFW_RELEASE) input->Keyboard.current_keys[event->key] = false;
    }
}

static void IvyInput_HandleMouseButtonEvent(IvyListener *listener, void *data)
{
    IvyInput *input = IVY_CONTAINER_OF(listener, IvyInput, MouseButton.listener);
    const IvyEventMouseButton *event = data;

    if (event->button >= 0 && event->button < IVY_MAX_MOUSE_BUTTONS)
    {
        if (event->action == GLFW_PRESS) input->MouseButton.current_buttons[event->button] = true;
        else if (event->action == GLFW_RELEASE) input->MouseButton.current_buttons[event->button] = false;
    }
}

static void IvyInput_HandleMouseMoveEvent(IvyListener *listener, void *data)
{
    IvyInput *input = IVY_CONTAINER_OF(listener, IvyInput, MousePosition.listener);
    const IvyEventMouseMove *event = data;

    input->MousePosition.delta_x = event->x - input->MousePosition.x;
    input->MousePosition.delta_y = event->y - input->MousePosition.y;
    input->MousePosition.x = event->x;
    input->MousePosition.y = event->y;
}

static void IvyInput_HandleMouseScrollEvent(IvyListener *listener, void *data)
{
    IvyInput *input = IVY_CONTAINER_OF(listener, IvyInput, MouseScroll.listener);
    const IvyEventMouseScroll *event = data;

    input->MouseScroll.x = event->x_offset;
    input->MouseScroll.y = event->y_offset;
}
