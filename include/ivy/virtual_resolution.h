#ifndef IVY_VIRTUAL_RESOLUTION_H
#define IVY_VIRTUAL_RESOLUTION_H

#include "ivy/types.h"
#include "ivy/forward.h"
#include "ivy/math.h"
#include "ivy/gfx.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IVY_VIRTUAL_RESOLUTION_WIDTH  640.0f
#define IVY_VIRTUAL_RESOLUTION_HEIGHT 360.0f

struct IvyVirtualResolution {
    IvyRenderTexture    target;         // 44
    IvyRectangle        source;         // 16
    IvyRectangle        destination;    // 16
    float               scale;          // 4
};                                      // (80)
IVY_ASSERT_STATIC(sizeof(IvyVirtualResolution) == 80, "[ERROR] [IvyVirtualResolution] Size must be 80 bytes!");

IvyVirtualResolution *Ivy_VirtualResolution_Init(IvyArenaLinear *arena, IvyVector2 size);
void Ivy_VirtualResolution_Update(IvyVirtualResolution *virtualRes, IvyVector2 size);
void Ivy_VirtualResolution_Draw(const IvyVirtualResolution *virtualRes);
void Ivy_VirtualResolution_Unload(const IvyVirtualResolution *virtualRes);

#ifdef __cplusplus
}
#endif

#endif
