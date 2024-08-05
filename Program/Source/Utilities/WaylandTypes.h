/**
 * @file Types.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Some type definitions to make the various Wayland and XDG
 * interface names make more sense.
 * @date 2024-07-27
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_WAYLAND_TYPES_
#define _MSENG_WAYLAND_TYPES_

#include <wayland-client-protocol.h>
#include <wayland-client.h>
#include <xdg-shell.h>

typedef struct wl_buffer pixel_buffer_t;
typedef struct wl_buffer_listener pixel_buffer_monitor_t;
typedef struct wl_output monitor_information_t;
typedef struct wl_output_listener monitor_information_monitor_t;
typedef enum wl_output_transform monitor_transform_t;
typedef struct wl_registry registry_t;
typedef struct xdg_wm_base window_manager_t;
typedef struct xdg_toplevel toplevel_t;
typedef struct xdg_wm_base_listener ping_listener_t;
typedef struct wl_surface_listener raw_window_listener_t;
typedef struct xdg_surface_listener window_manager_surface_monitor_t;
typedef struct xdg_toplevel_listener toplevel_monitor_t;
typedef struct wl_pointer_listener mouse_monitor_t;
typedef enum wl_pointer_button_state mouse_button_state_t;
typedef enum wl_pointer_axis mouse_axis_t;
typedef enum wl_pointer_axis_source mouse_axis_type_t;
typedef enum wl_pointer_axis_relative_direction mouse_axis_direction_t;
typedef struct wl_keyboard_listener keyboard_monitor_t;
typedef struct wl_seat_listener input_group_monitor_t;
typedef struct xdg_surface window_t;
typedef struct wl_seat input_group_t;
typedef struct wl_pointer mouse_object_t;
typedef enum wl_keyboard_keymap_format keyboard_keymap_format_t;
typedef enum wl_keyboard_key_state keyboard_key_state_t;
typedef struct wl_keyboard keyboard_object_t;
typedef struct wl_surface raw_window_t;
typedef struct wl_shm_pool shared_memory_pool_t;
typedef struct wl_shm shared_memory_buffer_t;
typedef struct wl_registry_listener registry_monitor_t;
typedef struct wl_subsurface raw_subwindow_t;
typedef struct wl_compositor compositor_t;
typedef struct wl_subcompositor subcompositor_t;
typedef enum wl_shm_format color_type_t;
typedef struct wl_display display_t;
typedef struct wl_registry registry_t;

#define XRGB WL_SHM_FORMAT_XRGB8888
#define ARGB WL_SHM_FORMAT_ARGB8888

#endif // _MSENG_WAYLAND_TYPES_
