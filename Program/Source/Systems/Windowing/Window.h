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
#include <Utilities/WaylandTypes.h>

typedef enum
{
    gap_filler_right,
    gap_filler_left,
    gap_centered_right,
    gap_centered_left,
    centered
} subsurface_position_t;

typedef enum
{
    gap_filler,
    gap_filler_centered,
    main_window,
    background_window
} subsurface_size_t;

typedef struct
{
    subsurface_size_t size;
    subsurface_position_t position;
    raw_window_t* window;
    raw_subwindow_t* subwindow;
} subwindow_t;

void CreateWindows(void);
void SetWindowPositions(int32_t suggested_width, int32_t suggested_height);
void DestroyWindows(void);

raw_window_t* CreateRawWindow(void);
raw_subwindow_t* CreateRawSubwindow(raw_window_t** window,
                                    raw_window_t* parent);

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

compositor_t* GetCompositor(void);
subcompositor_t* GetSubcompositor(void);

void BindCompositor(uint32_t name, uint32_t version);
void BindSubcompositor(uint32_t name, uint32_t version);

void UnbindCompositor(void);
void UnbindSubcompositor(void);

void NoteSuggestedBounds(int32_t width, int32_t height);
int32_t GetSuggestedWidth(void);
int32_t GetSuggestedHeight(void);
int32_t GetShortestSide(void);

void SetSubwindowPosition(subwindow_t* subwindow);
void SendBlankColor(raw_window_t* window, subsurface_size_t size,
                    uint32_t color);

#endif // _MSENG_WINDOW_SYSTEM_
