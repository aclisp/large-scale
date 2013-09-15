#include "radutil.h"
#include "radtest.h"
#include <string.h>
#include <stdio.h>

typedef struct _SHIFT_ARRAY {
#define SHIFT_ARRAY_MAXCNT 8
    lrad_shift_entry entry;
    int data[SHIFT_ARRAY_MAXCNT];
} SHIFT_ARRAY;

void shift_array_init(SHIFT_ARRAY* sa)
{
    memset(sa, 0, sizeof(SHIFT_ARRAY));
}

void shift_array_push_back(SHIFT_ARRAY* sa, int d)
{
    sa->data[
        lrad_shift_get_push_back_index(&sa->entry.fr,
                                       &sa->entry.to,
                                       SHIFT_ARRAY_MAXCNT)
        ] = d;
}

void shift_array_push_front(SHIFT_ARRAY* sa, int d)
{
    sa->data[
        lrad_shift_get_push_front_index(&sa->entry.fr,
                                        &sa->entry.to,
                                        SHIFT_ARRAY_MAXCNT)
        ] = d;
}

int shift_array_pop_back(SHIFT_ARRAY* sa)
{
    unsigned int n = lrad_shift_get_pop_back_index(&sa->entry.fr,
                                                   &sa->entry.to,
                                                   SHIFT_ARRAY_MAXCNT);
    if(n == LRAD_SHIFT_TOINFI)
        return -1;

    return sa->data[n];
}

int shift_array_pop_front(SHIFT_ARRAY* sa)
{
    unsigned int n = lrad_shift_get_pop_front_index(&sa->entry.fr,
                                                    &sa->entry.to,
                                                    SHIFT_ARRAY_MAXCNT);
    if(n == LRAD_SHIFT_TOINFI)
        return -1;

    return sa->data[n];
}

void shift_array_dump(char* buf, size_t len, const SHIFT_ARRAY* sa)
{
    int wb;

    wb = sNprintf(buf, len, "[");
    buf += wb;
    len -= wb;

    size_t cnt = LRAD_SHIFT_LENGTH(sa->entry.fr,
                                   sa->entry.to,
                                   SHIFT_ARRAY_MAXCNT);
    for(size_t n=0, i=sa->entry.fr; n<cnt;
        ++n, LRAD_SHIFT_INCIDX(i, SHIFT_ARRAY_MAXCNT)) {
        wb = sNprintf(buf, len, "%d, ", sa->data[i]);
        buf += wb;
        len -= wb;
    }

    sNprintf(buf, len, "]");
}

RAD_TEST(shift_array_pop_front)
{
    SHIFT_ARRAY array;
    char buf[256];

    shift_array_init(&array);
    rad_assert(array.entry.fr == 0);
    rad_assert(array.entry.to == 0);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[]") == 0);

    int i=1;

    for(; i<=8; ++i)
        shift_array_push_back(&array, i);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[1, 2, 3, 4, 5, 6, 7, 8, ]") == 0);

    rad_assert(shift_array_pop_front(&array) == 1);
    rad_assert(shift_array_pop_front(&array) == 2);
    rad_assert(shift_array_pop_front(&array) == 3);
    rad_assert(shift_array_pop_front(&array) == 4);
    rad_assert(shift_array_pop_front(&array) == 5);
    rad_assert(shift_array_pop_front(&array) == 6);
    rad_assert(shift_array_pop_front(&array) == 7);
    rad_assert(shift_array_pop_front(&array) == 8);
    rad_assert(shift_array_pop_front(&array) == -1);
    rad_assert(shift_array_pop_front(&array) == -1);

    for(; i<=28; ++i)
        shift_array_push_back(&array, i);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[21, 22, 23, 24, 25, 26, 27, 28, ]") == 0);

    rad_assert(shift_array_pop_front(&array) == 21);
    rad_assert(shift_array_pop_front(&array) == 22);
    rad_assert(shift_array_pop_front(&array) == 23);
    rad_assert(shift_array_pop_front(&array) == 24);
    rad_assert(shift_array_pop_front(&array) == 25);
    rad_assert(shift_array_pop_front(&array) == 26);
    rad_assert(shift_array_pop_front(&array) == 27);
    rad_assert(shift_array_pop_front(&array) == 28);
    rad_assert(shift_array_pop_front(&array) == -1);
    rad_assert(shift_array_pop_front(&array) == -1);

    for(i=1; i<=13; ++i)
        shift_array_push_back(&array, i);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[6, 7, 8, 9, 10, 11, 12, 13, ]") == 0);

    rad_assert(shift_array_pop_front(&array) == 6);
    rad_assert(shift_array_pop_back(&array) == 13);
    rad_assert(shift_array_pop_front(&array) == 7);
    rad_assert(shift_array_pop_back(&array) == 12);
}

