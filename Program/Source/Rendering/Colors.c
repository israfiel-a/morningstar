#include "Colors.h"
#include <Input/File.h>

void SendBlankColor(const panel_t* panel, uint32_t color)
{
    // uint32_t width = 0, height = 0;
    // switch (type)
    // {
    //     case bust:
    //         width = dimensions.gap_size / 1.25,
    //         height = dimensions.height / 1.25;
    //         break;
    //     case gameplay:
    //         width = dimensions.shortest_side,
    //         height = dimensions.shortest_side;
    //         break;
    //     case stat:
    //         width = dimensions.gap_size, height = dimensions.height;
    //         break;
    //     case backdrop:
    //         width = dimensions.width, height = dimensions.height;
    //         break;
    // }

    struct wl_buffer* pixels = CreateSolidPixelBuffer(
        panel->width, panel->height, WL_SHM_FORMAT_XRGB8888, color);
    wl_surface_attach(panel->_s, pixels, 0, 0);
    wl_surface_commit(panel->_s);
    wl_buffer_destroy(pixels);
}
