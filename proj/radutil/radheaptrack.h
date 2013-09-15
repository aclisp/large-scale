/* -*- mode: c -*- */

/* Author:   Huanghao@cellcom
 * Date:     2005/09/30 14:59:05
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RAD_HEAPTRACK_H__
#define __RAD_HEAPTRACK_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Ring helper */
typedef struct _lrad_ring_entry lrad_ring_entry;

struct _lrad_ring_entry {
    lrad_ring_entry *next;
    lrad_ring_entry *prev;
};

void lrad_ring_init_head(lrad_ring_entry* head);
int  lrad_ring_is_empty(lrad_ring_entry* head);
void lrad_ring_remove_entry(lrad_ring_entry* entry);
lrad_ring_entry* lrad_ring_remove_head(lrad_ring_entry* head);
lrad_ring_entry* lrad_ring_remove_tail(lrad_ring_entry* head);
void lrad_ring_insert_tail(lrad_ring_entry* head, lrad_ring_entry* entry);
void lrad_ring_insert_head(lrad_ring_entry* head, lrad_ring_entry* entry);

/* Interface */
void *rad_malloc_track(size_t size, const char *file, unsigned int line);
void  rad_free_track(void* mem);
char *rad_strdup_track(const char *s, const char *file, unsigned int line);

#define rad_malloc(size) rad_malloc_track(size, __FILE__, __LINE__)
#define rad_free(mem) rad_free_track(mem)
#define rad_strdup(s) rad_strdup_track(s, __FILE__, __LINE__)

/* Setup */
void rad_start_heap_tracking();
void rad_dump_heap_usage();
void rad_stop_heap_tracking();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RAD_HEAPTRACK_H__ */