RAD_TEST(shift_array_pop_back)
{
    SHIFT_ARRAY array;
    char buf[256];

    shift_array_init(&array);
    rad_assert(array.entry.fr == 0);
    rad_assert(array.entry.to == 0);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[]") == 0);

    int i=1;

    for(; i<=8; ++i)
        shift_array_push_front(&array, i);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[8, 7, 6, 5, 4, 3, 2, 1, ]") == 0);

    rad_assert(shift_array_pop_back(&array) == 1);
    rad_assert(shift_array_pop_back(&array) == 2);
    rad_assert(shift_array_pop_back(&array) == 3);
    rad_assert(shift_array_pop_back(&array) == 4);
    rad_assert(shift_array_pop_back(&array) == 5);
    rad_assert(shift_array_pop_back(&array) == 6);
    rad_assert(shift_array_pop_back(&array) == 7);
    rad_assert(shift_array_pop_back(&array) == 8);
    rad_assert(shift_array_pop_back(&array) == -1);
    rad_assert(shift_array_pop_back(&array) == -1);

    for(; i<=28; ++i)
        shift_array_push_front(&array, i);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[28, 27, 26, 25, 24, 23, 22, 21, ]") == 0);

    rad_assert(shift_array_pop_back(&array) == 21);
    rad_assert(shift_array_pop_back(&array) == 22);
    rad_assert(shift_array_pop_back(&array) == 23);
    rad_assert(shift_array_pop_back(&array) == 24);
    rad_assert(shift_array_pop_back(&array) == 25);
    rad_assert(shift_array_pop_back(&array) == 26);
    rad_assert(shift_array_pop_back(&array) == 27);
    rad_assert(shift_array_pop_back(&array) == 28);
    rad_assert(shift_array_pop_back(&array) == -1);
    rad_assert(shift_array_pop_back(&array) == -1);
}

RAD_TEST(shift_array_push_front)
{
    SHIFT_ARRAY array;
    char buf[256];

    shift_array_init(&array);
    rad_assert(array.entry.fr == 0);
    rad_assert(array.entry.to == 0);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[]") == 0);

    int i=1;

    for(; i<=5; ++i)
        shift_array_push_front(&array, i);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[5, 4, 3, 2, 1, ]") == 0);

    for(; i<=8; ++i)
        shift_array_push_front(&array, i);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[8, 7, 6, 5, 4, 3, 2, 1, ]") == 0);

    for(; i<=12; ++i)
        shift_array_push_front(&array, i);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[12, 11, 10, 9, 8, 7, 6, 5, ]") == 0);

    for(; i<=15; ++i)
        shift_array_push_front(&array, i);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[15, 14, 13, 12, 11, 10, 9, 8, ]") == 0);

    for(; i<=75; ++i)
        shift_array_push_front(&array, i);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[75, 74, 73, 72, 71, 70, 69, 68, ]") == 0);

    for(i=1; i<=2; ++i)
        shift_array_push_back(&array, i);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[73, 72, 71, 70, 69, 68, 1, 2, ]") == 0);

    for(; i<=7; ++i)
        shift_array_push_back(&array, i);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[68, 1, 2, 3, 4, 5, 6, 7, ]") == 0);
}

RAD_TEST(shift_array_push_back)
{
    SHIFT_ARRAY array;
    char buf[256];

    shift_array_init(&array);
    rad_assert(array.entry.fr == 0);
    rad_assert(array.entry.to == 0);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[]") == 0);

    int i=1;

    for(; i<=5; ++i)
        shift_array_push_back(&array, i);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[1, 2, 3, 4, 5, ]") == 0);

    for(; i<=8; ++i)
        shift_array_push_back(&array, i);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[1, 2, 3, 4, 5, 6, 7, 8, ]") == 0);

    for(; i<=12; ++i)
        shift_array_push_back(&array, i);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[5, 6, 7, 8, 9, 10, 11, 12, ]") == 0);

    for(; i<=15; ++i)
        shift_array_push_back(&array, i);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[8, 9, 10, 11, 12, 13, 14, 15, ]") == 0);

    for(; i<=75; ++i)
        shift_array_push_back(&array, i);
    shift_array_dump(buf, sizeof(buf), &array);
    rad_assert(strcmp(buf, "[68, 69, 70, 71, 72, 73, 74, 75, ]") == 0);
}

