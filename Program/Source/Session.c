#include "Session.h"
#include <Diagnostic/Time.h>  // Time functionality
#include <Input/File.h>       // Terminal argument parsing
#include <Windowing/System.h> // Display server setup

globals_t global_flags = {true, false, {0, 0, 0, 0}, paneled_full, full};

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

bool CheckSessionValidity(void)
{
    // Check to make sure the display isn't fucked and that the application
    // should still be running.
    CheckDisplayServer();
    return running;
}
