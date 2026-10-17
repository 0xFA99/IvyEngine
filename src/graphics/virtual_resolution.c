#include "ivy/core/fwd.h"
#include "ivy/core/types.h"
#include "ivy/graphics/virtual_resolution.h"

IvyStatus Ivy_VirtualResolution_Init(IvyVirtualResolution *virtual)
{
    IVY_ASSERT(virtual != NULL, "(IvyVirtualResolution) is NULL.");

    return IVY_SUCCESS;
}

void Ivy_VirtualResolution_Terminate(IvyVirtualResolution *virtual)
{
    IVY_ASSERT(virtual != NULL, "(IvyVirtualResolution) is NULL.");
}
