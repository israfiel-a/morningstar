#include "SharedMemory.h"
#include <Error.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>

void CreateRandomFileName(char* filename_buffer)
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

int32_t simple_round(double x)
{
    if (x < 0.0) return (int32_t)(x - 0.5);
    else return (int32_t)(x + 0.5);
}
