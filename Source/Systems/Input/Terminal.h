/**
 * @file Terminal.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides various utilities to be used when parsing command line
 * arguments passed to the executable.
 * @date 2024-07-29
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_TERMINAL_INPUT_SYSTEM_
#define _MSENG_TERMINAL_INPUT_SYSTEM_

// The master include file of the toolkit.
#include <Master.h>

/**
 * @brief A function to handle any arguments passed to the main executable.
 * This often is used to tweak performance settings.
 * @param argc The count of arguments as given by the command line.
 * @param argv The actual arguments.
 * @note UNIMPLEMENTED -- no valid params
 */
void HandleCommandLineArgs(int argc, char** argv);

#endif // _MSENG_TERMINAL_INPUT_SYSTEM_
