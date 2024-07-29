#include "Keyboard.h"
#include "Input.h"
#include <linux/input-event-codes.h>

extern input_callback_group_t input_callbacks;

/**
 * @brief The second-to-last key pressed by the user.
 */
static uint32_t key_pressed_before = 0;
/**
 * @brief The key last pressed by the user.
 */
static uint32_t last_key_pressed = 0;

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
    if (input_callbacks.keyboard_keymap != NULL)
    {
        assert(format == WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1);
        char* map_shm = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
        assert(map_shm != MAP_FAILED);

        input_callbacks.keyboard_keymap(map_shm);
        munmap(map_shm, size);
        close(fd);
    }
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
    if (input_callbacks.keyboard_enter != NULL)
        input_callbacks.keyboard_enter();
}

/**
 * @brief A function to handle the keyboard leaving the window.
 * @param d Nothing of use.
 * @param k Nothing of use.
 * @param s The serial number of the operation.
 * @param w Nothing of use.
 */
static void HKL(void* d, keyboard_object_t* k, uint32_t s, window_raw_t* w)
{
    if (input_callbacks.keyboard_leave != NULL)
        input_callbacks.keyboard_leave();
}

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
    if (state == WL_KEYBOARD_KEY_STATE_PRESSED &&
        input_callbacks.keyboard_keydown != NULL)
    {
        key_pressed_before = last_key_pressed;
        last_key_pressed = key;
        input_callbacks.keyboard_keydown(key);
    }
    if (state == WL_KEYBOARD_KEY_STATE_RELEASED &&
        input_callbacks.keyboard_keyup != NULL)
        input_callbacks.keyboard_keyup(key);
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
    if (input_callbacks.keyboard_modifier != NULL)
        input_callbacks.keyboard_modifier(pressed, toggled, locked);
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
const keyboard_monitor_t keyboard_listener = {HKM, HKE,  HKL,
                                              HKK, HKMO, HKR};

//-----------------------------------------------------------------------//
// The functions below this point are methods that access private values //
// linked only to this translation unit (i.e. the capabilities of the    //
// input group), and are simply getters. The reason they are not inline, //
// and are contained within this file instead of the header, is because  //
// of their private nature.                                              //
//-----------------------------------------------------------------------//

const uint32_t GetLastKeyPressed(void) { return last_key_pressed; }
const uint64_t GetLastKeyCombo(void)
{
    uint64_t key_combo =
        (uint64_t)key_pressed_before << 32 | last_key_pressed;
    return key_combo;
}
