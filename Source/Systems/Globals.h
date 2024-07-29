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

typedef struct
{
    bool application_running;
    bool monitor_already_polled;
    bool stdout_available;
    bool libnotify_available;
    enum
    {
        dark,
        light
    } application_theme;
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

#endif // _MSENG_APPLICATION_SYSTEM_
