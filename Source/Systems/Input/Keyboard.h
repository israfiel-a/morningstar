/**
 * @file Keyboard.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides functionality for the keyboard object.
 * @date 2024-07-29
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_KEYBOARD_INPUT_SYSTEM_
#define _MSENG_KEYBOARD_INPUT_SYSTEM_

// The master include file of the project.
#include <Master.h>

/**
 * @brief Get the 32-bit unsigned integer representation of the key just
 * pressed. If this returns 0, nothing has been pressed.
 * @return The key value.
 */
const uint32_t GetLastKeyPressed(void);

/**
 * @brief Since all key combos are two keys within the engine (and any game
 * made with it), just simply bitshift the keys to the left until we have a
 * 64-bit key combo. The user will retrieve these keys by un-shifting them.
 * @return A bitshifted record of the last two keys pressed.
 */
const uint64_t GetLastKeyCombo(void);

#endif // _MSENG_KEYBOARD_INPUT_SYSTEM_
