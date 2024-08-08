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

#define min(x, y) (x < y ? x : y)
#define max(x, y) (x > y ? x : y)

// note this only works with certain numbers
int32_t simple_round(double x);

inline static uint32_t GetMiddle(uint32_t first, uint32_t object)
{
    return (first - object) / 2;
}

#endif // _MSENG_MATH_UTILITIES_
