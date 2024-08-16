#include "Loop.h"
#include "System.h"
#include <GLAD/opengl.h>  // OpenGL function prototypes
#include <Output/Error.h> // Error reporting

void draw(panel_t* panel)
{
    // If the current surface is not this window, bind it.
    if (eglGetCurrentSurface(EGL_READ) != panel->_rt)
    {
        EGLBoolean made_current =
            eglMakeCurrent(GetEGLDisplay(), panel->_rt, panel->_rt,
                           GetEGLContext(panel->type));
        if (!made_current) ReportError(egl_window_made_current_failure);
    }

    // Fill the windows with a background color.
    if (panel->type == center_filler) glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    else glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    // Clear the color buffer and force all events to be done.
    glClear(GL_COLOR_BUFFER_BIT), glFlush();

    if (!eglSwapBuffers(GetEGLDisplay(), panel->_rt))
        ReportError(egl_swap_buffer_failure);
}
