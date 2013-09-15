#include "radutil.h"
#include "radsock.h"

#include <string.h>

#if defined(RAD_OS_WIN32)
  #include <winsock2.h>
#else
  #include <errno.h>
  #include <netdb.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>

  #define WSAGetLastError() (errno)
#endif

void log_socket_error(int level, const char* fmt, ...)
{
    char buf[512];
    size_t len;

    int errnum = WSAGetLastError();

    va_list		ap;
	va_start(ap, fmt);
    vsNprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

    len = strlen(buf);
    sNprintf(buf+len, sizeof(buf)-len, ": %d ", errnum);

    len = strlen(buf);
    strerror_r(errnum, buf+len, sizeof(buf)-len);

    radlog(level, buf);
}
