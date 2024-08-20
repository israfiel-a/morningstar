#include "Thread.h"
#include <Output/Error.h>
#include <errno.h>

const pthread_t CreateThread(void* (*func)(void*), void* args)
{
    pthread_t thread;
    int ret = pthread_create(&thread, NULL, func, args);
    switch (ret)
    {
        case EAGAIN: ReportError(thread_no_resources);
        case EINVAL: break; // impossible
        case EPERM:  ReportError(thread_open_denied);
        default:     break;
    }

    return thread;
}
