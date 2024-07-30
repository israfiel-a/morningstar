/**
 * @file Monitor.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides functionality to retrieve information about monitors /
 * the main monitor of the user. A lot of the code within this file is
 * repurposed from the wonderful windowing library GLFW. (GLFW::Monitor.c)
 * @date 2024-07-27
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_MONITOR_DIAGNOSTIC_SYSTEM_
#define _MSENG_MONITOR_DIAGNOSTIC_SYSTEM_

// The project's master system include file.
#include <Master.h>
// Wayland interface type definitions.
#include <Windowing/Types.h>

/**
 * @brief The information associated with a specific hardware monitor,
 * including width, height, refresh rate, and scale, among other things.
 * This object is filled out automatically by Wayland event listeners.
 */
typedef struct
{
    /**
     * @brief The physical width of the monitor. Wayland reports this in
     * millimeters.
     */
    int32_t physical_width;
    /**
     * @brief The physical height of the monitor. Wayland reports this in
     * millimeters.
     */
    int32_t physical_height;
    /**
     * @brief The width of the monitor in pixels.
     */
    int32_t width;
    /**
     * @brief The height of the monitor in pixels.
     */
    int32_t height;
    /**
     * @brief The smallest side of the monitor, used to contrict viewport
     * size and things of the like.
     */
    int32_t sside;
    /**
     * @brief The X position of the monitor within the global interface, in
     * pixels (I assume?).
     */
    int32_t x;
    /**
     * @brief The Y position of the monitor within the global interface, in
     * pixels (I assume?).
     */
    int32_t y;
    /**
     * @brief The vertical refresh rate of the monitor. Reported in
     * millihertz, recorded here in regular old hertz.
     */
    int16_t refresh_rate;
    /**
     * @brief The scale of the monitor, or the transform set by Wayland to
     * any buffers rendered. This is usually 1.
     */
    int16_t scale;
    /**
     * @brief Any addiitional transformation spells cast upon any buffers
     * sent to the renderer.
     */
    monitor_transform_t transform;
} monitor_t;

/**
 * @brief Bind a monitor to the registry if Wayland reports one exists.
 * This monitor object is destroyed quickly after being bound, as we just
 * grab some info from it and call it a day.
 * @param registry The registry to bind to.
 * @param name The numeric name of the monitor.
 * @param version The version of Wayland's monitor interface.
 */
void BindMonitor(registry_t* registry, const uint32_t name,
                 const uint32_t version);

/**
 * @brief Returns a pointer to the monitor which the game renders on; the
 * first monitor the Wayland compositor registers.
 * @return A pointer to the monitor.
 */
const monitor_t* GetMonitor(void);

/**
 * @brief Get the width of the primary monitor in pixels.
 * @return The pixel count.
 */
const int32_t GetMonitorWidth(void);

/**
 * @brief Get the height of the primary monitor in pixels.
 * @return The pixel count.
 */
const int32_t GetMonitorHeight(void);

/**
 * @brief Get the smallest side of the primary monitor.
 * @return The pixel count of the smallest side.
 */
const int32_t GetMonitorShortSide(void);

#endif // _MSENG_MONITOR_DIAGNOSTIC_SYSTEM_
