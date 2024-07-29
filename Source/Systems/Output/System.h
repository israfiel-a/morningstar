/**
 * @file System.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides some functions to deal with system calls.
 * @date 2024-07-29
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_SYSTEM_OUTPUT_SYSTEM_
#define _MSENG_SYSTEM_OUTPUT_SYSTEM_

#include <Master.h>

inline static int32_t GetSyscallReturnValue(int32_t value)
{
    if (!WIFEXITED(value)) exit(EXIT_FAILURE);
    return WEXITSTATUS(value);
}

int32_t SystemCall(const char* command);

#endif // _MSENG_SYSTEM_OUTPUT_SYSTEM_
