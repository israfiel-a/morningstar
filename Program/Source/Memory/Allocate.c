#include "Allocate.h"
#include <Output/System.h> // Error and warning reporting
#include <stdlib.h>        // Malloc, free, etc.

ptr_t AllocateBlock(size_t size)
{
    ptr_t created_ptr = {NULL, size};
    created_ptr._p = malloc(size);
    if (created_ptr._p == NULL) ReportError(allocation_failure);

    return created_ptr;
}

ptr_t AllocateZeroedBlock(size_t size)
{
    ptr_t created_ptr = {NULL, size};
    created_ptr._p = calloc(1, size);
    if (created_ptr._p == NULL) ReportError(allocation_failure);

    return created_ptr;
}

void FreeBlock(ptr_t* ptr)
{
    if (ptr == NULL || ptr->_p == NULL) ReportError(free_failure);
    free(ptr->_p);

    ptr->_p = NULL;
    ptr->size = 0;
}

void CopyBlock(ptr_t* dest, ptr_t src)
{
    if (dest == NULL || dest->_p == src._p || src._p == NULL)
    {
        ReportWarning(invalid_block_copy);
        return;
    }

    if (dest->_p != NULL) FreeBlock(dest);
    *dest = AllocateBlock(src.size);
    SetBlockContents(dest, src._p, src.size);
}

void ReallocateBlock(ptr_t* ptr, size_t new_size)
{
    if (new_size == ptr->size)
    {
        ReportWarning(same_block_size);
        return;
    }

    ptr->_p = realloc(ptr->_p, new_size);
    if (errno == ENOMEM) ReportError(allocation_failure);
    ptr->size = new_size;
}

void SetBlockContents(ptr_t* ptr, const void* content, size_t content_size)
{
    if (content == NULL)
    {
        ReportWarning(null_block_set);
        return;
    }

    if (ptr->size < content_size) ReportError(memory_bound_mismatch);
    memcpy(ptr->_p, content, content_size);
}

bool CheckBlockNull(const ptr_t ptr) { return ptr._p == NULL; }
