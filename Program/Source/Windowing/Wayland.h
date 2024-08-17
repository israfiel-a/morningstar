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

#include <inttypes.h>

/**
 * @brief A function to setup the Wayland server, the display environment
 * one used for the Linux distribution of Morningstar. This takes no
 * arguments and returns none, but creates, binds, and does the first poll
 * for the display interface and registry.
 */
void SetupWayland(void);

/**
 * @brief Destroy the Wayland display server data stored in our
 * application. This function is irreversible without effort.
 */
void DestroyWayland(void);

/**
 * @brief Check to make sure the Wayland display server is still
 * sending/processing events. If the server is no longer processing
 * events, this function kills the process immediately with a code of
 * 8, figuring something must be wrong with the user's computer.
 */
void CheckWayland(void);

struct wl_surface* CreateSurface(void);
void DestroySurface(struct wl_surface** surface);
struct wl_subsurface* CreateSubsurface(struct wl_surface** surface,
                                       struct wl_surface* parent);
void DestroySubsurface(struct wl_subsurface** subsurface);
void CommitSurface(struct wl_surface* surface);
void SetSubsurfacePosition(struct wl_subsurface* subsurface, int32_t x,
                           int32_t y);

struct wl_display* GetDisplay(void);

/**
 * @brief Get the Wayland registry object for the application. This
 * function returns NULL if the Wayland display server used is not Wayland.
 * @return A pointer to the registry.
 */
struct wl_registry* GetRegistry(void);

/**
 * @brief Get the Wayland compositor's compositor.
 * @return The Wayland compositor's compositor.
 */
struct wl_compositor* GetCompositor(void);

/**
 * @brief Get the Wayland compositor's compositor's compositor. Dear god.
 * @return The Wayland compositor's compositor's compositor.
 */
struct wl_subcompositor* GetSubcompositor(void);

#endif // _MSENG_SYSTEM_WINDOWING_SYSTEM_
