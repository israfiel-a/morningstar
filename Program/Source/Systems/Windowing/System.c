#include "System.h"
#include "Registry.h"
#include <Output/Error.h>

static display_t* display = NULL;

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
