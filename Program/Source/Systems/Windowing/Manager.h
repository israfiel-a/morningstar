/**
 * @file Manager.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides functionality for creating and maintaining the windowing
 * system on both Windows and Linux.
 * @date 2024-08-05
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_WINDOW_MANAGER_SYSTEM_
#define _MSENG_WINDOW_MANAGER_SYSTEM_

// The master include file for the project.
#include <Master.h>
// Wayland and XDG type definitions.
#include <Utilities/WaylandTypes.h>

/**
 * @brief A function to setup the Wayland server, the display environment
 * one used for the Linux distribution of Morningstar. This takes no
 * arguments and returns none, but creates, binds, and does the first poll
 * for the display interface and registry.
 */
void SetupWayland(void);

/**
 * @brief Destroy the Wayland display server data stored in our
 * application. All getters having to do with Wayland will return NULL
 * until @ref SetupWayland is called again.
 */
void DestroyWayland(void);

/**
 * @brief Check the validity of the current session; make sure we haven't
 * been requested to die, check to make sure the display server is still
 * sending display, etcetera.
 * @return true The session is still valid, continue.
 * @return false The session is no longer valid, kill the process.
 */
bool CheckSessionValidity(void);

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

void SetToplevelWindow(window_t* window);
toplevel_t* GetToplevelWindow(void);

raw_window_t* CreateRawWindow(void);
raw_subwindow_t* CreateRawSubwindow(raw_window_t** window,
                                    raw_window_t* parent);
window_t* WrapRawWindow(raw_window_t* raw_window);

#endif // _MSENG_WINDOW_MANAGER_SYSTEM_
