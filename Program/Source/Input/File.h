/**
 * @file Terminal.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides functionality to handle all sorts of file input, from
 * reading files to handling command line arguments and shared
 * memory files.
 * @date 2024-07-29
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_TERMINAL_INPUT_SYSTEM_
#define _MSENG_TERMINAL_INPUT_SYSTEM_

#include <inttypes.h>
#include <wayland-client-protocol.h>

/**
 * @brief A function to handle any arguments passed to the main executable.
 * This often is used to tweak performance settings.
 * @param argc The count of arguments as given by the command line.
 * @param argv The actual arguments.
 * @note UNIMPLEMENTED -- no valid params
 */
void HandleCommandLineArgs(int argc, char** argv);

struct wl_buffer* CreateSolidPixelBuffer(uint32_t width, uint32_t height,
                                         enum wl_shm_format format,
                                         uint32_t color);

int OpenSHM(size_t size);

void BindSHM(uint32_t name, uint32_t version);
void UnbindSHM(void);
struct wl_shm* GetSHM(void);

_Bool ReadFileContents(const char* file_path, char* buffer,
                       size_t buffer_length);

#endif // _MSENG_TERMINAL_INPUT_SYSTEM_
