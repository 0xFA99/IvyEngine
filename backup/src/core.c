#include "ivy/core.h"
#include "ivy/arena/linear.h"
#include "ivy/gfx.h"
#include "ivy/platform.h"

#include <string.h>

#define IVY_DEFAULT_SCREEN_WIDTH  800
#define IVY_DEFAULT_SCREEN_HEIGHT 450

IvyPlatformData IVY_PLATFORM = {0};
IvyCoreData     IVY_CORE     = {0};

static void IvyCore_InitPlatform(void)
{
    const char *title = IVY_CORE.window.title ? IVY_CORE.window.title : "Ivy";
    const int width = IVY_CORE.window.screen.width ? (int)IVY_CORE.window.screen.width : IVY_DEFAULT_SCREEN_WIDTH;
    const int height = IVY_CORE.window.screen.height ? (int)IVY_CORE.window.screen.height : IVY_DEFAULT_SCREEN_HEIGHT;

    const bool success = Ivy_Platform_Init(title, width, height);
    IVY_CHECK(success, "[Platform] Failed to initialize native platform!");

    IVY_CORE.time.prev = Ivy_Core_GetTime();
}

void Ivy_Core_InitWindow(IvyArenaLinear *arena, const u32 width, const u32 height, const char *title)
{
    IVY_CORE.arena = arena;

    IVY_CORE.window.screen.width  = width;
    IVY_CORE.window.screen.height = height;
    IVY_CORE.window.eventWaiting  = false;
    IVY_CORE.window.screenScale   = Ivy_Math_MatrixIdentity();

    if (IVY_LIKELY(title && title[0] != '\0'))
        IVY_CORE.window.title = title;

    memset(&IVY_CORE.input, 0, sizeof(IVY_CORE.input));
    IVY_CORE.input.keyboard.exitKey = IVY_KEY_ESCAPE;
    IVY_CORE.input.mouse.scale      = (IvyVector2){ 1.0f, 1.0f };
    IVY_CORE.input.mouse.cursor     = IVY_MOUSE_CURSOR_ARROW;

    IvyCore_InitPlatform();

    if (IVY_CORE.window.render.width == 0 || IVY_CORE.window.render.height == 0)
        IVY_CORE.window.render = IVY_CORE.window.screen;

    Ivy_Gfx_Init((int)IVY_CORE.window.render.width, (int)IVY_CORE.window.render.height);

    IVY_CORE.window.shouldClose = false;
}

void Ivy_Core_CloseWindow(void)
{
    IVY_CORE.window.ready = false;
    Ivy_Platform_Shutdown();
}

double Ivy_Core_GetTime(void)
{
    return Ivy_Platform_GetTime();
}

void Ivy_Core_SetConfigFlags(const u32 flags)
{
    IVY_FLAG_SET(IVY_CORE.window.flags, flags);
}