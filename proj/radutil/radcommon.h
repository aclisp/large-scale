/* -*- mode: c -*- */

/* Author:   Huanghao@cellcom
 * Date:     2005/09/29 13:54:21
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RAD_COMMON_H__
#define __RAD_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _LRAD_NAME_NUMBER {
    const char *name;
    int         number;
} LRAD_NAME_NUMBER;

int          lrad_str2int (
    const LRAD_NAME_NUMBER *table,
    const char             *name,
    int def
    );
const char * lrad_int2str (
    const LRAD_NAME_NUMBER *table,
    int                     number,
    const char *def
    );

typedef struct _LRAD_NAME_MAP {
    const char* name1;
    const char* name2;
} LRAD_NAME_MAP;

const char * lrad_stra2b (
    const LRAD_NAME_MAP *table,
    const char          *name1,
    const char *def
    );
const char * lrad_strb2a (
    const LRAD_NAME_MAP *table,
    const char          *name2,
    const char *def
    );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RAD_COMMON_H__ */
