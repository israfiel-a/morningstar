#include "Windowing.h"
#include "Input/File.h"
#include "Rendering/Colors.h"
#include "Wayland.h" // Wayland wrappers
#include "XDG.h"     // XDG wrappers
#include <Globals.h> // Global flags
#include <Memory/Thread.h>
#include <Output/System.h> // Output functions
#include <Rendering/Loop.h>
#include <Rendering/System.h> // EGL wrappers
#include <pthread.h>
#include <stdio.h>

static window_t window = {NULL, NULL, {NULL, 0, 0}, NULL, NULL};

void SetupWindow(void) { SetupWayland(), SetupEGL(); }

void CreateWindow(const char* window_title)
{
    if (GetDisplay() == NULL || GetRegistry() == NULL ||
        GetCompositor() == NULL || GetWindowManager() == NULL)
    {
        ReportWarning(preemptive_window_creation);
        return;
    }

    if (window._s != NULL || window._ws != NULL)
    {
        ReportWarning(double_window_creation);
        return;
    }

    window._s = CreateSurface();
    window._ws = WrapRawWindow(window._s, window_title);
    CommitSurface(window._s);
}

panel_t* GetPanel(size_t index)
{
    return (panel_t*)(window.panels._a[index]._p);
}

void IteratePanels(void (*func)(panel_t* panel, size_t panel_index))
{
    for (size_t i = 0; i < window.panels.occupied; i++)
        func(GetPanel(i), i);
}

void DestroyWindow(void)
{
    if (window._s == NULL || window._ws == NULL)
    {
        ReportWarning(double_window_free);
        return;
    }

    if (CheckArrayValidity(window.panels))
    {
        ReportWarning(preemptive_panel_free);
        return;
    }

    for (size_t i = 0; i < window.panels.occupied; i++)
    {
        panel_t* panel = GetPanel(i);
        if (panel->_s != NULL && panel->_ss != NULL) continue;

        DestroySurface(&panel->_s);
        DestroySubsurface(&panel->_ss);
        UnbindEGLContext(panel, i);
    }
    DestroyArray(&window.panels);

    UnwrapWindow(&window);
    DestroySurface(&window._s);
    window._s = NULL;
    window._ws = NULL;

    DestroyEGL();
    DestroyWayland();
}

void SetWindowTitle(const char* title) { SetWrappedWindowTitle(title); }

static pthread_mutex_t panel_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t panel_dim_cond = PTHREAD_COND_INITIALIZER;
static pthread_cond_t dims_recorded_cond = PTHREAD_COND_INITIALIZER;
static bool dims_recorded = false;
static void* PanelDimensionListener(void* index)
{
    size_t index_value = *(size_t*)index;

    pthread_mutex_lock(&panel_mutex);
    while (!dimensions.set)
        pthread_cond_wait(&panel_dim_cond, &panel_mutex);

    panel_t* affected = GetPanel(index_value);

    switch (affected->type)
    {
        case left_gap_filler:   break;
        case right_gap_filler:  break;
        case left_gap_floater:  break;
        case right_gap_floater: break;
        case center_filler:
            affected->width = dimensions.shortest_side,
            affected->height = dimensions.shortest_side;
            if (dimensions.shortest_side == dimensions.height)
                affected->x = dimensions.gap_size;
            else affected->y = dimensions.gap_size;
    }
    SetSubsurfacePosition(affected->_ss, affected->x, affected->y);
    ResizeEGLRenderingArea(affected);

    pthread_mutex_unlock(&panel_mutex);

    printf("\n%d :: %d :: %d :: %d", affected->x, affected->y,
           affected->width, affected->height);
    dims_recorded = true;
    pthread_cond_signal(&dims_recorded_cond);

    // struct wl_buffer* pixels =
    //     CreateSolidPixelBuffer(dimensions.width, dimensions.height,
    //                            WL_SHM_FORMAT_XRGB8888, BLACK);
    // wl_surface_attach(window._s, pixels, 0, 0);
    // wl_surface_commit(window._s);
    // wl_buffer_destroy(pixels);

    return NULL;
}

panel_t* CreatePanel(panel_type_t type)
{
    if (GetDisplay() == NULL || GetRegistry() == NULL)
    {
        ReportWarning(preemptive_window_creation);
        return NULL;
    }

    if (GetCompositor() == NULL || GetSubcompositor() == NULL ||
        GetEGLDisplay() == NULL || window._s == NULL || window._ws == NULL)
    {
        ReportWarning(preemptive_panel_creation);
        return NULL;
    }

    if (!CheckArrayValidity(window.panels))
    {
        ReportWarning(implicit_panel_array_creation);
        window.panels = CreateArray(1);
    }

    panel_t created_panel = {
        .type = type,
        ._s = CreateSurface(),
        ._ss = CreateSubsurface(&created_panel._s, window._s)};
    ptr_t panel_block = AllocateBlock(sizeof(panel_t));
    SetBlockContents(&panel_block, &created_panel, sizeof(panel_t));
    AddArrayValue(&window.panels, panel_block);
    FreeBlock(&panel_block);

    size_t panel_index = window.panels.occupied - 1;
    CreateThread(PanelDimensionListener, &panel_index);

    CommitSurface(GetPanel(panel_index)->_s);
    BindEGLContext(GetPanel(panel_index));
    return GetPanel(panel_index);
}

void SignalDimensionsSet_(void) { pthread_cond_signal(&panel_dim_cond); }

void WaitForDimensionSignal_(pthread_mutex_t* mutex)
{
    while (!dims_recorded) pthread_cond_wait(&dims_recorded_cond, mutex);
}

void run(void)
{
    while (running)
    {
        CheckWayland();
        TriggerRender();
        // do all the funny stuff
    }
}
