#include "ivy/virtual_resolution.h"

#include "ivy/arena/linear.h"

#define IVY_INVERSE_VIRTUAL_RESOLUTION_WIDTH  (1.0f / IVY_VIRTUAL_RESOLUTION_WIDTH)
#define IVY_INVERSE_VIRTUAL_RESOLUTION_HEIGHT (1.0f / IVY_VIRTUAL_RESOLUTION_HEIGHT)

IvyVirtualResolution *Ivy_VirtualResolution_Init(IvyArenaLinear *arena, const IvyVector2 size)
{
    IVY_ASSERT(arena != NULL, "[WARNING] [Arena] is NULL!");

    IvyVirtualResolution *virtualRes = Ivy_Arena_LinearAlloc(arena, sizeof(IvyVirtualResolution));
    IVY_ENSURE(virtualRes != NULL);

    virtualRes->target = Ivy_Gfx_LoadRenderTexture(IVY_VIRTUAL_RESOLUTION_WIDTH, IVY_VIRTUAL_RESOLUTION_HEIGHT);
    virtualRes->source = (IvyRectangle) { 0, 0, IVY_VIRTUAL_RESOLUTION_WIDTH, IVY_VIRTUAL_RESOLUTION_HEIGHT};

    Ivy_VirtualResolution_Update(virtualRes, size);

    return virtualRes;
}

void Ivy_VirtualResolution_Update(IvyVirtualResolution *virtualRes, const IvyVector2 size)
{
    if (IVY_UNLIKELY(size.x < 0 || size.y < 0)) return;

    const float scaleX = size.x * IVY_INVERSE_VIRTUAL_RESOLUTION_WIDTH;
    const float scaleY = size.y * IVY_INVERSE_VIRTUAL_RESOLUTION_HEIGHT;

    virtualRes->scale = (scaleX < scaleY) ? scaleX : scaleY;

    const float scaledW = IVY_VIRTUAL_RESOLUTION_WIDTH  * virtualRes->scale;
    const float scaledH = IVY_VIRTUAL_RESOLUTION_HEIGHT * virtualRes->scale;

    virtualRes->destination = (IvyRectangle) {
        .x      = Ivy_Math_FloorF((size.x - scaledW) * 0.5f),
        .y      = Ivy_Math_FloorF((size.y - scaledH) * 0.5f),
        .width  = Ivy_Math_FloorF(scaledW),
        .height = Ivy_Math_FloorF(scaledH)
    };
}

void Ivy_VirtualResolution_Draw(const IvyVirtualResolution *virtualRes)
{
    IVY_ASSERT(virtualRes != NULL, "[WARNING] [VirtualResolution] is NULL!");
    // TODO: Ivy_Gfx_DrawTexture();
}

void Ivy_VirtualResolution_Unload(const IvyVirtualResolution *virtualRes)
{
    IVY_ASSERT(virtualRes != NULL, "[WARNING] [VirtualResolution] is NULL!");

    if (IVY_LIKELY(virtualRes != NULL)) {
        // TODO: Ivy_Gfx_UnloadRenderTexture(virtualRes->target);
    }
}