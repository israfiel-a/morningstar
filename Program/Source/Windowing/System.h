/**
 * @file System.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides functionality for the creation / initialization,
 * destruction / deinitialization, and other management tasks to do with
 * within both the Linux and Windows windowing systems.
 * @date 2024-08-05
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_SYSTEM_WINDOWING_SYSTEM_
#define _MSENG_SYSTEM_WINDOWING_SYSTEM_

// The master include file for the project.
#include <Master.h>
// Wayland/XDG type definitions.
#include <Types.h>

/**
 * @brief Setup the interface to the display server for the application.
 * This function doesn't do a lot on Windows, as the display server is damn
 * near untouchable as a userspace application--not that we'd need to touch
 * it, of course.
 */
void SetupDisplayServer(void);

/**
 * @brief Clean up all things having to do with the display server of the
 * application. Like @ref SetupDisplayServer, this function doesn't do much
 * on Windows.
 */
void EndDisplayServer(void);

/**
 * @brief Check to make sure the Wayland display server is still
 * sending/processing events. If the server is no longer processing events,
 * this function kills the process immediately with a code of 8, figuring
 * something must be wrong with the user's computer.
 */
void CheckDisplayServer(void);

display_t* GetDisplay(void);

/**
 * @brief Get the Wayland registry object for the application. This
 * function returns NULL if the Wayland display server used is not Wayland.
 * @return A pointer to the registry.
 */
registry_t* GetRegistry(void);

/**
 * @brief Get the Wayland compositor's compositor.
 * @return The Wayland compositor's compositor.
 */
compositor_t* GetCompositor(void);

/**
 * @brief Get the Wayland compositor's compositor's compositor. Dear god.
 * @return The Wayland compositor's compositor's compositor.
 */
subcompositor_t* GetSubcompositor(void);

#endif // _MSENG_SYSTEM_WINDOWING_SYSTEM_
