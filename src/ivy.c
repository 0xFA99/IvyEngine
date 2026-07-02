#include "ivy/arena/linear.h"
#include "ivy/core.h"
#include "ivy/gfx.h"
#include "ivy/virtual_resolution.h"
#include "ivy/gl.h"

void DrawRectangleLines(int posX, int posY, int width, int height, IvyColor color)
{
    IvyMatrix mat = Ivy_Gfx_GetMatrixTransform();
    float xOffset = 0.5f/mat.m0;
    float yOffset = 0.5f/mat.m5;

    Ivy_Gfx_Begin(IVY_LINES);
    Ivy_Gfx_Color4UB(color.r, color.g, color.b, color.a);
    Ivy_Gfx_Vertex2F((float)posX + xOffset, (float)posY + yOffset);
    Ivy_Gfx_Vertex2F((float)posX + (float)width - xOffset, (float)posY + yOffset);

    Ivy_Gfx_Vertex2F((float)posX + (float)width - xOffset, (float)posY + yOffset);
    Ivy_Gfx_Vertex2F((float)posX + (float)width - xOffset, (float)posY + (float)height - yOffset);

    Ivy_Gfx_Vertex2F((float)posX + (float)width - xOffset, (float)posY + (float)height - yOffset);
    Ivy_Gfx_Vertex2F((float)posX + xOffset, (float)posY + (float)height - yOffset);

    Ivy_Gfx_Vertex2F((float)posX + xOffset, (float)posY + (float)height - yOffset);
    Ivy_Gfx_Vertex2F((float)posX + xOffset, (float)posY + yOffset);
    Ivy_Gfx_End();
}

void DrawTriangle(void)
{
    Ivy_Gfx_Begin(IVY_TRIANGLES);
    Ivy_Gfx_Color4UB(0, 0, 255, 255);
    Ivy_Gfx_Vertex2F(200.0f, 100.0f);
    Ivy_Gfx_Vertex2F(100.0f, 300.0f);
    Ivy_Gfx_Vertex2F(300.0f, 300.0f);
    Ivy_Gfx_End();
}

typedef struct {
    IvyArenaLinear arena;
    IvyArenaLinearSnapshot snapshot;
} IvyArena;

extern IvyCoreData IVY_CORE;

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    IvyArena arenaMain;
    Ivy_Arena_LinearInit(&arenaMain.arena, 10 * 1024 * 1024);
    arenaMain.snapshot = Ivy_Arena_LinearGetSnapshot(&arenaMain.arena);

    Ivy_Core_SetConfigFlags(IVY_FLAG_WINDOW_RESIZABLE | IVY_FLAG_MSAA_4X_HINT);
    Ivy_Core_InitWindow(&arenaMain.arena, 800, 600, "Ivy");

    IvyVirtualResolution *virtualRes = Ivy_VirtualResolution_Init(&arenaMain.arena, (IvyVector2){ 400, 300 });

    while (!Ivy_Window_ShouldClose())
    {
        if (Ivy_Window_IsResized())
        {
            __builtin_printf("RESIZED\n");
            Ivy_VirtualResolution_Update(virtualRes, (IvyVector2) { IVY_CORE.window.screen.width, IVY_CORE.window.screen.height });
        }

        Ivy_Gfx_BeginTextureMode(virtualRes->target);
            Ivy_Gfx_ClearBackground((IvyColor){ 18, 18, 18, 255 });
            DrawRectangleLines(10, 10, 50, 50, (IvyColor){ 255, 0, 0, 255 });
            DrawTriangle();
        Ivy_Gfx_EndTextureMode();

        Ivy_Gfx_BeginDrawing();
            Ivy_Gfx_ClearBackground((IvyColor){ 0, 0, 0, 255 });
            Ivy_VirtualResolution_Draw(virtualRes);
        Ivy_Gfx_EndDrawing();
    }

    Ivy_Core_CloseWindow();

    Ivy_Arena_LinearDestroy(&arenaMain.arena);

    return 0;
}