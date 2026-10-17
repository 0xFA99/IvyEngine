#include "ivy/graphics/renderer.h"
#include "ivy/core/fwd.h"
#include "ivy/core/types.h"
#include "ivy/core/log.h"
#include "ivy/graphics/gl_loader.h"
#include "ivy/platform/events.h"
#include "ivy/platform/window.h"

static void IvyRenderer_HandleWindowResize(IvyListener *listener, void *data);

void Ivy_Renderer_Init(IvyRenderer *restrict renderer, IvyWindow *restrict window)
{
    IVY_ASSERT(renderer != NULL, "(IvyRenderer) is NULL");
    IVY_ASSERT(window != NULL, "(IvyWindow) is NULL");

    Ivy_Listener_Init(&renderer->events.resize, IvyRenderer_HandleWindowResize);
    Ivy_Signal_Add(&window->events.resize, &renderer->events.resize);

    IVY_LOG_INFO("(IvyRenderer) Initialized and subscribed to resize signal.");
}

void Ivy_Renderer_Terminated(IvyRenderer *renderer)
{
    IVY_ASSERT(renderer != NULL, "IvyRenderer is NULL!");

    Ivy_Signal_Remove(&renderer->events.resize);

    IVY_LOG_INFO("(IvyRenderer) Terminated.");
}

static void IvyRenderer_HandleWindowResize(IvyListener *listener, void *data)
{
    const IvyRenderer *renderer = IVY_CONTAINER_OF(listener, IvyRenderer, events.resize);
    const IvyEventWindowResize *event = data;
    (void)renderer;

    glViewport(0, 0, event->width, event->height);

    IVY_LOG_INFO("(IvyRenderer) Viewport updated to %dx%d", event->width, event->height);
}
