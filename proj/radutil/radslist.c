#include "radutil.h"

lrad_slist *lrad_slist_cons(lrad_slist *node, lrad_slist *list)
{
    node->next = list;
    return node;
}

lrad_slist *lrad_slist_reverse(lrad_slist *list)
{
    lrad_slist *result, *next;
    if(list == NULL) return list;
    result = list;
    list = list->next;
    result->next = NULL;
    while(list) {
        next = list->next;
        list->next = result;
        result = list;
        list = next;
    }
    return result;
}
