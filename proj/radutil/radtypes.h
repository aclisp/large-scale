/* Author:   Huanghao@cellcom
 * Date:     2005/09/29 10:57:50
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RAD_TYPES_H__
#define __RAD_TYPES_H__

#include <radutil/radplatform.h>

#include <sys/types.h>

#if defined(RAD_OS_LINUX) || defined(RAD_MINGW)
  #include <stdint.h>
#endif /* RAD_OS_LINUX || RAD_MINGW */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(RAD_VC)

/* Simulate stdint.h */

/* 7.18.1.1  Exact-width integer types */
typedef signed char int8_t;
typedef unsigned char   uint8_t;
typedef short  int16_t;
typedef unsigned short  uint16_t;
typedef int  int32_t;
typedef unsigned   uint32_t;

/* The missing uintptr_t */
typedef unsigned uintptr_t;

#endif /* RAD_VC */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RAD_TYPES_H__ */
