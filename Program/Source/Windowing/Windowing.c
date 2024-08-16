#include "Windowing.h"
#include <Globals.h>                 // Global flags
#include <Output/Error.h>            // Error functionality
#include <Output/Warning.h>          // Warning functionality
#include <Rendering/System.h>        // EGL wrappers
#include <Windowing/Manager.h>       // XDG wrappers
#include <Windowing/System.h>        // Wayland wrappers
#include <stddef.h>                  // NULL
#include <stdlib.h>                  // malloc, free, etc.
#include <wayland-client-protocol.h> // Wayland functions

static window_t window;

void SetupWindow(void)
{
    SetupWayland();
    SetupEGL();
}

void CreateWindow(const char* window_title)
{
    if (GetDisplay() == NULL || GetRegistry() == NULL ||
        GetCompositor() == NULL || GetWindowManager() == NULL)
    {
        ReportWarning(preemptive_window_creation);
        return;
    }

    if (window._s != NULL || window._ws != NULL)
    {
        ReportWarning(double_window_creation);
        return;
    }

    window._s = wl_compositor_create_surface(GetCompositor());
    window._ws = WrapRawWindow(window._s, window_title);
    wl_surface_commit(window._s);
}

void DestroyWindow(void)
{
    if (window._s == NULL || window._ws == NULL)
    {
        ReportWarning(double_window_free);
        return;
    }

    if (window.panels == NULL)
    {
        ReportWarning(preemptive_panel_free);
        return;
    }

    // this will increase with the size of the panel array later
    for (size_t i = 0; i < 1; i++)
    {
        panel_t panel = window.panels[i];
        if (panel._s != NULL && panel._ss != NULL) continue;

        wl_surface_destroy(panel._s);
        wl_subsurface_destroy(panel._ss);
        UnbindEGLContext(&panel);
    }

    free(window.panels);
    window.panels = NULL;

    UnwrapWindow(&window);
    wl_surface_destroy(window._s);
    window._s = NULL;
    window._ws = NULL;

    DestroyEGL();
    DestroyWayland();
}

void SetWindowTitle(const char* title) { SetWrappedWindowTitle(title); }

panel_t* CreatePanel(panel_type_t type)
{
    if (GetDisplay() == NULL || GetRegistry() == NULL)
    {
        ReportWarning(preemptive_window_creation);
        return NULL;
    }

    if (GetCompositor() == NULL || GetSubcompositor() == NULL ||
        GetEGLDisplay() == NULL || window._s == NULL || window._ws == NULL)
    {
        ReportWarning(preemptive_panel_creation);
        return NULL;
    }

    // replace this shit with a dynamic_array type next update
    if (window.panels == NULL)
    {
        window.panels = malloc(sizeof(panel_t));
        if (window.panels == NULL) ReportError(allocation_failure);
    }

    // zero cause we only allow one panel for testing
    window.panels[0] =
        (panel_t){type,
                  0,
                  0,
                  0,
                  0,
                  wl_compositor_create_surface(GetCompositor()),
                  wl_subcompositor_get_subsurface(
                      GetSubcompositor(), window.panels[0]._s, window._s)};

    // type switch statement goes here later

    // Center the window either vertically or horizontally,
    // depending on which dimension is shorter.

    if (dimensions.shortest_side == dimensions.height)
        window.panels[0].x = dimensions.gap_size;
    else window.panels[0].y = dimensions.gap_size;

    wl_subsurface_set_position(window.panels[0]._ss, window.panels[0].x,
                               window.panels[0].y);
    // Update Wayland on our changes.
    wl_surface_commit(window.panels[0]._s);

    BindEGLContext(&window.panels[0]);

    return &window.panels[0];
}

void run(void)
{
    while (running)
    {
        CheckDisplayServer();
        // do all the funny stuff
    }
}
