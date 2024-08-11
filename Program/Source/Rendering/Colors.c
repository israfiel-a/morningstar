#include "Colors.h"
#include <Input/File.h>
#include <Session.h>

void SendBlankColor(raw_window_t* window, requested_window_t type,
                    uint32_t color)
{
    uint32_t width = 0, height = 0;
    switch (type)
    {
        case bust:
            width = dimensions.gap_size / 1.25,
            height = dimensions.height / 1.25;
            break;
        case gameplay:
            width = dimensions.shortest_side,
            height = dimensions.shortest_side;
            break;
        case stat:
            width = dimensions.gap_size, height = dimensions.height;
            break;
        case backdrop:
            width = dimensions.width, height = dimensions.height;
            break;
    }

    pixel_buffer_t* pixels =
        CreateSolidPixelBuffer(width, height, XRGB, color);
    wl_surface_attach(window, pixels, 0, 0);
    wl_surface_commit(window);
    wl_buffer_destroy(pixels);
}
