/**
 * @file Array.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief This file provides an easier-to-work-with abstraction over the
 * basic C-style array. Helper functions and all necessary dependencies are
 * included.
 * @date 2024-08-17
 *
 * @copyright (c) 2024 - Israfiel
 * This document is under the Creative Commons Zero 1.0 License.
 * It, and all of its contents, are licensed to be used in a creative,
 * commercial, or educational environment. You may modify or distribute
 * this file as you wish, however you may @b not use it in a trademark or
 * patent context. For more information, see the @file LICENSE.md file
 * attached to your distribution of the program.
 */

#ifndef _MSENG_ARRAY_
#define _MSENG_ARRAY_

#include "Allocate.h"
#include <inttypes.h>

/**
 * @brief A slightly easier-to-work-with abstraction over the C standard
 * array type. The contents of the array do not necessarily have to be one
 * size, although it is reccomended for sanity's sake.
 */
typedef struct
{
    /**
     * @brief The inner array of the, well, array. @warning Editing this
     * manually in any way will likely have unforseen consequences, some of
     * which may generate fatal errors.
     */
    ptr_t* _a;
    /**
     * @brief The total allocated size of the array. This does not
     * necessarily correlate with the @ref occupied value.
     */
    size_t size;
    /**
     * @brief The amount of slots occupied within the array. This is a
     * count of @b slots, not @b bytes.
     */
    size_t occupied;
} array_t;

array_t CreateArray(size_t size);

void DestroyArray(array_t* array);

bool CheckArrayValidity(array_t array);

void ResizeArray(array_t* array, size_t new_size);

void AddArrayValue(array_t* array, ptr_t value);
void AddArrayValueRaw(array_t* array, void* value, size_t size);

ptr_t* GetArrayValue(array_t array, size_t index);

ptr_t* GetArrayTail(array_t array);

#endif // _MSENG_ARRAY_
