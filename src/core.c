#include "ivy/core.h"
#include "ivy/arena/linear.h"
#include "ivy/glfw_loader.h"
#include "ivy/glad_loader.h"
#include "ivy/gfx.h"

#include <string.h>

#define IVY_DEFAULT_SCREEN_WIDTH  800
#define IVY_DEFAULT_SCREEN_HEIGHT 450

IvyPlatformData IVY_PLATFORM = {0};
IvyCoreData     IVY_CORE     = {0};

static void *Ivy_GLFW_Alloc(const usize size, void *user)
{
    return Ivy_Arena_LinearAlloc((IvyArenaLinear *)user, size);
}

static void *Ivy_GLFW_Realloc(void *block, usize size, void *user)
{
    (void)block; (void)size; (void)user;
    IVY_ASSERT(false, "[GLFW] Unexpected realloc — not supported by arena allocator!");
}

static void Ivy_GLFW_Free(void *block, void *user)
{
    (void)block; (void)user;
}

static void Ivy_GLFW_Error(int error, const char *description)
{
    IVY_ASSERT(false, "[GLFW] Error %i: %s", error, description);
}

static void Ivy_GL_LoadFunctions(void)
{
    GLFWglproc (*load)(const char*) = (GLFWglproc (*)(const char*))glfwGetProcAddress;

    // Vertex array & buffer objects
    glBindBuffer              = (PFNGLBINDBUFFERPROC)              load("glBindBuffer");
    glBindVertexArray         = (PFNGLBINDVERTEXARRAYPROC)         load("glBindVertexArray");
    glGenVertexArrays         = (PFNGLGENVERTEXARRAYSPROC)         load("glGenVertexArrays");
    glGenBuffers              = (PFNGLGENBUFFERSPROC)              load("glGenBuffers");
    glBufferData              = (PFNGLBUFFERDATAPROC)              load("glBufferData");
    glBufferSubData           = (PFNGLBUFFERSUBDATAPROC)           load("glBufferSubData");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) load("glEnableVertexAttribArray");
    glVertexAttribPointer     = (PFNGLVERTEXATTRIBPOINTERPROC)     load("glVertexAttribPointer");

    // Clear & depth
    glClearColor  = (PFNGLCLEARCOLORPROC) load("glClearColor");
    glClear       = (PFNGLCLEARPROC)      load("glClear");
    glClearDepth  = (PFNGLCLEARDEPTHPROC) load("glClearDepth");
    glDepthFunc   = (PFNGLDEPTHFUNCPROC)  load("glDepthFunc");

    // Enable/disable states
    glEnable  = (PFNGLENABLEPROC)  load("glEnable");
    glDisable = (PFNGLDISABLEPROC) load("glDisable");

    // Blending
    glBlendFunc = (PFNGLBLENDFUNCPROC) load("glBlendFunc");

    // Face culling
    glCullFace  = (PFNGLCULLFACEPROC)  load("glCullFace");
    glFrontFace = (PFNGLFRONTFACEPROC) load("glFrontFace");

    // Shader & program
    glCreateShader         = (PFNGLCREATESHADERPROC)         load("glCreateShader");
    glShaderSource         = (PFNGLSHADERSOURCEPROC)         load("glShaderSource");
    glCompileShader        = (PFNGLCOMPILESHADERPROC)        load("glCompileShader");
    glGetShaderiv          = (PFNGLGETSHADERIVPROC)          load("glGetShaderiv");
    glCreateProgram        = (PFNGLCREATEPROGRAMPROC)        load("glCreateProgram");
    glAttachShader         = (PFNGLATTACHSHADERPROC)         load("glAttachShader");
    glBindAttribLocation   = (PFNGLBINDATTRIBLOCATIONPROC)   load("glBindAttribLocation");
    glLinkProgram          = (PFNGLLINKPROGRAMPROC)          load("glLinkProgram");
    glGetProgramiv         = (PFNGLGETPROGRAMIVPROC)         load("glGetProgramiv");
    glUseProgram           = (PFNGLUSEPROGRAMPROC)           load("glUseProgram");
    glGetAttribLocation    = (PFNGLGETATTRIBLOCATIONPROC)    load("glGetAttribLocation");
    glGetUniformLocation   = (PFNGLGETUNIFORMLOCATIONPROC)   load("glGetUniformLocation");
    glDetachShader         = (PFNGLDETACHSHADERPROC)         load("glDetachShader");
    glDeleteShader         = (PFNGLDELETESHADERPROC)         load("glDeleteShader");
    glDeleteProgram        = (PFNGLDELETEPROGRAMPROC)        load("glDeleteProgram");

    // Textures
    glGenTextures      = (PFNGLGENTEXTURESPROC)      load("glGenTextures");
    glBindTexture      = (PFNGLBINDTEXTUREPROC)      load("glBindTexture");
    glTexImage2D       = (PFNGLTEXIMAGE2DPROC)       load("glTexImage2D");
    glTexParameteri    = (PFNGLTEXPARAMETERIPROC)    load("glTexParameteri");
    glTexParameteriv   = (PFNGLTEXPARAMETERIVPROC)   load("glTexParameteriv");
    glPixelStorei      = (PFNGLPIXELSTOREIPROC)      load("glPixelStorei");
}

