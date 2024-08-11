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

void SetupEGL(void);
void DestroyEGL(void);

void BindEGLContext(raw_window_t* raw);
void draw(void);

#endif // _MSENG_SYSTEM_RENDERING_SYSTEM_
