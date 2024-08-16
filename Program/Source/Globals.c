#include "Globals.h"
#include <Diagnostic/Time.h>  // Time functionality
#include <Input/File.h>       // Terminal argument parsing
#include <Output/Warning.h>   // Warning interface
#include <Windowing/System.h> // Display server setup
#include <Windowing/Windowing.h>

globals_t global_flags = {
    true, false, {0, 0, 0, 0}, full, {false, false, false, false, false}};

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
