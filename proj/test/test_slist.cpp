#include "radutil.h"
#include "radtest.h"

#include <string.h>

void slist_dump(char* buf, size_t len, const lrad_slist *list)
{
    int wb;

    wb = sNprintf(buf, len, "[");
    buf += wb;
    len -= wb;

    for(; list != NULL; list = list->next) {
        wb = sNprintf(buf, len, "%d, ", list->data);
        buf += wb;
        len -= wb;
    }

    sNprintf(buf, len, "]");
}

void slist_free(lrad_slist **list)
{
    lrad_slist *node;
    while(*list) {
        node = *list;
        *list = (*list)->next;
        rad_free(node);
    }
}

RAD_TEST(lrad_slist_reverse)
{
    lrad_slist *head = NULL, *node;
    int i;
    char buf[256];

    // -------------------------------------------------------------
    for(i=1; i<=5; ++i) {
        node = (lrad_slist*)rad_malloc(sizeof(lrad_slist));
        node->data = (void*)i;
        head = lrad_slist_cons(node, head);
    }

    slist_dump(buf, sizeof(buf), head);
    rad_assert(strcmp(buf, "[5, 4, 3, 2, 1, ]") == 0);

    head = lrad_slist_reverse(head);

    slist_dump(buf, sizeof(buf), head);
    rad_assert(strcmp(buf, "[1, 2, 3, 4, 5, ]") == 0);

    slist_free(&head);
    rad_assert(head == NULL);
    // -------------------------------------------------------------
    for(i=1; i<=1; ++i) {
        node = (lrad_slist*)rad_malloc(sizeof(lrad_slist));
        node->data = (void*)i;
        head = lrad_slist_cons(node, head);
    }

    slist_dump(buf, sizeof(buf), head);
    rad_assert(strcmp(buf, "[1, ]") == 0);

    head = lrad_slist_reverse(head);

    slist_dump(buf, sizeof(buf), head);
    rad_assert(strcmp(buf, "[1, ]") == 0);

    slist_free(&head);
    rad_assert(head == NULL);
    // -------------------------------------------------------------
    rad_assert( lrad_slist_reverse(NULL) == NULL );
}
