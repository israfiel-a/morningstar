/**
 * @file Time.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides functionality to retrieve all sorts of time/timing
 * information.
 * @date 2024-07-29
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_TIME_DIAGNOSTIC_SYSTEM_
#define _MSENG_TIME_DIAGNOSTIC_SYSTEM_

#include <Master.h>

#define NSEC_TO_MSEC(value) value / 1000000
uint64_t GetCurrentTime(void);

void GetTimeString(char* buffer, size_t buffer_length);

#endif // _MSENG_TIME_DIAGNOSTIC_SYSTEM_
