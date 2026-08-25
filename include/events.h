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

#ifdef __cplusplus
}
#endif

#endif