#include "radutil.h"
#include <stdlib.h>

void rad_assert_fail(const char *file, unsigned int line, const char* expr)
{
	radlog(L_FATAL|L_CONS, "ASSERTION FAILED in %s, line %u: %s", file, line, expr);
	abort();
}
