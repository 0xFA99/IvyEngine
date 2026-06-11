#include "ivy/arena/linear.h"
#include "ivy/core.h"
#include "ivy/gfx.h"

typedef struct {
    IvyArenaLinear arena;
    IvyArenaLinearSnapshot snapshot;
} IvyArena;

int main(void)
{
    IvyArena arenaMain;
    Ivy_Arena_LinearInit(&arenaMain.arena, 10 * 1024 * 1024); // 10 MB
    arenaMain.snapshot = Ivy_Arena_LinearGetSnapshot(&arenaMain.arena);

    Ivy_Core_InitWindow(&arenaMain.arena, 800, 600, "Ivy");

    while (!Ivy_Window_ShouldClose())
    {
        Ivy_Gfx_BeginDrawing();
        Ivy_Gfx_ClearBackground((IvyColor){ 18, 18, 18, 255 });
        Ivy_Gfx_EndDrawing();
    }

    Ivy_Core_CloseWindow();

    Ivy_Arena_LinearDestroy(&arenaMain.arena);

    return 0;
}