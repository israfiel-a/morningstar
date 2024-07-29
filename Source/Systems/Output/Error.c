#include "Error.h"
#include "Messages.h"
#include <Globals.h>

extern globals_t global_flags;

/**
 * @brief An array of explanations to go along with each error code. Every
 * error must have an explanation.
 */
static const char* error_explanations[] = {
    [memory_map_failure] = "failure to map memory",
    [memory_unmap_failure] = "failure to unmap memory",
    [shm_open_failure] = "shared memory file open failure",
    [child_process_error] = "child process error",
    [subshell_error] = "fatal subshell opening error",
    [wayland_display_fail] = "failed to connect to wayland display",
    [wayland_server_processing_fail] = "wayland server processing error",
    [wayland_missing_features] = "wayland server missing features"};

_Noreturn void ReportError_(const char* file, const char* function,
                            uint64_t line, error_t code, bool override)
{
    if (global_flags.stdout_available || override)
        fprintf(stderr,
                "\n\033[31m" ID " has run into a fatal "
                "error.\nLocation: %s() @ %s, ln. %lu\nCode: %d :: "
                "%s\n\n\033[0m",
                function, file, line, code, error_explanations[code]);
    else
        LogNotification(ID " Error Reporter",
                        ID " has run into a fatal error. Location: %s() @ "
                           "%s, ln. %lu. Code: %d :: %s.",
                        function, file, line, code);

    exit(EXIT_FAILURE);
}
