#include "Window.h"
#include "Manager.h"
#include <Diagnostic/Monitor.h>
#include <Utilities/Math.h>

static raw_window_t* background_raw = NULL;
static window_t* background = NULL;

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

    subwindow->subwindow =
        CreateRawSubwindow(&subwindow->window, background_raw);
    wl_subsurface_set_position(subwindow->subwindow, subwindow->x,
                               subwindow->y);
    wl_surface_commit(subwindow->window);
}

void CreateMainWindow(void)
{
    background_raw = CreateRawWindow();
    background = WrapRawWindow(background_raw);
    SetToplevelWindow(background);
    wl_surface_commit(background_raw);
}

void CreateUIWindows(void)
{
    CreateSubwindow(&gameplay_window, main_pane);
    CreateSubwindow(&bust_window, bust_pane);
    CreateSubwindow(&stat_window, information_pane);
}

void DestroyMainWindow(void)
{
    xdg_surface_destroy(background);
    wl_surface_destroy(background_raw);
}

void DestroyUIWindows(void)
{
    wl_surface_destroy(bust_window.window);
    wl_surface_destroy(gameplay_window.window);
    wl_surface_destroy(stat_window.window);
    wl_subsurface_destroy(bust_window.subwindow);
    wl_subsurface_destroy(gameplay_window.subwindow);
    wl_subsurface_destroy(stat_window.subwindow);
}

raw_window_t* GetBackgroundWindowRaw(void) { return background_raw; }
window_t* GetBackgroundWindow(void) { return background; }

const subwindow_t* GetBustWindow(void) { return &bust_window; }
const subwindow_t* GetGameplayWindow(void) { return &gameplay_window; }
const subwindow_t* GetStatWindow(void) { return &stat_window; }

raw_window_t* GetBustRawWindow(void) { return bust_window.window; }
raw_window_t* GetGameplayRawWindow(void) { return gameplay_window.window; }
raw_window_t* GetStatRawWindow(void) { return stat_window.window; }

raw_subwindow_t* GetBustRawSubwindow(void)
{
    return bust_window.subwindow;
}
raw_subwindow_t* GetGameplayRawSubwindow(void)
{
    return gameplay_window.subwindow;
}
raw_subwindow_t* GetStatRawSubwindow(void)
{
    return stat_window.subwindow;
}
