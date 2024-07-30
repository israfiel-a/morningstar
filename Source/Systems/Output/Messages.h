/**
 * @file Notification.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides utilities for logging messages and notifications
 * throughout the program.
 * @date 2024-07-29
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_MESSAGE_OUTPUT_SYSTEM_
#define _MSENG_MESSAGE_OUTPUT_SYSTEM_

// The master include for the project.
#include <Master.h>

/**
 * @brief Check for the @ref libnotify package on the user's system. This
 * sets a value called @ref libnotify_available within the global flags.
 * @return true The package was found.
 * @return false The package was not found.
 */
bool CheckForNotificationPackage(void);

/**
 * @brief A function to log a notification using @ref libnotify. If
 * libnotify does not exist on the user's device, this function simply does
 * nothing.
 * @param title The title of the message, "Error Logger", or something of
 * the like.
 * @param body The meat of the message, what you want the user to know.
 * This is used as a format string for the variable arguments.
 */
void LogNotification(const char* title, const char* body, ...);

// Only define the debug message interface if we're compiling in debug
// mode.
#ifdef DEBUG
void ReportMessage_(const char* body, ...);
    #define ReportMessage(...)                                            \
        ReportMessage_(" " FILENAME " :: " __VA_ARGS__)
#else
    #define ReportMessage(body)
#endif

#endif // _MSENG_MESSAGE_OUTPUT_SYSTEM_
