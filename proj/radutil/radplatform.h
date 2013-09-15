/* Author:   Huanghao@cellcom
 * Date:     2005/09/29 10:54:39
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RAD_PLATFORM_H__
#define __RAD_PLATFORM_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Distinguish Windows and Unix, but treat linux as special */
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
  #define RAD_OS_WIN32
  #define RAD_OS 0
#elif defined(linux) || defined(__linux) || defined(__linux__)
  #define RAD_OS_LINUX
  #define RAD_OS -1
#else
  #define RAD_OS_UNIX
  #define RAD_OS 1
#endif

/* We do not distinguish threading model, because radutil is assumed
 * to be always used under multi-thread environment. */
#ifndef _REENTRANT
#define _REENTRANT
#endif

/* Distinguish various compiler as possible as we can */
#if defined(_MSC_VER)
  #define RAD_COMPILER_VC
  #define RAD_COMPILER 0
  #if   _MSC_VER == 1200
    #define RAD_COMPILER_VC6
  #elif _MSC_VER == 1300
    #define RAD_COMPILER_VC7
  #elif _MSC_VER == 1310
    #define RAD_COMPILER_VC71
  #endif
#elif defined(__GNUC__)
  #define RAD_COMPILER_GCC
  #define RAD_COMPILER 1
  #if   __GNUC__ == 2
    #define RAD_COMPILER_GCC2
  #elif __GNUC__ == 3
    #define RAD_COMPILER_GCC3
  #elif __GNUC__ == 4
    #define RAD_COMPILER_GCC4
  #endif
#else
  #define RAD_COMPILER_UNKNOWN
  #define RAD_COMPILER -1
#endif

/* Distinguish develop environment */
#if defined(RAD_OS_WIN32) && defined(_MSC_VER)
  #define RAD_VC
#elif defined(RAD_OS_WIN32) && defined(__GNUC__)
  #define RAD_MINGW /* This guess may be violated by Cygwin? */
#elif defined(RAD_OS_LINUX) && defined(__GNUC__)
  #define RAD_POSIX /* How about this guess then? */
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RAD_PLATFORM_H__ */
