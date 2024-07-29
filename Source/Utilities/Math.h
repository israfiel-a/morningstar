/**
 * @file Math.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides some basic math functions that I need, so I don't have
 * to link the entire math library into the program for like two functions.
 * @date 2024-07-27
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_MATH_UTILITIES_
#define _MSENG_MATH_UTILITIES_

#include <Master.h>

#define min(x, y)                                                         \
    _Generic((x), uint32_t: uint32_min(x, y), int32_t: int32_min(x, y))
#define max(x, y)                                                         \
    _Generic((x), uint32_t: uint32_max(x, y), int32_t: int32_max(x, y))

inline static uint32_t uint32_min(uint32_t x, uint32_t y)
{
    return x < y ? x : y;
}

inline static uint32_t uint32_max(uint32_t x, uint32_t y)
{
    return x > y ? x : y;
}

inline static int32_t int32_min(int32_t x, int32_t y)
{
    return x < y ? x : y;
}

inline static int32_t int32_max(int32_t x, int32_t y)
{
    return x > y ? x : y;
}

// note this only works with certain numbers
int32_t simple_round(double x);

#endif // _MSENG_MATH_UTILITIES_
