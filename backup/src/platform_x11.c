#ifndef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "platform_internal.h"

int MapX11KeyToIvy(KeySym sym)
{
    if (sym >= 32 && sym <= 126) {

        if (sym >= 'a' && sym <= 'z') {
            return sym - 32;
        }
        return sym;
    }

    switch (sym) {
        case XK_Escape:       return IVY_KEY_ESCAPE;
        case XK_Return:       return IVY_KEY_ENTER;
        case XK_Tab:          return IVY_KEY_TAB;
        case XK_BackSpace:    return IVY_KEY_BACKSPACE;
        case XK_Insert:       return IVY_KEY_INSERT;
        case XK_Delete:       return IVY_KEY_DELETE;
        case XK_Right:        return IVY_KEY_RIGHT;
        case XK_Left:         return IVY_KEY_LEFT;
        case XK_Down:         return IVY_KEY_DOWN;
        case XK_Up:           return IVY_KEY_UP;
        case XK_Page_Up:      return IVY_KEY_PAGE_UP;
        case XK_Page_Down:    return IVY_KEY_PAGE_DOWN;
        case XK_Home:         return IVY_KEY_HOME;
        case XK_End:          return IVY_KEY_END;
        case XK_Caps_Lock:    return IVY_KEY_CAPS_LOCK;
        case XK_Scroll_Lock:  return IVY_KEY_SCROLL_LOCK;
        case XK_Num_Lock:     return IVY_KEY_NUM_LOCK;
        case XK_Print:        return IVY_KEY_PRINT_SCREEN;
        case XK_Pause:        return IVY_KEY_PAUSE;
        case XK_F1:           return IVY_KEY_F1;
        case XK_F2:           return IVY_KEY_F2;
        case XK_F3:           return IVY_KEY_F3;
        case XK_F4:           return IVY_KEY_F4;
        case XK_F5:           return IVY_KEY_F5;
        case XK_F6:           return IVY_KEY_F6;
        case XK_F7:           return IVY_KEY_F7;
        case XK_F8:           return IVY_KEY_F8;
        case XK_F9:           return IVY_KEY_F9;
        case XK_F10:          return IVY_KEY_F10;
        case XK_F11:          return IVY_KEY_F11;
        case XK_F12:          return IVY_KEY_F12;
        case XK_Shift_L:      return IVY_KEY_LEFT_SHIFT;
        case XK_Control_L:    return IVY_KEY_LEFT_CONTROL;
        case XK_Alt_L:        return IVY_KEY_LEFT_ALT;
        case XK_Super_L:      return IVY_KEY_LEFT_SUPER;
        case XK_Shift_R:      return IVY_KEY_RIGHT_SHIFT;
        case XK_Control_R:    return IVY_KEY_RIGHT_CONTROL;
        case XK_Alt_R:        return IVY_KEY_RIGHT_ALT;
        case XK_Super_R:      return IVY_KEY_RIGHT_SUPER;
        case XK_Menu:         return IVY_KEY_KB_MENU;
        case XK_KP_0:         return IVY_KEY_KP_0;
        case XK_KP_1:         return IVY_KEY_KP_1;
        case XK_KP_2:         return IVY_KEY_KP_2;
        case XK_KP_3:         return IVY_KEY_KP_3;
        case XK_KP_4:         return IVY_KEY_KP_4;
        case XK_KP_5:         return IVY_KEY_KP_5;
        case XK_KP_6:         return IVY_KEY_KP_6;
        case XK_KP_7:         return IVY_KEY_KP_7;
        case XK_KP_8:         return IVY_KEY_KP_8;
        case XK_KP_9:         return IVY_KEY_KP_9;
        case XK_KP_Decimal:   return IVY_KEY_KP_DECIMAL;
        case XK_KP_Divide:    return IVY_KEY_KP_DIVIDE;
        case XK_KP_Multiply:  return IVY_KEY_KP_MULTIPLY;
        case XK_KP_Subtract:  return IVY_KEY_KP_SUBTRACT;
        case XK_KP_Add:       return IVY_KEY_KP_ADD;
        case XK_KP_Enter:     return IVY_KEY_KP_ENTER;
        case XK_KP_Equal:     return IVY_KEY_KP_EQUAL;
        default:              return 0;
    }
}

