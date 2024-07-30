#include <Diagnostic/Time.h>
#include <Globals.h>
#include <Input/Terminal.h>
#include <Windowing/Client.h>

int main(int argc, char** argv)
{
    (void)GetCurrentTime(); // Start the clock.
    if (isatty(STDOUT_FILENO)) global_flags.stdout_available = true;
    HandleCommandLineArgs(argc, argv);

    SetupWayland();
    DestroyWayland();
    return 0;
}
