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

// The master include file for the project.
#include <Master.h>

/**
 * @brief A wrapped pointer. Includes the size of the pointer, alongside
 * the pointer itself.
 */
typedef struct
{
    /**
     * @brief An ambiguous pointer to an ambiguous location at memory.
     */
    void* inner;
    /**
     * @brief The size of the allocated pointer. Editing this will likely
     * cause problems as the interface either reads data that doesn't
     * exist, or cuts off data that @b does exist.
     */
    size_t size;
} ptr_t;

/**
 * @brief An empty wrapped pointer initializer.
 */
#define WRAPPED_POINTER_EMPTY                                             \
    (ptr_t) { NULL, 0 }

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
void CopyBlock(ptr_t* dest, const ptr_t src);

/**
 * @brief Shrink the given pointer's usable section of memory. This is not
 * guaranteed to keep the data inside the pointer. For that behavior, check
 * out @ref ShrinkBlockSafe. If the passed-in size is greater than or equal
 * to the pointer's current size, nothing is done.
 * @param ptr The pointer to shrink.
 * @param new_size The new size of the pointer.
 */
void ShrinkBlock(ptr_t* ptr, size_t new_size);

/**
 * @brief Grow the given pointer's usable section of memory. This is not
 * guaranteed to keep the data inside the pointer. For that behavior, check
 * out @ref ExpandBlockSafe. If the passed-in size is less than or equal
 * to the pointer's current size, nothing is done.
 * @param ptr The pointer to shrink.
 * @param new_size The new size of the pointer.
 */
void ExpandBlock(ptr_t* ptr, size_t new_size);

/**
 * @brief Resize the given pointer's usable section of memory. This,
 * depending on the size passed to the function, either calls @ref
 * ShrinkBlock or @ref ExpandBlock. If the size is equal to the given
 * pointer's current size, nothing is done.
 * @param ptr The pointer to be resized.
 * @param new_size The requested new size of the pointer.
 */
void ReallocateBlock(ptr_t* ptr, size_t new_size);

/**
 * @brief Shrink the given pointer's usable section of memory. This is
 * guaranteed to keep the data inside the pointer. If the passed-in size is
 * greater than or equal to the pointer's current size, nothing is done.
 * @param ptr The pointer to shrink.
 * @param new_size The new size of the pointer.
 */
void ShrinkBlockSafe(ptr_t* ptr, size_t new_size);

/**
 * @brief Grow the given pointer's usable section of memory. This is
 * guaranteed to keep the data inside the pointer. If the passed-in size is
 * less than or equal to the pointer's current size, nothing is done.
 * @param ptr The pointer to shrink.
 * @param new_size The new size of the pointer.
 */
void ExpandBlockSafe(ptr_t* ptr, size_t new_size);

/**
 * @brief Resize the given pointer's usable section of memory. This,
 * depending on the size passed to the function, either calls @ref
 * ShrinkBlockSafe or @ref ExpandBlockSafe. If the size is equal to the
 * given pointer's current size, nothing is done.
 * @param ptr The pointer to be resized.
 * @param new_size The requested new size of the pointer.
 */
void ReallocateBlockSafe(ptr_t* ptr, size_t new_size);

/**
 * @brief Set the contents of the given pointer to the given value.
 * @param ptr The pointer to set.
 * @param contents The new contents of the pointer.
 */
static inline void SetBlockContents(ptr_t* ptr, void* contents)
{
    if (contents == NULL) return;
    memcpy(ptr->inner, contents, ptr->size);
}

#endif // _MSENG_ALLOCATE_MEMORY_SYSTEM_
