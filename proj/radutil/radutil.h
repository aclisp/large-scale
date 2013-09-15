/* -*- mode: c -*- */

/* Author:   Huanghao@cellcom
 * Date:     2005/09/29 14:04:52
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RAD_UTIL_H__
#define __RAD_UTIL_H__

#include <stdarg.h>

#include <radutil/radplatform.h>
#include <radutil/radtypes.h>
#include <radutil/raddebug.h>
#include <radutil/radposix.h>
#include <radutil/radmacros.h>
#include <radutil/radcommon.h>
#include <radutil/radlog.h>
#include <radutil/radslist.h>
#include <radutil/radheaptrack.h>
#include <radutil/radarray.h>
#include <radutil/radtoken.h>
#include <radutil/radconffile.h>
#include <radutil/radipc.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* just like strncpy, but dst is always null terminated. */
char* strNcpy(char* dst, const char* src, size_t len);
char* strNcat(char* dst, const char* src, size_t len);
/* just like snprintf, but str is always null terminaed. */
int sNprintf(char* str, size_t len, const char* fmt, ...);
int vsNprintf(char* str, size_t len, const char* fmt, va_list args);
int trim_adjust(char** pcstr, const char* delims);
uint32_t ip_getaddr(const char *host);
char    *ip_ntoa(char *buffer, uint32_t ipaddr);
uint32_t ip_addr(const char *ip_str);
char    *ip_hostname(char *buf, size_t buflen, uint32_t ipaddr);
int print_file(const char* filename, const char* format, ...);
char* strRepl(char* dst, size_t n,
              const char* src, const LRAD_NAME_MAP* table);

#ifdef __cplusplus
}
#endif /* __cplusplus */

RAD_EXTERN_C int     librad_dodns;

#endif /* __RAD_UTIL_H__ */
