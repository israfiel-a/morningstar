#include "Window.h"
#include "Manager.h"
#include "System.h"
#include <Rendering/SHM.h>
#include <Utilities/Math.h>

static compositor_t* compositor = NULL;
static subcompositor_t* subcompositor = NULL;

static raw_window_t* background_raw = NULL;
static window_t* background = NULL;

/**
 * @brief A structure describing the various states of a particular
 * subwindow.
 */
typedef struct
{
    /**
     * @brief The actual subsurface interface of the window. This includes
     * position and sync information.
     */
    raw_subwindow_t* inner;
    /**
     * @brief The inner @ref wl_surface of the subwindow. This should
     * really not be edited by anything, beyond the occasional @ref
     * wl_surface_commit to push changes to the surface.
     */
    raw_window_t* _win;
} subwindow_t;

static subwindow_t bust_window = {NULL, NULL};
static subsurface_type_t bust_window_type = gap_centered_left;

static subwindow_t gameplay_window = {NULL, NULL};
static subsurface_type_t gameplay_window_type = central_window;

static subwindow_t stat_window = {NULL, NULL};
static subsurface_type_t stat_window_type = gap_filler_right;

static int32_t suggested_width = 0;
static int32_t suggested_height = 0;
#define SS int32_min(suggested_width, suggested_height)
#define GAP_SIZE ((float)suggested_width - SS) / 2

uint32_t CalculateWidth(subsurface_type_t size)
{
    switch (size)
    {
        case gap_filler_left:
        case gap_filler_right:   return GAP_SIZE;
        case gap_centered_left:
        case gap_centered_right: return GAP_SIZE / 1.25;
        case central_window:     return SS;
        case backdrop_window:    return suggested_width;
    }
    return 0;
}

uint32_t CalculateHeight(subsurface_type_t size)
{
    switch (size)
    {
        case gap_filler_left:
        case gap_filler_right:   return suggested_height;
        case gap_centered_left:
        case gap_centered_right: return suggested_height / 1.25;
        case central_window:     return SS;
        case backdrop_window:    return suggested_height;
    }
    return 0;
}

void SendBlankColor(raw_window_t* window, subsurface_type_t size,
                    uint32_t color)
{
    pixel_buffer_t* pixels = CreateSolidPixelBuffer(
        CalculateWidth(size), CalculateHeight(size), XRGB, color);
    wl_surface_attach(window, pixels, 0, 0);
    wl_surface_commit(window);
    wl_buffer_destroy(pixels);
}

void NoteSuggestedBounds(int32_t width, int32_t height)
{
    suggested_width = width;
    suggested_height = height;
}

void SetSubwindowPosition(subwindow_t* subwindow, subsurface_type_t type)
{
    uint32_t x = 0, y = 0;

    switch (type)
    {
        case gap_filler_right: x = suggested_width - GAP_SIZE; break;
        case gap_filler_left:  break;
        case gap_centered_right:
            x = suggested_width - CalculateWidth(gap_centered_left);
            y = (suggested_height - CalculateHeight(gap_centered_left)) /
                2;
            break;
        case gap_centered_left:
            x = (GAP_SIZE - CalculateWidth(gap_centered_left)) / 2;
            y = (suggested_height - CalculateHeight(gap_centered_left)) /
                2;
            break;
        case central_window:
            if (SS == suggested_height) x = GAP_SIZE;
            else y = GAP_SIZE;
            break;
        case backdrop_window: return; // This should never happen.
    }

    wl_subsurface_set_position(subwindow->inner, x, y);
    wl_surface_commit(subwindow->_win);
}

raw_window_t* CreateRawWindow(void)
{
    return wl_compositor_create_surface(GetCompositor());
}
raw_subwindow_t* CreateRawSubwindow(raw_window_t** window,
                                    raw_window_t* parent)
{
    *window = CreateRawWindow();
    return wl_subcompositor_get_subsurface(GetSubcompositor(), *window,
                                           parent);
}

void CreateWindows(void)
{
    background_raw = CreateRawWindow();
    background = WrapRawWindow(background_raw);
    wl_surface_commit(background_raw);

    bust_window.inner =
        CreateRawSubwindow(&bust_window._win, background_raw);
    gameplay_window.inner =
        CreateRawSubwindow(&gameplay_window._win, background_raw);
    stat_window.inner =
        CreateRawSubwindow(&stat_window._win, background_raw);
}

void SetWindowPositions(int32_t suggested_width, int32_t suggested_height)
{
    NoteSuggestedBounds(suggested_width, suggested_height);
    SetSubwindowPosition(&bust_window, bust_window_type);
    SetSubwindowPosition(&gameplay_window, gameplay_window_type);
    SetSubwindowPosition(&stat_window, stat_window_type);
}

void DestroyWindows(void)
{
    wl_surface_destroy(bust_window._win);
    wl_surface_destroy(gameplay_window._win);
    wl_surface_destroy(stat_window._win);
    wl_subsurface_destroy(bust_window.inner);
    wl_subsurface_destroy(gameplay_window.inner);
    wl_subsurface_destroy(stat_window.inner);
    xdg_surface_destroy(background);
    wl_surface_destroy(background_raw);
}

raw_window_t* GetBackgroundWindowRaw(void) { return background_raw; }
window_t* GetBackgroundWindow(void) { return background; }

raw_window_t* GetWindowRaw(requested_window_t requested)
{
    switch (requested)
    {
        case bust:     return bust_window._win;
        case stat:     return stat_window._win;
        case gameplay: return gameplay_window._win;
        case backdrop: return background_raw;
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
subsurface_type_t GetWindowType(requested_window_t requested)
{
    switch (requested)
    {
        case bust:     return bust_window_type;
        case stat:     return stat_window_type;
        case gameplay: return gameplay_window_type;
        case backdrop: return backdrop_window;
    }
    return backdrop_window; // unreachable, but the gcc compiler is stupid
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
