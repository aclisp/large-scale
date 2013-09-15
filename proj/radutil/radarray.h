/* -*- mode: c -*- */

/* Author:   Huanghao@cellcom
 * Date:     2005/10/13 09:07:27
 * Revision: 0.0.1
 * Function: Use C array to simulate various data structures.
 * Usage:
 */

#ifndef __RAD_ARRAY_H__
#define __RAD_ARRAY_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Shift array helper */
typedef struct _lrad_shift_entry lrad_shift_entry;

struct _lrad_shift_entry {
    unsigned int fr;
    unsigned int to;
};

#define LRAD_SHIFT_TOINFI    0xffffffff
#define LRAD_SHIFT_INCIDX(i, m) ( (i) == (m)-1 ? (i)=0 : ++(i) )
#define LRAD_SHIFT_DECIDX(i, m) ( (i) == 0 ? (i)=(m)-1 : --(i) )
#define LRAD_SHIFT_LENGTH(f, t, m) ( (t) == LRAD_SHIFT_TOINFI ? (m) : \
                                     (t) == (f) ? (0) :               \
                                     (t) >  (f) ? ((t)-(f)) :         \
                                                  ((t)+(m)-(f)) )

unsigned int lrad_shift_get_push_back_index(
    unsigned int *pfr, unsigned int *pto, size_t maxcnt);
unsigned int lrad_shift_get_push_front_index(
    unsigned int *pfr, unsigned int *pto, size_t maxcnt);
unsigned int lrad_shift_get_pop_back_index(
    unsigned int *pfr, unsigned int *pto, size_t maxcnt);
unsigned int lrad_shift_get_pop_front_index(
    unsigned int *pfr, unsigned int *pto, size_t maxcnt);

/* Ring-linked array helper
 * The lrad_link_entry must appear at the head of array elements! */
typedef struct _lrad_link_entry lrad_link_entry;

struct _lrad_link_entry {
    unsigned short next;
    unsigned short prev;
};

/* ARRAY is the array address, DSZ is the size of array elements! */
void lrad_link_remove_entry(lrad_link_entry* entry, char* array, size_t dsz);
void lrad_link_remove_entry_at(unsigned short entry_pos, char* array, size_t dsz);

lrad_link_entry* lrad_link_remove_head(char* array, size_t dsz);
lrad_link_entry* lrad_link_remove_tail(char* array, size_t dsz);

lrad_link_entry* lrad_link_insert_tail(unsigned short entry_pos, char* array, size_t dsz);
lrad_link_entry* lrad_link_insert_head(unsigned short entry_pos, char* array, size_t dsz);

void lrad_link_init_head(char* array, size_t dsz, size_t maxsz);
int  lrad_link_is_empty(char* array, size_t dsz);

unsigned short lrad_link_alloc_entry(char* array, size_t dsz, size_t maxsz);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RAD_ARRAY_H__ */
