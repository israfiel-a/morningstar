#include "Hardware.h"
#include <Session.h>
#include <Windowing/Registry.h>
#include <linux/input-event-codes.h>

extern const keyboard_monitor_t keyboard_listener;
extern const mouse_monitor_t mouse_listener;

/**
 * @brief The input group given to us by the compositor for use in the
 * application.
 */
static input_group_t* seat = NULL;

/**
 * @brief The mouse currently registered to the application. If none
 * are, then this value is set to NULL.
 */
static mouse_object_t* mouse = NULL;

/**
 * @brief One of the keyboards currently connected to the application.
 * Which keyboard this is (if the user has multiple connected) is mostly
 * arbitrary. If there is not a keyboard available, this object is NULL.
 */
static keyboard_object_t* keyboard = NULL;

/**
 * @brief The internal group of callbacks to be triggered by all the
 * various listener functions set up below.
 */
input_callback_group_t input_callbacks = {NULL, NULL, NULL, NULL, NULL,
                                          NULL, NULL, NULL, NULL, NULL,
                                          NULL, NULL, NULL, NULL};

/**
 * @brief Handles the connection / disconnection of input devices from the
 * host device.
 * @param d Nothing of use.
 * @param seat The input group these new gizmos have been added to.
 * @param capabilities The new capabilities of the input group with the
 * added objects.
 */
static void HIDC(void* d, input_group_t* seat, uint32_t capabilities)
{
    bool supports_mouse = capabilities & WL_SEAT_CAPABILITY_POINTER;
    if (supports_mouse && mouse == NULL)
    {
        mouse = wl_seat_get_pointer(seat);
        wl_pointer_add_listener(mouse, &mouse_listener, NULL);
        global_flags.application_mode = full;
    }
    else if (!supports_mouse && mouse != NULL)
    {
        wl_pointer_release(mouse);
        mouse = NULL;
        global_flags.application_mode = keyboard_only_locked;
    }

    bool supports_keyboard = capabilities & WL_SEAT_CAPABILITY_KEYBOARD;
    if (supports_keyboard && keyboard == NULL)
    {
        keyboard = wl_seat_get_keyboard(seat);
        wl_keyboard_add_listener(keyboard, &keyboard_listener, NULL);
    }
    else if (!supports_keyboard && keyboard != NULL)
    {
        wl_keyboard_release(keyboard);
        keyboard = NULL;
    }
}

/**
 * @brief An unused function for handling the change of name for an input
 * group.
 */
NOOP HSN(void* d, input_group_t* s, const char* n) {}

/**
 * @brief A listener for input groups that handles the connection /
 * disconnection of devices from the host machine.
 */
static const input_group_monitor_t input_group_listener = {HIDC, HSN};

//-----------------------------------------------------------------------//
// The functions below this point are methods that access private values //
// linked only to this translation unit (i.e. the capabilities of the    //
// input group), and are simply getters. The reason they are not inline, //
// and are contained within this file instead of the header, is because  //
// of their private nature.                                              //
//-----------------------------------------------------------------------//

void BindInputGroup(const uint32_t name, const uint32_t version)
{
    seat =
        wl_registry_bind(GetRegistry(), name, &wl_seat_interface, version);
    wl_seat_add_listener(seat, &input_group_listener, NULL);
}

void UnbindInputGroup(void)
{
    wl_pointer_release(mouse);
    wl_keyboard_release(keyboard);
    wl_seat_release(seat);
}

input_group_t* GetInputGroup(void) { return seat; }

void SetMouseEnterCallback(void (*func)(wl_fixed_t, wl_fixed_t))
{
    input_callbacks.mouse_enter = func;
}
void SetMouseLeaveCallback(void (*func)(void))
{
    input_callbacks.mouse_leave = func;
}
void SetMouseMoveCallback(void (*func)(uint32_t, wl_fixed_t, wl_fixed_t))
{
    input_callbacks.mouse_move = func;
}
void SetMouseButtonDownCallback(void (*func)(uint32_t, uint32_t))
{
    input_callbacks.mouse_button_down = func;
}
void SetMouseButtonUpCallback(void (*func)(uint32_t, uint32_t))
{
    input_callbacks.mouse_button_release = func;
}
void SetMouseScrollCallback(void (*func)(mouse_axis_type_t,
                                         mouse_axis_direction_t,
                                         wl_fixed_t, int32_t, uint32_t))
{
    input_callbacks.mouse_scroll = func;
}
void SetMouseRockCallback(void (*func)(mouse_axis_type_t,
                                       mouse_axis_direction_t, wl_fixed_t,
                                       int32_t, uint32_t))
{
    input_callbacks.mouse_rock = func;
}
void SetKeyboardEnterCallback(void (*func)(void))
{
    input_callbacks.keyboard_enter = func;
}
void SetKeyboardLeaveCallback(void (*func)(void))
{
    input_callbacks.keyboard_leave = func;
}
void SetKeyboardKeymapCallback(void (*func)(char*))
{
    input_callbacks.keyboard_keymap = func;
}
void SetKeyboardKeydownCallback(void (*func)(uint32_t))
{
    input_callbacks.keyboard_keydown = func;
}
void SetKeyboardKeyupCallback(void (*func)(uint32_t))
{
    input_callbacks.keyboard_keyup = func;
}
void SetKeyboardRepeatCallback(void (*func)(uint32_t))
{
    input_callbacks.keyboard_repeat = func;
}
void SetKeyboardModifierCallback(void (*func)(uint32_t, uint32_t,
                                              uint32_t))
{
    input_callbacks.keyboard_modifier = func;
}
