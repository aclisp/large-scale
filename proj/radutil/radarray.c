#include "radutil.h"
#include <string.h>

unsigned int lrad_shift_get_push_back_index(
    unsigned int *pfr, unsigned int *pto, size_t maxcnt)
{
    unsigned int index;
    size_t length = LRAD_SHIFT_LENGTH(*pfr, *pto, maxcnt);

    if(length == maxcnt) {
        index = *pfr;
        LRAD_SHIFT_INCIDX(*pfr, maxcnt);
    } else {
        index = *pto;
        length == maxcnt-1 ? *pto = LRAD_SHIFT_TOINFI :
            LRAD_SHIFT_INCIDX(*pto, maxcnt);
    }

    return index;
}

unsigned int lrad_shift_get_push_front_index(
    unsigned int *pfr, unsigned int *pto, size_t maxcnt)
{
    unsigned int index;
    size_t length = LRAD_SHIFT_LENGTH(*pfr, *pto, maxcnt);

    if(length == maxcnt) {
        index = LRAD_SHIFT_DECIDX(*pfr, maxcnt);
    } else {
        if(length == maxcnt-1)
            *pto = LRAD_SHIFT_TOINFI;
        index = LRAD_SHIFT_DECIDX(*pfr, maxcnt);
    }

    return index;
}

unsigned int lrad_shift_get_pop_back_index(
    unsigned int *pfr, unsigned int *pto, size_t maxcnt)
{
    unsigned int index;
    size_t length = LRAD_SHIFT_LENGTH(*pfr, *pto, maxcnt);

    if(length == maxcnt) {
        *pto = *pfr;
        index = LRAD_SHIFT_DECIDX(*pto, maxcnt);
    } else if(length == 0) {
        index = LRAD_SHIFT_TOINFI;
    } else {
        index = LRAD_SHIFT_DECIDX(*pto, maxcnt);
    }

    return index;
}

unsigned int lrad_shift_get_pop_front_index(
    unsigned int *pfr, unsigned int *pto, size_t maxcnt)
{
    unsigned int index;
    size_t length = LRAD_SHIFT_LENGTH(*pfr, *pto, maxcnt);

    if(length == maxcnt) {
        index = *pfr;
        *pto = *pfr;
        LRAD_SHIFT_INCIDX(*pfr, maxcnt);
    } else if(length == 0) {
        index = LRAD_SHIFT_TOINFI;
    } else {
        index = *pfr;
        LRAD_SHIFT_INCIDX(*pfr, maxcnt);
    }

    return index;
}

void lrad_link_remove_entry(lrad_link_entry* entry,
                            char* array, size_t dsz)
{
    lrad_link_entry* prev;
    lrad_link_entry* next;
    next = (lrad_link_entry*)(array+dsz*entry->next);
    prev = (lrad_link_entry*)(array+dsz*entry->prev);
    prev->next = entry->next;
    next->prev = entry->prev;
    entry->next = entry->prev = 0;
}

void lrad_link_remove_entry_at(unsigned short entry_pos,
                               char* array, size_t dsz)
{
    lrad_link_entry* entry;
    entry = (lrad_link_entry*)(array+dsz*entry_pos);
    lrad_link_remove_entry(entry, array, dsz);
}

lrad_link_entry* lrad_link_remove_head(char* array, size_t dsz)
{
    lrad_link_entry* next;
    lrad_link_entry* entry;
    entry = (lrad_link_entry*)(array+dsz*((lrad_link_entry*)(array+dsz))->next);
    next = (lrad_link_entry*)(array+dsz*entry->next);
    ((lrad_link_entry*)(array+dsz))->next = entry->next;
    next->prev = 1;
    entry->next = entry->prev = 0;
    return entry;
}

lrad_link_entry* lrad_link_remove_tail(char* array, size_t dsz)
{
    lrad_link_entry* prev;
    lrad_link_entry* entry;
    entry = (lrad_link_entry*)(array+dsz*((lrad_link_entry*)(array+dsz))->prev);
    prev = (lrad_link_entry*)(array+dsz*entry->prev);
    ((lrad_link_entry*)(array+dsz))->prev = entry->prev;
    prev->next = 1;
    entry->next = entry->prev = 0;
    return entry;
}

lrad_link_entry* lrad_link_insert_tail(unsigned short entry_pos,
                                       char* array, size_t dsz)
{
    lrad_link_entry* entry;
    lrad_link_entry* prev;
    prev = (lrad_link_entry*)(array+dsz*((lrad_link_entry*)(array+dsz))->prev);
    entry = (lrad_link_entry*)(array+dsz*entry_pos);
    entry->next = 1;
    entry->prev = ((lrad_link_entry*)(array+dsz))->prev;
    prev->next = entry_pos;
    ((lrad_link_entry*)(array+dsz))->prev = entry_pos;
    return entry;
}

lrad_link_entry* lrad_link_insert_head(unsigned short entry_pos,
                                       char* array, size_t dsz)
{
    lrad_link_entry* entry;
    lrad_link_entry* next;
    next = (lrad_link_entry*)(array+dsz*((lrad_link_entry*)(array+dsz))->next);
    entry = (lrad_link_entry*)(array+dsz*entry_pos);
    entry->next = ((lrad_link_entry*)(array+dsz))->next;
    entry->prev = 1;
    next->prev = entry_pos;
    ((lrad_link_entry*)(array+dsz))->next = entry_pos;
    return entry;
}

void lrad_link_init_head(char* array, size_t dsz, size_t maxsz)
{
    unsigned short dim = maxsz/dsz;
    rad_assert(dim >= 2);
    memset(array, 0, maxsz);
    ((lrad_link_entry*)(array+dsz))->next = 1;
    ((lrad_link_entry*)(array+dsz))->prev = 1;
}

int  lrad_link_is_empty(char* array, size_t dsz)
{
    return ((lrad_link_entry*)(array+dsz))->next == 1;
}

unsigned short lrad_link_alloc_entry(char* array, size_t dsz, size_t maxsz)
{
    unsigned short dim = maxsz/dsz;
    unsigned short i;
    lrad_link_entry* entry;
    for(i = 2; i < dim; ++i) {
        entry = (lrad_link_entry*)(array+dsz*i);
        if(entry->next == 0)
            return i;
    }
    return 0;
}
