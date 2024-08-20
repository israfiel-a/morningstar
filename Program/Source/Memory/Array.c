#include "Array.h"
#include <Output/System.h>
#include <stdio.h>
#include <stdlib.h>

array_t CreateArray(size_t size)
{
    array_t created_array = {NULL, size, 0};
    created_array._a = calloc(size, sizeof(ptr_t));
    if (created_array._a == NULL) ReportError(allocation_failure);
    created_array.occupied = -1;
    return created_array;
}

void DestroyArray(array_t* array)
{
    for (size_t i = 0; i < array->occupied; i++) FreeBlock(&array->_a[i]);
    free(array->_a);

    array->_a = NULL;
    array->occupied = -1;
    array->size = 0;
}

bool CheckArrayValidity(array_t array)
{
    return array.occupied != -1 && array.size != 0 && array._a != NULL;
}

void ResizeArray(array_t* array, size_t new_size)
{
    if (array->size == new_size)
    {
        ReportWarning(same_block_size);
        return;
    }

    if (array->occupied > new_size)
    {
        ReportWarning(array_implicit_data_free);
        for (size_t i = array->occupied; i > new_size; i--)
            FreeBlock(&array->_a[i]);
        array->occupied = new_size;
    }

    array->_a = realloc(array->_a, new_size);
    array->size = new_size;
}

void AddArrayValue(array_t* array, ptr_t value)
{
    if (array->_a == NULL)
    {
        ReportWarning(null_array_push);
        return;
    }

    array->occupied++;
    if (array->occupied > array->size)
    {
        ReportWarning(array_implicit_resize);
        ResizeArray(array, array->occupied);
    }

    array->_a[array->occupied] = AllocateBlock(value.size);
    CopyBlock(&array->_a[array->occupied], value);
}
