#include "System.h"
#include "Error.h"

int32_t SystemCall(const char* command)
{
    int32_t given_system_value = system(command);
    if (given_system_value == -1) ReportError(child_process_error, true);

    int32_t child_return_value = GetSyscallReturnValue(given_system_value);
    if (child_return_value == 127) ReportError(subshell_error, true);
    return child_return_value;
}
