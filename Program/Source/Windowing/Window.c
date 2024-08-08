#include "Window.h"
#include "Manager.h"        // Window wrapping
#include "System.h"         // Registry & compositor getter
#include <Session.h>        // Global values
#include <Utilities/Math.h> // Basic math functions

/**
 * @brief The application's window, and all its children.
 */
static window_t application_window = {NULL, NULL, NULL};

/**
 * @brief A macro for the bust display window.
 */
#define bust_window application_window.subwindows[0]

/**
 * @brief A macro for the central gameplay window.
 */
#define gameplay_window application_window.subwindows[1]

/**
 * @brief A macro for the stat window.
 */
#define stat_window application_window.subwindows[2]

void CreateUIWindows(void)
{
    // Make sure the function hasn't been called already.
    if (application_window._win != NULL ||
        application_window.inner != NULL ||
        application_window.subwindows != NULL)
        return;

    // Create the background and push our new toplevel to Wayland for
    // processing.
    application_window._win =
        wl_compositor_create_surface(GetCompositor());
    application_window.inner = WrapRawWindow(application_window._win);
    wl_surface_commit(application_window._win);

    application_window.subwindows = calloc(3, sizeof(subwindow_t));
    // Loop through the three now-created spaces fit for windows and fill
    // 'em with beautiful Wayland pointer objects.
    for (uint8_t i = 0; i < 3; i++)
    {
        application_window.subwindows[i]._win =
            wl_compositor_create_surface(GetCompositor());
        application_window.subwindows[i].inner =
            wl_subcompositor_get_subsurface(
                GetSubcompositor(), application_window.subwindows[i]._win,
                application_window._win);
    }
}

void DestroyUIWindows(void)
{
    // Make sure we haven't freed the windows yet.
    if (bust_window._win == NULL || gameplay_window._win == NULL ||
        stat_window._win == NULL || bust_window.inner == NULL ||
        gameplay_window.inner == NULL || stat_window.inner == NULL)
        return;

    wl_surface_destroy(bust_window._win),
        wl_surface_destroy(gameplay_window._win),
        wl_surface_destroy(stat_window._win);
    wl_subsurface_destroy(bust_window.inner),
        wl_subsurface_destroy(gameplay_window.inner),
        wl_subsurface_destroy(stat_window.inner);
    free(application_window.subwindows);

    // Make sure to free the XDG surface of the toplevel rather than just
    // the Wayland one.
    xdg_surface_destroy(application_window.inner);
    wl_surface_destroy(application_window._win);
}

void SetWindowPositions(void)
{
    // Loop over each subwindow.
    for (uint8_t i = 0; i < 3; i++)
    {
        uint32_t x = 0, y = 0;
        switch (i)
        {
            // Center the subwindow within the left gap of the UI.
            case bust:
                x = (dimensions.gap_size - (dimensions.gap_size / 1.25)) /
                    2;
                y = (dimensions.height - (dimensions.height / 1.25)) / 2;
                break;
            // Center the window either vertically or horizontally,
            // depending on which dimension is shorter.
            case gameplay:
                if (dimensions.shortest_side == dimensions.height)
                    x = dimensions.gap_size;
                else y = dimensions.gap_size;
                break;
            // Set the subwindow to be positioned at the top left corner of
            // the right gap.
            case stat: x = dimensions.width - dimensions.gap_size; break;
        }

        wl_subsurface_set_position(application_window.subwindows[i].inner,
                                   x, y);
        // Update Wayland on our changes.
        wl_surface_commit(application_window.subwindows[i]._win);
    }
}

wrapped_window_t* GetBackdrop(void) { return application_window.inner; }

raw_window_t* GetWindowRaw(requested_window_t requested)
{
    switch (requested)
    {
        case bust:     return bust_window._win;
        case stat:     return stat_window._win;
        case gameplay: return gameplay_window._win;
        case backdrop: return application_window._win;
    }
    return NULL; // unreachable, but the gcc compiler is stupid
}

raw_subwindow_t* GetSubwindowRaw(requested_window_t requested)
{
    switch (requested)
    {
        case bust:     return bust_window.inner;
        case stat:     return stat_window.inner;
        case gameplay: return gameplay_window.inner;
        case backdrop: return NULL;
    }
    return NULL; // unreachable, but the gcc compiler is stupid
}

void UnbindCompositors(void) {}
