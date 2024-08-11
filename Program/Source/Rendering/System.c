#include "System.h"
#include <GLAD/opengl.h>
#include <Output/Error.h>
#include <Session.h>
#include <Windowing/System.h>

static const EGLint ca[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};

static EGLDisplay display = NULL;
static EGLConfig config = NULL;
static EGLContext* contexts[backdrop];

void SetupEGL(void)
{
    EGLint major, minor;
    display = eglGetDisplay((EGLNativeDisplayType)GetDisplay());
    if (display == NULL) ReportError(egl_display_connect_failure);
    if (!eglInitialize(display, &major, &minor))
        ReportError(egl_initialization_failure);

    EGLint n, config_attribs[] = EGL_CONFIG_ATTRIBUTES;
    if (!eglChooseConfig(display, config_attribs, &config, 1, &n))
        ReportError(egl_config_failure);

    for (int i = 0; i < backdrop; i++)
    {
        contexts[i] = eglCreateContext(display, config, NULL, ca);
        if (contexts[i] == NULL) ReportError(egl_context_create_failure);
    }
}

void DestroyEGL(void)
{
    for (int i = 0; i < backdrop; i++)
        eglDestroyContext(display, contexts[i]);
    eglTerminate(display);
    eglReleaseThread();
}

void BindEGLContext(subwindow_t* subwindow, requested_window_t type)
{
    subwindow->_eglwin =
        wl_egl_window_create(subwindow->_win, GetSubwindowWidth(type),
                             GetSubwindowHeight(type));
    if (subwindow->_eglwin == NULL) ReportError(egl_window_create_failure);

    subwindow->render_target = eglCreateWindowSurface(
        display, config, (EGLNativeWindowType)subwindow->_eglwin, NULL);

    if (!eglMakeCurrent(display, subwindow->render_target,
                        subwindow->render_target, contexts[type]))
        ReportError(egl_window_made_current_failure);
}

void draw(subwindow_t* subwindow, requested_window_t type)
{
    if (eglGetCurrentSurface(EGL_DRAW) != subwindow->render_target ||
        eglGetCurrentSurface(EGL_READ) != subwindow->render_target)
    {
        EGLBoolean made_current =
            eglMakeCurrent(display, subwindow->render_target,
                           subwindow->render_target, contexts[type]);
        if (!made_current) ReportError(egl_window_made_current_failure);
    }

    if (type == gameplay) glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    else glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    if (!eglSwapBuffers(display, subwindow->render_target))
        ReportError(egl_swap_buffer_failure);
}
