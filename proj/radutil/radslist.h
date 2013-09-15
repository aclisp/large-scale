/* -*- mode: c -*- */

/* Author:   Huanghao@cellcom
 * Date:     2005/09/29 19:15:18
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RAD_SLIST_H__
#define __RAD_SLIST_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _lrad_slist lrad_slist;

struct _lrad_slist
{
    void       *data;
    lrad_slist *next;
};

lrad_slist *lrad_slist_cons(lrad_slist *node, lrad_slist *list);
lrad_slist *lrad_slist_reverse(lrad_slist *list);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RAD_SLIST_H__ */
