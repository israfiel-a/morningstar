/**
 * @file Hardware.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides functionality for both keyboard and mouse input in-game.
 * @date 2024-07-27
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_GENERAL_INPUT_SYSTEM_
#define _MSENG_GENERAL_INPUT_SYSTEM_

// The master include file of the project.
#include <Master.h>
// Wayland/XDG type definitions.
#include <WaylandTypes.h>

/**
 * @brief A group of callback functions that are triggered upon any of the
 * events they describe. If any are NULL, nothing is done during their
 * event.
 */
typedef struct
{
    /**
     * @brief A callback to handle when the mouse enters the window.
     * @param x The X position the mouse entered on.
     * @param y The Y position the mouse entered on.
     */
    void (*mouse_enter)(wl_fixed_t x, wl_fixed_t y);
    /**
     * @brief A callback to handle when the mouse leaves the window.
     */
    void (*mouse_leave)(void);
    /**
     * @brief A callback to handle the moving of the mouse.
     * @param time The time signature of the event down to the millisecond.
     * @param x The X position the mouse moved to.
     * @param y The Y position the mouse moved to.
     */
    void (*mouse_move)(uint32_t time, wl_fixed_t x, wl_fixed_t y);
    /**
     * @brief A callback to handle a mouse button being pressed down.
     * @param button The input code of the button pressed, defined in @file
     * linux/input-event-codes.h.
     * @param time The time signature of the event down to the millisecond.
     */
    void (*mouse_button_down)(uint32_t button, uint32_t time);
    /**
     * @brief A callback to handle a mouse button being released.
     * @param button The input code of the button pressed, defined in @file
     * linux/input-event-codes.h.
     * @param time The time signature of the event down to the millisecond.
     */
    void (*mouse_button_release)(uint32_t button, uint32_t time);
    /**
     * @brief A callback to handle a scroll event (vertical axis event).
     * @param type The type of hardware that emitted the event (touchpad,
     * mouse, etc.).
     * @param direction The direction of the scroll, up or down.
     * @param length The length of the scroll.
     * @param step The step of the scroll, or distance between each scroll
     * event. This is useful for smoothing.
     * @param time The time signature of the event down to the millisecond.
     */
    void (*mouse_scroll)(mouse_axis_type_t type,
                         mouse_axis_direction_t direction,
                         wl_fixed_t length, int32_t step, uint32_t time);
    /**
     * @brief A callback to handle a rock / swipe event (horizontal axis
     * event).
     * @param type The type of hardware that emitted the event (touchpad,
     * mouse, etc.).
     * @param direction The direction of the swipe, left or right.
     * @param length The length of the swipe.
     * @param step The step of the swipe, or the distance between each
     * swipe event. This is useful for scrolling, although horizontal axis
     * events are typically smooth.
     * @param time The time signature of the event down to the millisecond.
     */
    void (*mouse_rock)(mouse_axis_type_t type,
                       mouse_axis_direction_t direction, wl_fixed_t length,
                       int32_t step, uint32_t time);
    /**
     * @brief A callback for when the keyboard focus enters the window.
     */
    void (*keyboard_enter)(void);
    /**
     * @brief A callback for when the keyboard focus leaves the
     * application.
     */
    void (*keyboard_leave)(void);
    /**
     * @brief A callback for when the application recieves a map of the
     * user's keyboard.
     * @param keymap A string representing the keymap.
     */
    void (*keyboard_keymap)(char* keymap);
    /**
     * @brief A callback for when a key is pressed on the user's keyboard.
     * @param key The key pressed, as defined by @file
     * linux/input-event-codes.h.
     */
    void (*keyboard_keydown)(uint32_t key);
    /**
     * @brief A callback for when a key is released on the user's keyboard.
     * @param key The key released, as defined by @file
     * linux/input-event-codes.h.
     */
    void (*keyboard_keyup)(uint32_t key);
    /**
     * @brief A callback for when a key is in the repeat state on the
     * user's keyboard.
     * @param key The key being repeated, as defined by @file
     * linux/input-event-codes.h.
     * @note NOT IMPLEMENTED !!!
     */
    void (*keyboard_repeat)(uint32_t key);
    /**
     * @brief A callback for when a modifier key is pressed / released /
     * locked / toggled / whatever.
     * @param pressed The modifier key(s) pressed.
     * @param toggled The modifier key(s) toggled.
     * @param locked The modifier key(s) locked.
     */
    void (*keyboard_modifier)(uint32_t pressed, uint32_t toggled,
                              uint32_t locked);
} input_callback_group_t;

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
 * @brief Unbind the application's input group.
 */
