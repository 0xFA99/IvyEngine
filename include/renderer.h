#ifndef IVY_RENDERER_H
#define IVY_RENDERER_H

#include "fwd.h"
#include "signal.h"

#ifdef __cplusplus
extern "C" {
#endif

struct IvyRenderer {
    struct {
        IvyListener resize;
    } events;
};

void Ivy_Renderer_Init(IvyRenderer *restrict renderer, IvyWindow *restrict window);
void Ivy_Renderer_Terminated(IvyRenderer *renderer);

#ifdef __cplusplus
}
#endif

#endif