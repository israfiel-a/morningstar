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

typedef struct
{
    uint32_t width;
    uint32_t height;
    uint32_t x;
    uint32_t y;
    window_raw_t* window;
    subwindow_raw_t* subwindow;
} subwindow_t;

void AssignSubwindowDimensions(void);
void AssignSubwindowPositions(void);
void CreateSubwindows(void);

pixel_buffer_t* GenerateWindowBackground(void);

pixel_buffer_t* GenerateSubwindowBackground(const subwindow_t* window,
                                            uint32_t background_color);

const subwindow_t* GetBustWindow(void);
const subwindow_t* GetGameplayWindow(void);
const subwindow_t* GetStatWindow(void);

window_raw_t* GetBustRawWindow(void);
window_raw_t* GetGameplayRawWindow(void);
window_raw_t* GetStatRawWindow(void);

subwindow_raw_t* GetBustRawSubwindow(void);
subwindow_raw_t* GetGameplayRawSubwindow(void);
subwindow_raw_t* GetStatRawSubwindow(void);

void SetWindowTitle(const char* id, const char* title);

#endif // _MSENG_WINDOW_SYSTEM_
