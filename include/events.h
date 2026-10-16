#ifndef IVY_EVENTS_H
#define IVY_EVENTS_H

#include "fwd.h"

#ifdef __cplusplus
extern "C" {
#endif

struct IvyEventWindowResize
{
    IvyWindow *window;
    int width;
    int height;
};

struct IvyEventKey {
    int key;
    int scancode;
    int action;
    int mods;
};

#ifdef __cplusplus
}
#endif

#endif