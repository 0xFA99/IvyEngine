#ifndef IVY_ENGINE_H
#define IVY_ENGINE_H

#include "ivy/core/fwd.h"
#include "ivy/core/types.h"
#include "ivy/core/signal.h"
#include "ivy/core/time.h"
#include "ivy/platform/window.h"
#include "ivy/platform/input.h"
#include "ivy/graphics/renderer.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct IvyEngine
{
    IvyWindow window;
    IvyRenderer renderer;
    IvyInput input;
    IvyTime time;

    IvySignal event_signal;

    bool is_running;
};

IvyStatus   Ivy_Engine_Init(IvyEngine *engine);
void        Ivy_Engine_Run(IvyEngine *engine);
void        Ivy_Engine_Stop(IvyEngine *engine);
void        Ivy_Engine_Terminate(IvyEngine *engine);

#ifdef __cplusplus
}
#endif

#endif