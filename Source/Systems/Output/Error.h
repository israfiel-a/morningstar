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

/**
 * @brief An enumerator storing the various errors that the program may
 * throw. Each error code corresponds to a string defined within @file
 * Error.c.
 */
typedef enum
{
    /**
     * @brief We failed to create a memory map.
     */
    memory_map_failure,
    /**
     * @brief We failed to un-map a memory map. It's likely a read error
     * occurred.
     */
    memory_unmap_failure,
    /**
     * @brief We couldn't open a shared memory file for whatever reason.
     */
    shm_open_failure,
    /**
     * @brief A child process failed to open for whatever reason.
     */
    child_process_error,
    /**
     * @brief A subshell was unable to be opened.
     */
    subshell_error,
    /**
     * @brief We failed to connect to a Wayland display device. The user is
     * probably not running a Wayland session.
     */
    wayland_display_fail,
    /**
     * @brief The Wayland server timed out while processing our app's
     * requests. Something is seriously wrong here--either with the program
     * or the user's machine, we can't really be sure.
     */
    wayland_server_processing_fail,
    /**
     * @brief The version of Wayland the user is running is missing
     * features integral to the program running.
     */
    wayland_missing_features
} error_t;

/**
 * @brief A function to end the process due to a fatal error. Nothing will
 * run after the point of this function's calling.
 * @param file The file in which the error occurred.
 * @param function The function in which the error occurred.
 * @param line The line of code on which the error occurred.
 * @param code The error code of the error.
 * @param override Whether or not to override the global flag and print to
 * stderr, even if the flag says not to.
 */
_Noreturn void ReportError_(const char* file, const char* function,
                            uint64_t line, error_t code, bool override);

/**
 * @brief A macro to make the calling of the @ref ReportError_ function
 * much easier, by automatically filling in function, filename, and line
 * values.
 */
#define ReportError(code, override)                                       \
    ReportError_(FILENAME, __func__, __LINE__, code, override)

#endif // _MSENG_ERROR_OUTPUT_SYSTEM_
