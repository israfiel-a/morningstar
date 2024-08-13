#include "Messages.h"
#include "System.h"
#include <Diagnostic/Time.h>
#include <Session.h>

/**
 * @brief A flag to specify if libnotify has already been polled for, as
 * the @ref access function is not particularly inexpensive, and I would
 * like to avoid needless calls.
 */
static bool libnotify_polled = false;

static bool libnotify_available = false;

bool CheckForNotificationPackage(void)
{
    if (!libnotify_polled)
    {
        // Check for the notify-send binary since that's the only one we
        // use.
        libnotify_available = access("/usr/bin/notify-send", X_OK) == 0;
        libnotify_polled = true;
    }
    return libnotify_available;
}

void LogNotification(const char* title, const char* body, ...)
{
    // Fail silently if we can't send notifications.
    if (!CheckForNotificationPackage()) return;

    char error_system_call[512];
    va_list args;
    va_start(args, body);

    char error_body[256];
    vsnprintf(error_body, 256, body, args);
    va_end(args);

    snprintf(error_system_call, 512,
             "notify-send -a '" ID "' -u critical -t 0 "
             "'%s' \"%s\"",
             title, error_body);

    (void)SystemCall(error_system_call);
}

#ifdef DEBUG

void ReportMessage_(const char* body, ...)
{
    if (!global_flags.stdout_available) return;

    char time[512] = "\033[32m[";
    GetTimeString(time + 6, 511);
    time[18] = ']';
    printf("%s", time);

    va_list args;
    va_start(args, body);
    vprintf(body, args);

    printf("\033[0m\n");
    va_end(args);
}

#endif
