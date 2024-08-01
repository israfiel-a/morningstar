#include "Manager.h"
#include "Window.h"
#include <Diagnostic/Monitor.h>
#include <Globals.h>
#include <Rendering/Surfaces.h>

static void HandleWMPing(void* data, struct xdg_wm_base* xdg_wm_base,
                         uint32_t serial)
{
    // XDG will send us a ping to make certain the application hasn't gone
    // unresponsive. We must respond near immediately to not be marked for
    // force killing.
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static void HandleWMWindowConfigure(void* data,
                                    window_manager_window_t* xdg_surface,
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

static void HandleWMConfigure(void* data,
                              window_manager_root_t* xdg_toplevel,
                              int32_t width, int32_t height,
                              struct wl_array* states)
{
    xdg_toplevel_set_fullscreen(wm_data.xsh_toplevel, NULL);
    // No operation.
}

static void HandleWMWindowSizeAdvice(void* data,
                                     window_manager_root_t* xdg_toplevel,
                                     int32_t width, int32_t height)
{
    // No operation.
}

static void
HandleCompositorAbilityList(void* data,
                            window_manager_root_t* xdg_toplevel,
                            struct wl_array* capabilities)
{
    // No operation.
}

static void HandleWMClose(void* data, window_manager_root_t* xdg_toplevel)
{
    global_flags.application_running = false;
}

const window_manager_monitors_t wm_monitors = {
    {HandleWMPing},
    {HandleWMWindowConfigure},
    {HandleWMConfigure, HandleWMClose, HandleWMWindowSizeAdvice,
     HandleCompositorAbilityList}};

window_manager_data_t wm_data = {NULL, NULL, NULL, NULL};

void SetupWindowManager(void) {}
