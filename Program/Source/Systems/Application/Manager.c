#include "Manager.h"
#include "Window.h"
#include <Diagnostic/Monitor.h>
#include <Globals.h>
#include <Rendering/Surfaces.h>

static window_manager_t* base;
static toplevel_t* toplevel;
static compositor_t* compositor;
static subcompositor_t* subcompositor;

static void HP(void* data, struct xdg_wm_base* xdg_wm_base,
               uint32_t serial)
{
    // XDG will send us a ping to make certain the application hasn't gone
    // unresponsive. We must respond near immediately to not be marked for
    // force killing.
    xdg_wm_base_pong(xdg_wm_base, serial);
}
const static window_manager_monitor_t base_listener = {HP};

static void HandleWMWindowConfigure(void* data, window_t* xdg_surface,
                                    uint32_t serial)
{
    xdg_surface_ack_configure(xdg_surface, serial); // ack = acknowledge
    CreateUIWindows();

    SendBlankColor(GetBackgroundWindowRaw(), GetMonitorWidth(),
                   GetMonitorHeight(), BLACK);
    SendBlankColor(GetGameplayRawWindow(), GetGameplayWindow()->width,
                   GetGameplayWindow()->height, WHITE);
    SendBlankColor(GetBustRawWindow(), GetBustWindow()->width,
                   GetBustWindow()->height, RED);
    SendBlankColor(GetStatRawWindow(), GetStatWindow()->width,
                   GetStatWindow()->height, RED);
}

static void HandleWMConfigure(void* data, toplevel_t* xdg_toplevel,
                              int32_t width, int32_t height,
                              struct wl_array* states)
{
    xdg_toplevel_set_fullscreen(toplevel, NULL);
    // No operation.
}

static void HandleWMWindowSizeAdvice(void* data, toplevel_t* xdg_toplevel,
                                     int32_t width, int32_t height)
{
    // No operation.
}

static void HandleCompositorAbilityList(void* data,
                                        toplevel_t* xdg_toplevel,
                                        struct wl_array* capabilities)
{
    // No operation.
}

static void HandleWMClose(void* data, toplevel_t* xdg_toplevel)
{
    global_flags.application_running = false;
}
const static window_manager_root_monitor_t root_monitor = {
    HandleWMConfigure, HandleWMClose, HandleWMWindowSizeAdvice,
    HandleCompositorAbilityList};
const window_manager_surface_monitor_t surface_monitor = {
    HandleWMWindowConfigure,
};

void BindWindowManager(registry_t* registry, uint32_t name,
                       uint32_t version)
{
    base =
        wl_registry_bind(registry, name, &xdg_wm_base_interface, version);
    xdg_wm_base_add_listener(base, &base_listener, NULL);
    CreateMainWindow();
}

void BindCompositor(registry_t* registry, uint32_t name, uint32_t version)
{
    compositor = wl_registry_bind(registry, name, &wl_compositor_interface,
                                  version);
}
void BindSubcompositor(registry_t* registry, uint32_t name,
                       uint32_t version)
{
    subcompositor = wl_registry_bind(registry, name,
                                     &wl_subcompositor_interface, version);
}

void UnbindWindowManager(void)
{
    DestroyUIWindows();
    DestroyMainWindow();
    xdg_toplevel_destroy(toplevel);
    xdg_wm_base_destroy(base);
}

void UnbindCompositor(void) { wl_compositor_destroy(compositor); }
void UnbindSubcompositor(void) { wl_subcompositor_destroy(subcompositor); }

void SetToplevelWindow(window_t* window)
{
    if (toplevel == NULL && window != NULL)
    {
        toplevel = xdg_surface_get_toplevel(window);
        xdg_toplevel_add_listener(toplevel, &root_monitor, NULL);
        xdg_toplevel_set_app_id(toplevel, ID);
        xdg_toplevel_set_title(toplevel, TITLE);
    }
}

raw_window_t* CreateRawWindow(void)
{
    return wl_compositor_create_surface(compositor);
}
raw_subwindow_t* CreateRawSubwindow(raw_window_t** window,
                                    raw_window_t* parent)
{
    *window = CreateRawWindow();
    return wl_subcompositor_get_subsurface(subcompositor, *window, parent);
}

window_t* WrapRawWindow(raw_window_t* raw_window)
{
    window_t* window = xdg_wm_base_get_xdg_surface(base, raw_window);
    xdg_surface_add_listener(window, &surface_monitor, NULL);
    return window;
}
toplevel_t* GetToplevelWindow(void) { return toplevel; }
const window_manager_t* GetWindowManager(void) { return base; }
const compositor_t* GetCompositor(void) { return compositor; }
const subcompositor_t* GetSubcompositor(void) { return subcompositor; }
