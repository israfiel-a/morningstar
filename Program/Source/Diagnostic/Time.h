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

#include <inttypes.h>
#include <time.h>

/**
 * @brief A simple macro to convert nanoseconds to milliseconds. This is
 * literally just a division by 1000000.
 */
#define NSEC_TO_MSEC(value) (uint64_t) value / 1000000

/**
 * @brief Get the current time since the start of the application in
 * milliseconds. On first call, the function will always return 0.
 * @return The millisecond representation of the time.
 */
uint64_t GetCurrentTime(void);

/**
 * @brief Get a string-formatted version of the current time, in the format
 * of ms::s::m.
 * @param buffer The buffer to insert the string into. If the length of
 * this buffer is less than 13, the function will fail and set this value
 * to NULL.
 * @param buffer_length The length of the buffer.
 */
void GetTimeString(char* buffer, size_t buffer_length);

#endif // _MSENG_TIME_DIAGNOSTIC_SYSTEM_
