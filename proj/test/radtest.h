/* -*- mode: c++ -*- */

/* Author:   Huanghao@cellcom
 * Date:     2005/09/29 19:48:05
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RAD_TEST_H__
#define __RAD_TEST_H__

typedef void (* lrad_test_func)();

struct lrad_bootlink : public lrad_slist {
    lrad_bootlink(lrad_test_func func, const char* func_name) {
        data = (void*)func;
        next = NULL;
        name = func_name;
    }
    const char* name;
    static lrad_bootlink* first;
};

struct lrad_bootlink_init {
    lrad_bootlink_init(lrad_bootlink* node) {
        lrad_bootlink::first
            = static_cast<lrad_bootlink*>(lrad_slist_cons(node, lrad_bootlink::first));
    }
};

#define RAD_TEST(func) \
    static void func(); \
    lrad_bootlink func##_node(func, #func); \
    lrad_bootlink_init func##_init(&func##_node); \
    void func()

void rad_run_test();

#endif /* __RAD_TEST_H__ */
