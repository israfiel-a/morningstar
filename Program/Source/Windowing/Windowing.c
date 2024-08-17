#include "Windowing.h"
#include "Wayland.h"          // Wayland wrappers
#include "XDG.h"              // XDG wrappers
#include <Globals.h>          // Global flags
#include <Output/System.h>    // Output functions
#include <Rendering/System.h> // EGL wrappers

static window_t window = {NULL, NULL, NULL, NULL, NULL};

void SetupWindow(void) { SetupWayland(), SetupEGL(); }

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

    window._s = CreateSurface();
    window._ws = WrapRawWindow(window._s, window_title);
    CommitSurface(window._s);
}

void DestroyWindow(void)
{
    if (window._s == NULL || window._ws == NULL)
    {
        ReportWarning(double_window_free);
        return;
    }

    if (CheckBlockNull(window.panels))
    {
        ReportWarning(preemptive_panel_free);
        return;
    }

    // this will increase with the size of the panel array later
    for (size_t i = 0; i < 1; i++)
    {
        panel_t panel = ((panel_t*)window.panels.inner)[i];
        if (panel._s != NULL && panel._ss != NULL) continue;

        DestroySurface(&panel._s);
        DestroySubsurface(&panel._ss);
        UnbindEGLContext(&panel);
    }

    FreeBlock(&window.panels);
    UnwrapWindow(&window);
    DestroySurface(&window._s);
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
    if (GetNull) AllocateBlock(sizeof(panel_t*));

    // zero cause we only allow one panel for testing
    window.panels[0] =
        (panel_t){type,
                  0,
                  0,
                  0,
                  0,
                  CreateSurface(),
                  CreateSubsurface(&window.panels[0]._s, window._s)};

    // type switch statement goes here later

    // Center the window either vertically or horizontally,
    // depending on which dimension is shorter.

    if (dimensions.shortest_side == dimensions.height)
        window.panels[0].x = dimensions.gap_size;
    else window.panels[0].y = dimensions.gap_size;

    SetSubsurfacePosition(window.panels[0]._ss, window.panels[0].x,
                          window.panels[0].y);
    CommitSurface(window.panels[0]._s);

    BindEGLContext(&window.panels[0]);

    return &window.panels[0];
}

void run(void)
{
    while (running)
    {
        CheckWayland();
        // do all the funny stuff
    }
}
