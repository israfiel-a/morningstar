#include "Time.h"
#include <Globals.h>
#include <Output/Error.h>
#include <time.h>

uint64_t GetCurrentTime(void)
{
    // On Linux, clock_gettime(MONO) reports seconds since system booted,
    // so we just multiply it by a thousand to get milliseconds.
    struct timespec retrieved_time;
    int time_get_return = clock_gettime(CLOCK_MONOTONIC, &retrieved_time);
    if (time_get_return == -1) ReportError(time_get_error, false);

    if (global_flags.start_time == 0)
    {
        global_flags.start_time = NSEC_TO_MSEC(retrieved_time.tv_nsec);
        return 0;
    }
    return NSEC_TO_MSEC(retrieved_time.tv_nsec) - global_flags.start_time;
}

void GetTimeString(char* buffer, size_t buffer_length)
{
    if (buffer_length < 13)
    {
        buffer = NULL;
        return;
    }

    uint64_t ms = GetCurrentTime(), s = 0, m = 0;
    if (ms >= 1000)
    {
        s = ms / 1000;
        ms = ms % 1000;
    }
    if (s >= 60)
    {
        m = s / 60;
        s = s % 60;
    }

    snprintf(buffer, buffer_length, "%03lu::%02lu::%03lu", ms, s, m);
}
