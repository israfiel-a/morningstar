#include "Client.h"
#include "Manager.h"
#include "Window.h"
#include <Diagnostic/Monitor.h>
#include <Globals.h>
#include <Input/Hardware.h>
#include <Output/Error.h>
#include <Output/Messages.h>
#include <Rendering/SHM.h>
#include <Utilities/Macros.h>
#include <linux/input-event-codes.h>

static struct wl_display* display = NULL;
static struct wl_registry* registry = NULL;

static void HandleInterfaceAddition(void* data,
                                    struct wl_registry* registry,
                                    uint32_t name, const char* interface,
                                    uint32_t version)
{
    if (strcmp(interface, wl_shm_interface.name) == 0)
        BindSHM(registry, name, version);
    else if (strcmp(interface, wl_seat_interface.name) == 0)
        BindInputGroup(registry, name, version);
    else if (strcmp(interface, wl_compositor_interface.name) == 0)
        wm_data.compositor =
            wl_registry_bind(registry, name, &wl_compositor_interface, 1);
    else if (strcmp(interface, wl_subcompositor_interface.name) == 0)
        wm_data.subcompositor = wl_registry_bind(
            registry, name, &wl_subcompositor_interface, version);
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
    display = wl_display_connect(0);
    if (display == NULL) ReportError(wayland_display_fail, false);

    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);
    // Wait for the server to catch up, and if it can't, fail the
    // program.
    if (wl_display_roundtrip(display) == -1)
        ReportError(wayland_server_processing_fail, false);

    if (wm_data.compositor == NULL || wm_data.xsh_base == NULL)
        ReportError(wayland_missing_features, false);

    CreateMainWindow();

    SetMouseButtonDownCallback(temp_callback);

    //! TEMPORARY LOCATION UNTIL APPLICATION INTERFACE IS ADDED
    while (wl_display_dispatch(display) != -1 &&
           global_flags.application_running)
    {
        // Run the application.
    }
}

void DestroyWayland(void)
{
    UnbindInputGroup();
    UnbindSHM();
    DestroyUIWindows();
    DestroyMainWindow();
    xdg_toplevel_destroy(wm_data.xsh_toplevel);
    xdg_wm_base_destroy(wm_data.xsh_base);
    wl_compositor_destroy(wm_data.compositor);
    wl_subcompositor_destroy(wm_data.subcompositor);
    wl_registry_destroy(registry);
    wl_display_disconnect(display);
}