bool Ivy_Platform_Init_X11(const char *title, int width, int height)
{
    g_Platform.x11.display = XOpenDisplay(NULL);
    if (!g_Platform.x11.display) {
        fprintf(stderr, "[X11] Failed to open display\n");
        return false;
    }

    g_Platform.x11.screen = DefaultScreen(g_Platform.x11.display);

    if (!Ivy_EGL_Init((EGLNativeDisplayType)g_Platform.x11.display)) {
        XCloseDisplay(g_Platform.x11.display);
        return false;
    }

    EGLint visualId;
    eglGetConfigAttrib(g_Platform.eglDisplay, g_Platform.eglConfig, EGL_NATIVE_VISUAL_ID, &visualId);

    XVisualInfo visualTemplate;
    visualTemplate.visualid = visualId;
    int numVisuals;
    XVisualInfo *visualInfo = XGetVisualInfo(g_Platform.x11.display, VisualIDMask, &visualTemplate, &numVisuals);
    if (!visualInfo) {
        fprintf(stderr, "[X11] Failed to find matching visual info\n");
        Ivy_EGL_Shutdown();
        XCloseDisplay(g_Platform.x11.display);
        return false;
    }

    Colormap colormap = XCreateColormap(g_Platform.x11.display, RootWindow(g_Platform.x11.display, g_Platform.x11.screen), visualInfo->visual, AllocNone);

    XSetWindowAttributes windowAttribs;
    windowAttribs.colormap = colormap;
    windowAttribs.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask | FocusChangeMask;

    g_Platform.x11.window = XCreateWindow(
        g_Platform.x11.display,
        RootWindow(g_Platform.x11.display, g_Platform.x11.screen),
        0, 0, width, height, 0,
        visualInfo->depth,
        InputOutput,
        visualInfo->visual,
        CWColormap | CWEventMask,
        &windowAttribs
    );

    XFree(visualInfo);

    if (!g_Platform.x11.window) {
        fprintf(stderr, "[X11] Failed to create window\n");
        Ivy_EGL_Shutdown();
        XCloseDisplay(g_Platform.x11.display);
        return false;
    }

    XStoreName(g_Platform.x11.display, g_Platform.x11.window, title);

    g_Platform.x11.wmDeleteMessage = XInternAtom(g_Platform.x11.display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(g_Platform.x11.display, g_Platform.x11.window, &g_Platform.x11.wmDeleteMessage, 1);

    XMapWindow(g_Platform.x11.display, g_Platform.x11.window);

    if (!Ivy_EGL_CreateWindowSurface((EGLNativeWindowType)g_Platform.x11.window)) {
        XDestroyWindow(g_Platform.x11.display, g_Platform.x11.window);
        Ivy_EGL_Shutdown();
        XCloseDisplay(g_Platform.x11.display);
        return false;
    }

    IVY_CORE.window.ready = true;
    IVY_CORE.window.screen.width = width;
    IVY_CORE.window.screen.height = height;
    IVY_CORE.window.render = IVY_CORE.window.screen;
    IVY_CORE.window.currentFBO = IVY_CORE.window.screen;
    g_Platform.shouldClose = false;
    g_Platform.x11.isFullscreen = false;

    Ivy_Platform_SetWindowState(IVY_CORE.window.flags);

    return true;
}

void Ivy_Platform_Shutdown_X11(void)
{
    Ivy_EGL_Shutdown();
    if (g_Platform.x11.display) {
        if (g_Platform.x11.window) {
            XDestroyWindow(g_Platform.x11.display, g_Platform.x11.window);
            g_Platform.x11.window = None;
        }
        XCloseDisplay(g_Platform.x11.display);
        g_Platform.x11.display = NULL;
    }
}

void Ivy_Platform_PollEvents_X11(void)
{
    while (XPending(g_Platform.x11.display)) {
        XEvent event;
        XNextEvent(g_Platform.x11.display, &event);

        switch (event.type) {
            case ConfigureNotify: {
                int newWidth = event.xconfigure.width;
                int newHeight = event.xconfigure.height;

                if (newWidth != (int)IVY_CORE.window.screen.width || newHeight != (int)IVY_CORE.window.screen.height) {
                    IVY_CORE.window.screen.width = newWidth;
                    IVY_CORE.window.screen.height = newHeight;
                    IVY_CORE.window.render = IVY_CORE.window.screen;
                    IVY_CORE.window.currentFBO = IVY_CORE.window.screen;
                    IVY_CORE.window.resizedLastFrame = true;
                }
                break;
            }
            case ClientMessage: {
                if (event.xclient.data.l[0] == (long)g_Platform.x11.wmDeleteMessage) {
                    g_Platform.shouldClose = true;
                }
                break;
            }
            case KeyPress:
            case KeyRelease: {
                KeySym sym = XLookupKeysym(&event.xkey, 0);
                int key = MapX11KeyToIvy(sym);
                if (key > 0 && key < IVY_MAX_KEYBOARD_KEYS) {
                    IVY_CORE.input.keyboard.currentKeyState[key] = (event.type == KeyPress) ? 1 : 0;
                }
                break;
            }
            case ButtonPress:
            case ButtonRelease: {
                int button = -1;
                if (event.xbutton.button == Button1) button = 0;
                else if (event.xbutton.button == Button2) button = 2;
                else if (event.xbutton.button == Button3) button = 1;

                if (event.type == ButtonPress) {
                    if (event.xbutton.button == Button4) {
                        IVY_CORE.input.mouse.currentWheelMove.y = 1.0f;
                    }
                    else if (event.xbutton.button == Button5) {
                        IVY_CORE.input.mouse.currentWheelMove.y = -1.0f;
                    }
                }

                if (button >= 0 && button < IVY_MAX_MOUSE_BUTTONS) {
                    IVY_CORE.input.mouse.currentButtonState[button] = (event.type == ButtonPress) ? 1 : 0;
                }
                break;
            }
            case MotionNotify: {
                IVY_CORE.input.mouse.currentPosition.x = (float)event.xmotion.x;
                IVY_CORE.input.mouse.currentPosition.y = (float)event.xmotion.y;
                break;
            }
            case FocusIn: {
                IVY_FLAG_CLEAR(IVY_CORE.window.flags, IVY_FLAG_WINDOW_UNFOCUSED);
                break;
            }
            case FocusOut: {
                IVY_FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_UNFOCUSED);
                break;
            }
        }
    }
}

