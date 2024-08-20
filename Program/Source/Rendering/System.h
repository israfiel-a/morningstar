/**
 * @file System.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides functionality to help with the initialization /
 * management of EGL--the platform-independent OpenGL ES bridge we use--and
 * EGL rendering contexts.
 * @date 2024-08-11
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_SYSTEM_RENDERING_SYSTEM_
#define _MSENG_SYSTEM_RENDERING_SYSTEM_

// The subwindow interface.
#include <Windowing/Windowing-Types.h>

/**
 * @brief Setup our OpenGL ES bridge, EGL. This will fail if the Wayland
 * display has not been initialized. We configure EGL with the config
 * available in @file Types.h as @def EGL_CONFIG_ATTRIBUTES.
 */
void SetupEGL(void);

/**
 * @brief Destroy our EGL context. This will do nothing if @ref SetupEGL
 * has not been called.
 */
void DestroyEGL(void);

/**
 * @brief Bind an EGL context to the given subwindow. This creates both an
 * EGL window and EGL surface, and stores both within the @struct
 * subwindow_t passed into the function.
 * @param subwindow The subwindow we want to create a rendering context
 * for. If this is NULL, we, of course, do nothing.
 * @param type The type of window being passed in. If this value is @enum
 * backdrop, we do nothing.
 */
void BindEGLContext(panel_t* subwindow);

/**
 * @brief Unbind and destroy an EGL rendering context. This also sets both
 * the @struct _eglwin and @struct render_target members of the passed
 * @struct subwindow_t to NULL, if they are not already such.
 * @param subwindow The subwindow whose rendering context we want to
 * destroy. If this is NULL, we do nothing.
 * @param type The type of subwindow passed in. If this is @enum backdrop,
 * we do nothing.
 */
void UnbindEGLContext(panel_t* subwindow, size_t panel_index);

/**
 * @brief Resize the size of the rendering area bound to the given
 * subwindow. If the subwindow does not have a rendering area, we do
 * nothing.
 * @param subwindow The subwindow whose rendering area we are to resize. If
 * this is NULL, we do nothing.
 * @param type The type of subwindow we're being passed. If this is @enum
 * backdrop, we do nothing.
 */
void ResizeEGLRenderingArea(panel_t* subwindow);

void* GetEGLDisplay(void);

void* GetEGLContext(size_t panel_index);

void* CreateEGLContext(void* share_context);

#endif // _MSENG_SYSTEM_RENDERING_SYSTEM_
