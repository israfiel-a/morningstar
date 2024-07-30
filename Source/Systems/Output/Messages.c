#include "Messages.h"
#include "System.h"
#include <Diagnostic/Time.h>
#include <Globals.h>

extern globals_t global_flags;

static bool libnotify_polled = false;
bool CheckForNotificationPackage(void)
{
    if (!libnotify_polled)
    {
        global_flags.libnotify_available =
            access("/usr/bin/notify-send", X_OK) == 0;
        libnotify_polled = true;
    }
    return global_flags.libnotify_available;
}

void LogNotification(const char* title, const char* body, ...)
{
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
    char time[512] = "\033[32m[";
    GetTimeString(time + 6, 511);
    time[18] = ']';
    printf("%s", time);

    va_list args;
    va_start(args, body);
    vprintf(body, args);

    printf("\"\033[0m\n");
    va_end(args);
}

#endif
