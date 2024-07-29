/**
 * @file Input.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides functionality for both keyboard and mouse input in-game.
 * @date 2024-07-27
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_INPUT_SYSTEM_
#define _MSENG_INPUT_SYSTEM_

// The master include file of the project.
#include <Master.h>
// Wayland/XDG type definitions.
#include "Types.h"

/**
 * @brief An enumerator describing the various events that can pass in a
 * single frame of mouse input.
 */
typedef enum
{
    /**
     * @brief The mouse entered the window.
     */
    enter = 1 << 0,
    /**
     * @brief The mouse left the window.
     */
    leave = 1 << 1,
    /**
     * @brief The mouse moved on the window.
     */
    motion = 1 << 2,
    /**
     * @brief A button on the mouse was pressed.
     */
    press = 1 << 3,
    /**
     * @brief An axis event was triggered.
     */
    axis = 1 << 4,
    /**
     * @brief An axis event stopped during the frame.
     */
    axis_stop = 1 << 5,
    /**
     * @brief We were given information about an axis event's source.
     */
    axis_source = 1 << 6,
    /**
     * @brief We were given information about the axis' step.
     */
    axis_discrete = 1 << 7,
    /**
     * @brief We were given information on the direction of the axis
     * operation.
     */
    axis_direction = 1 << 8
} mouse_events_t;

/**
 * @brief A single mouse event structure. This contains all the information
 * that can possibly be put in a single frame of mouse use.
 */
typedef struct
{
    /**
     * @brief A list of bitwise-OR'ed events that occurred during the
     * frame.
     */
    uint32_t events;
    /**
     * @brief The serial code of the last event triggered.
     */
    uint32_t serial;
    /**
     * @brief The timestamp of the last triggered event.
     */
    uint32_t time;
    /**
     * @brief The button whose state changed, if one did. This value is
     * defined in @file linux/input-event-codes.h.
     */
    uint32_t button;
    /**
     * @brief The new state of the given button, either @ref pressed or
     * @ref released.
     */
    mouse_button_state_t state;
    /**
     * @brief The X position of the cursor after its last move.
     */
    wl_fixed_t x;
    /**
     * @brief The Y position of the cursor after its last move.
     */
    wl_fixed_t y;
    /**
     * @brief Since there are both horizontal and vertical axis devices, we
     * have to keep a separate record for each possible kind of axis (2).
     */
    struct
    {
        /**
         * @brief A flag simply representing if this axis was affected
         * during the now-past frame.
         */
        bool valid;
        /**
         * @brief The relative direction the axis event happened in.
         */
        mouse_axis_direction_t direction;
        /**
         * @brief The length of the axis event.
         */
        wl_fixed_t length;
        /**
         * @brief The step of the axis.
         */
        int32_t discrete;
    } axes[2];
    /**
     * @brief The source of the axis event--scroll button, touch pad, etc.
     */
    mouse_axis_type_t axis_information;
} mouse_event_t;

/**
 * @brief Bind a new input group to the registry. The game is only built to
 * have one of these, but adding multiple is @b theoretically possible.
 * @param registry The Wayland registry of the application.
 * @param name The name of the group.
 * @param version The version of the interface.
 */
void BindInputGroup(registry_t* registry, const uint32_t name,
                    const uint32_t version);

/**
 * @brief Set the cursor image displayed when the mouse hovers over the
 * window.
 * @param buffer The image to attach to it.
 * @note !! UNIMPLEMENTED
 */
void SetCursorImage(pixel_buffer_t* buffer);

/**
 * @brief Check if the application has a mouse connected to it.
 * @return true There is a mouse connected.
 * @return false There is not a mouse connected.
 */
const bool GetMouseCapability(void);

/**
 * @brief Check if the application has a keyboard connected to it.
 * @return true There is a keyboard connected.
 * @return false There is no keyboard connected.
 */
const bool GetKeyboardCapability(void);

#endif // _MSENG_INPUT_SYSTEM_
