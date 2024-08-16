#include "File.h"
#include <Globals.h>
#include <Output/Error.h>
#include <Output/Warning.h>
#include <Windowing/System.h>
#include <errno.h>
#include <fcntl.h>
#include <memory.h>
#include <stdio.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

void HandleCommandLineArgs(int argc, char** argv) {}

static void HandleBufferDeletion(void* data, struct wl_buffer* buffer)
{
    wl_buffer_destroy(buffer);
}

static const struct wl_buffer_listener buffer_listener = {
    HandleBufferDeletion};

static struct wl_shm* shm_buffer = NULL;

void BindSHM(uint32_t name, uint32_t version)
{
    if (GetRegistry() == NULL)
    {
        ReportWarning(preemptive_shm_creation);
        return;
    }

    if (devices.shm)
    {
        ReportWarning(double_shm_creation);
        return;
    }

    shm_buffer =
        wl_registry_bind(GetRegistry(), name, &wl_shm_interface, version);
    devices.shm = true;
}

void UnbindSHM(void)
{
    if (!devices.shm)
    {
        ReportWarning(preemptive_shm_free);
        return;
    }

    wl_shm_destroy(shm_buffer);
    devices.shm = false;
}

static void CreateRandomFileName(char* filename_buffer)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long r = ts.tv_nsec;
    for (int i = 0; i < 6; ++i)
    {
        filename_buffer[i] = 'A' + (r & 15) + (r & 16) * 2;
        r >>= 5;
    }
}

int OpenSHM(size_t size)
{
    uint8_t retries = 100;
    do {
        char name[] = "/wl_shm-XXXXXX";
        CreateRandomFileName(name + sizeof(name) - 7);
        --retries;
        int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
        if (fd >= 0)
        {
            shm_unlink(name);
            if (fd < 0) ReportError(shm_open_failure);

            int ret;
            do {
                ret = ftruncate(fd, size);
            } while (ret < 0 && errno == EINTR);
            if (ret < 0) ReportError(shm_open_failure);

            return fd;
        }
    } while (retries > 0 && errno == EEXIST);
    ReportError(shm_open_failure);
}

struct wl_buffer* CreateSolidPixelBuffer(uint32_t width, uint32_t height,
                                         enum wl_shm_format format,
                                         uint32_t color)
{
    // In the Wayland coloring interface, every pixel takes 4 bytes, ARGB
    // or XRGB. Multiply the width by 4 to get the amount of bytes per row
    // within the buffer.
    const int32_t stride = width * 4, size = height * stride;
    int fd = OpenSHM(size);

    uint32_t* frame_data =
        mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (frame_data == MAP_FAILED) ReportError(mmap_failure);

    struct wl_shm_pool* pool = wl_shm_create_pool(shm_buffer, fd, size);
    struct wl_buffer* buffer =
        wl_shm_pool_create_buffer(pool, 0, width, height, stride, format);
    wl_shm_pool_destroy(pool);
    close(fd);

    // Some bizzare code stolen from
    // https://stackoverflow.com/questions/108866/is-there-memset-that-accepts-integers-larger-than-char
    uintptr_t i;
    for (i = 0; i < (size & (~7)); i += 4)
        memcpy(((char*)frame_data) + i, &color, 4);
    for (; i < size; i++) ((char*)frame_data)[i] = ((char*)&color)[i & 7];

    if (munmap(frame_data, size) == -1) ReportError(unmmap_failure);

    wl_buffer_add_listener(buffer, &buffer_listener, NULL);
    return buffer;
}

struct wl_shm* GetSHM(void) { return shm_buffer; }

bool ReadFileContents(const char* file_path, char* buffer,
                      size_t buffer_length)
{
    FILE* opened_file = fopen(file_path, "rb");
    if (opened_file)
    {
        int position_set_code = fseek(opened_file, 0, SEEK_END);
        if (position_set_code == -1) return false;
        size_t file_length = ftell(opened_file);
        if (file_length == -1) return false;
        position_set_code = fseek(opened_file, 0, SEEK_SET);
        if (position_set_code == -1) return false;

        size_t items_written = fread(
            buffer, 1,
            (buffer_length < file_length ? buffer_length : file_length),
            opened_file);
        if (items_written !=
            (buffer_length < file_length ? buffer_length : file_length))
            return false;

        fclose(opened_file);

        return true;
    }
    return false;
}
