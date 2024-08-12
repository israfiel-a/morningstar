/**
 * @file System.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides functionality for initializing EGL, the
 * windowing-system-independent OpenGL bridge used by the toolkit.
 * @date 2024-08-11
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_SYSTEM_RENDERING_SYSTEM_
#define _MSENG_SYSTEM_RENDERING_SYSTEM_

#include <Master.h>
#include <Types.h>
#include <Windowing/Window.h>

void SetupEGL(void);
void DestroyEGL(void);

void BindEGLContext(subwindow_t* subwindow, requested_window_t type);
void UnbindEGLContext(subwindow_t* subwindow, requested_window_t type);
void ResizeEGLWindow(subwindow_t* subwindow, requested_window_t type);

void draw(subwindow_t* subwindow, requested_window_t type);

#endif // _MSENG_SYSTEM_RENDERING_SYSTEM_
