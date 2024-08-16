#include "Mouse.h"
#include "Hardware.h"                // Input callback group
#include <linux/input-event-codes.h> // Linux input codes

/**
 * @brief The internal input callback group.
 */
extern input_callback_group_t input_callbacks;

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
                raw_window_t* s, wl_fixed_t x_pos, wl_fixed_t y_pos)
{
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
                raw_window_t* s)
{
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
static void HMAHR(void* d, mouse_object_t* m, uint32_t a, int32_t v) {}

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
 * @brief Update the screen with all the various things Wayland reported
 * the user did during the last mouse sequence.
 * @param d Nothing of use.
 * @param m Nothing of use.
 */
static void HMF(void* d, mouse_object_t* m)
{
    // Big ol' if statement blob, this checks for each event, sees if a
    // corresponding callback has been set, and triggers it if it has.
    if (last_mouse_event.events & enter &&
        input_callbacks.mouse_enter != NULL)
        input_callbacks.mouse_enter(last_mouse_event.x,
                                    last_mouse_event.y);
    if (last_mouse_event.events & leave &&
        input_callbacks.mouse_leave != NULL)
        input_callbacks.mouse_leave();
    if (last_mouse_event.events & motion &&
        input_callbacks.mouse_move != NULL)
        input_callbacks.mouse_move(last_mouse_event.time,
                                   last_mouse_event.x, last_mouse_event.y);
    if (last_mouse_event.events & press)
    {
        if (last_mouse_event.state == WL_POINTER_BUTTON_STATE_PRESSED &&
            input_callbacks.mouse_button_down != NULL)
            input_callbacks.mouse_button_down(last_mouse_event.button,
                                              last_mouse_event.time);
        else if (last_mouse_event.state ==
                     WL_POINTER_BUTTON_STATE_RELEASED &&
                 input_callbacks.mouse_button_release != NULL)
            input_callbacks.mouse_button_release(last_mouse_event.button,
                                                 last_mouse_event.time);
    }
    // Just merge all axis events into one value, as we need to check for
    // all of them or none of them.
    uint32_t axis_event =
        axis | axis_stop | axis_source | axis_discrete | axis_direction;
    if (last_mouse_event.events & axis_event)
    {
        if (last_mouse_event.axes[0].valid &&
            input_callbacks.mouse_scroll != NULL)
            input_callbacks.mouse_scroll(
                last_mouse_event.axis_information,
                last_mouse_event.axes[0].direction,
                last_mouse_event.axes[0].length,
                last_mouse_event.axes[0].discrete, last_mouse_event.time);
        if (last_mouse_event.axes[1].valid &&
            input_callbacks.mouse_rock != NULL)
            input_callbacks.mouse_rock(last_mouse_event.axis_information,
                                       last_mouse_event.axes[1].direction,
                                       last_mouse_event.axes[1].length,
                                       last_mouse_event.axes[1].discrete,
                                       last_mouse_event.time);
    }

    // Reset the event to a NULL state.
    memset(&last_mouse_event, 0, sizeof(last_mouse_event));
}

/**
 * @brief The listener for the mouse. Contains events that will be
 * triggered in various situations, like the mouse entering the screen or a
 * mouse button being pressed.
 */
const mouse_monitor_t mouse_listener = {HME,  HML,  HMM,  HMP,   HMA,  HMF,
                                        HMAI, HMAS, HMAD, HMAHR, HMARD};

void SetCursorImage(pixel_buffer_t* buffer) {}
