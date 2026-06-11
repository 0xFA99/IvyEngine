#ifndef IVY_CORE_H
#define IVY_CORE_H

#include "ivy/types.h"
#include "ivy/window.h"
#include "ivy/input.h"
#include "ivy/forward.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    IVY_FLAG_VSYNC_HINT               = 0x00000040,
    IVY_FLAG_FULLSCREEN_MODE          = 0x00000002,
    IVY_FLAG_WINDOW_RESIZABLE         = 0x00000004,
    IVY_FLAG_WINDOW_UNDECORATED       = 0x00000008,
    IVY_FLAG_WINDOW_TRANSPARENT       = 0x00000010,
    IVY_FLAG_MSAA_4X_HINT             = 0x00000020,
    IVY_FLAG_WINDOW_HIDDEN            = 0x00000080,
    IVY_FLAG_WINDOW_ALWAYS_RUN        = 0x00000100,
    IVY_FLAG_WINDOW_MINIMIZED         = 0x00000200,
    IVY_FLAG_WINDOW_MAXIMIZED         = 0x00000400,
    IVY_FLAG_WINDOW_UNFOCUSED         = 0x00000800,
    IVY_FLAG_WINDOW_TOPMOST           = 0x00001000,
    IVY_FLAG_WINDOW_HIGHDPI           = 0x00002000,
    IVY_FLAG_WINDOW_MOUSE_PASSTHROUGH = 0x00004000,
    IVY_FLAG_BORDERLESS_WINDOWED_MODE = 0x00008000,
    IVY_FLAG_INTERLACED_HINT          = 0x00010000,
} IvyConfigFlags;

