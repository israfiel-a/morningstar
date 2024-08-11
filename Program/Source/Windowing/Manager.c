#include "Manager.h"
#include "System.h"           // Registry functionality
#include "Window.h"           // Windowing
#include <Output/Error.h>     // Error reporting
#include <Rendering/Colors.h> // Blank buffer creation
#include <Rendering/System.h>
#include <Session.h> // Global session data

/**
 * @brief The base of the XDG-shell window manager. This is bound by the
 * registry via the @ref BindWindowManager function.
 */
static window_manager_t* base = NULL;

/**
 * @brief The toplevel window of the application.
 */
static toplevel_t* toplevel = NULL;

/**
 * @brief A function to handle the ping request that XDG-shell will send us
 * to make sure we're not unresponsive.
 * @param d Nothing of use.
 * @param b Nothing of use.
 * @param serial The serial number of the ping request.
 */
static void HP(void* d, window_manager_t* b, uint32_t serial)
{
    xdg_wm_base_pong(base, serial);
}

/**
 * @brief A listener for ping requests sent by XDG-shell, that responds in
 * kind with a pong to keep our application from force quitting.
 */
const static ping_listener_t ponger = {HP};

/**
 * @brief Handle the toplevel configuration event sent by XDG. We simply
 * use this to set our application to fullscreen. None of the parameters
 * are important to us.
 */
static void HTLC(void* d, toplevel_t* t, int32_t w, int32_t h,
                 struct wl_array* s)
{
    xdg_toplevel_set_fullscreen(toplevel, NULL);
}

/**
 * @brief Handle the close event for the application. This is triggered by
 * many things, like an ALT+F4, close button, etc. None of the parameters
 * are important to us.
 */
static void HAC(void* d, toplevel_t* t) { running = false; }

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
static void HWSS(void* d, toplevel_t* t, int32_t width, int32_t height)
{
    if (width == 0 || height == 0) ReportError(monitor_measure_failure);

    dimensions.width = width, dimensions.height = height;
    dimensions.shortest_side =
        (dimensions.width > dimensions.height ? dimensions.height
                                              : dimensions.width);
    dimensions.gap_size =
        ((float)dimensions.width - dimensions.shortest_side) / 2;

    SetWindowPositions();
    BindEGLContext(GetSubwindow(gameplay), gameplay);
    BindEGLContext(GetSubwindow(bust), bust);
    BindEGLContext(GetSubwindow(stat), stat);
}

/**
 * @brief An unused function to handle the given list of compositor
 * abilities. We get this in the form of the @ref wl_registry bind event
 * calls, so we don't particularly care about this.
 */
static void HCAL(void* d, toplevel_t* t, struct wl_array* c) {}

/**
 * @brief A monitor for the application's toplevel, handling things like
 * closes and configures and all that jazz.
 */
const static toplevel_monitor_t toplevel_listener = {HTLC, HAC, HWSS,
                                                     HCAL};

/**
 * @brief Setup the top level of the application, including its ID and
 * title. This is only to be done once. If it is done more than once, the
 * application will fail itself.
 * @param window_raw The raw version of the background window.
 * @param window The XDG ("wrapped") version of the background window.
 */
static void SetToplevel(raw_window_t* restrict window_raw,
                        wrapped_window_t* restrict window)
{
    assert(window != NULL && toplevel == NULL);

    toplevel = xdg_surface_get_toplevel(window);
    xdg_toplevel_add_listener(toplevel, &toplevel_listener, NULL);
    wl_surface_commit(window_raw); // Update the window's state.

    // Set the app ID and title of the application, which show in a bunch
    // of places like the system monitor, titlebar, etc.
    xdg_toplevel_set_app_id(toplevel, ID);
    xdg_toplevel_set_title(toplevel, TITLE);
}

void BindWindowManager(uint32_t name, uint32_t version)
{
    base = wl_registry_bind(GetRegistry(), name, &xdg_wm_base_interface,
                            version);
    xdg_wm_base_add_listener(base, &ponger, NULL);
    CreateUIWindows();
    SetToplevel(GetWindowRaw(backdrop), GetBackdrop());
}

void UnbindWindowManager(void)
{
    DestroyUIWindows();
    xdg_toplevel_destroy(toplevel);
    xdg_wm_base_destroy(base);
}

/**
 * @brief Handle the configure event for an XDG surface. We only ever
 * create one of these, the background window, so we don't bother with the
 * passed in window object, instead just using our own background window
 * object.
 * @param d Nothing of use.
 * @param s Nothing of use.
 * @param serial The serial number of the configuration event.
 */
static void HWC(void* d, wrapped_window_t* s, uint32_t serial)
{
    xdg_surface_ack_configure(GetBackdrop(), serial);

    SendBlankColor(GetWindowRaw(backdrop), backdrop, BLACK);
    draw(GetSubwindow(gameplay), gameplay);
    draw(GetSubwindow(bust), bust);
    draw(GetSubwindow(stat), stat);
}

/**
 * @brief The listener for XDG surfaces, or the actual windows of the
 * application. We only ever create one; the background window of the
 * application.
 */
const static window_listener_t surface_listener = {HWC};

wrapped_window_t* WrapRawWindow(raw_window_t* raw_window)
{
    wrapped_window_t* window =
        xdg_wm_base_get_xdg_surface(base, raw_window);
    xdg_surface_add_listener(window, &surface_listener, NULL);
    return window;
}

window_manager_t* GetWindowManager(void) { return base; }
