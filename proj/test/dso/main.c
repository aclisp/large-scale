#include <radutil/radutil.h>
#include <assert.h>

RAD_SYMBOL
RAD_EXTERN_C int var;

RAD_SYMBOL
RAD_EXTERN_C int fun();

int main()
{
    var = 10;
    assert(fun() == 11);
    return 0;
}
