#include <Memory/Allocate.h>
#include <Session.h>
#include <malloc.h>

int main(int argc, char** argv)
{
    ptr_t ptr = AllocateZeroedBlock(150);

    int i = 340;
    SetBlockContents(&ptr, &i);
    printf("\n%d\n", *(int*)ptr.inner);

    ptr_t new_ptr = WRAPPED_POINTER_EMPTY;
    CopyBlock(&new_ptr, ptr);
    printf("\n%d\n", *(int*)new_ptr.inner);

    FreeBlock(&ptr);
    FreeBlock(&new_ptr);

    BeginSession(argc, argv);
    CleanupSession();
}
