#include "System.h"
#include "Manager.h"
#include "Window.h"
#include <Input/Hardware.h>
#include <Output/Error.h>
#include <Rendering/SHM.h>

/**
 * @brief The application's display object.
 */
static display_t* display = NULL;
/**
 * @brief The application's registry object, contained in this file for
 * cleanliness reasons.
 */
static registry_t* registry = NULL;

/**
 * @brief Basically a big switch statement that binds whatever interface
 * Wayland throws at us, so long as we have need of it.
 * @param data Nothing of use.
 * @param registry Nothing of use.
 * @param name The numerical ID of the object.
 * @param interface The interface to be bound.
 * @param version The version of the interface we're being given.
 */
static void HIA(void* d, registry_t* r, uint32_t name,
                const char* interface, uint32_t version)
{
    if (!strcmp(interface, wl_shm_interface.name)) BindSHM(name, version);
    else if (!strcmp(interface, wl_seat_interface.name))
        BindInputGroup(name, version);
    else if (!strcmp(interface, wl_compositor_interface.name))
        BindCompositor(name, version);
    else if (!strcmp(interface, wl_subcompositor_interface.name))
        BindSubcompositor(name, version);
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
static const registry_monitor_t registry_listener = {HIA, HID};

/**
 * @brief Grab the registry from the display server and setup a listener to
 * wait for more interfaces to be reported by Wayland.
 * @param display The display from which to grab the registry object.
 */
void BeginRegistry(display_t* display)
{
    assert(display != NULL);
    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);
}

/**
 * @brief Destroy the registry object, and de-allocate all things bound
 * within it.
 */
void DestroyRegistry(void)
{
    UnbindSHM();
    UnbindInputGroup();
    UnbindWindowManager();
    UnbindSubcompositor();
    UnbindCompositor();
    wl_registry_destroy(registry);
}

/**
 * @brief Grab the registry object of the application.
 * @return The registry object.
 */
registry_t* GetRegistry(void) { return registry; }

/**
 * @brief Check to make sure the features we require were reported by the
 * Wayland display server, and that they were bound properly within the
 * registry.
 * @return true Everything was bound properly.
 * @return false Something's amiss. This should trigger an error, probably
 * @enum wayland_missing_features.
 */
bool CheckRequiredFeatures(void)
{
    // We need the compositor, subcompositor, an input group, shared memory
    // file functionality, and XDG-shell protocol in order to run the
    // application.
    return GetCompositor() != NULL && GetSubcompositor() != NULL &&
           GetWindowManager() != NULL && GetSHM() != NULL &&
           GetInputGroup() != NULL;
}

/**
 * @brief A function to setup the Wayland server, the display environment
 * one used for the Linux distribution of Morningstar. This takes no
 * arguments and returns none, but creates, binds, and does the first poll
 * for the display interface and registry.
 */
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
    // By now we should have these items, and if we don't then trigger a
    // warning that something's wrong.
    if (!CheckRequiredFeatures())
        ReportError(wayland_missing_features, false);
}

/**
 * @brief Destroy the Wayland display server data stored in our
 * application. All getters having to do with Wayland will return NULL
 * until @ref SetupWayland is called again.
 */
void DestroyWayland(void)
{
    DestroyRegistry(); // This'll unbind all registry devices as well.
    wl_display_disconnect(display);
}

void SetupWindowingSystem(void)
{
    SetupWayland(); // just linux for now
}

void DestroyWindowingSystem(void)
{
    DestroyWayland(); // just linux for now
}

bool CheckDisplayServer(void)
{
    return wl_display_dispatch(display) != -1;
}
