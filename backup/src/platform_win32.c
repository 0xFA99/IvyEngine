#ifdef _WIN32
#include "platform_internal.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef WGL_CONTEXT_MAJOR_VERSION_ARB
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#endif
#ifndef WGL_CONTEXT_MINOR_VERSION_ARB
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#endif
#ifndef WGL_CONTEXT_PROFILE_MASK_ARB
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#endif
#ifndef WGL_CONTEXT_CORE_PROFILE_BIT_ARB
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#endif
#ifndef WGL_DRAW_TO_WINDOW_ARB
#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#endif
#ifndef WGL_ACCELERATION_ARB
#define WGL_ACCELERATION_ARB 0x2003
#endif
#ifndef WGL_SUPPORT_OPENGL_ARB
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#endif
#ifndef WGL_DOUBLE_BUFFER_ARB
#define WGL_DOUBLE_BUFFER_ARB 0x2011
#endif
#ifndef WGL_PIXEL_TYPE_ARB
#define WGL_PIXEL_TYPE_ARB 0x2013
#endif
#ifndef WGL_TYPE_RGBA_ARB
#define WGL_TYPE_RGBA_ARB 0x202B
#endif
#ifndef WGL_COLOR_BITS_ARB
#define WGL_COLOR_BITS_ARB 0x2014
#endif
#ifndef WGL_DEPTH_BITS_ARB
#define WGL_DEPTH_BITS_ARB 0x2022
#endif
#ifndef WGL_STENCIL_BITS_ARB
#define WGL_STENCIL_BITS_ARB 0x2023
#endif
#ifndef WGL_FULL_ACCELERATION_ARB
#define WGL_FULL_ACCELERATION_ARB 0x2027
#endif

typedef HGLRC(WINAPI *PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext, const int *attribList);
typedef BOOL(WINAPI *PFNWGLCHOOSEPIXELFORMATARBPROC)(HDC hdc, const int *piAttribIList, const float *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
typedef BOOL(WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);

static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;

static bool InitWGL(void)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSEXA dummyWc = {0};
    dummyWc.cbSize = sizeof(WNDCLASSEXA);
    dummyWc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    dummyWc.lpfnWndProc = DefWindowProcA;
    dummyWc.hInstance = hInstance;
    dummyWc.lpszClassName = "IvyDummyWindowClass";

    RegisterClassExA(&dummyWc);

    HWND dummyHWnd = CreateWindowExA(
        0, "IvyDummyWindowClass", "Dummy",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL
    );

    if (!dummyHWnd) {
        UnregisterClassA("IvyDummyWindowClass", hInstance);
        return false;
    }

    HDC dummyHDC = GetDC(dummyHWnd);

    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(dummyHDC, &pfd);
    SetPixelFormat(dummyHDC, pixelFormat, &pfd);

    HGLRC dummyHRC = wglCreateContext(dummyHDC);
    if (!dummyHRC) {
        ReleaseDC(dummyHWnd, dummyHDC);
        DestroyWindow(dummyHWnd);
        UnregisterClassA("IvyDummyWindowClass", hInstance);
        return false;
    }

    wglMakeCurrent(dummyHDC, dummyHRC);

    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(dummyHRC);
    ReleaseDC(dummyHWnd, dummyHDC);
    DestroyWindow(dummyHWnd);
    UnregisterClassA("IvyDummyWindowClass", hInstance);

    return true;
}

