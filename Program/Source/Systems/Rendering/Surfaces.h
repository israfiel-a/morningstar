/**
 * @file Surfaces.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Utilites for dealing with sending pixel buffers to surfaces /
 * subsurfaces.
 * @date 2024-08-01
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_SURFACES_RENDERING_SYSTEM_
#define _MSENG_SURFACES_RENDERING_SYSTEM_

#include <Master.h>
#include <Utilities/WaylandTypes.h>
#include <Windowing/Window.h>

void SendBlankColor(raw_window_t* window, subsurface_size_t size,
                    uint32_t color);

void NoteSuggestedBounds(int32_t width, int32_t height);
int32_t GetSuggestedWidth(void);
int32_t GetSuggestedHeight(void);
int32_t GetShortestSide(void);

void SetSubwindowPosition(subwindow_t* subwindow);

#endif // _MSENG_SURFACES_RENDERING_SYSTEM_
