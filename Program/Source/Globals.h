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

#include <inttypes.h>
#include <stdbool.h>

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

        bool set;
    } application_dimensions;
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
    struct
    {
        bool compositor;
        bool subcompositor;
        bool window_manager;
        bool shm;
        bool input_group;
    } connected_devices;
} globals_t;

/**
 * @brief The global flag structure for the application.
 */
extern globals_t global_flags;

#define devices global_flags.connected_devices

#define full_device_suite                                                 \
    (devices.compositor && devices.subcompositor &&                       \
     devices.window_manager && devices.shm && devices.input_group)

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

#endif // _MSENG_APPLICATION_SYSTEM_
