#include "radutil.h"
#include "radtest.h"
#include <stdio.h>

lrad_bootlink* lrad_bootlink::first = NULL;

void rad_run_test()
{
    radlog(L_INFO, "[rad_run_test] Begin");
    lrad_bootlink* next = lrad_bootlink::first;
    while(next) {
        lrad_test_func func = (lrad_test_func)next->data;
        printf("Test case [%s] ... ", next->name);
        func();
        printf("ok\n");
        next = static_cast<lrad_bootlink*>(next->next);
    }
    radlog(L_INFO, "[rad_run_test] End");
}