typedef enum {
    IVY_KEY_NULL = 0,

    // Printable
    IVY_KEY_SPACE          = 32,
    IVY_KEY_APOSTROPHE     = 39,
    IVY_KEY_COMMA          = 44,
    IVY_KEY_MINUS          = 45,
    IVY_KEY_PERIOD         = 46,
    IVY_KEY_SLASH          = 47,
    IVY_KEY_ZERO           = 48,
    IVY_KEY_ONE            = 49,
    IVY_KEY_TWO            = 50,
    IVY_KEY_THREE          = 51,
    IVY_KEY_FOUR           = 52,
    IVY_KEY_FIVE           = 53,
    IVY_KEY_SIX            = 54,
    IVY_KEY_SEVEN          = 55,
    IVY_KEY_EIGHT          = 56,
    IVY_KEY_NINE           = 57,
    IVY_KEY_SEMICOLON      = 59,
    IVY_KEY_EQUAL          = 61,
    IVY_KEY_A              = 65,
    IVY_KEY_B              = 66,
    IVY_KEY_C              = 67,
    IVY_KEY_D              = 68,
    IVY_KEY_E              = 69,
    IVY_KEY_F              = 70,
    IVY_KEY_G              = 71,
    IVY_KEY_H              = 72,
    IVY_KEY_I              = 73,
    IVY_KEY_J              = 74,
    IVY_KEY_K              = 75,
    IVY_KEY_L              = 76,
    IVY_KEY_M              = 77,
    IVY_KEY_N              = 78,
    IVY_KEY_O              = 79,
    IVY_KEY_P              = 80,
    IVY_KEY_Q              = 81,
    IVY_KEY_R              = 82,
    IVY_KEY_S              = 83,
    IVY_KEY_T              = 84,
    IVY_KEY_U              = 85,
    IVY_KEY_V              = 86,
    IVY_KEY_W              = 87,
    IVY_KEY_X              = 88,
    IVY_KEY_Y              = 89,
    IVY_KEY_Z              = 90,
    IVY_KEY_LEFT_BRACKET   = 91,
    IVY_KEY_BACKSLASH      = 92,
    IVY_KEY_RIGHT_BRACKET  = 93,
    IVY_KEY_GRAVE          = 96,

    // Function / control
    IVY_KEY_ESCAPE         = 256,
    IVY_KEY_ENTER          = 257,
    IVY_KEY_TAB            = 258,
    IVY_KEY_BACKSPACE      = 259,
    IVY_KEY_INSERT         = 260,
    IVY_KEY_DELETE         = 261,
    IVY_KEY_RIGHT          = 262,
    IVY_KEY_LEFT           = 263,
    IVY_KEY_DOWN           = 264,
    IVY_KEY_UP             = 265,
    IVY_KEY_PAGE_UP        = 266,
    IVY_KEY_PAGE_DOWN      = 267,
    IVY_KEY_HOME           = 268,
    IVY_KEY_END            = 269,
    IVY_KEY_CAPS_LOCK      = 280,
    IVY_KEY_SCROLL_LOCK    = 281,
    IVY_KEY_NUM_LOCK       = 282,
    IVY_KEY_PRINT_SCREEN   = 283,
    IVY_KEY_PAUSE          = 284,
    IVY_KEY_F1             = 290,
    IVY_KEY_F2             = 291,
    IVY_KEY_F3             = 292,
    IVY_KEY_F4             = 293,
    IVY_KEY_F5             = 294,
    IVY_KEY_F6             = 295,
    IVY_KEY_F7             = 296,
    IVY_KEY_F8             = 297,
    IVY_KEY_F9             = 298,
    IVY_KEY_F10            = 299,
    IVY_KEY_F11            = 300,
    IVY_KEY_F12            = 301,
    IVY_KEY_LEFT_SHIFT     = 340,
    IVY_KEY_LEFT_CONTROL   = 341,
    IVY_KEY_LEFT_ALT       = 342,
    IVY_KEY_LEFT_SUPER     = 343,
    IVY_KEY_RIGHT_SHIFT    = 344,
    IVY_KEY_RIGHT_CONTROL  = 345,
    IVY_KEY_RIGHT_ALT      = 346,
    IVY_KEY_RIGHT_SUPER    = 347,
    IVY_KEY_KB_MENU        = 348,

    // Keypad
    IVY_KEY_KP_0           = 320,
    IVY_KEY_KP_1           = 321,
    IVY_KEY_KP_2           = 322,
    IVY_KEY_KP_3           = 323,
    IVY_KEY_KP_4           = 324,
    IVY_KEY_KP_5           = 325,
    IVY_KEY_KP_6           = 326,
    IVY_KEY_KP_7           = 327,
    IVY_KEY_KP_8           = 328,
    IVY_KEY_KP_9           = 329,
    IVY_KEY_KP_DECIMAL     = 330,
    IVY_KEY_KP_DIVIDE      = 331,
    IVY_KEY_KP_MULTIPLY    = 332,
    IVY_KEY_KP_SUBTRACT    = 333,
    IVY_KEY_KP_ADD         = 334,
    IVY_KEY_KP_ENTER       = 335,
    IVY_KEY_KP_EQUAL       = 336,
} IvyKeyboardKey;

typedef struct {
    GLFWwindow *handle;
} IvyPlatformData;

typedef struct {
    double current;
    double prev;
    double update;
    double draw;
    double frame;
    double target;
    double delta;
    int    frameCounter;
} IvyTime;

typedef struct {
    IvyInput  input;
    IvyWindow window;
    IvyTime   time;
    IvyArenaLinear *arena;
} IvyCoreData;

#define FLAG_SET(n, f)    ((n) |=  (f))
#define FLAG_CLEAR(n, f)  ((n) &= ~(f))
#define FLAG_TOGGLE(n, f) ((n) ^=  (f))
#define FLAG_IS_SET(n, f) (((n) & (f)) == (f))

void   Ivy_Core_InitWindow(IvyArenaLinear *arena, u32 width, u32 height, const char *title);
void   Ivy_Core_CloseWindow(void);

double Ivy_Core_GetTime(void);

#ifdef __cplusplus
}
#endif

#endif
