#include "Window.h"
#include "Manager.h"
#include "Monitor.h"
#include <Error.h>
#include <Utilities/SharedMemory.h>

static void HandleBufferDeletion(void* data, pixel_buffer_t* buffer)
{
    wl_buffer_destroy(buffer);
}

static const pixel_buffer_monitor_t wl_buffer_listener = {
    HandleBufferDeletion};

void SetWindowTitle(const char* id, const char* title)
{
    assert(title != NULL);
    xdg_toplevel_set_app_id(wm_data.xsh_toplevel, id);
    xdg_toplevel_set_title(wm_data.xsh_toplevel, title);
}

pixel_buffer_t* PaintWindowBackground(uint32_t background_color)
{
    // Since we use 4-channel RGBA formatting for each pixel, and the count
    // of pixels is the window's width, multiply it by four to get the
    // total amount of bytes per row.
    const uint32_t stride = (uint32_t)GetMonitorShortSide() * 4;
    // Get the total size of the needed buffer.
    const uint32_t size = (uint32_t)GetMonitorShortSide() * stride;

    int fd = OpenSHM(size);

    uint32_t* frame_data =
        mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (frame_data == MAP_FAILED) ReportError(memory_map_failure);

    shared_memory_pool_t* pool =
        wl_shm_create_pool(wm_data.wl_sharedmemory, fd, (int32_t)size);
    pixel_buffer_t* buffer = wl_shm_pool_create_buffer(
        pool, 0, GetMonitorShortSide(), GetMonitorShortSide(),
        (int32_t)stride, WL_SHM_FORMAT_ARGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);

    (void)memset(frame_data, background_color, size);
    if (munmap(frame_data, size) == -1) ReportError(memory_unmap_failure);

    wl_buffer_add_listener(buffer, &wl_buffer_listener, NULL);
    return buffer;
}
