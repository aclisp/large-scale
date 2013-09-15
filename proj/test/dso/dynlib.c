#include <radutil/radutil.h>

RAD_SYMBOL
int var = 0;

RAD_SYMBOL
int fun()
{
    var += 1;
    return var;
}
