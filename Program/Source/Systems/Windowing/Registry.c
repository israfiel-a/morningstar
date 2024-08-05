#include "Registry.h"
#include "Manager.h"
#include "Window.h"
#include <Input/Hardware.h>
#include <Rendering/SHM.h>

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

void BeginRegistry(display_t* display)
{
    assert(display != NULL);
    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);
}

void DestroyRegistry(void)
{
    UnbindSHM();
    UnbindInputGroup();
    UnbindWindowManager();
    UnbindSubcompositor();
    UnbindCompositor();
    wl_registry_destroy(registry);
}

registry_t* GetRegistry(void) { return registry; }

bool CheckRequiredFeatures(void)
{
    // We need the compositor, subcompositor, an input group, shared memory
    // file functionality, and XDG-shell protocol in order to run the
    // application.
    return GetCompositor() != NULL && GetSubcompositor() != NULL &&
           GetWindowManager() != NULL && GetSHM() != NULL &&
           GetInputGroup() != NULL;
}
