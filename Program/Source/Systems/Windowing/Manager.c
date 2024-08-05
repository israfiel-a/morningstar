#include "Manager.h"
#include "Registry.h"
#include "Window.h"
#include <Output/Error.h>
#include <Rendering/Surfaces.h>
#include <Session.h>

static window_manager_t* base = NULL;
static toplevel_t* toplevel = NULL;
static display_t* display = NULL;

void SetupWayland(void)
{
    // Connect to the default Wayland compositor (Wayland-0).
    display = wl_display_connect(0);
    if (display == NULL) ReportError(wayland_display_fail, false);
    BeginRegistry(display);

    // Wait for the server to catch up, and if it can't, fail the
    // program.
    if (wl_display_roundtrip(display) == -1)
        ReportError(wayland_server_processing_fail, false);

    if (GetCompositor() == NULL || GetSubcompositor() == NULL ||
        base == NULL)
        ReportError(wayland_missing_features, false);
}

void DestroyWayland(void)
{
    DestroyRegistry(); // This'll unbind all registry devices as well.
    wl_display_disconnect(display);
}

bool CheckSessionValidity(void)
{
    // Check to make sure the display isn't fucked and that the application
    // should still be running.
    return wl_display_dispatch(display) != -1 &&
           global_flags.application_running;
}

/**
 * @brief A function to handle the ping request that XDG-shell will send us
 * to make sure we're not unresponsive.
 * @param d Nothing of use.
 * @param b Nothing of use.
 * @param serial The serial number of the ping request.
 */
static void HP(void* d, window_manager_t* b, uint32_t serial)
{
    xdg_wm_base_pong(base, serial);
}

/**
 * @brief A listener for ping requests sent by XDG-shell, that responds in
 * kind with a pong to keep our application from force quitting.
 */
const static ping_listener_t ponger = {HP};

void BindWindowManager(uint32_t name, uint32_t version)
{
    base = wl_registry_bind(GetRegistry(), name, &xdg_wm_base_interface,
                            version);
    xdg_wm_base_add_listener(base, &ponger, NULL);
    CreateWindows();
}

void UnbindWindowManager(void)
{
    DestroyWindows();
    xdg_toplevel_destroy(toplevel);
    xdg_wm_base_destroy(base);
}

/**
 * @brief Handle the toplevel configuration event sent by XDG. We simply
 * use this to set our application to fullscreen. None of the parameters
 * are important to us.
 */
static void HTLC(void* d, toplevel_t* t, int32_t w, int32_t h,
                 struct wl_array* s)
{
    xdg_toplevel_set_fullscreen(toplevel, NULL);
}

/**
 * @brief Handle the close event for the application. This is triggered by
 * many things, like an ALT+F4, close button, etc. None of the parameters
 * are important to us.
 */
static void HAC(void* d, toplevel_t* t)
{
    global_flags.application_running = false;
}

/**
 * @brief Handle the suggested bounds of the window. This is typically the
 * size of a monitor minus all the panels and other UI components that we
 * don't want our surface overlapping.
 * @param d Nothing of use.
 * @param t Nothing of use.
 * @param width The suggested width of the window, or 0 if the bounds are
 * unknown.
 * @param height The suggested height of the window, or 0 if the bounds are
 * unknown.
 */
static void HWSS(void* d, toplevel_t* t, int32_t width, int32_t height)
{
    SetWindowPositions(width, height);
}

/**
 * @brief An unused function to handle the given list of compositor
 * abilities. We get this in the form of the @ref wl_registry bind event
 * calls, so we don't particularly care about this.
 */
NOOP HCAL(void* d, toplevel_t* t, struct wl_array* c) {}

/**
 * @brief A monitor for the application's toplevel, handling things like
 * closes and configures and all that jazz.
 */
const static toplevel_monitor_t toplevel_listener = {HTLC, HAC, HWSS,
                                                     HCAL};

void SetToplevelWindow(window_t* window)
{
    if (toplevel == NULL && window != NULL)
    {
        toplevel = xdg_surface_get_toplevel(window);
        xdg_toplevel_add_listener(toplevel, &toplevel_listener, NULL);
        xdg_toplevel_set_app_id(toplevel, ID);
        xdg_toplevel_set_title(toplevel, TITLE);
    }
}

static void HandleWMWindowConfigure(void* data, window_t* xdg_surface,
                                    uint32_t serial)
{
    xdg_surface_ack_configure(xdg_surface, serial); // ack = acknowledge

    SendBlankColor(GetBackgroundWindowRaw(), background_window, BLACK);
    SendBlankColor(GetGameplayRawWindow(), GetGameplayWindow()->size,
                   WHITE);
    SendBlankColor(GetBustRawWindow(), GetBustWindow()->size, RED);
    SendBlankColor(GetStatRawWindow(), GetStatWindow()->size, RED);
}
const window_manager_surface_monitor_t surface_monitor = {
    HandleWMWindowConfigure,
};

raw_window_t* CreateRawWindow(void)
{
    return wl_compositor_create_surface(GetCompositor());
}
raw_subwindow_t* CreateRawSubwindow(raw_window_t** window,
                                    raw_window_t* parent)
{
    *window = CreateRawWindow();
    return wl_subcompositor_get_subsurface(GetSubcompositor(), *window,
                                           parent);
}

window_t* WrapRawWindow(raw_window_t* raw_window)
{
    window_t* window = xdg_wm_base_get_xdg_surface(base, raw_window);
    xdg_surface_add_listener(window, &surface_monitor, NULL);
    return window;
}
toplevel_t* GetToplevelWindow(void) { return toplevel; }
