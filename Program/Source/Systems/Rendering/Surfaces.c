#include "Surfaces.h"
#include "SHM.h"
#include <Utilities/Math.h>

static int32_t suggested_width = 0;
static int32_t suggested_height = 0;
#define SS int32_min(suggested_width, suggested_height)
#define GAP_SIZE ((float)suggested_width - SS) / 2

uint32_t CalculateWidth(subsurface_size_t size)
{
    switch (size)
    {
        case gap_filler:          return GAP_SIZE;
        case gap_filler_centered: return GAP_SIZE / 1.25;
        case main_window:         return SS;
        case background_window:   return suggested_width;
    }
    return 0;
}

uint32_t CalculateHeight(subsurface_size_t size)
{
    switch (size)
    {
        case gap_filler:          return suggested_height;
        case gap_filler_centered: return suggested_height / 1.25;
        case main_window:         return SS;
        case background_window:   return suggested_height;
    }
    return 0;
}

void SendBlankColor(raw_window_t* window, subsurface_size_t size,
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

int32_t GetSuggestedWidth(void) { return suggested_width; }
int32_t GetSuggestedHeight(void) { return suggested_height; }
int32_t GetShortestSide(void) { return SS; }

void SetSubwindowPosition(subwindow_t* subwindow)
{
    uint32_t x = 0, y = 0;
    switch (subwindow->position)
    {
        case gap_filler_right: x = suggested_width - GAP_SIZE; break;
        case gap_filler_left:  break;
        case gap_centered_right:
            x = suggested_width - CalculateWidth(gap_filler_centered);
            y = (suggested_height - CalculateHeight(gap_filler_centered)) /
                2;
            break;
        case gap_centered_left:
            x = (GAP_SIZE - CalculateWidth(gap_filler_centered)) / 2;
            y = (suggested_height - CalculateHeight(gap_filler_centered)) /
                2;
            break;
        case centered:
            if (SS == suggested_height) x = GAP_SIZE;
            else y = GAP_SIZE;
            break;
    }

    wl_subsurface_set_position(subwindow->subwindow, x, y);
    wl_surface_commit(subwindow->window);
}
