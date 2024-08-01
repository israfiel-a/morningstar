/**
 * @file WindowManager.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides functions needed to convert Wayland surfaces to XDG
 * windows.
 * @date 2024-07-25
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_WINDOW_MANAGER_SYSTEM_
#define _MSENG_WINDOW_MANAGER_SYSTEM_

#include "Types.h"

typedef struct
{
    const window_manager_monitor_t xsh_monitor;
    const window_manager_surface_monitor_t xsh_surface_monitor;
    const window_manager_root_monitor_t xsh_toplevel_monitor;
} window_manager_monitors_t;
extern const window_manager_monitors_t wm_monitors;

typedef struct
{
    window_manager_base_t* xsh_base;
    window_manager_root_t* xsh_toplevel;
    window_manager_window_t* xsh_surface;
    window_raw_t* wl_window;
    compositor_t* compositor;
    subcompositor_t* subcompositor;
} window_manager_data_t;
extern window_manager_data_t wm_data;

void SetupWindowManager(void);

#endif // _MSENG_WINDOW_MANAGER_SYSTEM_
