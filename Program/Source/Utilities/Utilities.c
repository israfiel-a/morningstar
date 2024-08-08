#include <Output/Error.h>
#include <fcntl.h>
#include <sys/mman.h>

int32_t simple_round(double x)
{
    if (x < 0.0) return (int32_t)(x - 0.5);
    else return (int32_t)(x + 0.5);
}
