#include "Hardware.h"
#include <Globals.h>
#include <Output/Warning.h>
#include <Windowing/Wayland.h>       // Registry functions
#include <linux/input-event-codes.h> // Linux input codes
#include <stdbool.h>

/**
 * @brief The application's keyboard listener as created by @file
 * Keyboard.c.
 */
extern const struct wl_keyboard_listener keyboard_listener;

/**
 * @brief The application's mouse listener as created by @file Mouse.c.
 */
extern const struct wl_pointer_listener mouse_listener;

/**
 * @brief The input group given to us by the compositor for use in the
 * application.
 */
static struct wl_seat* seat = NULL;

/**
 * @brief The mouse currently registered to the application. If none
 * are, then this value is set to NULL.
 */
static struct wl_pointer* mouse = NULL;

/**
 * @brief One of the keyboards currently connected to the application.
 * Which keyboard this is (if the user has multiple connected) is mostly
 * arbitrary. If there is not a keyboard available, this object is NULL.
 */
static struct wl_keyboard* keyboard = NULL;

/**
 * @brief The internal group of callbacks to be triggered by all the
 * various listener functions defined in @file Mouse.c and @file
 * Keyboard.c.
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
static void HIDC(void* d, struct wl_seat* seat, uint32_t capabilities)
{
    bool supports_mouse = capabilities & WL_SEAT_CAPABILITY_POINTER;
    if (supports_mouse && mouse == NULL)
    {
        mouse = wl_seat_get_pointer(seat);
        wl_pointer_add_listener(mouse, &mouse_listener, NULL);
        global_flags.input_mode = full;
    }
    else if (!supports_mouse && mouse != NULL)
    {
        wl_pointer_release(mouse);
        mouse = NULL;
        global_flags.input_mode = keyboard_only_locked;
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
static void HSN(void* d, struct wl_seat* s, const char* n) {}

/**
 * @brief A listener for input groups that handles the connection /
 * disconnection of devices from the host machine.
 */
static const struct wl_seat_listener input_group_listener = {HIDC, HSN};

void BindInputGroup(const uint32_t name, const uint32_t version)
{
    if (GetRegistry() == NULL)
    {
        ReportWarning(preemptive_seat_creation);
        return;
    }

    if (devices.input_group)
    {
        ReportWarning(double_seat_creation);
        return;
    }

    seat =
        wl_registry_bind(GetRegistry(), name, &wl_seat_interface, version);
    wl_seat_add_listener(seat, &input_group_listener, NULL);
    devices.input_group = true;
}

void UnbindInputGroup(void)
{
    if (!devices.input_group)
    {
        ReportWarning(preemptive_seat_free);
        return;
    }

    if (mouse != NULL) wl_pointer_release(mouse);
    if (keyboard != NULL) wl_keyboard_release(keyboard);
    wl_seat_release(seat);
    devices.input_group = false;
}

struct wl_seat* GetInputGroup(void) { return seat; }

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
void SetMouseScrollCallback(void (*func)(
    enum wl_pointer_axis_source, enum wl_pointer_axis_relative_direction,
    wl_fixed_t, int32_t, uint32_t))
{
    input_callbacks.mouse_scroll = func;
}
void SetMouseRockCallback(void (*func)(
    enum wl_pointer_axis_source, enum wl_pointer_axis_relative_direction,
    wl_fixed_t, int32_t, uint32_t))
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
