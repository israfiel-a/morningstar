#include "System.h"
#include <GLAD/opengl.h>
#include <Output/Error.h>
#include <Session.h>
#include <Windowing/System.h>

static EGLDisplay egl_display = NULL;
static EGLConfig egl_config = NULL;
static EGLContext egl_context = NULL;
static EGLSurface egl_surface = NULL; //! temp location
static opengl_window_t* egl_window = NULL;

void SetupEGL(void)
{
    EGLint major, minor, count, n, size;
    EGLConfig* configs;
    int i;
    EGLint config_attribs[] = {EGL_SURFACE_TYPE,
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

    static const EGLint context_attribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2,
                                             EGL_NONE};

    egl_display = eglGetDisplay((EGLNativeDisplayType)GetDisplay());
    if (egl_display == EGL_NO_DISPLAY)
    {
        fprintf(stderr, "Can't create egl display\n");
        exit(1);
    }
    else { fprintf(stderr, "Created egl display\n"); }

    if (eglInitialize(egl_display, &major, &minor) != EGL_TRUE)
    {
        fprintf(stderr, "Can't initialise egl display\n");
        exit(1);
    }
    printf("EGL major: %d, minor %d\n", major, minor);

    eglGetConfigs(egl_display, NULL, 0, &count);
    printf("EGL has %d configs\n", count);

    configs = calloc(count, sizeof *configs);

    eglChooseConfig(egl_display, config_attribs, configs, count, &n);

    for (i = 0; i < n; i++)
    {
        eglGetConfigAttrib(egl_display, configs[i], EGL_BUFFER_SIZE,
                           &size);
        printf("Buffer size for config %d is %d\n", i, size);
        eglGetConfigAttrib(egl_display, configs[i], EGL_RED_SIZE, &size);
        printf("Red size for config %d is %d\n", i, size);

        // just choose the first one
        egl_config = configs[i];
        break;
    }

    egl_context = eglCreateContext(egl_display, egl_config, EGL_NO_CONTEXT,
                                   context_attribs);
    free(configs);
}

void DestroyEGL(void)
{
    wl_egl_window_destroy(egl_window);
    eglDestroyContext(egl_display, egl_context);
    eglTerminate(egl_display);
    eglReleaseThread();
}

void BindEGLContext(raw_window_t* window)
{
    egl_window =
        wl_egl_window_create(window, dimensions.width, dimensions.height);
    if (egl_window == EGL_NO_SURFACE)
    {
        fprintf(stderr, "Can't create egl window\n");
        exit(1);
    }
    else { fprintf(stderr, "Created egl window\n"); }

    egl_surface = eglCreateWindowSurface(
        egl_display, egl_config, (EGLNativeWindowType)egl_window, NULL);

    if (eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context))
    {
        fprintf(stderr, "Made current\n");
    }
    else { fprintf(stderr, "Made current failed\n"); }
}

void draw(void)
{
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    if (eglSwapBuffers(egl_display, egl_surface))
    {
        fprintf(stderr, "Swapped buffers\n");
    }
    else { fprintf(stderr, "Swapped buffers failed\n"); }
}
