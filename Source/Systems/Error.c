#include "Error.h"

const char* error_explanations[] = {
    [no_error] = "no error",
    [wayland_display_fail] = "failed to connect to wayland display",
    [wayland_server_processing_fail] = "wayland server processing error",
    [wayland_missing_features] = "wayland server missing features",
    [memory_map_failure] = "failure to map memory",
    [memory_unmap_failure] = "failure to unmap memory",
    [shm_open_failure] = "shared memory file open failure"};
static_assert(sizeof(error_explanations) / sizeof(*error_explanations) ==
                  error_count,
              "Not enough error strings.");

_Noreturn void InternalReportError(const char* file, const char* function,
                                   uint64_t line, error_t code)
{
    fprintf(stderr,
            "\n\033[31mConcrete Jungle has run into a fatal "
            "error.\nLocation: %s() @ %s, ln. %lu\nCode: %d :: "
            "%s\n\n\033[0m",
            function, file, line, code, error_explanations[code]);
    exit(EXIT_FAILURE);
}
