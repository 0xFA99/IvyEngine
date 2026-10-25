#include "ivy/core/fwd.h"
#include "ivy/core/types.h"
#include "ivy/graphics/virtual_resolution.h"

#include "ivy/core/log.h"
#include "ivy/platform/events.h"
#include "ivy/platform/window.h"

static void IvyVirtualResolution_HandleResize(IvyListener *listener, void *data);

IvyStatus Ivy_VirtualResolution_Init(IvyVirtualResolution *virtual, int width, int height, IvyScaleMode mode, IvyWindow *window)
{
    IVY_ASSERT(virtual != NULL, "(IvyVirtualResolution) is NULL.");
    IVY_ASSERT(window != NULL, "(IvyWindow) is NULL.");

    virtual->width = width;
    virtual->height = height;
    virtual->mode = mode;

    // FBO
    glGenFramebuffers(1, &virtual->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, virtual->fbo);

    // Attach texture
    glGenTextures(1, &virtual->texture);
    glBindTexture(GL_TEXTURE_2D, virtual->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, virtual->texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        IVY_LOG_ERROR("(IvyVirtualResolution) Framebuffer incomplete!");
        return IVY_ERROR_LOAD_FAILED;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Ivy_VirtualResolution_Update(virtual, window->width, window->height);

    Ivy_Listener_Init(&virtual->resize_listener, IvyVirtualResolution_HandleResize);
    Ivy_Signal_Add(&window->events.resize, &virtual->resize_listener);

    IVY_LOG_INFO("(IvyVirtualResolution) Initialized: %dx%d (Window: %dx%d)", width, height, window->width, window->height);

    return IVY_SUCCESS;
}

void Ivy_VirtualResolution_Update(IvyVirtualResolution *virtual, const int width, const int height)
{
    const float scale_x = (float)width / (float)virtual->width;
    const float scale_y = (float)height / (float)virtual->height;
    float scale = (scale_x < scale_y) ? scale_x : scale_y;

    if (virtual->mode == IVY_SCALE_INTEGER) {
        scale = (float)((int)scale > 1 ? (int)scale : 1);
    }
    else if (virtual->mode == IVY_SCALE_STRETCH) {
        virtual->viewport.x = 0;
        virtual->viewport.y = 0;
        virtual->viewport.width = width;
        virtual->viewport.height = height;
        virtual->viewport.scale = 1.0f;

        return;
    }

    const int vp_w = (int)((float)virtual->width * scale);
    const int vp_h  = (int)((float)virtual->height * scale);

    virtual->viewport.width = vp_w;
    virtual->viewport.height = vp_h;
    virtual->viewport.x = (width - vp_w) / 2;
    virtual->viewport.y = (height - vp_h) / 2;
    virtual->viewport.scale = scale;
}

void Ivy_VirtualResolution_Begin(const IvyVirtualResolution *virtual)
{
    glBindFramebuffer(GL_FRAMEBUFFER, virtual->fbo);
    glViewport(0, 0, virtual->width, virtual->height);
}

void Ivy_VirtualResolution_End(const IvyVirtualResolution *virtual, int width, int height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, virtual->fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(
        0, 0, virtual->width, virtual->height,
        virtual->viewport.x, virtual->viewport.y,
        virtual->viewport.x + virtual->viewport.width,
        virtual->viewport.y + virtual->viewport.height,
        GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Ivy_VirtualResolution_Terminate(IvyVirtualResolution *virtual)
{
    IVY_ASSERT(virtual != NULL, "(IvyVirtualResolution) is NULL.");

    Ivy_Signal_Remove(&virtual->resize_listener);

    if (virtual->texture) {
        glDeleteTextures(1, &virtual->texture);
        virtual->texture = 0;
    }

    if (virtual->fbo) {
        glDeleteFramebuffers(1, &virtual->fbo);
        virtual->fbo = 0;
    }
}

static void IvyVirtualResolution_HandleResize(IvyListener *listener, void *data)
{
    IvyVirtualResolution *virtual = IVY_CONTAINER_OF(listener, IvyVirtualResolution, resize_listener);
    const IvyEventWindowResize *event = (const IvyEventWindowResize *)data;

    Ivy_VirtualResolution_Update(virtual, event->width, event->height);

    IVY_LOG_INFO("(IvyVirtualResolution) Viewport updated: %dx%d at (%d, %d)",
        virtual->viewport.width, virtual->viewport.height,
        virtual->viewport.x, virtual->viewport.y);
}
