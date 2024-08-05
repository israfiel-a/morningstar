#include "Session.h"
#include <Diagnostic/Time.h>
#include <Input/Terminal.h>
#include <Windowing/System.h>

globals_t global_flags = {true, false,        false, false,
                          0,    paneled_full, light, nothing};

bool CheckSessionValidity(void)
{
    // Check to make sure the display isn't fucked and that the application
    // should still be running.
    return CheckDisplayServer() && global_flags.application_running;
}

void BeginSession(int argument_count, char** arguments)
{
    (void)GetCurrentTime(); // Start the clock.
    if (isatty(STDOUT_FILENO)) global_flags.stdout_available = true;
    HandleCommandLineArgs(argument_count, arguments);

    SetupWindowingSystem();
    while (CheckSessionValidity())
    {
        // Run the application.
    }
}

void CleanupSession(void) { DestroyWindowingSystem(); }
