#include "radutil.h"
#include "radalloc.hpp"

void * rad_track_alloc::allocate(size_t size,
                                 const char *file,
                                 unsigned int line)
{
    return rad_malloc_track(size, file, line);
}

void rad_track_alloc::deallocate(void *p)
{
    rad_free_track(p);
}
