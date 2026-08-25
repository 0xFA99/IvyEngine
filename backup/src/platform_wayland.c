#ifndef _WIN32
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include "platform_internal.h"
#include "xdg-shell-client-protocol.h"

extern int MapX11KeyToIvy(KeySym sym);

static void registry_handle_global(void *data, struct wl_registry *registry,
                                   uint32_t id, const char *interface, uint32_t version)
{
    (void)version;
    if (strcmp(interface, "wl_compositor") == 0) {
        g_Platform.wayland.compositor = wl_registry_bind(registry, id, &wl_compositor_interface, 1);
    } else if (strcmp(interface, "xdg_wm_base") == 0) {
        g_Platform.wayland.xdg_wm_base = wl_registry_bind(registry, id, &xdg_wm_base_interface, 1);
    } else if (strcmp(interface, "wl_shm") == 0) {
        g_Platform.wayland.shm = wl_registry_bind(registry, id, &wl_shm_interface, 1);
    } else if (strcmp(interface, "wl_seat") == 0) {
        g_Platform.wayland.seat = wl_registry_bind(registry, id, &wl_seat_interface, 1);
    }
}

static void registry_handle_global_remove(void *data, struct wl_registry *registry, uint32_t name)
{
    (void)data; (void)registry; (void)name;
}

static const struct wl_registry_listener registry_listener = {
    registry_handle_global,
    registry_handle_global_remove
};

static void keyboard_handle_keymap(void *data, struct wl_keyboard *keyboard,
                                   uint32_t format, int32_t fd, uint32_t size)
{
    (void)keyboard;
    if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) {
        close(fd);
        return;
    }

    char *map_str = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
    if (map_str == MAP_FAILED) {
        close(fd);
        return;
    }

    if (g_Platform.wayland.xkb_keymap) {
        xkb_keymap_unref(g_Platform.wayland.xkb_keymap);
    }
    g_Platform.wayland.xkb_keymap = xkb_keymap_new_from_string(
        g_Platform.wayland.xkb_context, map_str, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);

    munmap(map_str, size);
    close(fd);

    if (g_Platform.wayland.xkb_state) {
        xkb_state_unref(g_Platform.wayland.xkb_state);
    }
    g_Platform.wayland.xkb_state = xkb_state_new(g_Platform.wayland.xkb_keymap);
}

static void keyboard_handle_enter(void *data, struct wl_keyboard *keyboard,
                                  uint32_t serial, struct wl_surface *surface,
                                  struct wl_array *keys)
{
    (void)keyboard; (void)serial; (void)surface; (void)keys;
    IVY_FLAG_CLEAR(IVY_CORE.window.flags, IVY_FLAG_WINDOW_UNFOCUSED);
}

static void keyboard_handle_leave(void *data, struct wl_keyboard *keyboard,
                                  uint32_t serial, struct wl_surface *surface)
{
    (void)keyboard; (void)serial; (void)surface;
    IVY_FLAG_SET(IVY_CORE.window.flags, IVY_FLAG_WINDOW_UNFOCUSED);
}

static void keyboard_handle_key(void *data, struct wl_keyboard *keyboard,
                                uint32_t serial, uint32_t time, uint32_t key, uint32_t state)
{
    (void)keyboard; (void)serial; (void)time;
    if (!g_Platform.wayland.xkb_state) return;

    xkb_keycode_t xkb_key = key + 8;
    xkb_keysym_t sym = xkb_state_key_get_one_sym(g_Platform.wayland.xkb_state, xkb_key);

    int ivy_key = MapX11KeyToIvy((KeySym)sym);
    if (ivy_key > 0 && ivy_key < IVY_MAX_KEYBOARD_KEYS) {
        IVY_CORE.input.keyboard.currentKeyState[ivy_key] = (state == WL_KEYBOARD_KEY_STATE_PRESSED) ? 1 : 0;
    }
}

static void keyboard_handle_modifiers(void *data, struct wl_keyboard *keyboard,
                                      uint32_t serial, uint32_t mods_depressed,
                                      uint32_t mods_latched, uint32_t mods_locked, uint32_t group)
{
    (void)keyboard; (void)serial;
    if (!g_Platform.wayland.xkb_state) return;

    xkb_state_update_mask(g_Platform.wayland.xkb_state,
                          mods_depressed, mods_latched, mods_locked, 0, 0, group);
}

