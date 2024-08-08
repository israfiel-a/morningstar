/**
 * @file Globals.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief A struct of global values to be kept throughout the
 * program--specifically ones that should be available throughout many
 * translation units.
 * @date 2024-07-25
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_APPLICATION_SYSTEM_
#define _MSENG_APPLICATION_SYSTEM_

#include <Master.h>

#define NOOP static void

#define dimensions global_flags.application_dimensions

typedef struct
{
    bool application_running;
    bool monitor_already_polled;
    bool stdout_available;
    bool libnotify_available;
    uint64_t start_time;
    struct
    {
        /**
         * @brief The width of the monitor as reported by XDG-shell.
         */
        int32_t width;
        /**
         * @brief The height of the monitor as reported by XDG-shell.
         */
        int32_t height;
        /**
         * @brief The shortest side of the monitor; width or height.
         */
        int32_t shortest_side;
        /**
         * @brief The size of the gaps between the end of the x-border of
         * the gameplay window and end of the backdrop window.
         */
        int32_t gap_size;
    } application_dimensions;
    enum
    {
        unified,            // just gameplay window
        unified_locked,     // locked in unified mode
        paneled_full,       // all three panels
        paneled_half,       // just stats + gameplay
        paneled_half_locked // locked at stats + gameplay
    } ui_mode;
    enum
    {
        keyboard_only,
        keyboard_only_locked,
        full,
        full_locked,
        nothing
    } application_mode;
} globals_t;
extern globals_t global_flags;

void BeginSession(int argument_count, char** arguments);
void CleanupSession(void);

/**
 * @brief Check the validity of the current session; make sure we haven't
 * been requested to die, check to make sure the display server is still
 * sending display, etcetera.
 * @return true The session is still valid, continue.
 * @return false The session is no longer valid, kill the process.
 */
bool CheckSessionValidity(void);

#endif // _MSENG_APPLICATION_SYSTEM_
