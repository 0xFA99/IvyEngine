#ifndef IVY_TIME_H
#define IVY_TIME_H

#include "ivy/core/fwd.h"

#ifdef __cplusplus
extern "C" {
#endif

struct IvyTime {
    double start_time;
    double current_time;
    double previous_time;
    float delta_time;

    int fps;
    int frame_counter;
    double fps_timer;
};

void Ivy_Time_Init(IvyTime *time);
void Ivy_Time_Update(IvyTime *time);

float Ivy_Time_GetDeltaTime(const IvyTime *time);
double Ivy_Time_GetTime(const IvyTime *time);
int Ivy_Time_GetFPS(const IvyTime *time);

#ifdef __cplusplus
}
#endif

#endif