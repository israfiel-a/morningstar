/**
 * @file Client.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief The implementation of a Wayland client bridge. This is what the
 * windowing system of CJ is built off of.
 * @date 2024-07-24
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_WAYLAND_CLIENT_SYSTEM_
#define _MSENG_WAYLAND_CLIENT_SYSTEM_

// Error reporting functionality.
#include <Error.h>

/**
 * @brief Set up the Wayland protocol and ready the program to create a
 * window.
 */
void SetupWayland(void);

/**
 * @brief Clean up all data having to do with the Wayland protocol, @b NOT
 * including window data. That must be destroyed beforehand, using the
 * designated functions within @file Window.h.
 */
void DestroyWayland(void);

#endif // _MSENG_WAYLAND_CLIENT_SYSTEM_
