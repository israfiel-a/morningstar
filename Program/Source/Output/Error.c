#include "Error.h"
#include "Messages.h"
#include <Globals.h>
#include <stdio.h>
#include <stdlib.h>

// outline of new system (to be implemented):
// report function: (file, function, line, code)
// severity array: program_error, external_error, os_error
// override functionality based on severity
// descriptive error strings reported alongside code

typedef enum __attribute__((__packed__))
{
    program_error,
    external_error,
    os_error
} error_severity_t;

typedef struct
{
    error_severity_t severity;
    const char* message;
} error_t;

static const error_t errors[] = {
    [allocation_failure] = {os_error, "failed to allocate memory"},
    [free_failure] = {program_error, "null pointer passed to free"},
    [memory_bound_mismatch] = {os_error,
                               "smaller area to copy into than copy size"},
    [mmap_failure] = {os_error, "failed to map memory"},
    [unmmap_failure] = {os_error, "failed to unmap memory"},
    [shm_open_failure] = {program_error, "failed to open shm file"},
    [child_open_failure] = {external_error,
                            "failed to open child process"},
    [subshell_open_failure] = {external_error, "failed to open subshell"},
    [display_connect_failure] = {program_error,
                                 "failed to connect to wayland display"},
    [server_processing_failure] = {external_error,
                                   "wayland processing failed"},
    [compositor_missing_features] =
        {external_error, "wayland compositor missing features"},
    [monitor_measure_failure] = {external_error,
                                 "xdg-shell failed to get monitor size"},
    [time_get_failure] = {program_error, "failed to get time"},
    [egl_display_connect_failure] = {external_error,
                                     "failed to get EGL display"},
    [egl_bad_display] = {program_error, "bad EGL display value"},
    [egl_initialization_failure] = {program_error,
                                    "failed to initialize EGL"},
    [egl_config_failure] = {program_error, "failed to configure EGL"},
    [egl_context_create_failure] = {program_error,
                                    "failed to create EGL context"},
    [egl_context_bind_failure] = {external_error,
                                  "failed to bind EGL context"},
    [egl_window_made_current_failure] =
        {program_error, "failed to make egl window current"},
    [egl_surface_create_failure] = {program_error,
                                    "failed to create EGL surface"},
    [egl_swap_buffer_failure] =
        {program_error,
         "failed to swap buffers -- failed to prevent hanging"},
    [opengl_api_bind_failure] = {program_error,
                                 "failed to bind the OpenGL api"},
    [opengl_shader_creation_failure] = {
        program_error, "failed to create an opengl shader object"}};

_Noreturn void ReportError_(const char* file, const char* function,
                            uint64_t line, error_code_t code)
{
    error_t err = errors[code];
    if (global_flags.stdout_available || err.severity != program_error)
    {
        fprintf(stderr,
                "\n\033[1m\033[31m-- Morningstar Error Reporter "
                "--\n-- Fatal Error Reported.\n-- "
                "Location:\033[0m\033[31m %s() at %s, ln. %lu\n\033[1m-- "
                "Severity:\033[0m\033[31m %u\n\033[1m-- "
                "Description:\033[0m\033[31m cd. %d, %s\033[0m\n\n",
                function, file, line, err.severity, code, err.message);
    }
    else
    {
        LogNotification("Morningstar Error Reporter",
                        ID " has run into a fatal error. Location: %s() @ "
                           "%s, ln. %lu. Code: %d :: %s.",
                        function, file, line, code);
    }

    exit(EXIT_FAILURE);
}
