#include "System.h"
#include <GLAD/opengl.h>  // OpenGL function prototypes
#include <Output/Error.h> // Error reporting
#include <Output/Warning.h>
#include <Windowing/Wayland.h> // Wayland display
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <wayland-egl-core.h>

/**
 * @brief The EGL display object of the application. This is initialized by
 * @ref SetupEGL and set back to NULL by @ref DestroyEGL.
 */
static EGLDisplay display = NULL;

/**
 * @brief The EGL configuration we're using. This is set by @ref SetupEGL
 * and read by said function and @ref BindEGLContext.
 */
static EGLConfig config = NULL;

/**
 * @brief A list of contexts that we use to store the rendering contexts
 * for all surfaces.
 */
static EGLContext* contexts = NULL;

static size_t context_count = 0;

void SetupEGL(void)
{
    if (GetDisplay() == NULL)
    {
        ReportWarning(preemptive_egl_setup);
        return;
    }

    if (display != NULL || config != NULL || contexts != NULL)
    {
        ReportWarning(double_egl_setup);
        return;
    }

    display = eglGetDisplay((EGLNativeDisplayType)GetDisplay());
    if (display == NULL) ReportError(egl_display_connect_failure);
    if (!eglInitialize(display, NULL, NULL))
        ReportError(egl_initialization_failure);

    EGLint n, config_attribs[] = {EGL_SURFACE_TYPE,
                                  EGL_WINDOW_BIT,
                                  EGL_RED_SIZE,
                                  8,
                                  EGL_GREEN_SIZE,
                                  8,
                                  EGL_BLUE_SIZE,
                                  8,
                                  EGL_ALPHA_SIZE,
                                  8,
                                  EGL_RENDERABLE_TYPE,
                                  EGL_OPENGL_ES2_BIT,
                                  EGL_NONE};
    if (!eglChooseConfig(display, config_attribs, &config, 1, &n))
        ReportError(egl_config_failure);
    contexts = malloc(sizeof(void*));
}

void DestroyEGL(void)
{
    if (display == NULL || config == NULL)
    {
        ReportWarning(preemptive_egl_free);
        return;
    }

    eglTerminate(display);
    display = NULL;
    config = NULL;
    free(contexts);

    eglReleaseThread();
}

void BindEGLContext(panel_t* panel)
{
    if (display == NULL || config == NULL)
    {
        ReportWarning(preemptive_egl_context_create);
        return;
    }
    if (panel == NULL)
    {
        ReportWarning(unknown_egl_context);
        return;
    }

    // Fill out each context with the information we need, including the
    // fact that we're using OpenGL ES v2.0.
    EGLint context_attribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    context_count++;
    contexts = realloc(contexts, sizeof(void*) * context_count);
    contexts[context_count - 1] =
        eglCreateContext(display, config, NULL, context_attribs);

    panel->_es = wl_egl_window_create(panel->_s, 1, 1);
    if (panel->_es == NULL) ReportError(allocation_failure);

    panel->_rt = eglCreateWindowSurface(
        display, config, (EGLNativeWindowType)panel->_es, NULL);
    if (panel->_rt == NULL) ReportError(egl_surface_create_failure);

    // EGLBoolean made_current = eglMakeCurrent(
    //     display, panel->_rt, panel->_rt, contexts[context_count - 1]);
    // if (made_current == EGL_FALSE)
    //     ReportError(egl_window_made_current_failure);
}

void UnbindEGLContext(panel_t* panel, size_t panel_index)
{
    if (panel == NULL || panel->_es == NULL || panel->_rt == NULL)
    {
        ReportWarning(preemptive_egl_context_free);
        return;
    }

    wl_egl_window_destroy(panel->_es);
    eglDestroySurface(display, panel->_rt);
    panel->_es = NULL;
    panel->_rt = NULL;

    eglDestroyContext(display, contexts[panel_index]);
}

void ResizeEGLRenderingArea(panel_t* panel)
{
    if (panel == NULL || panel->_es == NULL) return;

    wl_egl_window_resize(panel->_es, panel->width, panel->height, 0, 0);
}

EGLDisplay GetEGLDisplay(void) { return display; }

EGLContext GetEGLContext(size_t panel_index)
{
    return contexts[panel_index];
}

void* CreateEGLContext(EGLContext share_context)
{
    EGLint context_attribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    return eglCreateContext(display, config, share_context,
                            context_attribs);
}
