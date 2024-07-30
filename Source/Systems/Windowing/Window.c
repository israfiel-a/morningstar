#include "Window.h"
#include "Manager.h"
#include <Diagnostic/Monitor.h>
#include <Output/Error.h>
#include <Utilities/Math.h>
#include <Utilities/SharedMemory.h>

static subwindow_t bust_window = {0, 0, NULL, NULL};
static subwindow_t gameplay_window = {0, 0, NULL, NULL};
static subwindow_t stat_window = {0, 0, NULL, NULL};

static void HandleBufferDeletion(void* data, pixel_buffer_t* buffer)
{
    wl_buffer_destroy(buffer);
}

static const pixel_buffer_monitor_t wl_buffer_listener = {
    HandleBufferDeletion};

void CreateSubwindows(void)
{
    const uint32_t monitor_width_middle =
        GetMiddle(GetMonitorWidth(), GetMonitorShortSide());

    bust_window.height = GetMonitorHeight() / 1.25;
    bust_window.width = GetMonitorWidth() / 6;
    bust_window.window = wl_compositor_create_surface(wm_data.compositor);
    bust_window.subwindow = wl_subcompositor_get_subsurface(
        wm_data.subcompositor, bust_window.window, wm_data.wl_window);
    wl_subsurface_set_position(
        bust_window.subwindow,
        (monitor_width_middle - bust_window.width) / 2,
        GetMiddle(GetMonitorHeight(), bust_window.height));

    gameplay_window.window =
        wl_compositor_create_surface(wm_data.compositor);
    gameplay_window.subwindow = wl_subcompositor_get_subsurface(
        wm_data.subcompositor, gameplay_window.window, wm_data.wl_window);
    wl_subsurface_set_position(gameplay_window.subwindow,
                               monitor_width_middle, 0);

    gameplay_window.height = GetMonitorShortSide();
    gameplay_window.width = GetMonitorShortSide();

    stat_window.height = GetMonitorHeight();
    stat_window.width = (GetMonitorWidth() - gameplay_window.width) / 2;
    stat_window.window = wl_compositor_create_surface(wm_data.compositor);
    stat_window.subwindow = wl_subcompositor_get_subsurface(
        wm_data.subcompositor, stat_window.window, wm_data.wl_window);
    wl_subsurface_set_position(stat_window.subwindow,
                               GetMonitorWidth() - stat_window.width, 0);
}

void SetWindowTitle(const char* id, const char* title)
{
    assert(title != NULL);
    xdg_toplevel_set_app_id(wm_data.xsh_toplevel, id);
    xdg_toplevel_set_title(wm_data.xsh_toplevel, title);
}

pixel_buffer_t* GenerateWindowBackground(void)
{
    const uint32_t stride = (uint32_t)GetMonitorWidth() * 4;
    const uint32_t size = (uint32_t)GetMonitorHeight() * stride;

    int fd = OpenSHM(size);

    uint32_t* frame_data =
        mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (frame_data == MAP_FAILED) ReportError(memory_map_failure, false);

    shared_memory_pool_t* pool =
        wl_shm_create_pool(wm_data.wl_sharedmemory, fd, (int32_t)size);
    pixel_buffer_t* buffer = wl_shm_pool_create_buffer(
        pool, 0, GetMonitorWidth(), GetMonitorHeight(), (int32_t)stride,
        WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);

    FillMemory32(frame_data, BLACK, size);
    if (munmap(frame_data, size) == -1)
        ReportError(memory_unmap_failure, false);

    wl_buffer_add_listener(buffer, &wl_buffer_listener, NULL);
    return buffer;
}

pixel_buffer_t* GenerateSubwindowBackground(const subwindow_t* window,
                                            uint32_t background_color)
{
    // Since we use 4-channel RGBA formatting for each pixel, and
    // the count of pixels is the window's width, multiply it by
    // four to get the total amount of bytes per row.
    const int32_t stride = (uint32_t)window->width * 4,
                  size = window->height * stride;

    int fd = OpenSHM(size);

    uint32_t* frame_data =
        mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (frame_data == MAP_FAILED) ReportError(memory_map_failure, false);

    shared_memory_pool_t* pool =
        wl_shm_create_pool(wm_data.wl_sharedmemory, fd, size);
    pixel_buffer_t* buffer =
        wl_shm_pool_create_buffer(pool, 0, window->width, window->height,
                                  stride, WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);

    FillMemory32(frame_data, background_color, size);
    if (munmap(frame_data, size) == -1)
        ReportError(memory_unmap_failure, false);

    wl_buffer_add_listener(buffer, &wl_buffer_listener, NULL);
    return buffer;
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
