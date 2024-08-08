#include "Session.h"
#include <Diagnostic/Time.h>
#include <Input/Terminal.h>
#include <Windowing/System.h>

globals_t global_flags = {true, false,        false,        false,
                          0,    {0, 0, 0, 0}, paneled_full, full};

bool CheckSessionValidity(void)
{
    // Check to make sure the display isn't fucked and that the application
    // should still be running.
    CheckDisplayServer();
    return global_flags.application_running;
}

void BeginSession(int argument_count, char** arguments)
{
    (void)GetCurrentTime(); // Start the clock.
    if (isatty(STDOUT_FILENO)) global_flags.stdout_available = true;
    HandleCommandLineArgs(argument_count, arguments);

    SetupDisplayServer();
    while (CheckSessionValidity())
    {
        // Run the application.
    }
}

void CleanupSession(void) { EndDisplayServer(); }
