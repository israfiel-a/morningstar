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

#include "Error.h"    // IWYU pragma: keep
#include "Messages.h" // IWYU pragma: keep
#include "Warning.h"  // IWYU pragma: keep

int32_t SystemCall(const char* command);

#endif // _MSENG_SYSTEM_OUTPUT_SYSTEM_
