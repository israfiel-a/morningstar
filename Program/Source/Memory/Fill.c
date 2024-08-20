#include "Fill.h"

void ZeroBlock(ptr_t* ptr) { memset(ptr->_p, 0, ptr->size); }
void FillBlock(ptr_t* ptr, uint32_t value, size_t size)
{
    size_t i = 0;
    for (; i < (size & (~7)); i += 4) memcpy(((char*)ptr) + i, &value, 4);
    for (; i < size; i++) ((char*)ptr)[i] = ((char*)&value)[i & 7];
}
