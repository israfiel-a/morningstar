#include "SHM.h"
#include <Memory/Fill.h>
#include <Output/Error.h>
#include <Windowing/System.h>
#include <fcntl.h>
#include <time.h>

static void HandleBufferDeletion(void* data, pixel_buffer_t* buffer)
{
    wl_buffer_destroy(buffer);
}

static const pixel_buffer_monitor_t buffer_listener = {
    HandleBufferDeletion};

static shared_memory_buffer_t* shm_buffer = NULL;

void BindSHM(uint32_t name, uint32_t version)
{
    shm_buffer =
        wl_registry_bind(GetRegistry(), name, &wl_shm_interface, version);
}

void UnbindSHM(void) { wl_shm_destroy(shm_buffer); }

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

pixel_buffer_t* CreateSolidPixelBuffer(uint32_t width, uint32_t height,
                                       color_type_t format, uint32_t color)
{
    // In the Wayland coloring interface, every pixel takes 4 bytes, ARGB
    // or XRGB. Multiply the width by 4 to get the amount of bytes per row
    // within the buffer.
    const int32_t stride = width * 4, size = height * stride;
    int fd = OpenSHM(size);

    uint32_t* frame_data =
        mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (frame_data == MAP_FAILED) ReportError(mmap_failure);

    shared_memory_pool_t* pool = wl_shm_create_pool(shm_buffer, fd, size);
    pixel_buffer_t* buffer =
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

shared_memory_buffer_t* GetSHM(void) { return shm_buffer; }
