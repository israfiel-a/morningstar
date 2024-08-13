#include "System.h"
#include <GLAD/opengl.h>      // OpenGL function prototypes
#include <Output/Error.h>     // Error reporting
#include <Windowing/System.h> // Wayland display

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
static EGLContext* contexts[backdrop] = {NULL, NULL, NULL};

/**
 * @brief Check to see if any of the EGL contexts have been edited yet.
 * @return true None of the contexts have been edited.
 * @return false One or more of the contexts have been edited.
 */
static bool CheckEGLContexts(void)
{
    return contexts[0] == NULL && contexts[1] == NULL &&
           contexts[2] == NULL;
}

void SetupEGL(void)
{
    if (display != NULL || config != NULL || !CheckEGLContexts()) return;

    display = eglGetDisplay((EGLNativeDisplayType)GetDisplay());
    if (display == NULL) ReportError(egl_display_connect_failure);
    if (!eglInitialize(display, NULL, NULL))
        ReportError(egl_initialization_failure);

    EGLint n, config_attribs[] = EGL_CONFIG_ATTRIBUTES;
    if (!eglChooseConfig(display, config_attribs, &config, 1, &n))
        ReportError(egl_config_failure);

    // Fill out each context with the information we need, including the
    // fact that we're using OpenGL ES v2.0.
    EGLint context_attribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    for (int i = 0; i < backdrop; i++)
    {
        contexts[i] =
            eglCreateContext(display, config, NULL, context_attribs);
        if (contexts[i] == NULL) ReportError(egl_context_create_failure);
    }
}

void DestroyEGL(void)
{
    if (display == NULL) return;

    eglTerminate(display);
    display = NULL;
    config = NULL;

    eglReleaseThread();
}

void BindEGLContext(subwindow_t* subwindow, requested_window_t type)
{
    if (subwindow == NULL || type == backdrop) return;

    subwindow->_eglwin = wl_egl_window_create(subwindow->_win, 1, 1);
    if (subwindow->_eglwin == NULL) ReportError(egl_window_create_failure);

    subwindow->render_target = eglCreateWindowSurface(
        display, config, (EGLNativeWindowType)subwindow->_eglwin, NULL);
    if (subwindow->render_target == NULL)
        ReportError(egl_surface_create_failure);

    if (!eglMakeCurrent(display, subwindow->render_target,
                        subwindow->render_target, contexts[type]))
        ReportError(egl_window_made_current_failure);
}

void UnbindEGLContext(subwindow_t* subwindow, requested_window_t type)
{
    if (subwindow == NULL || subwindow->_eglwin == NULL ||
        subwindow->render_target == NULL || type == backdrop)
        return;

    wl_egl_window_destroy(subwindow->_eglwin);
    eglDestroySurface(display, subwindow->render_target);
    subwindow->_eglwin = NULL;
    subwindow->render_target = NULL;

    eglDestroyContext(display, contexts[type]);
    contexts[type] = NULL;
}

void ResizeEGLRenderingArea(subwindow_t* subwindow,
                            requested_window_t type)
{
    if (subwindow == NULL || subwindow->_eglwin == NULL ||
        type == backdrop)
        return;

    wl_egl_window_resize(subwindow->_eglwin, GetSubwindowWidth(type),
                         GetSubwindowHeight(type), 0, 0);
}

EGLDisplay GetEGLDisplay(void) { return display; }

EGLContext GetEGLContext(requested_window_t type)
{
    if (type == backdrop) return NULL;
    return contexts[type];
}
