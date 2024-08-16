#include "Windowing.h"
#include <Rendering/System.h> // EGL functionality
#include <Windowing/System.h> // Wayland functionality

void SetupWindow(bool lock_unified)
{
    if (lock_unified) global_flags.panel_mode = unified_locked;
    SetupWayland();
    SetupEGL();
}

void CreateWindow(const char* window_title)
{
    CreateBackdrop(window_title); // toplevel
    CreatePanels();               // subwindows & render targets
}

void DestroyWindow(void)
{
    DestroyPanels();
    DestroyBackdrop();
    DestroyEGL();
    DestroyWayland();
}

void run(void)
{
    while (wl_display_dispatch(GetDisplay()) != -1 && running)
    {
        // do all the funny stuff
    }
}
