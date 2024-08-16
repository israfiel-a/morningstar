#include "System.h"
#include "Manager.h" // Window managing
#include <Globals.h>
#include <Input/File.h>     // Shared memory file functionality
#include <Input/Hardware.h> // Mouse/keyboard functionality
#include <Output/Error.h>   // Error reporting
#include <Output/Warning.h>
#include <Rendering/System.h>
#include <XDGS/xdg-shell.h>
#include <string.h>

/**
 * @brief The application's display object.
 */
static struct wl_display* display = NULL;

/**
 * @brief The application's registry object, contained in this file for
 * cleanliness reasons.
 */
static struct wl_registry* registry = NULL;

/**
 * @brief The Wayland compositor's compositor. It is responsible for
 * creating surfaces and regions.
 */
static struct wl_compositor* compositor = NULL;

/**
 * @brief The Wayland's compositor's compositor's compositor. The nesting
 * is crazy here. This is responsible for creating subsurfaces.
 */
static struct wl_subcompositor* subcompositor = NULL;

/**
 * @brief Basically a big switch statement that binds whatever interface
 * Wayland throws at us, so long as we have need of it.
 * @param data Nothing of use.
 * @param registry Nothing of use.
 * @param name The numerical ID of the object.
 * @param interface The interface to be bound.
 * @param version The version of the interface we're being given.
 */
static void HIA(void* d, struct wl_registry* r, uint32_t name,
                const char* interface, uint32_t version)
{
    if (!strcmp(interface, wl_shm_interface.name)) BindSHM(name, version);
    else if (!strcmp(interface, wl_seat_interface.name))
        BindInputGroup(name, version);
    else if (!strcmp(interface, wl_compositor_interface.name))
    {
        compositor = wl_registry_bind(GetRegistry(), name,
                                      &wl_compositor_interface, version);
        devices.compositor = true;
    }
    else if (!strcmp(interface, wl_subcompositor_interface.name))
    {
        subcompositor = wl_registry_bind(
            GetRegistry(), name, &wl_subcompositor_interface, version);
        devices.subcompositor = true;
    }
    else if (!strcmp(interface, xdg_wm_base_interface.name))
        BindWindowManager(name, version);
}

/**
 * @brief An unused function to handle the removal of global objects.
 */
static void HID(void* d, struct wl_registry* r, uint32_t n) {}

/**
 * @brief A listener for the addition and removal of global objects from
 * the Wayland registry. We only have functionality for the case of
 * addition, not deletion.
 */
static const struct wl_registry_listener registry_listener = {HIA, HID};

void SetupWayland(void)
{
    if (display != NULL || registry != NULL)
    {
        ReportWarning(double_display_setup);
        return;
    }

    // Connect to the default Wayland compositor (Wayland-0).
    display = wl_display_connect(0);
    if (display == NULL) ReportError(display_connect_failure);
    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);

    // Wait for the server to catch up, and if it can't, fail the
    // program.
    if (wl_display_roundtrip(display) == -1)
        ReportError(server_processing_failure);

    if (!full_device_suite) ReportError(compositor_missing_features);
}

void DestroyWayland(void)
{
    UnbindSHM(), UnbindInputGroup();
    UnbindWindowManager();
    wl_subcompositor_destroy(subcompositor);
    wl_compositor_destroy(compositor);
    wl_registry_destroy(registry);
    wl_display_disconnect(display);

    devices.compositor = false;
    devices.subcompositor = false;
}

void CheckDisplayServer(void)
{
    if (wl_display_dispatch(display) == -1)
        ReportError(server_processing_failure);
}

struct wl_display* GetDisplay(void) { return display; }
struct wl_registry* GetRegistry(void) { return registry; }
struct wl_compositor* GetCompositor(void) { return compositor; }
struct wl_subcompositor* GetSubcompositor(void) { return subcompositor; }
