#include "Window.h"
#include "Manager.h"
#include "Registry.h"
#include <Rendering/Surfaces.h>
#include <Utilities/Math.h>

static compositor_t* compositor = NULL;
static subcompositor_t* subcompositor = NULL;

static raw_window_t* background_raw = NULL;
static window_t* background = NULL;

static subwindow_t bust_window = {gap_filler_centered, gap_centered_left,
                                  NULL};
static subwindow_t gameplay_window = {main_window, centered, NULL};
static subwindow_t stat_window = {gap_filler, gap_filler_right, NULL};

void CreateWindows(void)
{
    background_raw = CreateRawWindow();
    background = WrapRawWindow(background_raw);
    SetToplevelWindow(background);
    wl_surface_commit(background_raw);

    bust_window.subwindow =
        CreateRawSubwindow(&bust_window.window, background_raw);
    gameplay_window.subwindow =
        CreateRawSubwindow(&gameplay_window.window, background_raw);
    stat_window.subwindow =
        CreateRawSubwindow(&stat_window.window, background_raw);
}

void SetWindowPositions(int32_t suggested_width, int32_t suggested_height)
{
    NoteSuggestedBounds(suggested_width, suggested_height);
    SetSubwindowPosition(&bust_window);
    SetSubwindowPosition(&gameplay_window);
    SetSubwindowPosition(&stat_window);
}

void DestroyWindows(void)
{
    wl_surface_destroy(bust_window.window);
    wl_surface_destroy(gameplay_window.window);
    wl_surface_destroy(stat_window.window);
    wl_subsurface_destroy(bust_window.subwindow);
    wl_subsurface_destroy(gameplay_window.subwindow);
    wl_subsurface_destroy(stat_window.subwindow);
    xdg_surface_destroy(background);
    wl_surface_destroy(background_raw);
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

void BindCompositor(uint32_t name, uint32_t version)
{
    compositor = wl_registry_bind(GetRegistry(), name,
                                  &wl_compositor_interface, version);
}
void BindSubcompositor(uint32_t name, uint32_t version)
{
    subcompositor = wl_registry_bind(GetRegistry(), name,
                                     &wl_subcompositor_interface, version);
}

compositor_t* GetCompositor(void) { return compositor; }
subcompositor_t* GetSubcompositor(void) { return subcompositor; }

void UnbindCompositor(void) { wl_compositor_destroy(compositor); }
void UnbindSubcompositor(void) { wl_subcompositor_destroy(subcompositor); }
