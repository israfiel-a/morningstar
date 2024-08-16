/**
 * @file Manager.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides functionality for creating and maintaining the windowing
 * system on both Windows and Linux.
 * @date 2024-08-05
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_MANAGER_WINDOWING_SYSTEM_
#define _MSENG_MANAGER_WINDOWING_SYSTEM_

// The master include file for the project.
#include <Master.h>
#include <Windowing/Window.h>
// Wayland and XDG type definitions.
#include <Types.h>

/**
 * @brief Bind the XDG-shell interface to the application's registry.
 * @param name The numerical name of the shell interface.
 * @param version The version of the interface.
 */
void BindWindowManager(uint32_t name, uint32_t version);

/**
 * @brief Unbind the window manager (and all the windows) from the
 * registry, and free all data having to do with them.
 */
void UnbindWindowManager(void);

/**
 * @brief Wrap a raw @ref wl_surface into an XDG-shell managed, well,
 * window.
 * @param raw_window The window to wrap.
 * @return The created XDG window.
 */
wrapped_window_t* WrapRawWindow(raw_window_t* raw_window,
                                const char* title);

void UnwrapWindow(window_t* window);

/**
 * @brief Grab the XDG base object.
 * @return The XDG base object.
 */
window_manager_t* GetWindowManager(void);

#endif // _MSENG_MANAGER_WINDOWING_SYSTEM_
