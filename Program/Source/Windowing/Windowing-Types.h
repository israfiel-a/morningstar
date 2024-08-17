/**
 * @file Windowing-Types.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief This file provides some basic structures and enumerators needed
 * in many parts of the toolkit's windowing interface. This includes things
 * like UI panels, the application window, panel types, and more.
 * @date 2024-08-16
 * @version 1.0.0
 *
 * @copyright (c) 2024 - Israfiel
 * This document is under the Creative Commons Zero 1.0 License.
 * It, and all of its contents, are licensed to be used in a creative,
 * commercial, or educational environment. You may modify or distribute
 * this file as you wish, however you may @b not use it in a trademark or
 * patent context. For more information, see the @file LICENSE.md file
 * attached to your distribution of the program.
 */

#ifndef _MSENG_WINDOWING_TYPES_
#define _MSENG_WINDOWING_TYPES_

// Portable, set-width types.
#include <inttypes.h>
// Safer, easier-to-allocate memory chunks.
#include <Memory/Allocate.h>

/**
 * @brief An enumerator to describe a specific sub-panel of the background
 * window.
 */
typedef enum
{
    /**
     * @brief The specified window fills the entire left gap, between the
     * edge of the central window and the edge of the monitor. If the
     * monitor is longer tall than it is wide, this item is placed on the
     * bottom of the central window.
     */
    left_gap_filler,
    /**
     * @brief The specified window fills the entire right gap, between the
     * edge of the central window and the edge of the monitor. If the
     * monitor is longer tall than it is wide, this item is placed on the
     * top of the central window.
     */
    right_gap_filler,
    /**
     * @brief The specified window fills 75% of the left gap between the
     * edge of the central window and the edge of the monitor. These
     * windows will @b not be moved to the bottom in the case of a vertical
     * monitor, instead the toolkit will refuse to create them, or destroy
     * any already created. Plan accordingly.
     */
    left_gap_floater,
    /**
     * @brief The specified window fills 75% of the right gap between the
     * edge of the central window and the edge of the monitor. These
     * windows will @b not be moved to the top in the case of a vertical
     * monitor, instead the toolkit will refuse to create them, or destroy
     * any already created. Plan accordingly.
     */
    right_gap_floater,
    /**
     * @brief The specified window is a perfectly square object that lives
     * in the center of the application window. Its size is the length of
     * the smallest side of the monitor. Therefore, it is centered on one
     * axis; the longer one.
     */
    center_filler
} panel_type_t;

/**
 * @brief A specific panel created to actually be rendered onto. Includes
 * vital information like width, height, x, y, type, along with pointers to
 * private objects like the render target and Wayland subsurface.
 */
typedef struct
{
    /**
     * @brief The type of panel it is. See @ref panel_type_t for
     * explanation. This value should never change past the creation of the
     * object.
     */
    panel_type_t type;
    /**
     * @brief The width of the panel in pixels. @note Changing this value
     * will do nothing but potentially messing up rendering proportions
     * later on.
     */
    uint32_t width;
    /**
     * @brief The height of the panel in pixels. @note Changing this value
     * will do nothing but potentially messing up rendering proportions
     * later on.
     */
    uint32_t height;
    /**
     * @brief The X-coordinate of the panel in screen coordinates. @note
     * Changing this value will do nothing. It is simply here for reading.
     */
    int32_t x;
    /**
     * @brief The Y-coordinate of the panel in screen coordinates. @note
     * Changing this value will do nothing. It is simply here for reading.
     */
    int32_t y;
    /**
     * @brief The Wayland surface of the window. @note There is no
     * reason to edit this. Let functions help you.
     */
    struct wl_surface* _s;
    /**
     * @brief The Wayland subsurface marker of the window. @note There
     * is no reason to edit this. Let functions help you.
     */
    struct wl_subsurface* _ss;
    /**
     * @brief The Wayland EGL binding of the window. @warning Editing this
     * will prevent @b any render calls from going to the window, and
     * likely crash the rendering loop.
     */
    struct wl_egl_window* _es;
    /**
     * @brief The EGL rendering target of the panel. This is an @struct
     * EGLSurface. @warning Editing this will instantaneously crash the
     * rendering loop.
     */
    void* _rt;
} panel_t;

typedef struct
{
    const char* title;
    const char* id;
    ptr_t panels;

    struct wl_surface* _s;
    struct xdg_surface* _ws;
} window_t;

#endif // _MSENG_WINDOWING_TYPES_
