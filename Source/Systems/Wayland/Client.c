#include "Client.h"
#include "Manager.h"
#include "Window.h"
#include <Diagnostic/Monitor.h>
#include <Globals.h>
#include <Input/Hardware.h>
#include <Output/Error.h>
#include <Output/Messages.h>
#include <linux/input-event-codes.h>

static struct wl_display* display = NULL;
static struct wl_registry* registry = NULL;
static struct wl_compositor* compositor = NULL;

static void HandleInterfaceAddition(void* data,
                                    struct wl_registry* registry,
                                    uint32_t name, const char* interface,
                                    uint32_t version)
{
    if (strcmp(interface, wl_shm_interface.name) == 0)
        wm_data.wl_sharedmemory =
            wl_registry_bind(registry, name, &wl_shm_interface, 1);
    else if (strcmp(interface, wl_seat_interface.name) == 0)
        BindInputGroup(registry, name, version);
    else if (strcmp(interface, wl_compositor_interface.name) == 0)
        compositor =
            wl_registry_bind(registry, name, &wl_compositor_interface, 1);
    else if (strcmp(interface, xdg_wm_base_interface.name) == 0)
    {
        wm_data.xsh_base =
            wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(wm_data.xsh_base,
                                 &wm_monitors.xsh_monitor, NULL);
    }
    else if (strcmp(interface, wl_output_interface.name) == 0)
        BindMonitor(registry, name, version);
}

static const interface_monitor_t registry_listener = {
    HandleInterfaceAddition};

void temp_callback(uint32_t button, uint32_t time)
{
    if (button == BTN_RIGHT) global_flags.application_running = false;
}

void SetupWayland(void)
{
    // Connect to the default Wayland compositor (Wayland-0).
    display = wl_display_connect(NULL);
    if (display == NULL) ReportError(wayland_display_fail, false);

    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);
    // Wait for the server to catch up, and if it can't, fail the
    // program.
    if (wl_display_roundtrip(display) == -1)
        ReportError(wayland_server_processing_fail, false);

    if (compositor == NULL || wm_data.xsh_base == NULL)
        ReportError(wayland_missing_features, false);

    wm_data.wl_window = wl_compositor_create_surface(compositor);
    wm_data.xsh_surface =
        xdg_wm_base_get_xdg_surface(wm_data.xsh_base, wm_data.wl_window);
    wm_data.xsh_toplevel = xdg_surface_get_toplevel(wm_data.xsh_surface);

    xdg_surface_add_listener(wm_data.xsh_surface,
                             &wm_monitors.xsh_surface_monitor, NULL);
    xdg_toplevel_add_listener(wm_data.xsh_toplevel,
                              &wm_monitors.xsh_toplevel_monitor, NULL);

    SetWindowTitle(ID, TITLE);
    SetMouseButtonDownCallback(temp_callback);

    xdg_toplevel_set_fullscreen(wm_data.xsh_toplevel, NULL);
    wl_surface_commit(wm_data.wl_window);

    //! TEMPORARY LOCATION
    while (wl_display_dispatch(display) != -1 &&
           global_flags.application_running)
    {
        // Run the application.
    }
}

void DestroyWayland(void)
{
    xdg_toplevel_destroy(wm_data.xsh_toplevel);
    xdg_surface_destroy(wm_data.xsh_surface);
    wl_surface_destroy(wm_data.wl_window);
}
