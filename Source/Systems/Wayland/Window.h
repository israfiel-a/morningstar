/**
 * @file Window.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Functionality for the creation of the window surface within the
 * application.
 * @date 2024-07-27
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_WINDOW_SYSTEM_
#define _MSENG_WINDOW_SYSTEM_

#include "Types.h"
#include <Master.h>
#include <Utilities/Colors.h>

void SetWindowTitle(const char* id, const char* title);

pixel_buffer_t* PaintWindowBackground(uint32_t background_color);

#endif // _MSENG_WINDOW_SYSTEM_
