/**
 * @file Registry.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief A short and sweet file to handle the addition and deletion of
 * registry objects.
 * @date 2024-08-05
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_REGISTRY_WINDOWING_SYSTEM_
#define _MSENG_REGISTRY_WINDOWING_SYSTEM_

// The master include file for the project.
#include <Master.h>
// Wayland and XDG type definitions.
#include <Utilities/WaylandTypes.h>

/**
 * @brief Grab the registry from the display server and setup a listener to
 * wait for more interfaces to be reported by Wayland.
 * @param display The display from which to grab the registry object.
 */
void BeginRegistry(display_t* display);

/**
 * @brief Destroy the registry object, and de-allocate all things bound
 * within it.
 */
void DestroyRegistry(void);

/**
 * @brief Grab the registry object of the application.
 * @return The registry object.
 */
registry_t* GetRegistry(void);

/**
 * @brief Check to make sure the features we require were reported by the
 * Wayland display server, and that they were bound properly within the
 * registry.
 * @return true Everything was bound properly.
 * @return false Something's amiss. This should trigger an error, probably
 * @enum wayland_missing_features.
 */
bool CheckRequiredFeatures(void);

#endif // _MSENG_REGISTRY_WINDOWING_SYSTEM_
