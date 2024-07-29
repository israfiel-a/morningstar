#include "Input.h"
#include <Globals.h>
#include <linux/input-event-codes.h>

/**
 * @brief A boolean value representing if the user has a mouse currently
 * connected to the system. This value is polled by Wayland whenever a seat
 * device changes. If this device is disconnected, the program is locked
 * into @ref keyboard_only mode.
 */
static bool supports_mouse = false;

/**
 * @brief A boolean value representing if the user has a keyboard device
 * connected to the computer. If this device, at the very least, is not
 * connected, the application cannot be run.
 */
static bool supports_keyboard = false;

/**
 * @brief The mouse currently registered to the application. If none are,
 * then this value is set to NULL.
 */
static mouse_object_t* mouse = NULL;

/**
 * @brief The last mouse event triggered by the application. This event is
 * reset every time a mouse "frame" has passed, and filled out with new
 * data over the course of the next.
 */
static mouse_event_t last_mouse_event;

/**
 * @brief Handle the mouse entering the window area.
 * @param d Nothing of use.
 * @param m Nothing of use.
 * @param serial The event's serial number.
 * @param s Nothing of use.
 * @param x_pos The X coordinate the mouse entered on.
 * @param y_pos The Y coordinate the mouse entered on.
 */
static void HME(void* d, mouse_object_t* m, uint32_t serial,
                window_raw_t* s, wl_fixed_t x_pos, wl_fixed_t y_pos)
{
    last_mouse_event.serial = serial;
    last_mouse_event.events |= enter;
    last_mouse_event.x = x_pos;
    last_mouse_event.y = y_pos;
}

/**
 * @brief Handle the mouse leaving the window area.
 * @param d Nothing of use.
 * @param m Nothing of use.
 * @param serial The serial number of the event.
 * @param s Nothing of use.
 */
static void HML(void* d, mouse_object_t* m, uint32_t serial,
                window_raw_t* s)
{
    last_mouse_event.serial = serial;
    last_mouse_event.events |= leave;
}

/**
 * @brief Handle the mouse moving.
 * @param d Nothing of use.
 * @param m Nothing of use.
 * @param time Millisecond timestamp of the move.
 * @param x_pos The X coordinate the mouse moved onto.
 * @param y_pos The Y coordinate the mouse moved onto.
 */
static void HMM(void* d, mouse_object_t* m, uint32_t time,
                wl_fixed_t x_pos, wl_fixed_t y_pos)
{
    last_mouse_event.events |= motion;
    last_mouse_event.time = time;
    last_mouse_event.x = x_pos;
    last_mouse_event.y = y_pos;
}

/**
 * @brief Handle the press/release of one of the mouse's various buttons,
 * including any side buttons.
 * @param d Nothing of use.
 * @param m Nothing of use.
 * @param serial The serial code of the event.
 * @param time The timestamp of the button's press down to the millisecond.
 * @param button The button pressed.
 * @param state The state of the button--pressed or released.
 */
static void HMP(void* d, mouse_object_t* m, uint32_t serial, uint32_t time,
                uint32_t button, mouse_button_state_t state)
{
    last_mouse_event.serial = serial;
    last_mouse_event.events |= press;
    last_mouse_event.time = time;
    last_mouse_event.button = button;
    last_mouse_event.state = state;
}

/**
 * @brief Handle a mouse axis event, like scrolling.
 * @param d Nothing of use.
 * @param m Nothing of use.
 * @param time The timestamp of the axis event down to the millisecond.
 * @param axis The axis affected, horizontal or vertical.
 * @param value The length of the axis change.
 */
static void HMA(void* d, mouse_object_t* m, uint32_t time,
                mouse_axis_t axis, wl_fixed_t value)
{
    last_mouse_event.events |= axis;
    last_mouse_event.time = time;
    last_mouse_event.axes[axis].valid = true;
    last_mouse_event.axes[axis].length = value;
}

/**
 * @brief Handle the information passed to us about what kind of axis
 * operation the user did.
 * @param d Nothing of use.
 * @param m Nothing of use.
 * @param axis_source The type of axis movement.
 */
static void HMAI(void* d, mouse_object_t* m, mouse_axis_type_t axis_source)
{
    last_mouse_event.events |= axis_source;
    last_mouse_event.axis_information = axis_source;
}

/**
 * @brief Handle the end of an axis event.
 * @param d Nothing of use.
 * @param m Nothing of use.
 * @param time The timestamp of the axis event down to the millisecond.
 * @param axis The axis whose event stopped.
 */
static void HMAS(void* d, mouse_object_t* m, uint32_t time,
                 mouse_axis_t axis)
{
    last_mouse_event.time = time;
    last_mouse_event.events |= axis_stop;
    last_mouse_event.axes[axis].valid = true;
}

/**
 * @brief Handle the giving of discrete axis information, which is really
 * just step information for scrolling and other such things.
 * @param d Nothing of use.
 * @param m Nothing of use.
 * @param axis The axis whose event is currently being triggered.
 * @param discrete The step of the axis.
 */
static void HMAD(void* d, mouse_object_t* m, mouse_axis_t axis,
                 int32_t discrete)
{
    last_mouse_event.events |= axis_discrete;
    last_mouse_event.axes[axis].valid = true;
    last_mouse_event.axes[axis].discrete = discrete;
}

/**
 * @brief An unused function to handle high-resolution axis discrete
 * information. We don't need this, and as such do not use it.
 */
