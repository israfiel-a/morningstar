#include "Monitor.h"
#include <Application/Manager.h>
#include <Globals.h>
#include <Utilities/Math.h>

/**
 * @brief An object full of the various data we need about the user's
 * primary monitor. All values are initialized to zero by default.
 */
static monitor_t primary_monitor = {0, 0, 0, 0, 0, 0, 0.0f, 0, 0, 0, 0, 0};

/**
 * @brief The actual Wayland-reported monitor interface. This object is
 * freed (and set to NULL!) during the @ref HandleMonitorInformationSent
 * function, after which it is no longer needed.
 */
static monitor_information_t* monitor_information;

/**
 * @brief Handle the actual physical geometry of the monitor.
 * @param d Nothing of use.
 * @param m Nothing of use.
 * @param x The X position of the monitor within the Wayland global
 * compositor space.
 * @param y The Y position of the monitor within the Wayland global
 * compositor space.
 * @param physical_width The physical width of the monitor in millimeters.
 * @param physical_height The physical height of the monitor in
 * millimeters.
 * @param s Nothing of use.
 * @param mk Nothing of use.
 * @param md Nothing of use.
 * @param transform Any additional transformations applied to buffers
 * during rendering.
 */
static void HMG(void* d, monitor_information_t* m, int32_t x, int32_t y,
                int32_t physical_width, int32_t physical_height, int32_t s,
                const char* mk, const char* md, int32_t transform)
{
    primary_monitor.x = x;
    primary_monitor.y = y;
    primary_monitor.physical_width = physical_width;
    primary_monitor.physical_height = physical_height;
    primary_monitor.transform = transform;
}

/**
 * @brief Handle the video mode of the monitor, and all parameters that
 * come with that.
 * @param d Nothing of use.
 * @param m Nothing of use.
 * @param flags Nothing of use.
 * @param width The width of the monitor in pixels.
 * @param height The height of the monitor in pixels.
 * @param refresh The refresh rate of the monitor in millihertz.
 */
static void HMV(void* d, monitor_information_t* m, uint32_t f,
                int32_t width, int32_t height, int32_t refresh)
{
    primary_monitor.width = width;
    primary_monitor.height = height;
    primary_monitor.refresh_rate =
        (int16_t)simple_round(refresh / 10000.0);
}

/**
 * @brief Handle the final bits of information before the stream of data
 * ends. We use this time to calculate some parameters and release others.
 * @param d Nothing of use.
 * @param m Nothing of use.
 */
static void HMD(void* d, monitor_information_t* m)
{
    primary_monitor.shortest_size_measurement =
        int32_min(primary_monitor.width, primary_monitor.height);
    if (primary_monitor.shortest_size_measurement ==
            primary_monitor.height &&
        primary_monitor.shortest_size_measurement == primary_monitor.width)
        primary_monitor.shortest_side = monitor_square;
    else if (primary_monitor.shortest_size_measurement ==
             primary_monitor.height)
        primary_monitor.shortest_side = monitor_height;
    else
    {
        global_flags.application_mode = unified_locked;
        primary_monitor.shortest_side = monitor_width;
    }

    if (primary_monitor.scale == 0) primary_monitor.scale = 1;
    primary_monitor.ratio = (float)primary_monitor.physical_width /
                            primary_monitor.physical_height;
    if (primary_monitor.ratio == 1)
        global_flags.application_mode = unified_locked;

    // We no longer need the full list of monitor information since
    // we've recorded all we need.
    wl_output_release(monitor_information);
    monitor_information = NULL;
    global_flags.monitor_already_polled = true;
}

/**
 * @brief Handle information having to do with scaling buffers and
 * geometry.
 * @param d Nothing of use.
 * @param m Nothing of use.
 * @param factor The scale factor Wayland uses when transforming buffers
 */
static void HMS(void* d, monitor_information_t* m, int32_t factor)
{
    primary_monitor.scale = factor;
}

/**
 * @brief Unused function to poll for the given monitor's model name.
 */
NOOP HMN(void* d, monitor_information_t* m, const char* n) {}

/**
 * @brief Unused function to poll for the given monitor's description.
 */
NOOP HMDE(void* d, monitor_information_t* m, const char* s) {}

/**
 * @brief The listener for a monitor. The functions held within the
 * listener will fire off as Wayland polls for info about a monitor, and we
 * will fill out the primary monitor's details. This listener is only used
 * once, at which point it is freed with the Wayland monitor object.
 */
static const monitor_information_monitor_t monitor_listener = {
    HMG, HMV, HMD, HMS, HMN, HMDE};

//-----------------------------------------------------------------------//
// The functions below this point are methods that access private values //
// linked only to this translation unit (i.e. the primary monitor), and  //
// are simply getters. The reason they are not inline, and are           //
// contained within this file instead of the header, is because of their //
// private nature.                                                       //
//-----------------------------------------------------------------------//

const monitor_t* GetMonitor(void) { return &primary_monitor; }
const int32_t GetMonitorWidth(void) { return primary_monitor.width; }
const int32_t GetMonitorHeight(void) { return primary_monitor.height; }
const int32_t GetMonitorShortSide(void)
{
    return primary_monitor.shortest_size_measurement;
}
const monitor_shortest_side_t GetMonitorShortSideName(void)
{
    return primary_monitor.shortest_side;
}
const float GetMonitorSizeRatio(void) { return primary_monitor.ratio; }

void BindMonitor(registry_t* registry, const uint32_t name,
                 const uint32_t version)
{
    // Make sure we're given information about the monitor by Wayland once
    // it procures it.
    monitor_information =
        wl_registry_bind(registry, name, &wl_output_interface, version);
    wl_output_add_listener(monitor_information, &monitor_listener, NULL);
}
