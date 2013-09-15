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

int recvn(int fd, void *vptr, size_t n)
{
    int     err;
	size_t	nleft;
	int 	nread;
	char	*ptr;

	ptr = (char*)vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nread = recv(fd, ptr, nleft, 0)) == SOCKET_ERROR) {
            err = WSAGetLastError();
			if (err == WSAEINTR) {
				nread = 0;		/* and call read() again */
            }
			else {
                log_socket_error(L_ERROR|L_CONS, "[recvn] "
                                 "recv failed on fd(%d)", fd);
				return -1;
            }
		} else if (nread == 0)
			break;				/* EOF */

		nleft -= nread;
		ptr   += nread;
	}
	return (n - nleft);		/* return >= 0 */
}
