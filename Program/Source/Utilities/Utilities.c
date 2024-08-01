#include "Memory.h"
#include <Output/Error.h>
#include <fcntl.h>
#include <sys/mman.h>

int32_t simple_round(double x)
{
    if (x < 0.0) return (int32_t)(x - 0.5);
    else return (int32_t)(x + 0.5);
}

void FillMemory64(void* dest, uint64_t value, uintptr_t size)
{
    // Some bizzare code stolen from
    // https://stackoverflow.com/questions/108866/is-there-memset-that-accepts-integers-larger-than-char
    uintptr_t i;
    for (i = 0; i < (size & (~7)); i += 8)
        memcpy(((char*)dest) + i, &value, 8);
    for (; i < size; i++) ((char*)dest)[i] = ((char*)&value)[i & 7];
}
void FillMemory32(void* dest, uint32_t value, uintptr_t size)
{
    // Some bizzare code stolen from
    // https://stackoverflow.com/questions/108866/is-there-memset-that-accepts-integers-larger-than-char
    uintptr_t i;
    for (i = 0; i < (size & (~7)); i += 4)
        memcpy(((char*)dest) + i, &value, 4);
    for (; i < size; i++) ((char*)dest)[i] = ((char*)&value)[i & 7];
}