static int MapWin32KeyToIvy(WPARAM wParam)
{

    if ((wParam >= 'A' && wParam <= 'Z') || (wParam >= '0' && wParam <= '9') || wParam == VK_SPACE) {
        return (int)wParam;
    }

    switch (wParam) {
        case VK_ESCAPE:     return IVY_KEY_ESCAPE;
        case VK_RETURN:     return IVY_KEY_ENTER;
        case VK_TAB:        return IVY_KEY_TAB;
        case VK_BACK:       return IVY_KEY_BACKSPACE;
        case VK_INSERT:     return IVY_KEY_INSERT;
        case VK_DELETE:     return IVY_KEY_DELETE;
        case VK_RIGHT:      return IVY_KEY_RIGHT;
        case VK_LEFT:       return IVY_KEY_LEFT;
        case VK_DOWN:       return IVY_KEY_DOWN;
        case VK_UP:         return IVY_KEY_UP;
        case VK_PRIOR:      return IVY_KEY_PAGE_UP;
        case VK_NEXT:       return IVY_KEY_PAGE_DOWN;
        case VK_HOME:       return IVY_KEY_HOME;
        case VK_END:        return IVY_KEY_END;
        case VK_CAPITAL:    return IVY_KEY_CAPS_LOCK;
        case VK_SCROLL:     return IVY_KEY_SCROLL_LOCK;
        case VK_NUMLOCK:    return IVY_KEY_NUM_LOCK;
        case VK_SNAPSHOT:   return IVY_KEY_PRINT_SCREEN;
        case VK_PAUSE:      return IVY_KEY_PAUSE;
        case VK_F1:         return IVY_KEY_F1;
        case VK_F2:         return IVY_KEY_F2;
        case VK_F3:         return IVY_KEY_F3;
        case VK_F4:         return IVY_KEY_F4;
        case VK_F5:         return IVY_KEY_F5;
        case VK_F6:         return IVY_KEY_F6;
        case VK_F7:         return IVY_KEY_F7;
        case VK_F8:         return IVY_KEY_F8;
        case VK_F9:         return IVY_KEY_F9;
        case VK_F10:        return IVY_KEY_F10;
        case VK_F11:        return IVY_KEY_F11;
        case VK_F12:        return IVY_KEY_F12;
        case VK_LSHIFT:     return IVY_KEY_LEFT_SHIFT;
        case VK_LCONTROL:   return IVY_KEY_LEFT_CONTROL;
        case VK_LMENU:      return IVY_KEY_LEFT_ALT;
        case VK_LWIN:       return IVY_KEY_LEFT_SUPER;
        case VK_RSHIFT:     return IVY_KEY_RIGHT_SHIFT;
        case VK_RCONTROL:   return IVY_KEY_RIGHT_CONTROL;
        case VK_RMENU:      return IVY_KEY_RIGHT_ALT;
        case VK_RWIN:       return IVY_KEY_RIGHT_SUPER;
        case VK_APPS:       return IVY_KEY_KB_MENU;
        case VK_NUMPAD0:    return IVY_KEY_KP_0;
        case VK_NUMPAD1:    return IVY_KEY_KP_1;
        case VK_NUMPAD2:    return IVY_KEY_KP_2;
        case VK_NUMPAD3:    return IVY_KEY_KP_3;
        case VK_NUMPAD4:    return IVY_KEY_KP_4;
        case VK_NUMPAD5:    return IVY_KEY_KP_5;
        case VK_NUMPAD6:    return IVY_KEY_KP_6;
        case VK_NUMPAD7:    return IVY_KEY_KP_7;
        case VK_NUMPAD8:    return IVY_KEY_KP_8;
        case VK_NUMPAD9:    return IVY_KEY_KP_9;
        case VK_DECIMAL:    return IVY_KEY_KP_DECIMAL;
        case VK_DIVIDE:     return IVY_KEY_KP_DIVIDE;
        case VK_MULTIPLY:   return IVY_KEY_KP_MULTIPLY;
        case VK_SUBTRACT:   return IVY_KEY_KP_SUBTRACT;
        case VK_ADD:        return IVY_KEY_KP_ADD;
        default:            return 0;
    }
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            if (width > 0 && height > 0) {
                if (width != (int)IVY_CORE.window.screen.width || height != (int)IVY_CORE.window.screen.height) {
                    IVY_CORE.window.screen.width = width;
                    IVY_CORE.window.screen.height = height;
                    IVY_CORE.window.render = IVY_CORE.window.screen;
                    IVY_CORE.window.currentFBO = IVY_CORE.window.screen;
                    IVY_CORE.window.resizedLastFrame = true;
                }
            }
            return 0;
        }
        case WM_CLOSE: {
            g_Platform.shouldClose = true;
            return 0;
        }
        case WM_KEYDOWN:
        case WM_KEYUP: {
            int key = MapWin32KeyToIvy(wParam);
            if (key > 0 && key < IVY_MAX_KEYBOARD_KEYS) {
                IVY_CORE.input.keyboard.currentKeyState[key] = (message == WM_KEYDOWN) ? 1 : 0;
            }
            return 0;
        }
        case WM_MOUSEMOVE: {
            IVY_CORE.input.mouse.currentPosition.x = (float)LOWORD(lParam);
            IVY_CORE.input.mouse.currentPosition.y = (float)HIWORD(lParam);
            return 0;
        }
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP: {
            IVY_CORE.input.mouse.currentButtonState[0] = (message == WM_LBUTTONDOWN) ? 1 : 0;
            return 0;
        }
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP: {
            IVY_CORE.input.mouse.currentButtonState[1] = (message == WM_RBUTTONDOWN) ? 1 : 0;
            return 0;
        }
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP: {
            IVY_CORE.input.mouse.currentButtonState[2] = (message == WM_MBUTTONDOWN) ? 1 : 0;
            return 0;
        }
        case WM_MOUSEWHEEL: {
            int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            IVY_CORE.input.mouse.currentWheelMove.y = (delta > 0) ? 1.0f : -1.0f;
            return 0;
        }
        case WM_SETFOCUS: {
            IVY_FLAG_CLEAR(IVY_CORE.window.flags, IVY_FLAG_WINDOW_UNFOCUSED);
            return 0;
        }
        case WM_KILLFOCUS: {
            IVY_FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_UNFOCUSED);
            return 0;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

