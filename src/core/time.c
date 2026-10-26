#include "ivy/core/time.h"
#include "ivy/core/types.h"
#include "ivy/platform/glfw_loader.h"

void Ivy_Time_Init(IvyTime *time)
{
    IVY_ASSERT(time != NULL, "(IvyTime) is NULL");

    const double now = glfwGetTime();
    time->start_time = now;
    time->current_time = now;
    time->previous_time = now;
    time->delta_time = 0.0f;

    time->fps = 0;
    time->frame_counter = 0;
    time->fps_timer = now;
}

void Ivy_Time_Update(IvyTime *time)
{
    IVY_ASSERT(time != NULL, "(IvyTime) is NULL");

    time->previous_time = time->current_time;
    time->current_time = glfwGetTime();

    time->delta_time = time->current_time - time->previous_time;

    time->frame_counter++;
    const double elapsed = time->current_time - time->fps_timer;

    if (elapsed >= 0.5) {
        time->fps = (int)((double)time->frame_counter / elapsed + 0.5);
        time->frame_counter = 0;
        time->fps_timer = time->current_time;
    }
}

float Ivy_Time_GetDeltaTime(const IvyTime *time)
{
    return time->delta_time;
}

double Ivy_Time_GetTime(const IvyTime *time)
{
    return time->current_time - time->start_time;
}

int Ivy_Time_GetFPS(const IvyTime *time)
{
    return time->fps;
}