static void keyboard_handle_repeat_info(void *data, struct wl_keyboard *keyboard,
                                        int32_t rate, int32_t delay)
{
    (void)keyboard; (void)rate; (void)delay;
}

static const struct wl_keyboard_listener keyboard_listener = {
    keyboard_handle_keymap,
    keyboard_handle_enter,
    keyboard_handle_leave,
    keyboard_handle_key,
    keyboard_handle_modifiers,
    keyboard_handle_repeat_info
};

static void pointer_handle_enter(void *data, struct wl_pointer *pointer,
                                 uint32_t serial, struct wl_surface *surface,
                                 wl_fixed_t sx, wl_fixed_t sy)
{
    (void)pointer; (void)serial; (void)surface;
    IVY_CORE.input.mouse.currentPosition.x = (float)wl_fixed_to_double(sx);
    IVY_CORE.input.mouse.currentPosition.y = (float)wl_fixed_to_double(sy);
}

static void pointer_handle_leave(void *data, struct wl_pointer *pointer,
                                 uint32_t serial, struct wl_surface *surface)
{
    (void)pointer; (void)serial; (void)surface;
}

static void pointer_handle_motion(void *data, struct wl_pointer *pointer,
                                  uint32_t time, wl_fixed_t sx, wl_fixed_t sy)
{
    (void)pointer; (void)time;
    IVY_CORE.input.mouse.currentPosition.x = (float)wl_fixed_to_double(sx);
    IVY_CORE.input.mouse.currentPosition.y = (float)wl_fixed_to_double(sy);
}

static void pointer_handle_button(void *data, struct wl_pointer *pointer,
                                  uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
{
    (void)pointer; (void)serial; (void)time;
    int ivy_button = -1;

    if (button == 0x110)      ivy_button = 0;
    else if (button == 0x111) ivy_button = 1;
    else if (button == 0x112) ivy_button = 2;

    if (ivy_button >= 0 && ivy_button < IVY_MAX_MOUSE_BUTTONS) {
        IVY_CORE.input.mouse.currentButtonState[ivy_button] = (state == WL_POINTER_BUTTON_STATE_PRESSED) ? 1 : 0;
    }
}

static void pointer_handle_axis(void *data, struct wl_pointer *pointer,
                                uint32_t time, uint32_t axis, wl_fixed_t value)
{
    (void)pointer; (void)time;

    if (axis == WL_POINTER_AXIS_VERTICAL_SCROLL) {
        double val = wl_fixed_to_double(value);
        if (val > 0)      IVY_CORE.input.mouse.currentWheelMove.y = -1.0f;
        else if (val < 0) IVY_CORE.input.mouse.currentWheelMove.y = 1.0f;
    }
}

static const struct wl_pointer_listener pointer_listener = {
    pointer_handle_enter,
    pointer_handle_leave,
    pointer_handle_motion,
    pointer_handle_button,
    pointer_handle_axis
};

static void seat_handle_capabilities(void *data, struct wl_seat *seat, uint32_t caps)
{
    if ((caps & WL_SEAT_CAPABILITY_KEYBOARD) && !g_Platform.wayland.keyboard) {
        g_Platform.wayland.keyboard = wl_seat_get_keyboard(seat);
        wl_keyboard_add_listener(g_Platform.wayland.keyboard, &keyboard_listener, NULL);
    } else if (!(caps & WL_SEAT_CAPABILITY_KEYBOARD) && g_Platform.wayland.keyboard) {
        wl_keyboard_destroy(g_Platform.wayland.keyboard);
        g_Platform.wayland.keyboard = NULL;
    }

    if ((caps & WL_SEAT_CAPABILITY_POINTER) && !g_Platform.wayland.pointer) {
        g_Platform.wayland.pointer = wl_seat_get_pointer(seat);
        wl_pointer_add_listener(g_Platform.wayland.pointer, &pointer_listener, NULL);
    } else if (!(caps & WL_SEAT_CAPABILITY_POINTER) && g_Platform.wayland.pointer) {
        wl_pointer_destroy(g_Platform.wayland.pointer);
        g_Platform.wayland.pointer = NULL;
    }
}

static void seat_handle_name(void *data, struct wl_seat *seat, const char *name)
{
    (void)seat; (void)name;
}

static const struct wl_seat_listener seat_listener = {
    seat_handle_capabilities,
    seat_handle_name
};

static void xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    xdg_wm_base_ping
};

