/**
 * @file Fill.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides utility for filling blocks of memory.
 * @date 2024-08-08
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_FILL_MEMORY_SYSTEM_
#define _MSENG_FILL_MEMORY_SYSTEM_

#include "Allocate.h"

void ZeroBlock(ptr_t* ptr);
void FillBlock(ptr_t* ptr, uint32_t value, size_t size);

#endif // _MSENG_FILL_MEMORY_SYSTEM_