bool Ivy_Platform_Init_Win32(const char *title, int width, int height)
{
    g_Platform.win32.hInstance = GetModuleHandle(NULL);

    WNDCLASSEXA wc;
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = g_Platform.win32.hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "IvyWindowClass";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassExA(&wc)) {
        fprintf(stderr, "[Win32] Failed to register window class\n");
        return false;
    }

    RECT rect = { 0, 0, width, height };
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW);

    g_Platform.win32.hWnd = CreateWindowExA(
        WS_EX_APPWINDOW,
        "IvyWindowClass",
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        NULL,
        NULL,
        g_Platform.win32.hInstance,
        NULL
    );

    if (!g_Platform.win32.hWnd) {
        fprintf(stderr, "[Win32] Failed to create window\n");
        return false;
    }

    g_Platform.win32.hDC = GetDC(g_Platform.win32.hWnd);

    if (!InitWGL()) {
        DestroyWindow(g_Platform.win32.hWnd);
        return false;
    }

    int format = 0;
    UINT numFormats = 0;
    PIXELFORMATDESCRIPTOR pfd = {0};

    #ifndef WGL_SAMPLE_BUFFERS_ARB
    #define WGL_SAMPLE_BUFFERS_ARB 0x2041
    #endif
    #ifndef WGL_SAMPLES_ARB
    #define WGL_SAMPLES_ARB 0x2042
    #endif

    if (wglChoosePixelFormatARB) {
        int pixelFormatAttribs[24] = {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB, 32,
            WGL_DEPTH_BITS_ARB, 24,
            WGL_STENCIL_BITS_ARB, 8,
        };
        int attribIndex = 16;
        if (IVY_CORE.window.flags & IVY_FLAG_MSAA_4X_HINT) {
            pixelFormatAttribs[attribIndex++] = WGL_SAMPLE_BUFFERS_ARB;
            pixelFormatAttribs[attribIndex++] = GL_TRUE;
            pixelFormatAttribs[attribIndex++] = WGL_SAMPLES_ARB;
            pixelFormatAttribs[attribIndex++] = 4;
        }
        pixelFormatAttribs[attribIndex] = 0;

        wglChoosePixelFormatARB(g_Platform.win32.hDC, pixelFormatAttribs, NULL, 1, &format, &numFormats);
        if (numFormats == 0 || format == 0) {
            if (IVY_CORE.window.flags & IVY_FLAG_MSAA_4X_HINT) {
                printf("[WGL WARNING] MSAA 4x not supported, falling back to no MSAA\n");
                pixelFormatAttribs[16] = 0;
                wglChoosePixelFormatARB(g_Platform.win32.hDC, pixelFormatAttribs, NULL, 1, &format, &numFormats);
            }
        }
    }

    if (numFormats == 0 || format == 0) {
        PIXELFORMATDESCRIPTOR fallbackPfd = {0};
        fallbackPfd.nSize = sizeof(fallbackPfd);
        fallbackPfd.nVersion = 1;
        fallbackPfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        fallbackPfd.iPixelType = PFD_TYPE_RGBA;
        fallbackPfd.cColorBits = 32;
        fallbackPfd.cDepthBits = 24;
        fallbackPfd.cStencilBits = 8;
        fallbackPfd.iLayerType = PFD_MAIN_PLANE;
        format = ChoosePixelFormat(g_Platform.win32.hDC, &fallbackPfd);
    }

    DescribePixelFormat(g_Platform.win32.hDC, format, sizeof(pfd), &pfd);
    SetPixelFormat(g_Platform.win32.hDC, format, &pfd);

    if (wglCreateContextAttribsARB) {
        int contextAttribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
            WGL_CONTEXT_MINOR_VERSION_ARB, 3,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };
        g_Platform.win32.hRC = wglCreateContextAttribsARB(g_Platform.win32.hDC, NULL, contextAttribs);
    }

    if (!g_Platform.win32.hRC) {
        g_Platform.win32.hRC = wglCreateContext(g_Platform.win32.hDC);
    }

    if (!g_Platform.win32.hRC) {
        fprintf(stderr, "[Win32] Failed to create OpenGL context\n");
        DestroyWindow(g_Platform.win32.hWnd);
        return false;
    }

    if (!wglMakeCurrent(g_Platform.win32.hDC, g_Platform.win32.hRC)) {
        fprintf(stderr, "[Win32] Failed to make OpenGL context current\n");
        wglDeleteContext(g_Platform.win32.hRC);
        DestroyWindow(g_Platform.win32.hWnd);
        return false;
    }

    if (wglSwapIntervalEXT) {
        wglSwapIntervalEXT(1);
    }

    ShowWindow(g_Platform.win32.hWnd, SW_SHOW);
    UpdateWindow(g_Platform.win32.hWnd);

    IVY_CORE.window.ready = true;
    IVY_CORE.window.screen.width = width;
    IVY_CORE.window.screen.height = height;
    IVY_CORE.window.render = IVY_CORE.window.screen;
    IVY_CORE.window.currentFBO = IVY_CORE.window.screen;
    g_Platform.shouldClose = false;

    return true;
}

