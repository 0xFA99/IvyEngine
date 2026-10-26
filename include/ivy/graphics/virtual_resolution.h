#ifndef IVY_VIRTUAL_RESOLUTION_H
#define IVY_VIRTUAL_RESOLUTION_H

#include "ivy/core/fwd.h"
#include "ivy/core/types.h"
#include "ivy/core/signal.h"
#include "ivy/graphics/gl_loader.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    IVY_SCALE_FIT,
    IVY_SCALE_INTEGER,
    IVY_SCALE_STRETCH
} IvyScaleMode;

typedef struct {
    int x, y;
    int width, height;
    float scale;
} IvyViewport;

struct IvyVirtualResolution
{
    int width, height;

    IvyScaleMode mode;
    IvyViewport viewport;

    GLuint fbo;
    GLuint texture;
    GLuint depth;

    IvyListener resize_listener;
};

IvyStatus Ivy_VirtualResolution_Init(IvyVirtualResolution *virtual, int width, int height, IvyScaleMode mode, IvyWindow *window);
void Ivy_VirtualResolution_Update(IvyVirtualResolution *virtual, int width, int height);
void Ivy_VirtualResolution_Begin(const IvyVirtualResolution *virtual);
void Ivy_VirtualResolution_End(const IvyVirtualResolution *virtual, int width, int height);
void Ivy_VirtualResolution_Terminate(IvyVirtualResolution *virtual);

bool Ivy_VirtualResolution_ScreenToVirtual(const IvyVirtualResolution *virtual, int windowHeight, double screenX, double screenY, float *outVirtualX, float *outVirtualY);

#ifdef __cplusplus
}
#endif

#endif