#include <Memory/Allocate.h>
#include <Session.h>

int main(int argc, char** argv)
{
    BeginSession(argc, argv);
    CleanupSession();
}
