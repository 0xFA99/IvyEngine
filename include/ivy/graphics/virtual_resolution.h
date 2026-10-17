#ifndef IVY_VIRTUAL_RESOLUTION_H
#define IVY_VIRTUAL_RESOLUTION_H

#include "ivy/core/fwd.h"
#include "ivy/core/types.h"
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
};

IvyStatus Ivy_VirtualResolution_Init(IvyVirtualResolution *virtual);
void Ivy_VirtualResolution_Terminate(IvyVirtualResolution *virtual);

#ifdef __cplusplus
}
#endif

#endif