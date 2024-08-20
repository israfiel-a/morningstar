/**
 * @file Warning.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides an in-application @def errno functionality, for catching
 * warnings set by functions. This is incredibly useful for catching errors
 * before they arise.
 * @date 2024-08-13
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_WARNING_OUTPUT_SYSTEM_
#define _MSENG_WARNING_OUTPUT_SYSTEM_

typedef enum
{
    no_warning,

    invalid_block_copy,
    same_block_size,
    null_block_set,
    null_array_push,
    array_implicit_resize,
    array_implicit_data_free,

    double_display_setup,

    preemptive_egl_setup,
    double_egl_setup,
    preemptive_egl_free,

    preemptive_egl_context_free,
    unknown_egl_context,

    preemptive_wm_creation,
    double_wm_creation,

    preemptive_window_creation,
    double_window_creation,
    preemptive_window_free,
    double_window_free,

    implicit_panel_array_creation,
    preemptive_panel_creation,
    double_panel_creation,
    preemptive_panel_free,
    double_panel_free,

    preemptive_window_unwrapping,
    double_window_unwrapping,

    preemptive_window_title_set,
    invalid_title_value,

    preemptive_seat_creation,
    double_seat_creation,
    preemptive_seat_free,

    preemptive_shm_creation,
    double_shm_creation,
    preemptive_shm_free
} warning_code_t;

typedef struct
{
    const char* setter;
    warning_code_t code;
} warning_t;

void ReportWarning_(const char* function, warning_code_t code);

#define ReportWarning(code) ReportWarning_(__func__, code)

const warning_t* ReadWarning(void);

#endif // _MSENG_WARNING_OUTPUT_SYSTEM_
