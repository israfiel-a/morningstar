#include <Globals.h>
#include <Input/Terminal.h>
#include <Wayland/Client.h>

extern globals_t global_flags;

int main(int argc, char** argv)
{
    if (isatty(STDOUT_FILENO)) global_flags.stdout_available = true;
    HandleCommandLineArgs(argc, argv);

    SetupWayland();
    DestroyWayland();
    return 0;
}
