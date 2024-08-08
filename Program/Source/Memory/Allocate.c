#include "Allocate.h"
#include <Output/Error.h> // Error reporting

ptr_t AllocateBlock(size_t size)
{
    ptr_t created_ptr = {NULL, size};
    created_ptr.inner = malloc(size);
    if (created_ptr.inner == NULL)
        ReportError(memory_allocation_failure, false);

    return created_ptr;
}

ptr_t AllocateZeroedBlock(size_t size)
{
    ptr_t created_ptr = {NULL, size};
    created_ptr.inner = calloc(1, size);
    if (created_ptr.inner == NULL)
        ReportError(memory_allocation_failure, false);

    return created_ptr;
}

void FreeBlock(ptr_t* ptr)
{
    if (ptr == NULL || ptr->inner == NULL)
        ReportError(memory_free_failure, false);
    free(ptr->inner);

    ptr->inner = NULL;
    ptr->size = 0;
}

void CopyBlock(ptr_t* dest, const ptr_t src)
{
    if (dest == NULL || dest->inner == src.inner) return;
    if (src.inner == NULL) return;

    if (dest->inner != NULL) FreeBlock(dest);
    *dest = AllocateBlock(src.size);
    SetBlockContents(dest, src.inner);
}

static void ChangeBlockSize(ptr_t* ptr, size_t new_size)
{
    if (ptr->inner != NULL) FreeBlock(ptr);
    *ptr = AllocateBlock(new_size);
}

void ShrinkBlock(ptr_t* ptr, size_t new_size)
{
    if (new_size >= ptr->size) return;
    ChangeBlockSize(ptr, new_size);
}

void ExpandBlock(ptr_t* ptr, size_t new_size)
{
    if (new_size <= ptr->size) return;
    ChangeBlockSize(ptr, new_size);
}

void ReallocateBlock(ptr_t* ptr, size_t new_size)
{
    if (new_size == ptr->size) return;
    if (new_size > ptr->size) ExpandBlock(ptr, new_size);
    else ShrinkBlock(ptr, new_size);
}

static void ChangeBlockSizeSafe(ptr_t* ptr, size_t new_size)
{
    ptr_t contents = AllocateBlock(new_size);
    if (ptr->inner != NULL)
    {
        SetBlockContents(&contents, ptr->inner);
        FreeBlock(ptr);
    }
    *ptr = AllocateBlock(new_size);
    SetBlockContents(ptr, contents.inner);
}

void ShrinkBlockSafe(ptr_t* ptr, size_t new_size)
{
    if (new_size >= ptr->size) return;
    ChangeBlockSizeSafe(ptr, new_size);
}

void ExpandBlockSafe(ptr_t* ptr, size_t new_size)
{
    if (new_size <= ptr->size) return;
    ChangeBlockSizeSafe(ptr, new_size);
}

void ReallocateBlockSafe(ptr_t* ptr, size_t new_size)
{
    if (new_size == ptr->size) return;
    if (new_size > ptr->size) ExpandBlockSafe(ptr, new_size);
    else ShrinkBlockSafe(ptr, new_size);
}
