#include "Window.h"
#include "Manager.h" // Window wrapping
#include "System.h"  // Registry & compositor getter
#include <Output/Error.h>
#include <Output/Warning.h>
#include <Rendering/System.h>

/**
 * @brief The application's window, and all its children.
 */
static window_t application_window = {NULL, NULL, NULL};

/**
 * @brief A macro for the bust display window.
 */
#define bust_window application_window.subwindows[0]

/**
 * @brief A macro for the central gameplay window.
 */
#define gameplay_window application_window.subwindows[1]

/**
 * @brief A macro for the stat window.
 */
#define stat_window application_window.subwindows[2]

bool CheckSubwindow(subwindow_t* subwindow)
{
    return subwindow != NULL && subwindow->_win != NULL &&
           subwindow->inner != NULL;
}

void CreateBackdrop(const char* window_title)
{
    if (GetDisplay() == NULL || GetRegistry() == NULL)
    {
        ReportWarning(preemptive_window_creation);
        return;
    }

    if (GetCompositor() == NULL || GetWindowManager() == NULL)
    {
        ReportWarning(preemptive_backdrop_creation);
        return;
    }

    if (application_window._win != NULL ||
        application_window.inner != NULL)
    {
        ReportWarning(double_backdrop_creation);
        return;
    }

    application_window._win =
        wl_compositor_create_surface(GetCompositor());
    application_window.inner =
        WrapRawWindow(application_window._win, window_title);
    wl_surface_commit(application_window._win);
}

void CreatePanels(void)
{
    if (GetDisplay() == NULL || GetRegistry() == NULL)
    {
        ReportWarning(preemptive_window_creation);
        return;
    }

    if (GetCompositor() == NULL || GetSubcompositor() == NULL ||
        GetEGLDisplay() == NULL || application_window._win == NULL ||
        application_window.inner == NULL)
    {
        ReportWarning(preemptive_panel_creation);
        return;
    }

    if (application_window.subwindows != NULL)
    {
        ReportWarning(double_panel_creation);
        return;
    }

    application_window.subwindows = calloc(3, sizeof(subwindow_t));
    if (application_window.subwindows == NULL)
        ReportError(allocation_failure);

    // Loop through the three now-created spaces fit for windows
    // and fill 'em with beautiful Wayland pointer objects.
    for (uint8_t i = 0; i < 3; i++)
    {
        application_window.subwindows[i]._win =
            wl_compositor_create_surface(GetCompositor());
        application_window.subwindows[i].inner =
            wl_subcompositor_get_subsurface(
                GetSubcompositor(), application_window.subwindows[i]._win,
                application_window._win);
        BindEGLContext(&application_window.subwindows[i], i);
    }
}

void DestroyBackdrop(void)
{
    if (application_window.subwindows != NULL)
    {
        ReportWarning(preemptive_backdrop_free);
        return;
    }

    if (application_window._win == NULL)
    {
        ReportWarning(double_backdrop_free);
        return;
    }

    UnwrapWindow(&application_window);
    wl_surface_destroy(application_window._win);

    application_window._win = NULL;
    application_window.inner = NULL;
}

void DestroyPanels(void)
{
    if (application_window.subwindows == NULL)
    {
        ReportWarning(preemptive_panel_free);
        return;
    }

    if (!CheckSubwindow(&bust_window) ||
        !CheckSubwindow(&gameplay_window) || !CheckSubwindow(&stat_window))
    {
        ReportWarning(double_panel_free);
        return;
    }

    wl_surface_destroy(bust_window._win),
        wl_surface_destroy(gameplay_window._win),
        wl_surface_destroy(stat_window._win);
    wl_subsurface_destroy(bust_window.inner),
        wl_subsurface_destroy(gameplay_window.inner),
        wl_subsurface_destroy(stat_window.inner);
    // We don't check for EGL-related warnings here cause this function
    // does it for us.
    UnbindEGLContext(&bust_window, bust),
        UnbindEGLContext(&gameplay_window, gameplay),
        UnbindEGLContext(&stat_window, stat);

    free(application_window.subwindows);
    application_window.subwindows = NULL;
}

void SetWindowPositions(void)
{
    // Loop over each subwindow.
    for (uint8_t i = 0; i < 3; i++)
    {
        uint32_t x = 0, y = 0;
        switch (i)
        {
            // Center the subwindow within the left gap of the UI.
            case bust:
                x = (dimensions.gap_size - (dimensions.gap_size / 1.25)) /
                    2;
                y = (dimensions.height - (dimensions.height / 1.25)) / 2;
                break;
            // Center the window either vertically or horizontally,
            // depending on which dimension is shorter.
            case gameplay:
                if (dimensions.shortest_side == dimensions.height)
                    x = dimensions.gap_size;
                else y = dimensions.gap_size;
                break;
            // Set the subwindow to be positioned at the top left corner of
            // the right gap.
            case stat: x = dimensions.width - dimensions.gap_size; break;
        }

        wl_subsurface_set_position(application_window.subwindows[i].inner,
                                   x, y);
        // Update Wayland on our changes.
        wl_surface_commit(application_window.subwindows[i]._win);
    }
}

wrapped_window_t* GetBackdrop(void) { return application_window.inner; }

raw_window_t* GetWindowRaw(requested_window_t requested)
{
    switch (requested)
    {
        case bust:     return bust_window._win;
        case stat:     return stat_window._win;
        case gameplay: return gameplay_window._win;
        case backdrop: return application_window._win;
    }
    return NULL; // Unreachable, but the gcc compiler hates me
}

subwindow_t* GetSubwindow(requested_window_t requested)
{
    switch (requested)
    {
        case bust:     return &bust_window;
        case stat:     return &stat_window;
        case gameplay: return &gameplay_window;
        case backdrop: return NULL;
    }
    return NULL; // Unreachable, but the gcc compiler hates me
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
    return NULL; // Unreachable, but the gcc compiler hates me
}

uint32_t GetSubwindowWidth(requested_window_t requested)
{
    switch (requested)
    {
        case bust:     return dimensions.gap_size / 1.25;
        case gameplay: return dimensions.shortest_side;
        case stat:     return dimensions.gap_size;
        case backdrop: return dimensions.width;
    }
    return 0; // Unreachable, but the gcc compiler hates me
}

uint32_t GetSubwindowHeight(requested_window_t requested)
{
    switch (requested)
    {
        case bust:     return dimensions.height / 1.25;
        case gameplay: return dimensions.shortest_side;
        case stat:     return dimensions.height;
        case backdrop: return dimensions.height;
    }
    return 0; // Unreachable, but the gcc compiler hates me
}
