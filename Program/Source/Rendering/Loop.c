#include "Loop.h"
#include "System.h"
#include <GLAD/opengl.h> // OpenGL function prototypes
#include <Globals.h>
#include <Memory/Thread.h>
#include <Output/Error.h> // Error reporting
#include <Windowing/Windowing.h>
#include <pthread.h>
#include <stdio.h>

static void draw(panel_t* panel, size_t panel_index)
{
    EGLContext context = CreateEGLContext(GetEGLContext(panel_index));
    if (context == NULL) ReportError(egl_context_create_failure);
    EGLBoolean made_current =
        eglMakeCurrent(GetEGLDisplay(), panel->_rt, panel->_rt, context);
    if (!made_current)
    {
        printf("\n%d\n", eglGetError());
        ReportError(egl_window_made_current_failure);
    }

    // Fill the windows with a background color.
    if (panel->type == center_filler) glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    else glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    // Clear the color buffer and force all events to be done.
    glClear(GL_COLOR_BUFFER_BIT), glFlush();

    if (!eglSwapBuffers(GetEGLDisplay(), panel->_rt))
        ReportError(egl_swap_buffer_failure);
}

static pthread_mutex_t render_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t render_cond = PTHREAD_COND_INITIALIZER;
static void* DrawFunction(void* data)
{
    pthread_mutex_lock(&render_mutex);
    while (running)
    {
        WaitForDimensionSignal_(&render_mutex);
        pthread_cond_wait(&render_cond, &render_mutex);
        pthread_cond_destroy(&render_cond);
        render_cond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;

        printf("\nrenderingloop\n");
        IteratePanels(draw);
    }
    pthread_mutex_unlock(&render_mutex);
    return NULL;
}

void CreateRenderingThread(void) { CreateThread(DrawFunction, NULL); }
void TriggerRender(void) { pthread_cond_signal(&render_cond); }
