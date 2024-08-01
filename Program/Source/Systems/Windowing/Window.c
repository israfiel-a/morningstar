#include "Window.h"
#include "Manager.h"
#include <Diagnostic/Monitor.h>
#include <Output/Error.h>
#include <Rendering/SHM.h>
#include <Utilities/Math.h>

static subwindow_t bust_window = {0, 0, 0, 0, NULL, NULL};
static subwindow_t gameplay_window = {0, 0, 0, 0, NULL, NULL};
static subwindow_t stat_window = {0, 0, 0, 0, NULL, NULL};

typedef enum
{
    bust_pane,
    information_pane,
    main_pane
} window_type_t;

static void CreateSubwindow(subwindow_t* subwindow, window_type_t type)
{

    // The middles of each side of the monitor. We use these measurements
    // multiple times, so might as well just make them constants.
    const uint32_t width_middle =
                       GetMiddle(GetMonitorWidth(), GetMonitorHeight()),
                   height_middle = GetMiddle(GetMonitorHeight(),
                                             GetMonitorShortSide());
    switch (type)
    {
        case bust_pane:
            subwindow->width = width_middle / 1.25;
            subwindow->height = GetMonitorHeight() / 1.25;
            subwindow->x = GetMiddle(width_middle, subwindow->width);
            subwindow->y =
                GetMiddle(GetMonitorHeight(), subwindow->height);
            break;
        case information_pane:
            subwindow->width = width_middle;
            subwindow->height = GetMonitorHeight();
            subwindow->x = GetMonitorWidth() - subwindow->width;
            break;
        case main_pane:
            subwindow->width = GetMonitorShortSide();
            subwindow->height = GetMonitorShortSide();
            if (GetMonitorShortSideName() == monitor_height)
                subwindow->x = width_middle;
            else subwindow->y = height_middle;
            break;
    }

    subwindow->window = wl_compositor_create_surface(wm_data.compositor);
    subwindow->subwindow = wl_subcompositor_get_subsurface(
        wm_data.subcompositor, subwindow->window, wm_data.wl_window);
    wl_subsurface_set_position(subwindow->subwindow, subwindow->x,
                               subwindow->y);
}

void CreateSubwindows(void)
{
    CreateSubwindow(&gameplay_window, main_pane);
    CreateSubwindow(&bust_window, bust_pane);
    CreateSubwindow(&stat_window, information_pane);
}

void SetWindowTitle(const char* id, const char* title)
{
    assert(title != NULL);
    xdg_toplevel_set_app_id(wm_data.xsh_toplevel, id);
    xdg_toplevel_set_title(wm_data.xsh_toplevel, title);
}

pixel_buffer_t* GenerateWindowBackground(void)
{
    return CreateSolidPixelBuffer(GetMonitorWidth(), GetMonitorHeight(),
                                  XRGB, BLACK);
}

pixel_buffer_t* GenerateSubwindowBackground(const subwindow_t* window,
                                            uint32_t background_color)
{
    return CreateSolidPixelBuffer(window->width, window->height, XRGB,
                                  background_color);
}

const subwindow_t* GetBustWindow(void) { return &bust_window; }
const subwindow_t* GetGameplayWindow(void) { return &gameplay_window; }
const subwindow_t* GetStatWindow(void) { return &stat_window; }

window_raw_t* GetBustRawWindow(void) { return bust_window.window; }
window_raw_t* GetGameplayRawWindow(void) { return gameplay_window.window; }
window_raw_t* GetStatRawWindow(void) { return stat_window.window; }

subwindow_raw_t* GetBustRawSubwindow(void)
{
    return bust_window.subwindow;
}
subwindow_raw_t* GetGameplayRawSubwindow(void)
{
    return gameplay_window.subwindow;
}
subwindow_raw_t* GetStatRawSubwindow(void)
{
    return stat_window.subwindow;
}
