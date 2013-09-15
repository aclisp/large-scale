#include "radutil.h"
#include <string.h>
#include <stddef.h> /* for offsetof */
#include <stdlib.h> /* for malloc and free */
#if defined(RAD_OS_WIN32) /* for lock */
  #include <windows.h>
#else
  #include <pthread.h>
#endif

/*  Calculate the address of the base of the structure given its type, and an */
/*  address of a field within the structure. */
#ifndef CONTAINING_RECORD
#define CONTAINING_RECORD(p, record_type, member_name) \
    (record_type*)(((char*)(p)) - offsetof(record_type, member_name))
#endif

typedef struct _rad_heap_tracking_record
{
    lrad_ring_entry link;
    size_t size;
    const char* file;
    unsigned int line;
} rad_heap_tracking_record;

/* the doubly linked list (ring) header */
static lrad_ring_entry tracking_records = {NULL, NULL};
static rad_lock_t lock;
static unsigned int ref = 0;

void rad_start_heap_tracking()
{
    if(++ref == 1) {
        rad_lock_init(&lock);
        lrad_ring_init_head(&tracking_records);
    }
}

void rad_dump_heap_usage()
{
    if(!lrad_ring_is_empty(&tracking_records))
    {
        lrad_ring_entry* p;
        rad_lock_acquire(&lock);
        for(p=tracking_records.next; p!=&tracking_records; p=p->next)
        {
            rad_heap_tracking_record* q =
                CONTAINING_RECORD(p, rad_heap_tracking_record, link);
            radlog(L_FATAL, "Memory leak: %u bytes allocated in %s:%u",
                   q->size, q->file, q->line);
        }
        rad_lock_release(&lock);
    }
}

void rad_stop_heap_tracking()
{
    if(--ref == 0) {
        if(!lrad_ring_is_empty(&tracking_records)) {
            radlog(L_FATAL|L_CONS, "MEMORY LEAK DETECTED! "
                   "Heap usage dumps here:");
            rad_dump_heap_usage();
        }
        rad_lock_destroy(&lock);
    }
}

void *rad_malloc_track(size_t size, const char *file, unsigned int line)
{
    rad_heap_tracking_record* mem;

    if(tracking_records.next == NULL) {
        radlog(L_FATAL|L_CONS, "[rad_malloc_track] call `rad_start_heap_tracking()' first");
        exit(1);
    }

    mem = malloc(sizeof(rad_heap_tracking_record) + size);

    if(mem) {
        mem->size = size;
        mem->file = file;
        mem->line = line;
        rad_lock_acquire(&lock);
        lrad_ring_insert_tail(&tracking_records, &mem->link);
        rad_lock_release(&lock);
        return mem+1;
    }
    else {
        radlog(L_FATAL|L_CONS, "[rad_malloc_track] no memory");
        exit(1);
    }

    return NULL;
}

void  rad_free_track(void* mem)
{
    if(mem) {
        rad_heap_tracking_record* p =
            (rad_heap_tracking_record*)
            ((char*)mem-sizeof(rad_heap_tracking_record));
        rad_lock_acquire(&lock);
        lrad_ring_remove_entry(&p->link);
        rad_lock_release(&lock);
        free(p);
    }
}

char *rad_strdup_track(const char *string,
                       const char *file, unsigned int line)
{
    char *memory;
    size_t stringlen;

    if (!string)
        return NULL;

    stringlen = strlen(string);
    memory = rad_malloc_track(stringlen + 1, file, line);
    if(memory)
        return memcpy(memory, string, stringlen + 1);

    return NULL;
}

void lrad_ring_init_head(lrad_ring_entry* head)
{
    head->prev = head->next = head;
}

int  lrad_ring_is_empty(lrad_ring_entry* head)
{
    return head->next == head;
}

void lrad_ring_remove_entry(lrad_ring_entry* entry)
{
    lrad_ring_entry* prev;
    lrad_ring_entry* next;
    next = entry->next;
    prev = entry->prev;
    prev->next = next;
    next->prev = prev;
}

lrad_ring_entry* lrad_ring_remove_head(lrad_ring_entry* head)
{
    lrad_ring_entry* next;
    lrad_ring_entry* entry;
    entry = head->next;
    next = entry->next;
    head->next = next;
    next->prev = head;
    return entry;
}

lrad_ring_entry* lrad_ring_remove_tail(lrad_ring_entry* head)
{
    lrad_ring_entry* prev;
    lrad_ring_entry* entry;
    entry = head->prev;
    prev = entry->prev;
    head->prev = prev;
    prev->next = head;
    return entry;
}

void lrad_ring_insert_tail(lrad_ring_entry* head, lrad_ring_entry* entry)
{
    lrad_ring_entry* prev;
    prev = head->prev;
    entry->next = head;
    entry->prev = prev;
    prev->next = entry;
    head->prev = entry;
}

void lrad_ring_insert_head(lrad_ring_entry* head, lrad_ring_entry* entry)
{
    lrad_ring_entry* next;
    next = head->next;
    entry->next = next;
    entry->prev = head;
    next->prev = entry;
    head->next = entry;
}
