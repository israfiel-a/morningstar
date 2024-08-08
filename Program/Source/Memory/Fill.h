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
#include <Master.h>

void FillMemory64(void* dest, uint64_t value, uintptr_t size);
void FillMemory32(void* dest, uint32_t value, uintptr_t size);

void ZeroBlock(ptr_t* ptr);

#endif // _MSENG_FILL_MEMORY_SYSTEM_
