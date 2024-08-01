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

#include <Master.h>
#include <Utilities/Colors.h>
#include <WaylandTypes.h>

typedef struct
{
    uint32_t width;
    uint32_t height;
    uint32_t x;
    uint32_t y;
    raw_window_t* window;
    raw_subwindow_t* subwindow;
} subwindow_t;

void CreateMainWindow(void);
void CreateUIWindows(void);
void DestroyMainWindow(void);
void DestroyUIWindows(void);

window_t* GetBackgroundWindow(void);
const subwindow_t* GetBustWindow(void);
const subwindow_t* GetGameplayWindow(void);
const subwindow_t* GetStatWindow(void);

raw_window_t* GetBackgroundWindowRaw(void);
raw_window_t* GetBustRawWindow(void);
raw_window_t* GetGameplayRawWindow(void);
raw_window_t* GetStatRawWindow(void);

raw_subwindow_t* GetBustRawSubwindow(void);
raw_subwindow_t* GetGameplayRawSubwindow(void);
raw_subwindow_t* GetStatRawSubwindow(void);

#endif // _MSENG_WINDOW_SYSTEM_
