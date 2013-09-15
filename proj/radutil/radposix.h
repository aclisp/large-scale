/* -*- mode: c -*- */

/* Author:   Huanghao@cellcom
 * Date:     2005/09/30 12:38:25
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RAD_POSIX_H__
#define __RAD_POSIX_H__

#include <radutil/radtypes.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(RAD_VC)
#define strcasecmp(x, y) stricmp(x, y)
#define strncasecmp(x, y, n) strnicmp(x, y, n)
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#include <direct.h>
#define mkdir(x, y) mkdir(x)
#endif /* RAD_VC */

#if defined(RAD_MINGW)
#include <io.h>
#define mkdir(x, y) mkdir(x)
#endif /* RAD_MINGW */

#if defined(RAD_OS_WIN32)
struct tm *localtime_r(const time_t *l_clock, struct tm *result);
struct timeval;
int gettimeofday(struct timeval *tv, void *tz);
int strerror_r(int errnum, char *buf, size_t n);
char *strtok_r(char *string, const char *delim, char **ptrptr);
char *strsep(char **stringp, const char *delim);
#define RTLD_NOW    0
#define RTLD_GLOBAL 0
void *dlopen (const char *filename, int flag);
const char *dlerror(void);
void *dlsym(void *handle, char *symbol);
int dlclose (void *handle);
struct in_addr;
int inet_aton(const char *cp, struct in_addr *inp);
typedef struct _pthread_t {
    unsigned id;
    uintptr_t handle;
} pthread_t;
typedef struct _pthread_attr_t pthread_attr_t;
int pthread_create(pthread_t *thread, pthread_attr_t *attr,
                   unsigned (__stdcall *start_routine)(void *), void *arg);
int pthread_join(pthread_t th, void **thread_return);
#define THREAD_CALLBACK  unsigned __stdcall
#define THREAD_ID(th)    ((th)->id)
#define THREAD_SELFID()  (GetCurrentThreadId())
#define PROCESS_SELFID() (GetCurrentProcessId())
#define sleep(s) Sleep((s)*1000)
#else
#define THREAD_CALLBACK  void *
#define THREAD_ID(th)    (th)
#define THREAD_SELFID()  (pthread_self())
#define PROCESS_SELFID() (getpid())
void Sleep(unsigned int msec);
#endif /* RAD_OS_WIN32 */

#if defined(RAD_OS_UNIX)
int strerror_r(int errnum, char *buf, size_t n);
char *strsep(char **stringp, const char *delim);
struct in_addr;
int inet_aton(const char *cp, struct in_addr *inp);
#include <strings.h> /* for strcasecmp & strncasecmp */
#endif /* RAD_OS_UNIX */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RAD_POSIX_H__ */
