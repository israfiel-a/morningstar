#include "Session.h"
#include <Diagnostic/Time.h>
#include <Input/Terminal.h>
#include <Windowing/Manager.h>

globals_t global_flags = {true, false,        false, false,
                          0,    paneled_full, light, nothing};

void BeginSession(int argument_count, char** arguments)
{
    (void)GetCurrentTime(); // Start the clock.
    if (isatty(STDOUT_FILENO)) global_flags.stdout_available = true;
    HandleCommandLineArgs(argument_count, arguments);

    SetupWayland();
    while (CheckSessionValidity())
    {
        // Run the application.
    }
}

void CleanupSession(void) { DestroyWayland(); }
