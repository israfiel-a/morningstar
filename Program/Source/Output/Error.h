/**
 * @file Error.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief The error reporter interface of the application.
 * @date 2024-07-24
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_ERROR_OUTPUT_SYSTEM_
#define _MSENG_ERROR_OUTPUT_SYSTEM_

// The master include file for the project.
#include <Master.h>

typedef enum
{
    allocation_failure,
    free_failure,
    memory_bound_mismatch,
    mmap_failure,
    unmmap_failure,
    shm_open_failure,
    child_open_failure,
    subshell_open_failure,
    display_connect_failure,
    server_processing_failure,
    compositor_missing_features,
    monitor_measure_failure,
    time_get_failure,
    egl_display_connect_failure,
    egl_bad_display,
    egl_initialization_failure,
    egl_config_failure,
    egl_context_create_failure,
    egl_context_bind_failure,
    egl_window_made_current_failure,
    egl_window_create_failure,
    egl_swap_buffer_failure,
    opengl_api_bind_failure,
    opengl_shader_creation_failure
} error_code_t;

_Noreturn void ReportError_(const char* file, const char* function,
                            uint64_t line, error_code_t code);

/**
 * @brief A macro to make the calling of the @ref ReportError_ function
 * much easier, by automatically filling in function, filename, and line
 * values.
 */
#define ReportError(code) ReportError_(FILENAME, __func__, __LINE__, code)

#endif // _MSENG_ERROR_OUTPUT_SYSTEM_
