#include "ivy/arena/linear.h"
#include "ivy/core.h"
#include "ivy/gfx.h"
#include "ivy/virtual_resolution.h"

typedef struct {
    IvyArenaLinear arena;
    IvyArenaLinearSnapshot snapshot;
} IvyArena;

extern IvyCoreData IVY_CORE;

int main(void)
{
    IvyArena arenaMain;
    Ivy_Arena_LinearInit(&arenaMain.arena, 10 * 1024 * 1024); // 10 MB
    arenaMain.snapshot = Ivy_Arena_LinearGetSnapshot(&arenaMain.arena);

    Ivy_Core_SetConfigFlags(IVY_FLAG_WINDOW_RESIZABLE);
    Ivy_Core_InitWindow(&arenaMain.arena, 800, 600, "Ivy");

    IvyVirtualResolution *virtualRes = Ivy_VirtualResolution_Init(&arenaMain.arena, (IvyVector2){ 800, 600 });

    while (!Ivy_Window_ShouldClose())
    {
        Ivy_VirtualResolution_Update(virtualRes, (IvyVector2) { IVY_CORE.window.screen.width, IVY_CORE.window.screen.height });

        Ivy_Gfx_BeginTextureMode(virtualRes->target);
            Ivy_Gfx_ClearBackground((IvyColor){ 230, 41, 55, 255 });
        Ivy_Gfx_EndTextureMode();

        Ivy_Gfx_BeginDrawing();
            Ivy_Gfx_ClearBackground((IvyColor){ 18, 18, 18, 255 });
            if (virtualRes->target.texture.id != 0) {
                Ivy_VirtualResolution_Draw(virtualRes);
            } else {
                __builtin_printf("Render Texture invalid");
            }
        Ivy_Gfx_EndDrawing();
    }

    Ivy_Core_CloseWindow();

    Ivy_Arena_LinearDestroy(&arenaMain.arena);

    return 0;
}