/**
 * @file Window.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Functionality for the creation of the window surface within the
 * application.
 * @date 2024-07-27
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_WINDOW_SYSTEM_
#define _MSENG_WINDOW_SYSTEM_

// The master include file for the project.
#include <Master.h>
// Wayland/XDG type definitions.
#include <Types.h>

/**
 * @brief An enum to specify which window you're trying to access.
 */
typedef enum __attribute__((__packed__))
{
    /**
     * @brief You're trying to access the window at index 0, the character
     * bust display.
     */
    bust,
    /**
     * @brief You're trying to access the window at index 1, the actual
     * gameplay window.
     */
    gameplay,
    /**
     * @brief You're trying to access the stat window at index 2.
     */
    stat,
    /**
     * @brief You're trying to access the window behind the rest, the
     * backdrop window. This one doesn't have an index, as its object
     * contains the array.
     */
    backdrop
} requested_window_t;

/**
 * @brief A structure describing the various states of a particular
 * subwindow.
 */
typedef struct
{
    /**
     * @brief The actual subsurface interface of the window. This includes
     * position and sync information.
     */
    raw_subwindow_t* inner;
    /**
     * @brief The inner @ref wl_surface of the subwindow. This should
     * really not be edited by anything, beyond the occasional @ref
     * wl_surface_commit to push changes to the surface.
     */
    raw_window_t* _win;
} subwindow_t;

/**
 * @brief A structure to describe the various states of a particular
 * window. In the base, unmodified version of the toolkit, there is only
 * one version of this object ever created.
 */
typedef struct
{
    /**
     * @brief The raw Wayland surface within the XDG-shell one.
     */
    raw_window_t* _win;
    /**
     * @brief The XDG-shell surface. An actual "window", one could say.
     */
    wrapped_window_t* inner;
    /**
     * @brief An array of subwindows.
     */
    subwindow_t* subwindows;
} window_t;

/**
 * @brief Create the UI windows our application will render to in a couple
 * of milliseconds. These regions start out with neither a size nor a
 * position, those values are to be set with the functions @ref
 * SendBlankColor and @ref SetUIWindowPositions respectively.
 */
void CreateUIWindows(void);

/**
 * @brief Destroy the UI windows of the application. UI WINDOWS, not UI
 * PANELS. Panels and decorations draw by the game are to be disposed of
 * far before this function is called.
 */
void DestroyUIWindows(void);

/**
 * @brief Set the positions of the windows. This function should be called
 * by the @ref HWSS method called from @file Manager.c in response of
 * XDG-shell reporting monitor dimensions. Any other calls are pointless,
 * and will be ignored.
 */
void SetWindowPositions(void);

/**
 * @brief Get the XDG-shell surface corresponding to the backdrop window of
 * the application.
 * @return The backdrop window.
 */
wrapped_window_t* GetBackdrop(void);

/**
 * @brief Get a raw window from one of the application's full windows or
 * subwindows.
 * @param requested The requested window.
 * @return The requested raw window.
 */
raw_window_t* GetWindowRaw(requested_window_t requested);

/**
 * @brief Get a raw subwindow from one of the application's subwindows. If
 * @enum backdrop is passed to this function, NULL is returned.
 * @param requested The requested subwindow.
 * @return The requested raw subwindow.
 */
raw_subwindow_t* GetSubwindowRaw(requested_window_t requested);

#endif // _MSENG_WINDOW_SYSTEM_
