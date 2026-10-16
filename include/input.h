#ifndef IVY_INPUT_H
#define IVY_INPUT_H

#include "fwd.h"
#include "types.h"
#include "signal.h"

#include <stdbool.h>

#define IVY_MAX_KEYS 512

#ifdef __cplusplus
extern "C" {
#endif

struct IvyInput {
    bool current_keys[IVY_MAX_KEYS];
    bool previous_keys[IVY_MAX_KEYS];
    IvyListener key_listener;
};

IvyStatus Ivy_Input_Init(IvyInput *restrict input, const IvyWindow *restrict window);
void Ivy_Input_Update(IvyInput *input);

bool Ivy_Input_IsKeyDown(const IvyInput *input, int key);
bool Ivy_Input_IsKeyPressed(const IvyInput *input, int key);
bool Ivy_Input_IsKeyReleased(const IvyInput *input, int key);

#ifdef __cplusplus
}
#endif

#endif