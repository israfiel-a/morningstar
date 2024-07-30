/**
 * @file SharedMemory.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Utilities to create a shared memory file. Uses some public domain
 * code snippets from https://wayland-book.com/surfaces/shared-memory.html.
 * @date 2024-07-26
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_SHARED_MEMORY_UTILITIES_
#define _MSENG_SHARED_MEMORY_UTILITIES_

#include <Master.h>

void CreateRandomFileName(char* filename_buffer);
int OpenSHM(size_t size);

void FillMemory64(void* dest, uint64_t value, uintptr_t size);
void FillMemory32(void* dest, uint32_t value, uintptr_t size);

#endif // _MSENG_SHARED_MEMORY_UTILITIES_