void Ivy_Platform_Shutdown_Win32(void)
{
    if (g_Platform.win32.hRC) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(g_Platform.win32.hRC);
        g_Platform.win32.hRC = NULL;
    }
    if (g_Platform.win32.hWnd) {
        if (g_Platform.win32.hDC) {
            ReleaseDC(g_Platform.win32.hWnd, g_Platform.win32.hDC);
            g_Platform.win32.hDC = NULL;
        }
        DestroyWindow(g_Platform.win32.hWnd);
        g_Platform.win32.hWnd = NULL;
    }
    UnregisterClassA("IvyWindowClass", g_Platform.win32.hInstance);
}

void Ivy_Platform_PollEvents_Win32(void)
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Ivy_Platform_SetWindowPosition_Win32(int x, int y)
{
    SetWindowPos(g_Platform.win32.hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Ivy_Platform_SetWindowState_Win32(u32 flags)
{
    (void)flags;
}

void Ivy_Platform_ToggleFullscreen_Win32(void)
{
    static RECT rectSaved;
    static DWORD styleSaved;
    if (!g_Platform.win32.isFullscreen) {
        styleSaved = GetWindowLong(g_Platform.win32.hWnd, GWL_STYLE);
        GetWindowRect(g_Platform.win32.hWnd, &rectSaved);

        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);

        SetWindowLong(g_Platform.win32.hWnd, GWL_STYLE, styleSaved & ~WS_OVERLAPPEDWINDOW);
        SetWindowPos(g_Platform.win32.hWnd, HWND_TOP, 0, 0, width, height, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        g_Platform.win32.isFullscreen = true;
    } else {
        SetWindowLong(g_Platform.win32.hWnd, GWL_STYLE, styleSaved);
        SetWindowPos(g_Platform.win32.hWnd, NULL, rectSaved.left, rectSaved.top,
                     rectSaved.right - rectSaved.left, rectSaved.bottom - rectSaved.top,
                     SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
        g_Platform.win32.isFullscreen = false;
    }
}

void Ivy_Platform_ToggleBorderlessWindow_Win32(void)
{
    Ivy_Platform_ToggleFullscreen_Win32();
}

void Ivy_Platform_SwapBuffers_Win32(void)
{
    SwapBuffers(g_Platform.win32.hDC);
}

int Ivy_Platform_GetCurrentMonitor_Win32(void)
{
    return 0;
}

IvyVector2 Ivy_Platform_GetWindowScaleDPI_Win32(void)
{
    return (IvyVector2){1.0f, 1.0f};
}

void *Ivy_Platform_GetProcAddress_Win32(const char *name)
{
    void *p = (void *)wglGetProcAddress(name);
    if (p == 0 || (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) || (p == (void*)-1)) {
        static HMODULE openGL32_lib = NULL;
        if (!openGL32_lib) {
            openGL32_lib = LoadLibraryA("opengl32.dll");
        }
        if (openGL32_lib) {
            p = (void *)GetProcAddress(openGL32_lib, name);
        }
    }
    return p;
}

#endif
