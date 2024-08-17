#include "XDG.h"
#include "Wayland.h" // Registry functionality
#include <Globals.h>
#include <Output/Error.h> // Error reporting
#include <Output/Warning.h>
#include <Rendering/Colors.h> // Blank buffer creation
#include <Rendering/Loop.h>   // Rendering functions
#include <Rendering/System.h> // EGL functions
#include <XDGS/xdg-shell.h>

/**
 * @brief The base of the XDG-shell window manager. This is bound by the
 * registry via the @ref BindWindowManager function.
 */
static struct xdg_wm_base* base = NULL;

/**
 * @brief The toplevel window of the application.
 */
static struct xdg_toplevel* toplevel = NULL;

/**
 * @brief A function to handle the ping request that XDG-shell will send us
 * to make sure we're not unresponsive.
 * @param d Nothing of use.
 * @param b Nothing of use.
 * @param serial The serial number of the ping request.
 */
static void HP(void* d, struct xdg_wm_base* b, uint32_t serial)
{
    xdg_wm_base_pong(base, serial);
}

/**
 * @brief A listener for ping requests sent by XDG-shell, that responds in
 * kind with a pong to keep our application from force quitting.
 */
const static struct xdg_wm_base_listener ponger = {HP};

/**
 * @brief Handle the toplevel configuration event sent by XDG. We simply
 * use this to set our application to fullscreen. None of the parameters
 * are important to us.
 */
static void HTLC(void* d, struct xdg_toplevel* t, int32_t w, int32_t h,
                 struct wl_array* s)
{
    xdg_toplevel_set_fullscreen(toplevel, NULL);
}

/**
 * @brief Handle the close event for the application. This is triggered by
 * many things, like an ALT+F4, close button, etc. None of the parameters
 * are important to us.
 */
static void HAC(void* d, struct xdg_toplevel* t) { running = false; }

/**
 * @brief Handle the suggested bounds of the window. This is typically the
 * size of a monitor minus all the panels and other UI components that we
 * don't want our surface overlapping.
 * @param d Nothing of use.
 * @param t Nothing of use.
 * @param width The suggested width of the window, or 0 if the bounds are
 * unknown.
 * @param height The suggested height of the window, or 0 if the bounds are
 * unknown.
 */
static void HWSS(void* d, struct xdg_toplevel* t, int32_t width,
                 int32_t height)
{
    if (width == 0 || height == 0) ReportError(monitor_measure_failure);

    dimensions.width = width, dimensions.height = height;
    dimensions.shortest_side = (width > height ? height : width);
    dimensions.gap_size = ((float)width - dimensions.shortest_side) / 2;

    // SetWindowPositions();
    // ResizeEGLRenderingArea(GetSubwindow(gameplay)),
    //     ResizeEGLRenderingArea(GetSubwindow(bust)),
    //     ResizeEGLRenderingArea(GetSubwindow(stat));
}

/**
 * @brief An unused function to handle the given list of compositor
 * abilities. We get this in the form of the @ref wl_registry bind event
 * calls, so we don't particularly care about this.
 */
static void HCAL(void* d, struct xdg_toplevel* t, struct wl_array* c) {}

/**
 * @brief A monitor for the application's toplevel, handling things like
 * closes and configures and all that jazz.
 */
const static struct xdg_toplevel_listener toplevel_listener = {HTLC, HAC,
                                                               HWSS, HCAL};

/**
 * @brief Handle the configure event for an XDG surface. We only ever
 * create one of these, the background window, so we don't bother with the
 * passed in window object, instead just using our own background window
 * object.
 * @param d Nothing of use.
 * @param s Nothing of use.
 * @param serial The serial number of the configuration event.
 */
static void HWC(void* d, struct xdg_surface* s, uint32_t serial)
{
    xdg_surface_ack_configure(s, serial);

    // SendBlankColor(GetWindowRaw(backdrop), BLACK);
    // draw(GetSubwindow(gameplay));
    // draw(GetSubwindow(bust));
    // draw(GetSubwindow(stat));
}

/**
 * @brief The listener for XDG surfaces, or the actual windows of the
 * application. We only ever create one; the background window of the
 * application.
 */
const static struct xdg_surface_listener surface_listener = {HWC};

void BindWindowManager(uint32_t name, uint32_t version)
{
    if (GetRegistry() == NULL)
    {
        ReportWarning(preemptive_wm_creation);
        return;
    }

    if (devices.window_manager)
    {
        ReportWarning(double_wm_creation);
        return;
    }

    base = wl_registry_bind(GetRegistry(), name, &xdg_wm_base_interface,
                            version);
    xdg_wm_base_add_listener(base, &ponger, NULL);
    devices.window_manager = true;
}

void UnbindWindowManager(void)
{
    xdg_wm_base_destroy(base);
    devices.window_manager = false;
}

struct xdg_surface* WrapRawWindow(struct wl_surface* raw_window,
                                  const char* title)
{
    struct xdg_surface* window =
        xdg_wm_base_get_xdg_surface(base, raw_window);
    xdg_surface_add_listener(window, &surface_listener, NULL);

    toplevel = xdg_surface_get_toplevel(window);
    xdg_toplevel_add_listener(toplevel, &toplevel_listener, NULL);
    wl_surface_commit(raw_window); // Update the window's state.

    // Set the app ID and title of the application, which show in a bunch
    // of places like the system monitor, titlebar, etc.
    xdg_toplevel_set_app_id(toplevel, ID);
    xdg_toplevel_set_title(toplevel, title);

    return window;
}

void UnwrapWindow(window_t* window)
{
    if (toplevel == NULL)
    {
        ReportWarning(preemptive_window_unwrapping);
        return;
    }

    if (window == NULL || window->_ws == NULL)
    {
        ReportWarning(double_window_unwrapping);
        return;
    }

    xdg_surface_destroy(window->_ws);
    xdg_toplevel_destroy(toplevel);

    window->_ws = NULL;
    toplevel = NULL;
}

void SetWrappedWindowTitle(const char* title)
{
    if (toplevel == NULL)
    {
        ReportWarning(preemptive_window_title_set);
        return;
    }

    if (title == NULL)
    {
        ReportWarning(invalid_title_value);
        return;
    }

    xdg_toplevel_set_title(toplevel, title);
}

struct xdg_wm_base* GetWindowManager(void) { return base; }