static void xdg_surface_configure(void *data, struct xdg_surface *xdg_surface, uint32_t serial)
{

    xdg_surface_ack_configure(xdg_surface, serial);
}

static const struct xdg_surface_listener xdg_surface_listener = {
    xdg_surface_configure
};

static void xdg_toplevel_configure(void *data, struct xdg_toplevel *xdg_toplevel,
                                    int32_t width, int32_t height, struct wl_array *states)
{
    (void)xdg_toplevel; (void)states;
    if (width > 0 && height > 0) {
        g_Platform.wayland.width = width;
        g_Platform.wayland.height = height;

        if (g_Platform.wayland.egl_window) {
            wl_egl_window_resize(g_Platform.wayland.egl_window, width, height, 0, 0);
        }

        if (width != (int)IVY_CORE.window.screen.width || height != (int)IVY_CORE.window.screen.height) {
            IVY_CORE.window.screen.width = width;
            IVY_CORE.window.screen.height = height;
            IVY_CORE.window.render = IVY_CORE.window.screen;
            IVY_CORE.window.currentFBO = IVY_CORE.window.screen;
            IVY_CORE.window.resizedLastFrame = true;
        }
    }
}

static void xdg_toplevel_close(void *data, struct xdg_toplevel *xdg_toplevel)
{
    (void)xdg_toplevel;
    g_Platform.shouldClose = true;
}

static const struct xdg_toplevel_listener xdg_toplevel_listener = {
    xdg_toplevel_configure,
    xdg_toplevel_close
};

bool Ivy_Platform_Init_Wayland(const char *title, int width, int height)
{
    g_Platform.wayland.display = wl_display_connect(NULL);
    if (!g_Platform.wayland.display) return false;

    g_Platform.wayland.xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    if (!g_Platform.wayland.xkb_context) {
        wl_display_disconnect(g_Platform.wayland.display);
        return false;
    }

    g_Platform.wayland.registry = wl_display_get_registry(g_Platform.wayland.display);
    wl_registry_add_listener(g_Platform.wayland.registry, &registry_listener, NULL);
    wl_display_roundtrip(g_Platform.wayland.display);

    if (!g_Platform.wayland.compositor || !g_Platform.wayland.xdg_wm_base) {
        fprintf(stderr, "[Wayland] Compositor or XDG WM Base interface missing\n");
        xkb_context_unref(g_Platform.wayland.xkb_context);
        wl_display_disconnect(g_Platform.wayland.display);
        return false;
    }

    xdg_wm_base_add_listener(g_Platform.wayland.xdg_wm_base, &xdg_wm_base_listener, NULL);

    if (g_Platform.wayland.seat) {
        wl_seat_add_listener(g_Platform.wayland.seat, &seat_listener, NULL);
        wl_display_roundtrip(g_Platform.wayland.display);
    }

    if (!Ivy_EGL_Init((EGLNativeDisplayType)g_Platform.wayland.display)) {
        xkb_context_unref(g_Platform.wayland.xkb_context);
        wl_display_disconnect(g_Platform.wayland.display);
        return false;
    }

    g_Platform.wayland.surface = wl_compositor_create_surface(g_Platform.wayland.compositor);
    g_Platform.wayland.xdg_surface = xdg_wm_base_get_xdg_surface(g_Platform.wayland.xdg_wm_base, g_Platform.wayland.surface);
    xdg_surface_add_listener(g_Platform.wayland.xdg_surface, &xdg_surface_listener, NULL);

    g_Platform.wayland.xdg_toplevel = xdg_surface_get_toplevel(g_Platform.wayland.xdg_surface);
    xdg_toplevel_add_listener(g_Platform.wayland.xdg_toplevel, &xdg_toplevel_listener, NULL);
    xdg_toplevel_set_title(g_Platform.wayland.xdg_toplevel, title);

    wl_surface_commit(g_Platform.wayland.surface);
    wl_display_roundtrip(g_Platform.wayland.display);

    g_Platform.wayland.width = width;
    g_Platform.wayland.height = height;

    g_Platform.wayland.egl_window = wl_egl_window_create(g_Platform.wayland.surface, width, height);
    if (!g_Platform.wayland.egl_window) {
        fprintf(stderr, "[Wayland] Failed to create EGL window surface\n");
        return false;
    }

    if (!Ivy_EGL_CreateWindowSurface((EGLNativeWindowType)g_Platform.wayland.egl_window)) {
        return false;
    }

    IVY_CORE.window.ready = true;
    IVY_CORE.window.screen.width = width;
    IVY_CORE.window.screen.height = height;
    IVY_CORE.window.render = IVY_CORE.window.screen;
    IVY_CORE.window.currentFBO = IVY_CORE.window.screen;
    g_Platform.shouldClose = false;
    g_Platform.wayland.isFullscreen = false;

    return true;
}

