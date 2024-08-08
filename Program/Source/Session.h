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

// The master include file for the project.
#include <Master.h>

/**
 * @brief A structure built to contain the various global values the
 * application needs to record in order to run properly. The values here
 * exist here because they neither fit nor belong anywhere else.
 */
typedef struct
{
    /**
     * @brief The global application "running" flag. Bit of a no-brainer,
     * this one.
     */
    bool application_running;
    /**
     * @brief This flag describes whether or not a shell process is tied to
     * this process, and whether or not we can print to it.
     */
    bool stdout_available;
    /**
     * @brief A structure that describes the dimensions of the application.
     * This is stored globally simply because a lot of files need it, and
     * the import web becomes quite complicated to manage.
     */
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
    /**
     * @brief An enumerator describing the current UI mode of the
     * application. This describes how many panels are on screen and where
     * those panels exist.
     */
    enum
    {
        /**
         * @brief Only the gameplay window (and perhaps some of the
         * backdrop) is visible to the user.
         */
        unified,
        /**
         * @brief The same as @enum unified mode, except this is locked.
         * The user cannot change it. This is only toggled when the user's
         * monitor dimensions prevent the sensible distribution of multiple
         * screens on the monitor.
         */
        unified_locked,
        /**
         * @brief The monitor only has space for the stat window and
         * gameplay, so that's what we show. This is typically recommended
         * for 4:3 monitors.
         */
        paneled_half,
        /**
         * @brief Same as @enum paneled_half, except this mode is locked.
         * However, despite the lock, the user can still set their UI mode
         * to unified.
         */
        paneled_half_locked,
        /**
         * @brief All three panels; bust, gameplay, and stats are fully
         * visible. This is typically reserved for those playing on 16:9
         * monitors.
         */
        paneled_full
    } ui_mode;
    /**
     * @brief An enumerator to describe the input mode of the application.
     * This can be affected by a lot of things, like user settings, device
     * availability, and more.
     */
    enum
    {
        /**
         * @brief Only the keyboard does anything in-game. This option is
         * great for those who @b really like shortcut combos.
         */
        keyboard_only,
        /**
         * @brief The same as @enum keyboard_only, except the mode is
         * locked on. This is for players without a mouse plugged into
         * their computer at all.
         */
        keyboard_only_locked,
        /**
         * @brief The user has both keyboard and mouse connected to their
         * computer.
         */
        full,
        /**
         * @brief There is not a single input device connected to the
         * user's computer! While this mode is active, a perpetual
         * pause/warning screen is displayed.
         */
        nothing
    } input_mode;
} globals_t;

/**
 * @brief The global flag structure for the application.
 */
extern globals_t global_flags;

/**
 * @brief A short macro to make accessing the application's dimensions
 * easier.
 */
#define dimensions global_flags.application_dimensions
/**
 * @brief A short macro to make accessing the application's running flag
 * easier.
 */
#define running global_flags.application_running

/**
 * @brief Begin a new session of the game, and then run the processing loop
 * until the game closes. This should be one of two functions contained
 * within your Main.c file.
 * @param argument_count The count of arguments passed from the command
 * line, typically know as @ref argc.
 * @param arguments The actual string arguments passed from the command
 * line, typically known as @ref argv.
 */
void BeginSession(int argument_count, char** arguments);

/**
 * @brief Clean up all memory associated with Morningstar.
 */
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