RAD_TEST(_dimof)
{
    typedef struct _USER_INFO {
        char name[15];
        unsigned char age;
        char addr[64];
    } USER_INFO;

    USER_INFO user_info[10];
    memset(user_info, 0, sizeof(user_info));

    rad_assert(dimof(user_info) == 10);
}

typedef struct _LINK_ARRAY_ITEM {
    lrad_link_entry entry;
    int data;
} LINK_ARRAY_ITEM;

void linked_array_dump(char* buf, size_t len, const LINK_ARRAY_ITEM* la)
{
    int wb;

    wb = sNprintf(buf, len, "[");
    buf += wb;
    len -= wb;

    for(size_t i = la[1].entry.next; i != 1; i = la[i].entry.next) {
        wb = sNprintf(buf, len, "%d, ", la[i].data);
        buf += wb;
        len -= wb;
    }

    sNprintf(buf, len, "]");
}

unsigned int linked_array_find(const LINK_ARRAY_ITEM* la, int data)
{
    for(size_t i = la[1].entry.next; i != 1; i = la[i].entry.next) {
        if(la[i].data == data)
            return i;
    }
    return 0;
}

RAD_TEST(linked_array)
{
    char buf[256];
    LINK_ARRAY_ITEM array[10];
    unsigned short index;
    unsigned short maxidx;
    LINK_ARRAY_ITEM* item;

    lrad_link_init_head((char*)array, sizeof(array[0]), sizeof(array));
    maxidx = dimof(array) - 1;
    rad_assert(lrad_link_is_empty((char*)array, sizeof(array[0])));

    index = lrad_link_alloc_entry((char*)array, sizeof(array[0]), sizeof(array));
    rad_assert(index > 1 && index <= maxidx);
    item = (LINK_ARRAY_ITEM*)lrad_link_insert_tail(index, (char*)array, sizeof(array[0]));
    item->data = 0;

    index = lrad_link_alloc_entry((char*)array, sizeof(array[0]), sizeof(array));
    rad_assert(index > 1 && index <= maxidx);
    item = (LINK_ARRAY_ITEM*)lrad_link_insert_tail(index, (char*)array, sizeof(array[0]));
    item->data = 1;

    index = lrad_link_alloc_entry((char*)array, sizeof(array[0]), sizeof(array));
    rad_assert(index > 1 && index <= maxidx);
    item = (LINK_ARRAY_ITEM*)lrad_link_insert_tail(index, (char*)array, sizeof(array[0]));
    item->data = 2;

    index = lrad_link_alloc_entry((char*)array, sizeof(array[0]), sizeof(array));
    rad_assert(index > 1 && index <= maxidx);
    item = (LINK_ARRAY_ITEM*)lrad_link_insert_tail(index, (char*)array, sizeof(array[0]));
    item->data = 3;

    index = lrad_link_alloc_entry((char*)array, sizeof(array[0]), sizeof(array));
    rad_assert(index > 1 && index <= maxidx);
    item = (LINK_ARRAY_ITEM*)lrad_link_insert_tail(index, (char*)array, sizeof(array[0]));
    item->data = 4;

    index = lrad_link_alloc_entry((char*)array, sizeof(array[0]), sizeof(array));
    rad_assert(index > 1 && index <= maxidx);
    item = (LINK_ARRAY_ITEM*)lrad_link_insert_tail(index, (char*)array, sizeof(array[0]));
    item->data = 5;

    index = lrad_link_alloc_entry((char*)array, sizeof(array[0]), sizeof(array));
    rad_assert(index > 1 && index <= maxidx);
    item = (LINK_ARRAY_ITEM*)lrad_link_insert_tail(index, (char*)array, sizeof(array[0]));
    item->data = 6;

    index = lrad_link_alloc_entry((char*)array, sizeof(array[0]), sizeof(array));
    rad_assert(index > 1 && index <= maxidx);
    item = (LINK_ARRAY_ITEM*)lrad_link_insert_tail(index, (char*)array, sizeof(array[0]));
    item->data = 7;

    index = lrad_link_alloc_entry((char*)array, sizeof(array[0]), sizeof(array));
    rad_assert(index == 0); // Can not alloc any more

    linked_array_dump(buf, sizeof(buf), array);
    rad_assert(strcmp(buf, "[0, 1, 2, 3, 4, 5, 6, 7, ]") == 0);

    item = (LINK_ARRAY_ITEM*)lrad_link_remove_head((char*)array, sizeof(array[0]));
    rad_assert(item->data == 0);

    item = (LINK_ARRAY_ITEM*)lrad_link_remove_head((char*)array, sizeof(array[0]));
    rad_assert(item->data == 1);

    item = (LINK_ARRAY_ITEM*)lrad_link_remove_head((char*)array, sizeof(array[0]));
    rad_assert(item->data == 2);

    item = (LINK_ARRAY_ITEM*)lrad_link_remove_tail((char*)array, sizeof(array[0]));
    rad_assert(item->data == 7);

    item = (LINK_ARRAY_ITEM*)lrad_link_remove_tail((char*)array, sizeof(array[0]));
    rad_assert(item->data == 6);

    linked_array_dump(buf, sizeof(buf), array);
    rad_assert(strcmp(buf, "[3, 4, 5, ]") == 0);

    index = lrad_link_alloc_entry((char*)array, sizeof(array[0]), sizeof(array));
    rad_assert(index > 1 && index <= maxidx);
    item = (LINK_ARRAY_ITEM*)lrad_link_insert_head(index, (char*)array, sizeof(array[0]));
    item->data = 10;

    index = lrad_link_alloc_entry((char*)array, sizeof(array[0]), sizeof(array));
    rad_assert(index > 1 && index <= maxidx);
    item = (LINK_ARRAY_ITEM*)lrad_link_insert_head(index, (char*)array, sizeof(array[0]));
    item->data = 11;

    index = lrad_link_alloc_entry((char*)array, sizeof(array[0]), sizeof(array));
    rad_assert(index > 1 && index <= maxidx);
    item = (LINK_ARRAY_ITEM*)lrad_link_insert_head(index, (char*)array, sizeof(array[0]));
    item->data = 12;

    index = lrad_link_alloc_entry((char*)array, sizeof(array[0]), sizeof(array));
    rad_assert(index > 1 && index <= maxidx);
    item = (LINK_ARRAY_ITEM*)lrad_link_insert_tail(index, (char*)array, sizeof(array[0]));
    item->data = 13;

    index = lrad_link_alloc_entry((char*)array, sizeof(array[0]), sizeof(array));
    rad_assert(index > 1 && index <= maxidx);
    item = (LINK_ARRAY_ITEM*)lrad_link_insert_head(index, (char*)array, sizeof(array[0]));
    item->data = 14;

    linked_array_dump(buf, sizeof(buf), array);
    rad_assert(strcmp(buf, "[14, 12, 11, 10, 3, 4, 5, 13, ]") == 0);

    index = linked_array_find(array, 1);
    rad_assert(index == 0);

    index = linked_array_find(array, 10);
    rad_assert(index > 1 && index <= maxidx);
    lrad_link_remove_entry_at(index, (char*)array, sizeof(array[0]));
    linked_array_dump(buf, sizeof(buf), array);
    rad_assert(strcmp(buf, "[14, 12, 11, 3, 4, 5, 13, ]") == 0);

    index = linked_array_find(array, 5);
    rad_assert(index > 1 && index <= maxidx);
    lrad_link_remove_entry_at(index, (char*)array, sizeof(array[0]));
    linked_array_dump(buf, sizeof(buf), array);
    rad_assert(strcmp(buf, "[14, 12, 11, 3, 4, 13, ]") == 0);

    index = linked_array_find(array, 14);
    rad_assert(index > 1 && index <= maxidx);
    lrad_link_remove_entry_at(index, (char*)array, sizeof(array[0]));
    linked_array_dump(buf, sizeof(buf), array);
    rad_assert(strcmp(buf, "[12, 11, 3, 4, 13, ]") == 0);

    index = linked_array_find(array, 13);
    rad_assert(index > 1 && index <= maxidx);
    lrad_link_remove_entry_at(index, (char*)array, sizeof(array[0]));
    linked_array_dump(buf, sizeof(buf), array);
    rad_assert(strcmp(buf, "[12, 11, 3, 4, ]") == 0);
}