void UnbindInputGroup(void);

/**
 * @brief Add a callback to the mouse enter callback. See @ref
 * input_callback_group_t::mouse_enter for function information.
 */
void SetMouseEnterCallback(void (*)(wl_fixed_t, wl_fixed_t));
/**
 * @brief Add a callback to the mouse leave callback. See @ref
 * input_callback_group_t::mouse_leave for function information.
 */
void SetMouseLeaveCallback(void (*)(void));
/**
 * @brief Add a callback to the mouse move callback. See @ref
 * input_callback_group_t::mouse_move for function information.
 */
void SetMouseMoveCallback(void (*)(uint32_t, wl_fixed_t, wl_fixed_t));
/**
 * @brief Add a callback to the mouse button down callback. See @ref
 * input_callback_group_t::mouse_button_down for function information.
 */
void SetMouseButtonDownCallback(void (*)(uint32_t, uint32_t));
/**
 * @brief Add a callback to the mouse button up callback. See @ref
 * input_callback_group_t::mouse_button_up for function information.
 */
void SetMouseButtonUpCallback(void (*)(uint32_t, uint32_t));
/**
 * @brief Add a callback to the mouse scroll callback. See @ref
 * input_callback_group_t::mouse_scroll for function information.
 */
void SetMouseScrollCallback(void (*)(mouse_axis_type_t,
                                     mouse_axis_direction_t, wl_fixed_t,
                                     int32_t, uint32_t));
/**
 * @brief Add a callback to the mouse rock callback. See @ref
 * input_callback_group_t::mouse_rock for function information.
 */
void SetMouseRockCallback(void (*)(mouse_axis_type_t,
                                   mouse_axis_direction_t, wl_fixed_t,
                                   int32_t, uint32_t));
/**
 * @brief Add a callback to the keyboard enter callback. See @ref
 * input_callback_group_t::keyboard_enter for function information.
 */
void SetKeyboardEnterCallback(void (*)(void));
/**
 * @brief Add a callback to the keyboard leave callback. See @ref
 * input_callback_group_t::keyboard_leave for function information.
 */
void SetKeyboardLeaveCallback(void (*)(void));
/**
 * @brief Add a callback to the keyboard keymap callback. See @ref
 * input_callback_group_t::keyboard_keymap for function information.
 */
void SetKeyboardKeymapCallback(void (*)(char*));
/**
 * @brief Add a callback to the keyboard keydown callback. See @ref
 * input_callback_group_t::keyboard_keydown for function information.
 */
void SetKeyboardKeydownCallback(void (*)(uint32_t));
/**
 * @brief Add a callback to the keyboard keyup callback. See @ref
 * input_callback_group_t::keyboard_keyup for function information.
 */
void SetKeyboardKeyupCallback(void (*)(uint32_t));
/**
 * @brief Add a callback to the keyboard repeat callback. See @ref
 * input_callback_group_t::keyboard_repeat for function information.
 */
void SetKeyboardRepeatCallback(void (*)(uint32_t));
/**
 * @brief Add a callback to the keyboard modifier callback. See @ref
 * input_callback_group_t::keyboard_modifier for function information.
 */
void SetKeyboardModifierCallback(void (*)(uint32_t, uint32_t, uint32_t));

#endif // _MSENG_GENERAL_INPUT_SYSTEM_
