#include <Memory/Allocate.h>
#include <Session.h>

int main(int argc, char** argv)
{
    ptr_t ptr = AllocateBlock(125);
    ShrinkBlockSafe(&ptr, 21);
    FreeBlock(&ptr);

    BeginSession(argc, argv);
    CleanupSession();
}
