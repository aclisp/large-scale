#include "radutil.h"
#include "radtest.h"

static void* mem0 = NULL;
static void* mem1 = NULL;
static void* mem2 = NULL;
static void* mem3 = NULL;

/* Note: The test case appears earlier will be invoked later. */

RAD_TEST(leak3)
{
    rad_free(mem3);
    rad_free(mem1);
}

RAD_TEST(leak2)
{
    rad_free(mem2);
    rad_free(mem0);
}

RAD_TEST(leak1)
{
    void* mem = NULL;
    rad_verify(mem  = rad_malloc(4));

    rad_verify(mem0 = rad_malloc(0));
    rad_verify(mem1 = rad_malloc(1));
    rad_verify(mem2 = rad_malloc(2));
    rad_verify(mem3 = rad_malloc(3));

    rad_free(mem);
}
