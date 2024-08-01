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

#include <WaylandTypes.h>

void BindWindowManager(registry_t* registry, uint32_t name,
                       uint32_t version);

void BindCompositor(registry_t* registry, uint32_t name, uint32_t version);
void BindSubcompositor(registry_t* registry, uint32_t name,
                       uint32_t version);

void UnbindWindowManager(void);
void UnbindCompositor(void);
void UnbindSubcompositor(void);

void SetToplevelWindow(window_t* window);
toplevel_t* GetToplevelWindow(void);

raw_window_t* CreateRawWindow(void);
raw_subwindow_t* CreateRawSubwindow(raw_window_t** window,
                                    raw_window_t* parent);
window_t* WrapRawWindow(raw_window_t* raw_window);

const window_manager_t* GetWindowManager(void);
const compositor_t* GetCompositor(void);
const subcompositor_t* GetSubcompositor(void);

#endif // _MSENG_WINDOW_MANAGER_SYSTEM_
