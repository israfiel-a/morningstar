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
#include <WaylandTypes.h>

void SendBlankColor(raw_window_t* window, uint32_t width, uint32_t height,
                    uint32_t color);

#endif // _MSENG_SURFACES_RENDERING_SYSTEM_
