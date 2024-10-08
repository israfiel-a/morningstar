#include "System.h"
#include "Error.h"
#include <stdlib.h>

inline static int32_t GetSyscallReturnValue(int32_t value)
{
    if (!WIFEXITED(value)) exit(EXIT_FAILURE);
    return WEXITSTATUS(value);
}

int32_t SystemCall(const char* command)
{
    int32_t given_system_value = system(command);
    if (given_system_value == -1) ReportError(child_open_failure);

    int32_t child_return_value = GetSyscallReturnValue(given_system_value);
    if (child_return_value == 127) ReportError(subshell_open_failure);
    return child_return_value;
}
