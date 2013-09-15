/* Author:   Huanghao@cellcom
 * Date:     2005/09/29 10:55:52
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RAD_MACROS_H__
#define __RAD_MACROS_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Provide definitions for some commonly used macros.
 *  Some of them are only provided if they haven't already
 *  been defined. It is assumed that if they are already
 *  defined then the current definition is correct.
 */
#ifndef NULL
#  ifdef __cplusplus
#    define NULL        (0L)
#  else /* !__cplusplus */
#    define NULL        ((void*) 0)
#  endif /* !__cplusplus */
#endif

/* Provide locking semantics. Following declarations should be
 * presented before use.
 *
 *   #if defined(RAD_OS_WIN32)
 *   #include <windows.h>
 *   #else
 *   #include <pthread.h>
 *   #endif
 *
 */
#if defined(RAD_NO_THREAD)
  #define rad_lock_t int
  #define rad_lock_init(plock) ((void)0)
  #define rad_lock_acquire(plock) ((void)0)
  #define rad_lock_release(plock) ((void)0)
  #define rad_lock_destroy(plock) ((void)0)
#elif defined(RAD_OS_WIN32)
  #define rad_lock_t CRITICAL_SECTION
  #define rad_lock_init(plock) InitializeCriticalSection(plock)
  #define rad_lock_acquire(plock) EnterCriticalSection(plock)
  #define rad_lock_release(plock) LeaveCriticalSection(plock)
  #define rad_lock_destroy(plock) DeleteCriticalSection(plock)
#else
  #define rad_lock_t pthread_mutex_t
  #define rad_lock_init(plock) rad_verify(pthread_mutex_init(plock, NULL) == 0)
  #define rad_lock_acquire(plock) rad_verify(pthread_mutex_lock(plock) == 0)
  #define rad_lock_release(plock) rad_verify(pthread_mutex_unlock(plock) == 0)
  #define rad_lock_destroy(plock) rad_verify(pthread_mutex_destroy(plock) == 0)
#endif

#if defined(RAD_OS_WIN32)
  #if defined(RAD_COMPILER_VC)
  /* Using MSVC++ or Borland C++ to compile. If compiling library as a
   * DLL, define RAD_DLL. If compiling as a static library, define
   * RAD_STATIC. If compiling an application that is to be statically
   * linked, define RAD_STATIC (if the application is to be dynamically
   * linked, there is no need to define any of these macros).
   */
    #if defined (RAD_DLL) && defined(RAD_STATIC)
    #error "Both RAD_DLL and RAD_STATIC are defined."
    #elif defined(RAD_DLL)
    #define RAD_SYMBOL __declspec(dllexport)
    #elif !defined(RAD_STATIC)
    #define RAD_SYMBOL __declspec(dllimport)
    #elif defined(RAD_STATIC)
    #define RAD_SYMBOL
    #endif /* RAD_DLL && RAD_STATIC */
    /* non ¨C DLL-interface classkey 'identifier' used as base for
     * DLL-interface classkey 'identifier'
     * An exported class was derived from a class that was not exported.
     */
    #pragma warning( disable : 4275 )
  #else /* !RAD_COMPILER_VC */
    #define RAD_SYMBOL
  #endif
#else /* !RAD_OS_WIN32 */
  #define RAD_SYMBOL
#endif

/* VC6 hacks */
#if defined(__cplusplus) && defined(RAD_COMPILER_VC6)
#define for if(0); else for
#pragma warning( disable : 4786 ) /* identifier was truncated in the debug information */
#pragma warning( disable : 4503 ) /* decorated name length exceeded, name was truncated */
#endif /* __cplusplus && RAD_COMPILER_VC6 */

/* Secure string copy */
#define strscpy(x,y) do { strncpy(x,y,sizeof(x)); x[sizeof(x)-1]=0; } while(0)

/*  Calculate the address of the base of the structure given its type, and an */
/*  address of a field within the structure. @see CONTAINING_RECORD WinDDK */
#define baseptrof(p, record_type, member_name) \
    ((record_type*)(((char*)(p)) - offsetof(record_type, member_name)))

/* Calculate array dimension */
#define dimof(a) (sizeof(a)/sizeof(a[0]))

/* Calculate the total size of variable-length structure */
#define vsizeof(record_type, member_name, maxsize) \
    (sizeof(record_type) + sizeof(((record_type*)0)->member_name[0]) * maxsize)

/* Compile-time assertion */
#define rad_static_assert(pred) \
    switch(0){case 0:case pred:;}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#ifdef __cplusplus
#define RAD_EXTERN_C extern "C"
#else /* !__cplusplus */
#define RAD_EXTERN_C extern
#endif /* !__cplusplus */

#endif /* __RAD_MACROS_H__ */
