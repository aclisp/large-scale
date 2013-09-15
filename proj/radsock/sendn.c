#include "radutil.h"
#include "radsock.h"

#if defined(RAD_OS_WIN32)
  #include <winsock2.h>
#else
  #include <errno.h>
  #include <netdb.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>

  #define WSAGetLastError() (errno)
  #define WSAEINTR EINTR
  #define SOCKET_ERROR -1
#endif

int sendn(int fd, const void *vptr, size_t n)
{
    int         err;
	size_t		nleft;
	int 		nwritten;
	const char	*ptr;

	ptr = (const char*)vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = send(fd, ptr, nleft, 0)) == SOCKET_ERROR) {
            err = WSAGetLastError();
			if (err == WSAEINTR) {
				nwritten = 0;		/* and call write() again */
            }
			else {
                log_socket_error(L_ERROR|L_CONS, "[sendn] "
                                 "send failed on fd(%d)", fd);
				return -1;			/* error */
            }
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return n;
}
