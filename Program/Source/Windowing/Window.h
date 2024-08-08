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

// The master include file for the project.
#include <Master.h>
// XRGB888 color codes.
#include <Rendering/Colors.h>
// Wayland/XDG type definitions.
#include <Utilities/WaylandTypes.h>

/**
 * @brief An enumerator for the various positions an application subwindow
 * can take. On a monitor much smaller than 16:9, none of these but @enum
 * centered matter at all.
 */
typedef enum __attribute__((__packed__))
{
    /**
     * @brief This window fills the gap to the right of the gameplay
     * window.
     */
    gap_filler_right,
    /**
     * @brief This window fills the gap to the left of the gameplay window.
     */
    gap_filler_left,
    /**
     * @brief This window fills the gap to the right of the gameplay
     * window, with a padding of .25 and centered.
     */
    gap_centered_right,
    /**
     * @brief This window fills the gap to the left of the gameplay window,
     * with a padding of .25 and centered.
     */
    gap_centered_left,
    /**
     * @brief This window is the gameplay window, and as such is centered
     * on either the Y or X axis, whichever is larger.
     */
    central_window,
    /**
     * @brief This window is the background surface for the application.
     * This surface is always a sheer black, with no other rendering done
     * to it beyond painting said black.
     */
    backdrop_window
} subsurface_type_t;

typedef enum __attribute__((__packed__))
{
    bust,
    stat,
    gameplay,
    backdrop
} requested_window_t;

void CreateWindows(void);
void SetWindowPositions(int32_t suggested_width, int32_t suggested_height);
void DestroyWindows(void);

raw_window_t* CreateRawWindow(void);
raw_subwindow_t* CreateRawSubwindow(raw_window_t** window,
                                    raw_window_t* parent);

window_t* GetBackgroundWindow(void);

raw_window_t* GetWindowRaw(requested_window_t requested);
raw_subwindow_t* GetSubwindowRaw(requested_window_t requested);
subsurface_type_t GetWindowType(requested_window_t requested);

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

void SendBlankColor(raw_window_t* window, subsurface_type_t type,
                    uint32_t color);

#endif // _MSENG_WINDOW_SYSTEM_
