#include "System.h"
#include <GLAD/opengl.h>  // OpenGL function prototypes
#include <Output/Error.h> // Error reporting
#include <Output/Warning.h>
#include <Windowing/Wayland.h> // Wayland display
#include <stdbool.h>
#include <stddef.h>
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
 * @brief A list of contexts @enum backdrop values long that we use to
 * store the rendering contexts for all subwindows.
 */
//! better way!!!
static EGLContext* contexts[center_filler] = {NULL, NULL, NULL};

static bool CheckEGLContextsEdited(void)
{
    return contexts[0] != NULL && contexts[1] != NULL &&
           contexts[2] != NULL;
}

void SetupEGL(void)
{
    if (GetDisplay() == NULL)
    {
        ReportWarning(preemptive_egl_setup);
        return;
    }

    if (display != NULL || config != NULL || CheckEGLContextsEdited())
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

    // Fill out each context with the information we need, including the
    // fact that we're using OpenGL ES v2.0.
    EGLint context_attribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    for (int i = 0; i < center_filler; i++)
    {
        contexts[i] =
            eglCreateContext(display, config, NULL, context_attribs);
        if (contexts[i] == NULL) ReportError(egl_context_create_failure);
    }
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

    eglReleaseThread();
}

void BindEGLContext(panel_t* panel)
{
    if (panel == NULL) return;

    panel->_es = wl_egl_window_create(panel->_s, 1, 1);
    if (panel->_es == NULL) ReportError(allocation_failure);

    panel->_rt = eglCreateWindowSurface(
        display, config, (EGLNativeWindowType)panel->_es, NULL);
    if (panel->_rt == NULL) ReportError(egl_surface_create_failure);
}

void UnbindEGLContext(panel_t* panel)
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

    eglDestroyContext(display, contexts[panel->type]);
    contexts[panel->type] = NULL;
}

void ResizeEGLRenderingArea(panel_t* panel)
{
    if (panel == NULL || panel->_es == NULL) return;

    wl_egl_window_resize(panel->_es, panel->width, panel->height, 0, 0);
}

EGLDisplay GetEGLDisplay(void) { return display; }

EGLContext GetEGLContext(panel_type_t type) { return contexts[type]; }
