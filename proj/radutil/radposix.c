#include "radutil.h"
#include <time.h>
#include <string.h>
#include <stdio.h>

#if defined(RAD_OS_WIN32)
  #include <sys/timeb.h>
  #include <winsock2.h>
  #include <windows.h>
  #include <process.h>
#else
  #include <sys/time.h>
  #include <sys/select.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
#endif

#if defined(RAD_OS_WIN32)

/*
 *  We use localtime_r() by default in the server.
 *
 *  For systems which do NOT have localtime_r(), we make the
 *  assumption that localtime() is re-entrant, and returns a
 *  per-thread data structure.
 *
 *  Even if localtime is NOT re-entrant, this function will
 *  lower the possibility of race conditions.
 */
struct tm *localtime_r(const time_t *l_clock, struct tm *result)
{
    memcpy(result, localtime(l_clock), sizeof(*result));
    return result;
}

int gettimeofday(struct timeval *tv, void *tz)
{
    struct _timeb tb;
    _ftime(&tb);
    tv->tv_sec = tb.time;
    tv->tv_usec = tb.millitm * 1000;
    return 0;
}

int strerror_r(int errnum, char *buf, size_t n)
{
    void* pMsgBuf;
    DWORD dwSize;

    dwSize =
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS ,
            NULL,
            (DWORD) errnum,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
            (LPTSTR) &pMsgBuf,
            0,
            NULL);
    if(dwSize == 0)
        return -1;

    strncpy(buf, (const char*)pMsgBuf, n);
    buf[n-1] = '\0';
    LocalFree(pMsgBuf);

    return 0;
}

char *strtok_r(char *string, const char *delim, char **ptrptr)
{
    unsigned char *str;
    const unsigned char *ctrl = delim;

    unsigned char map[32];
    int count;

    /* Clear control map */
    for (count = 0; count < 32; count++)
        map[count] = 0;

    /* Set bits in delimiter table */
    do {
        map[*ctrl >> 3] |= (1 << (*ctrl & 7));
    } while (*ctrl++);

    /* Initialize str. If string is NULL, set str to the saved
     * pointer (i.e., continue breaking tokens out of the string
     * from the last strtok call) */
    if (string)
        str = string;
    else
        str = *ptrptr;

    /* Find beginning of token (skip over leading delimiters). Note that
     * there is no token iff this loop sets str to point to the terminal
     * null (*str == '\0') */
    while ( (map[*str >> 3] & (1 << (*str & 7))) && *str )
        str++;

    string = str;

    /* Find the end of the token. If it is not the end of the string,
     * put a null there. */
    for ( ; *str ; str++ )
        if ( map[*str >> 3] & (1 << (*str & 7)) ) {
            *str++ = '\0';
            break;
        }

    /* Update nextoken (or the corresponding field in the per-thread data
     * structure */
    *ptrptr = str;

    /* Determine if a token has been found. */
    if ( (unsigned char*)string == str )
        return NULL;
    else
        return string;
}

void *dlopen (const char *filename, int flag)
{
    return (void *)LoadLibrary(filename);
}

const char *dlerror(void)
{
    static char dlerr[256];
    strerror_r(GetLastError(), dlerr, sizeof(dlerr));
    return dlerr;
}

void *dlsym(void *handle, char *symbol)
{
    return (void *)GetProcAddress((HMODULE)handle, symbol);
}

int dlclose (void *handle)
{
    return FreeLibrary((HMODULE)handle) ? 0 : -1;
}

int pthread_create(pthread_t *thread, pthread_attr_t *attr,
                   unsigned (__stdcall *start_routine)(void *), void *arg)
{
    uintptr_t handle =
        _beginthreadex(NULL, /* security */
                       0,    /* stack_size */
                       start_routine,
                       arg,
                       0, /* initflag: (0 for running or
                           * CREATE_SUSPENDED for suspended) */
                       &thread->id);

    if(handle == 0) { /* _beginthreadex returns 0 on an error */
        return errno;
    }

    thread->handle = handle;
    return 0;
}

/* It is safer to use _beginthreadex than _beginthread. If the thread
 * spawned by _beginthread exits quickly, the handle returned to the
 * caller of _beginthread may be invalid or, worse, point to another
 * thread. However, the handle returned by _beginthreadex has to be
 * closed by the caller of _beginthreadex, so it is guaranteed to be a
 * valid handle if _beginthreadex did not return an error.
 */
int pthread_join(pthread_t th, void **thread_return)
{
    char buf[256];
    int errnum;
    DWORD dwExitCode;

    if(WaitForSingleObject((HANDLE)th.handle, INFINITE) != WAIT_OBJECT_0) {
        errnum = GetLastError();
        strerror_r(errnum, buf, sizeof(buf));
        radlog(L_ERROR|L_CONS, "[pthread_join] WaitForSingleObject: %d %s", errnum, buf);
        return errnum;
    }

    if(thread_return) {
        if(!GetExitCodeThread((HANDLE)th.handle, &dwExitCode)) {
            errnum = GetLastError();
            strerror_r(errnum, buf, sizeof(buf));
            radlog(L_ERROR|L_CONS, "[pthread_join] GetExitCodeThread: %d %s", errnum, buf);
            return errnum;
        }
        *thread_return = (void*)dwExitCode;
    }

    if(!CloseHandle((HANDLE)th.handle)) {
        errnum = GetLastError();
        strerror_r(errnum, buf, sizeof(buf));
        radlog(L_ERROR|L_CONS, "[pthread_join] CloseHandle: %d %s", errnum, buf);
        return errnum;
    }

    return 0;
}

#else /* !RAD_OS_WIN32 */

void Sleep(unsigned int msec)
{
    struct timeval tv;
    tv.tv_sec = msec/1000;
    tv.tv_usec = (msec%1000)*1000;
    select(0, NULL, NULL, NULL, &tv);
}

#endif /* RAD_OS_WIN32 */

#if defined(RAD_OS_UNIX) || defined(RAD_OS_WIN32)

int inet_aton(const char *cp, struct in_addr *inp)
{
    int a1, a2, a3, a4;

    if (sscanf(cp, "%d.%d.%d.%d", &a1, &a2, &a3, &a4) != 4)
        return 0;

    inp->s_addr = htonl((a1 << 24) + (a2 << 16) + (a3 << 8) + a4);
    return 1;
}

/*
 *	Get next token from string *stringp, where tokens are
 *	possibly-empty strings separated by characters from delim.
 *
 *	Writes NULs into the string at *stringp to end tokens.
 *	delim need not remain constant from call to call.  On
 *	return, *stringp points past the last NUL written (if there
 *	might be further tokens), or is NULL (if there are
 *	definitely no more tokens).
 *
 *	If *stringp is NULL, strsep returns NULL.
 */
char *strsep(char **stringp, const char *delim)
{
	char *s;
	const char *spanp;
	int c, sc;
	char *tok;

	if ((s = *stringp) == NULL)
		return (NULL);

	for (tok = s;;) {
		c = *s++;
		spanp = delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				*stringp = s;
				return (tok);
			}
		} while (sc != 0);
	}

	return NULL;		/* NOTREACHED, but the compiler complains */
}

#endif /* RAD_OS_UNIX || RAD_OS_WIN32 */

#if defined(RAD_OS_UNIX)
int strerror_r(int errnum, char *buf, size_t n)
{
    strncpy(buf, strerror(errnum), n);
    buf[n-1] = '\0';
    return 0;
}
#endif /* RAD_OS_UNIX */