void Ivy_Platform_Shutdown_Wayland(void)
{
    Ivy_EGL_Shutdown();

    if (g_Platform.wayland.xkb_state) xkb_state_unref(g_Platform.wayland.xkb_state);
    if (g_Platform.wayland.xkb_keymap) xkb_keymap_unref(g_Platform.wayland.xkb_keymap);
    if (g_Platform.wayland.xkb_context) xkb_context_unref(g_Platform.wayland.xkb_context);

    if (g_Platform.wayland.egl_window) wl_egl_window_destroy(g_Platform.wayland.egl_window);
    if (g_Platform.wayland.xdg_toplevel) xdg_toplevel_destroy(g_Platform.wayland.xdg_toplevel);
    if (g_Platform.wayland.xdg_surface) xdg_surface_destroy(g_Platform.wayland.xdg_surface);
    if (g_Platform.wayland.surface) wl_surface_destroy(g_Platform.wayland.surface);
    if (g_Platform.wayland.keyboard) wl_keyboard_destroy(g_Platform.wayland.keyboard);
    if (g_Platform.wayland.pointer) wl_pointer_destroy(g_Platform.wayland.pointer);
    if (g_Platform.wayland.seat) wl_seat_destroy(g_Platform.wayland.seat);
    if (g_Platform.wayland.xdg_wm_base) xdg_wm_base_destroy(g_Platform.wayland.xdg_wm_base);
    if (g_Platform.wayland.compositor) wl_compositor_destroy(g_Platform.wayland.compositor);
    if (g_Platform.wayland.registry) wl_registry_destroy(g_Platform.wayland.registry);
    if (g_Platform.wayland.display) wl_display_disconnect(g_Platform.wayland.display);
}

void Ivy_Platform_PollEvents_Wayland(void)
{
    struct wl_display *display = g_Platform.wayland.display;
    wl_display_dispatch_pending(display);
    wl_display_flush(display);

    struct pollfd pfd = {
        .fd = wl_display_get_fd(display),
        .events = POLLIN,
        .revents = 0
    };

    int ret = poll(&pfd, 1, 0);
    if (ret > 0 && (pfd.revents & POLLIN)) {
        wl_display_dispatch(display);
    }
}

void Ivy_Platform_SetWindowPosition_Wayland(int x, int y)
{
    (void)x; (void)y;
}

void Ivy_Platform_SetWindowState_Wayland(u32 flags)
{
    (void)flags;
}

void Ivy_Platform_ToggleFullscreen_Wayland(void)
{
    if (g_Platform.wayland.xdg_toplevel) {
        if (g_Platform.wayland.isFullscreen) {
            xdg_toplevel_unset_fullscreen(g_Platform.wayland.xdg_toplevel);
        } else {
            xdg_toplevel_set_fullscreen(g_Platform.wayland.xdg_toplevel, NULL);
        }
        g_Platform.wayland.isFullscreen = !g_Platform.wayland.isFullscreen;
    }
}

void Ivy_Platform_ToggleBorderlessWindow_Wayland(void)
{
    Ivy_Platform_ToggleFullscreen_Wayland();
}

void Ivy_Platform_SwapBuffers_Wayland(void)
{

    if (IVY_CORE.window.resizedLastFrame) {
        eglSwapInterval(g_Platform.eglDisplay, 0);
    } else {
        eglSwapInterval(g_Platform.eglDisplay, 1);
    }
    eglSwapBuffers(g_Platform.eglDisplay, g_Platform.eglSurface);
}

int Ivy_Platform_GetCurrentMonitor_Wayland(void)
{
    return 0;
}

IvyVector2 Ivy_Platform_GetWindowScaleDPI_Wayland(void)
{
    return (IvyVector2){1.0f, 1.0f};
}

void *Ivy_Platform_GetProcAddress_Wayland(const char *name)
{
    return (void *)eglGetProcAddress(name);
}

#endif
