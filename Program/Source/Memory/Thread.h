/**
 * @file Thread.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Some utilities for thread creation and management.
 * @date 2024-08-20
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_THREAD_MEMORY_SYSTEM_
#define _MSENG_THREAD_MEMORY_SYSTEM_

#include <pthread.h>

const pthread_t CreateThread(void* (*func)(void*), void* args);

#endif // _MSENG_THREAD_MEMORY_SYSTEM_