void Ivy_Platform_SetWindowPosition_X11(int x, int y)
{
    XMoveWindow(g_Platform.x11.display, g_Platform.x11.window, x, y);
    XFlush(g_Platform.x11.display);
}

void Ivy_Platform_SetWindowState_X11(u32 flags)
{

    XSizeHints *hints = XAllocSizeHints();
    if (hints) {
        if (IVY_FLAG_IS_SET(flags, IVY_FLAG_WINDOW_RESIZABLE)) {
            hints->flags = 0;
        } else {
            hints->flags = PMinSize | PMaxSize;
            hints->min_width = hints->max_width = IVY_CORE.window.screen.width;
            hints->min_height = hints->max_height = IVY_CORE.window.screen.height;
        }
        XSetWMNormalHints(g_Platform.x11.display, g_Platform.x11.window, hints);
        XFree(hints);
    }

    Atom motifHintsAtom = XInternAtom(g_Platform.x11.display, "_MOTIF_WM_HINTS", True);
    if (motifHintsAtom != None) {
        struct {
            unsigned long flags;
            unsigned long functions;
            unsigned long decorations;
            long input_mode;
            unsigned long status;
        } motifHints;
        memset(&motifHints, 0, sizeof(motifHints));
        motifHints.flags = 2;
        motifHints.decorations = IVY_FLAG_IS_SET(flags, IVY_FLAG_WINDOW_UNDECORATED) ? 0 : 1;
        XChangeProperty(g_Platform.x11.display, g_Platform.x11.window, motifHintsAtom, motifHintsAtom, 32, PropModeReplace, (unsigned char *)&motifHints, 5);
    }

    XFlush(g_Platform.x11.display);
}

void Ivy_Platform_ToggleFullscreen_X11(void)
{
    Atom wmState = XInternAtom(g_Platform.x11.display, "_NET_WM_STATE", False);
    Atom wmFullscreen = XInternAtom(g_Platform.x11.display, "_NET_WM_STATE_FULLSCREEN", False);

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = g_Platform.x11.window;
    xev.xclient.message_type = wmState;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = g_Platform.x11.isFullscreen ? 0 : 1;
    xev.xclient.data.l[1] = wmFullscreen;
    xev.xclient.data.l[2] = 0;

    XSendEvent(
        g_Platform.x11.display,
        DefaultRootWindow(g_Platform.x11.display),
        False,
        SubstructureNotifyMask | SubstructureRedirectMask,
        &xev
    );

    g_Platform.x11.isFullscreen = !g_Platform.x11.isFullscreen;
    XFlush(g_Platform.x11.display);
}

void Ivy_Platform_ToggleBorderlessWindow_X11(void)
{

    if (IVY_FLAG_IS_SET(IVY_CORE.window.flags, IVY_FLAG_BORDERLESS_WINDOWED_MODE)) {

        IVY_FLAG_CLEAR(IVY_CORE.window.flags, IVY_FLAG_WINDOW_UNDECORATED);
        IVY_FLAG_CLEAR(IVY_CORE.window.flags, IVY_FLAG_BORDERLESS_WINDOWED_MODE);
        Ivy_Platform_SetWindowState(IVY_CORE.window.flags);
        XResizeWindow(g_Platform.x11.display, g_Platform.x11.window, IVY_CORE.window.prevScreen.width, IVY_CORE.window.prevScreen.height);
    } else {

        IVY_CORE.window.prevScreen = IVY_CORE.window.screen;

        IVY_FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_UNDECORATED);
        IVY_FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_BORDERLESS_WINDOWED_MODE);
        Ivy_Platform_SetWindowState(IVY_CORE.window.flags);

        int scr = DefaultScreen(g_Platform.x11.display);
        int scrW = DisplayWidth(g_Platform.x11.display, scr);
        int scrH = DisplayHeight(g_Platform.x11.display, scr);
        XMoveResizeWindow(g_Platform.x11.display, g_Platform.x11.window, 0, 0, scrW, scrH);
    }
    XFlush(g_Platform.x11.display);
}

void Ivy_Platform_SwapBuffers_X11(void)
{
    eglSwapBuffers(g_Platform.eglDisplay, g_Platform.eglSurface);
}

int Ivy_Platform_GetCurrentMonitor_X11(void)
{

    return 0;
}

IvyVector2 Ivy_Platform_GetWindowScaleDPI_X11(void)
{

    return (IvyVector2){1.0f, 1.0f};
}

void *Ivy_Platform_GetProcAddress_X11(const char *name)
{
    return (void *)eglGetProcAddress(name);
}

#endif
