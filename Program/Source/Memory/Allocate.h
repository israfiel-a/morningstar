/**
 * @file Allocate.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides utilities for safe(r) allocation of memory.
 * @date 2024-08-08
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_ALLOCATE_MEMORY_SYSTEM_
#define _MSENG_ALLOCATE_MEMORY_SYSTEM_

#include <inttypes.h> // Fixed-width integers
#include <memory.h>   // Memory functions like memcpy
#include <stdbool.h>  // Boolean values

/**
 * @brief A wrapped pointer. Includes the size of the pointer, alongside
 * the pointer itself.
 */
typedef struct
{
    /**
     * @brief An ambiguous pointer to an ambiguous location at memory.
     */
    void* _p;
    /**
     * @brief The size of the associated pointer. Editing this will likely
     * cause problems as the interface either reads data that doesn't
     * exist, or cuts off data that @b does exist.
     */
    size_t size;
} ptr_t;

/**
 * @brief Allocate a block of @param size. The block is initialized to
 * nothing, and will be (probably) filled with garbage data.
 * @param size Size of the allocated block.
 * @return The pointer created, alongside its assigned size.
 */
ptr_t AllocateBlock(size_t size);

/**
 * @brief Much the same as @ref AllocateBlock, allocate a block of @param
 * size. This block, however, is fully initialized to the value 0.
 * @param size The size of the allocated block.
 * @return The pointer created, alongside its assigned size.
 */
ptr_t AllocateZeroedBlock(size_t size);

/**
 * @brief Free the given pointer. If this is passed a NULL pointer, the
 * fatal @enum memory_free_failure exception is raised. The pointer
 * associated with the given @struct ptr_t object is set to NULL, and the
 * size is set to 0.
 * @param ptr The pointer to be freed.
 */
void FreeBlock(ptr_t* ptr);

/**
 * @brief Copy the contents and size of a wrapped pointer to the shell of
 * another. @param dest is overwritten during this operation, but no
 * changes are made to @param src. If the two pointers are the same, if
 * either are NULL, or if the contents of @param src is NULL, nothing is
 * done.
 * @param dest The destination pointer, where things should be copied to.
 * @param src The source pointer, where things are to be copied from.
 */
void CopyBlock(ptr_t* dest, ptr_t src);

/**
 * @brief Re-allocate the size of the given pointer. The contents of the
 * pointer are the same up until the boundary of the new memory section.
 * @param ptr The pointer to reallocate.
 * @param new_size The requested new size of the block.
 */
void ReallocateBlock(ptr_t* ptr, size_t new_size);

/**
 * @brief Set the contents of the given pointer to the given value.
 * @param ptr The pointer to set.
 * @param contents The new contents of the pointer.
 */
void SetBlockContents(ptr_t* ptr, const void* content,
                      size_t content_size);

/**
 * @brief Check if the given block is NULL.
 * @param ptr The block to check.
 * @return true The block is NULL.
 * @return false The block is not NULL.
 */
bool CheckBlockNull(const ptr_t ptr);

#endif // _MSENG_ALLOCATE_MEMORY_SYSTEM_
