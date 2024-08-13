#include "Loop.h"
#include "System.h"
#include <GLAD/opengl.h>  // OpenGL function prototypes
#include <Output/Error.h> // Error reporting

void draw(subwindow_t* subwindow, requested_window_t type)
{
    // If the current surface is not this window, bind it.
    if (eglGetCurrentSurface(EGL_READ) != subwindow->render_target)
    {
        EGLBoolean made_current =
            eglMakeCurrent(GetEGLDisplay(), subwindow->render_target,
                           subwindow->render_target, GetEGLContext(type));
        if (!made_current) ReportError(egl_window_made_current_failure);
    }

    // Fill the windows with a background color.
    if (type == gameplay) glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    else glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    // Clear the color buffer and force all events to be done.
    glClear(GL_COLOR_BUFFER_BIT), glFlush();

    if (!eglSwapBuffers(GetEGLDisplay(), subwindow->render_target))
        ReportError(egl_swap_buffer_failure);
}
