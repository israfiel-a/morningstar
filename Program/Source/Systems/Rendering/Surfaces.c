#include "Surfaces.h"
#include "SHM.h"

void SendBlankColor(window_raw_t* window, uint32_t width, uint32_t height,
                    uint32_t color)
{
    pixel_buffer_t* pixels =
        CreateSolidPixelBuffer(width, height, XRGB, color);
    wl_surface_attach(window, pixels, 0, 0);
    wl_surface_commit(window);
    wl_buffer_destroy(pixels);
}
