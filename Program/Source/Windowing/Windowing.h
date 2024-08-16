/**
 * @file Windowing.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief This file provides the public windowing interface of the
 * Morningstar toolkit. Functions within this file will deal with the
 * window manager, window creation and destruction. All warnings and errors
 * are documented.
 * @date 2024-08-14
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_WINDOWING_
#define _MSENG_WINDOWING_

// C standard library boolean type.
#include <stdbool.h>

/**
 * @brief Set up the data having to do with the application's window. This,
 * on Wayland, initializes the display server and registry, starts the
 * window manager, starts EGL, and records a bunch of other irrelevant
 * information. On Windows, well, this basically just sets up EGL.
 *
 * ERRORS
 *
 * Many, many errors can occur within this function on Wayland--seven in
 * total. Within the called @ref SetupWayland routine, 3 can occur; first,
 * @enum display_connect_failure for a failure to connect to a Wayland
 * display. This often means the user is not running within a Wayland
 * session. Second, @enum server_processing_failure will be triggered when
 * Wayland refuses to process the events we've sent to the server. Finally,
 * we do a check to make sure the registry was connected to all the objects
 * we need, and if any were @b not connected, we throw a @enum
 * compositor_missing_features error.
 *
 * Within the @ref SetupEGL function, we first connect EGL to our display
 * (i.e the Wayland compositor on Wayland, etc.). This operation can fail,
 * and it if does we throw an @enum egl_display_connect_failure error. If
 * we get past that but then fail to actually initialize EGL via the @ref
 * eglInitialize function, we hit an @enum egl_initialization_failure
 * error. Next, if we make it to choosing a configuration and fail there,
 * we throw an @enum egl_config_failure. After the configuration is chosen,
 * we have to initialize the rendering contexts we use for the sub-panels,
 * and if this fails an @enum egl_context_create_failure error is thrown.
 *
 * WARNINGS
 *
 * This function can throw three warnings; a @enum double_display_setup
 * inside @ref SetupWayland if the function's already been called, a @enum
 * preemptive_egl_setup in @ref SetupEGL if the Wayland display has not
 * been setup yet, and a @enum double_egl_setup in the same function if
 * it's been called already.
 *
 * PARAMETERS / RETURN VALUE
 *
 * @param lock_unified A boolean flag to lock the application in unified
 * mode, with a single gameplay window as the only rendered object. This is
 * the only lock your software can directly place on the panel layout of
 * the application.
 */
void SetupWindow(bool lock_unified);

/**
 * @brief Create the application window. This will initialize the
 * sub-panels as well, though nothing is drawn to them. If on Wayland,
 * nothing will be done if the display or registry has not yet been
 * initialized, and the warning code @enum preemptive_window_creation will
 * be reported.
 *
 * ERRORS
 *
 * Only four errors can occur within the time-frame of this function. All
 * three are caught within @ref CreatePanels. An @enum allocation_failure
 * may be triggered if the application is not given the memory we need to
 * create the panel list.
 *
 * The rest of the errors will be caught within the function @ref
 * BindEGLContext, during the creation of the sub-panels' rendering
 * contexts. Number one would be another @enum allocation_failure that's
 * reported if Wayland fails to get the proper amount of space for the
 * required @struct wl_egl_window. The second error would be an @enum
 * egl_surface_create_failure that's triggered if EGL fails to create a
 * render target for the window.
 *
 * WARNINGS
 *
 * Down the line, within both @ref CreateBackdrop and @ref CreatePanels,
 * the application will check the Wayland compositor and subcompositor, the
 * EGL display, and the XDG-shell window manager to make certain all three
 * are initialized. If they are not, this function will do nothing and
 * various warnings will be thrown, depending on where the issue is caught.
 * These possible warnings are @enum preemptive_backdrop_creation and
 * @enum preemptive_panel_creation.
 *
 * The opposite is also true; if this function is called multiple times,
 * the warnings @enum double_backdrop_creation and @enum
 * double_panel_creations can/will be generated.
 *
 * PARAMETERS / RETURN VALUE
 *
 * @param window_title The requested title of the window. This is only
 * visible in certain places, as on neither Windows nor Linux is there a
 * titlebar associated with a Morningstar window, but it is still valuable
 * for viewing within a system monitor or something of the like.
 */
void CreateWindow(const char* window_title);

/**
 * @brief Destroy the application window, and most of the data associated
 * with it, minus some rendering context stuff that'll stick around until
 * the process ends. This function should (nearly always) be called towards
 * the tail-end of application runtime. The only thing that @b maybe should
 * come after is disk-saving data.
 *
 * ERRORS
 *
 * None to note.
 *
 * WARNINGS
 *
 * A @b lot of warnings can occur here, all of which are either @enum
 * double- warnings or @enum preemptive- warnings. Starting off with the
 * first subroutine called, @ref DestroyPanels, the warning @enum
 * preemptive_panel_free will be reported if the subwindow list has not yet
 * been intialized. If any of the panels are NULL, the warning @enum
 * double_panel_free will be triggered. All warnings are also inherited
 * from the @ref UnbindEGLContext function.
 *
 * In the next function, @ref DestroyBackdrop, four errors can arise. If
 * the application's subwindow list is NULL, the warning @enum
 * preemptive_backdrop_free will be reported. If the application window's
 * Wayland context is NULL, the @enum double_backdrop_free warning will
 * be reported. Within the sub-subroutine @ref UnwrapWindow, if the
 * XDG-shell window manager's toplevel has not been intialized, the @enum
 * preemptive_window_unwrapping will be called. If the passed-in window's
 * XDG context has not been created, the @enum double_window_unwrapping
 * will be raised.
 *
 * All warnings are also inherited from @ref DestroyWayland and @ref
 * DestroyEGL. Those are omitted here to maintain some semblance of
 * brevity.
 *
 * PARAMETERS / RETURN VALUE
 *
 * None to note.
 */
void DestroyWindow(void);

//! temp until better location found
void run(void);

#endif // _MSENG_WINDOWING_
