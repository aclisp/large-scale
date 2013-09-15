/* Author:   Huanghao@cellcom
 * Date:     2005/09/29 10:57:01
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RAD_DEBUG_H__
#define __RAD_DEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Distinguish current debugging mode */
#if defined(NDEBUG)
  #if defined(_DEBUG)
    #error ANSI's NDEBUG is turned on, but CRT's _DEBUG is also turned on.
  #endif
  #define rad_assert(expr) ((void)0)
  #define rad_verify(expr) (expr)
#else
  #define rad_assert(expr) \
    ((expr) ? (void)0 : rad_assert_fail(__FILE__, __LINE__, #expr))
  #define rad_verify(expr) rad_assert(expr)
#endif

void rad_assert_fail(const char *file, unsigned int line, const char* expr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RAD_DEBUG_H__ */