NOOP HMAHR(void* d, mouse_object_t* m, uint32_t a, int32_t v) {}

/**
 * @brief Handle the relative direction of the axis operation. Think of
 * like "up" and "down" when one is scrolling.
 * @param d Nothing of use.
 * @param m Nothing of use.
 * @param axis The axis being directed.
 * @param direction The direction of the axis.
 */
static void HMARD(void* d, mouse_object_t* m, mouse_axis_t axis,
                  mouse_axis_direction_t direction)
{
    last_mouse_event.events |= axis_direction;
    last_mouse_event.axes[axis].valid = true;
    last_mouse_event.axes[axis].direction = direction;
}

/**
 * @brief Update the screen uint32_twith all the various things Wayland
 * reported the user did during the last mouse sequence.
 * @param d Nothing of use.
 * @param m Nothing of use.
 */
static void HMF(void* d, mouse_object_t* m)
{
    if (last_mouse_event.button == BTN_RIGHT &&
        last_mouse_event.state == WL_POINTER_BUTTON_STATE_PRESSED)
        global_flags.application_running = false;

    // Reset the event to a NULL state.
    memset(&last_mouse_event, 0, sizeof(last_mouse_event));
}

/**
 * @brief The listener for the mouse. Contains events that will be
 * triggered in various situations, like the mouse entering the screen or a
 * mouse button being pressed.
 */
static const mouse_monitor_t mouse_listener = {
    HME, HML, HMM, HMP, HMA, HMF, HMAI, HMAS, HMAD, HMAHR, HMARD};

/**
 * @brief One of the keyboards currently connected to the application.
 * Which keyboard this is (if the user has multiple connected) is mostly
 * arbitrary. If there is not a keyboard available, this object is NULL.
 */
static keyboard_object_t* keyboard = NULL;

/**
 * @brief Provides a file descriptor that when mapped in read-only mode
 * will describe the user's keymap configuration.
 * @param d Nothing of use.
 * @param k Nothing of use.
 * @param format The format of the given keymap.
 * @param fd The file descriptor of the keymap.
 * @param size The size of the keymap in bytes.
 */
static void HKM(void* d, keyboard_object_t* k,
                keyboard_keymap_format_t format, int32_t fd, uint32_t size)
{
    // No operation.
}

/**
 * @brief Handle the keyboard entering the window.
 * @param d Nothing of use.
 * @param k Nothing of use.
 * @param serial The serial code of the event.
 * @param s Nothing of use.
 * @param keys The keys currently down.
 */
static void HKE(void* d, keyboard_object_t* k, uint32_t serial,
                window_raw_t* s, struct wl_array* keys)
{
    // No operation.
}

/**
 * @brief An unused function to handle the keyboard leaving the window. We
 * figure that if the keyboard leaves the window, the toplevel is now
 * unfocused, and as such the game has automatically paused through use of
 * events elsewhere in the game.
 */
NOOP HKL(void* d, keyboard_object_t* k, uint32_t c, window_raw_t* s) {}

/**
 * @brief Handle the press of a key on the keyboard.
 * @param d Nothing of use.
 * @param k Nothing of use.
 * @param serial The serial code of the event.
 * @param time The timestamp of the key's state changing down to the
 * milliseconds.
 * @param key The key pressed. The code for this is provided by @file
 * linux/input-event-codes.h.
 * @param state The new state of the key.
 */
static void HKK(void* d, keyboard_object_t* k, uint32_t serial,
                uint32_t time, uint32_t key, keyboard_key_state_t state)
{
    // No operation.
}

/**
 * @brief Handle the application of modifiers on keyboard keys.
 * @param d Nothing of use.
 * @param k Nothing of use.
 * @param serial The serial code of the event.
 * @param pressed Modifier keys pressed.
 * @param toggled Modifier keys toggled on.
 * @param locked Modifier keys locked.
 * @param group The keyboard layout of the user.
 */
static void HKMO(void* d, keyboard_object_t* k, uint32_t serial,
                 uint32_t pressed, uint32_t toggled, uint32_t locked,
                 uint32_t group)
{
    // No operation.
}

/**
 * @brief Handle information given to use about the keyboard's repeat
 * parameters.
 * @param d Nothing of use.
 * @param k Nothing of use.
 * @param rate The rate of repeat in characters per second.
 * @param delay The delay in milliseconds before the repeat state is
 * triggered.
 */
static void HKR(void* d, keyboard_object_t* k, int32_t rate, int32_t delay)
{
    // No operation.
}

/**
 * @brief The listener for the keyboard. This triggers events during
 * important keyboard events like a keypress or modifier toggle.
 */
static const keyboard_monitor_t keyboard_listener = {HKM, HKE,  HKL,
                                                     HKK, HKMO, HKR};

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
    supports_mouse = capabilities & WL_SEAT_CAPABILITY_POINTER;
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

    supports_keyboard = capabilities & WL_SEAT_CAPABILITY_KEYBOARD;
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

void BindInputGroup(registry_t* registry, const uint32_t name,
                    const uint32_t version)
{
    input_group_t* seat =
        wl_registry_bind(registry, name, &wl_seat_interface, version);
    wl_seat_add_listener(seat, &input_group_listener, NULL);
}

void SetCursorImage(pixel_buffer_t* buffer) {}

const bool GetMouseCapability(void) { return supports_mouse; }
const bool GetKeyboardCapability(void) { return supports_keyboard; }