static void Ivy_Core_InitPlatform(IvyArenaLinear *arena)
{
    IVY_ASSERT(arena != NULL, "[ERROR] Arena is NULL!");

    IVY_CHECK(Ivy_GLFW_LoadFunctions() == 0, "[GLFW] Failed to initialize GLFW!");

    glfwSetErrorCallback(Ivy_GLFW_Error);

    const GLFWallocator allocator = {
        .allocate   = Ivy_GLFW_Alloc,
        .reallocate = Ivy_GLFW_Realloc,
        .deallocate = Ivy_GLFW_Free,
        .user       = arena,
    };
    glfwInitAllocator(&allocator);

    IVY_CHECK(glfwInit(), "[GLFW] Failed to initialize");   // Size: 207324 bytes

    glfwDefaultWindowHints();

    // OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,        GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE);

    // Window hints from flags
    glfwWindowHint(GLFW_RESIZABLE,  FLAG_IS_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_RESIZABLE)  ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_VISIBLE,   !FLAG_IS_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_HIDDEN)     ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, !FLAG_IS_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_UNDECORATED) ? GLFW_TRUE : GLFW_FALSE);

    if (FLAG_IS_SET(IVY_CORE.window.flags, IVY_FLAG_MSAA_4X_HINT))
        glfwWindowHint(GLFW_SAMPLES, 4);

    // Clear pre-init minimized/maximized hints — they'll be applied after creation
    FLAG_CLEAR(IVY_CORE.window.flags, IVY_FLAG_WINDOW_MINIMIZED);
    FLAG_CLEAR(IVY_CORE.window.flags, IVY_FLAG_WINDOW_MAXIMIZED);

    // Auto-fullscreen if no size given
    if (IVY_CORE.window.screen.width == 0 || IVY_CORE.window.screen.height == 0)
        FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_FULLSCREEN_MODE);

    // Create window
    if (FLAG_IS_SET(IVY_CORE.window.flags, IVY_FLAG_FULLSCREEN_MODE)) {
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        IVY_CORE.window.display.width  = mode->width;
        IVY_CORE.window.display.height = mode->height;

        if (IVY_CORE.window.screen.width == 0 || IVY_CORE.window.screen.height == 0) {
            IVY_CORE.window.prevScreen.width  = IVY_DEFAULT_SCREEN_WIDTH;
            IVY_CORE.window.prevScreen.height = IVY_DEFAULT_SCREEN_HEIGHT;
        }
        else {
            IVY_CORE.window.prevScreen = IVY_CORE.window.screen;
        }

        IVY_CORE.window.screen = IVY_CORE.window.display;

        IVY_PLATFORM.handle = glfwCreateWindow(
            (int)IVY_CORE.window.screen.width,
            (int)IVY_CORE.window.screen.height,
            IVY_CORE.window.title ? IVY_CORE.window.title : " ",
            monitor, NULL);
    }
    else {
        const int w = IVY_CORE.window.screen.width  ? (int)IVY_CORE.window.screen.width  : 1;
        const int h = IVY_CORE.window.screen.height ? (int)IVY_CORE.window.screen.height : 1;

        // Size: 4340 bytes
        IVY_PLATFORM.handle = glfwCreateWindow(
            w, h,
            IVY_CORE.window.title ? IVY_CORE.window.title : " ",
            NULL, NULL);

        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        IVY_CORE.window.display.width  = mode->width;
        IVY_CORE.window.display.height = mode->height;
        IVY_CORE.window.render         = IVY_CORE.window.screen;
    }

    IVY_CHECK(IVY_PLATFORM.handle != NULL, "[GLFW] Failed to create window");

    glfwMakeContextCurrent(IVY_PLATFORM.handle);
    const int err = glfwGetError(NULL);
    if (err != GLFW_NO_WINDOW_CONTEXT && err != GLFW_PLATFORM_ERROR)
        IVY_CORE.window.ready = true;

    IVY_CHECK(IVY_CORE.window.ready, "[GL] Failed to make context current");

    glfwSwapInterval(FLAG_IS_SET(IVY_CORE.window.flags, IVY_FLAG_VSYNC_HINT) ? 1 : 0);

    IVY_CORE.window.render     = IVY_CORE.window.screen;
    IVY_CORE.window.currentFBO = IVY_CORE.window.render;

    // Center window on its monitor
    int monitorCount = 0;
    GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);
    GLFWmonitor  *monitor  = monitors[Ivy_Window_GetCurrentMonitor()];

    int mx = 0, my = 0, mw = 0, mh = 0;
    glfwGetMonitorWorkarea(monitor, &mx, &my, &mw, &mh);
    IVY_CORE.window.position.x = (int)(mx + (mw - IVY_CORE.window.render.width)  * 0.5);
    IVY_CORE.window.position.y = (int)(my + (mh - IVY_CORE.window.render.height) * 0.5);
    Ivy_Window_SetWindowPosition(IVY_CORE.window.position.x, IVY_CORE.window.position.y);

    // Apply any remaining state flags
    Ivy_Window_SetWindowState(IVY_CORE.window.flags);

    // Load OpenGL function pointers
    Ivy_GL_LoadFunctions();

    // Register callbacks
    glfwSetWindowSizeCallback(IVY_PLATFORM.handle,      Ivy_Window_SizeCallback);
    glfwSetFramebufferSizeCallback(IVY_PLATFORM.handle, Ivy_Window_FramebufferSizeCallback);
    glfwSetWindowPosCallback(IVY_PLATFORM.handle,       Ivy_Window_PosCallback);
    glfwSetWindowIconifyCallback(IVY_PLATFORM.handle,   Ivy_Window_IconifyCallback);
    glfwSetWindowMaximizeCallback(IVY_PLATFORM.handle,  Ivy_Window_MaximizeCallback);
    glfwSetWindowFocusCallback(IVY_PLATFORM.handle,     Ivy_Window_FocusCallback);

    if (FLAG_IS_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_HIGHDPI)) {
        glfwSetWindowContentScaleCallback(IVY_PLATFORM.handle, Ivy_Window_ContentScaleCallback);
    }

    glfwSetKeyCallback(IVY_PLATFORM.handle,         Ivy_Input_KeyCallback);
    glfwSetMouseButtonCallback(IVY_PLATFORM.handle, Ivy_Input_MouseButtonCallback);
    glfwSetCursorPosCallback(IVY_PLATFORM.handle,   Ivy_Input_CursorPosCallback);

    glfwSetInputMode(IVY_PLATFORM.handle, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

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
    IVY_CORE.input.keyboard.exitKey    = IVY_KEY_ESCAPE;
    IVY_CORE.input.mouse.scale         = (IvyVector2){ 1.0f, 1.0f };
    IVY_CORE.input.mouse.cursor        = IVY_MOUSE_CURSOR_ARROW;

    Ivy_Core_InitPlatform(arena);

    if (IVY_CORE.window.render.width == 0 || IVY_CORE.window.render.height == 0)
        IVY_CORE.window.render = IVY_CORE.window.screen;

    Ivy_Gfx_Init((int)IVY_CORE.window.render.width, (int)IVY_CORE.window.render.height);

    IVY_CORE.window.shouldClose = false;
}

void Ivy_Core_CloseWindow(void)
{
    IVY_CORE.window.ready = false;
    glfwDestroyWindow(IVY_PLATFORM.handle);
    glfwTerminate();

    Ivy_GLFW_UnloadFunctions();
}

double Ivy_Core_GetTime(void)
{
    return glfwGetTime();
}