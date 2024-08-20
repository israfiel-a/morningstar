#include "Globals.h"

globals_t global_flags = {true,
                          false,
                          {0, 0, 0, 0, false},
                          full,
                          {false, false, false, false, false}};

// void BeginSession(int argument_count, char** arguments)
// {
//     (void)GetCurrentTime(); // Start the clock.
//     if (isatty(STDOUT_FILENO)) global_flags.stdout_available = true;
//     HandleCommandLineArgs(argument_count, arguments);

//     while (CheckSessionValidity())
//     {
//         // Run the application.
//     }
// }
