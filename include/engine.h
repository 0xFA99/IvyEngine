#ifndef IVY_ENGINE_H
#define IVY_ENGINE_H

#include "fwd.h"
#include "types.h"
#include "window.h"
#include "renderer.h"
#include "input.h"
#include "signal.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct IvyEngine
{
    IvyWindow window;
    IvyRenderer renderer;
    IvyInput input;

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