#include "radutil.h"
#include "radtest.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const LRAD_NAME_MAP names[] = {
    { "__a__", "1", }, /* order is important! */
    { "__b__", "2", },
    { "__c__", "3", },
    { "__d__", "4", },
    { "__e__", "5", },
    { "__f__", "6", },
    { "__g__", "7", },
    { NULL   , NULL,},
};

RAD_TEST(lrad_stra2b)
{
    rad_assert(strcmp(lrad_stra2b(names, "__d__", NULL), "4") == 0);
    rad_assert(lrad_stra2b(names, "__h__", NULL) == NULL);
}

RAD_TEST(lrad_strb2a)
{
    rad_assert(strcmp(lrad_strb2a(names, "7", "__a__"), "__g__") == 0);
    rad_assert(strcmp(lrad_strb2a(names, "8", "__?__"), "__?__") == 0);
}

static const LRAD_NAME_NUMBER tokens[] = {
    { "__a__", 1, }, /* order is important! */
    { "__b__", 2, },
    { "__c__", 3, },
    { "__d__", 4, },
    { "__e__", 5, },
    { "__f__", 6, },
    { "__g__", 7, },
    { NULL   , 0, },
};

RAD_TEST(lrad_str2int)
{
    rad_assert(lrad_str2int(tokens, "__d__", 0) == 4);
    rad_assert(lrad_str2int(tokens, "__h__", -1) == -1);
}

RAD_TEST(lrad_int2str)
{
    rad_assert(strcmp(lrad_int2str(tokens, 7, "__a__"), "__g__") == 0);
    rad_assert(strcmp(lrad_int2str(tokens, 8, "__?__"), "__?__") == 0);
}
