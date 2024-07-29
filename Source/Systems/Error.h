/**
 * @file Error.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief The error reporter interface of the application.
 * @date 2024-07-24
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_ERROR_SYSTEM_
#define _MSENG_ERROR_SYSTEM_

#include <Master.h>

typedef enum
{
    no_error,
    wayland_display_fail,
    wayland_server_processing_fail,
    wayland_missing_features,
    memory_map_failure,
    memory_unmap_failure,
    shm_open_failure,
    error_count
} error_t;

#define ReportError(code)                                                 \
    InternalReportError(FILENAME, __func__, __LINE__, code)

_Noreturn void InternalReportError(const char* file, const char* function,
                                   uint64_t line, error_t code);

#endif // _MSENG_ERROR_SYSTEM
