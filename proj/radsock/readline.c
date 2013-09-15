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
#endif

/* Read a line from a descriptor.  Read the line one byte at a time,
 * looking for '\n'. The line, along with '\n', is stored in the
 * buffer, and is then followed with a '\0'.  The number of characters
 * (excluding '\0') is returned.
 */
int readline(int fd, char* ptr, size_t maxlen, char delim)
{
    size_t n;
    int rc;
    char c; /* Stores the character read by readn */
    int err;

    for ( n = 1; n < maxlen; n++ ) {

        if ( (rc = recv(fd, &c, 1, 0)) == 1 ) {
            *ptr++ = c;
            if ( c == delim ) /* We want to stop reading when the char is \n */
                break;
        }
        else if ( rc == 0 ) {
            if ( n == 1 )
                return 0; /* EOF, no data read */
            else
                break; /* EOF, some data was read */
        }
        else {
            err = WSAGetLastError();
            if ( err == WSAEINTR )
                continue;

            log_socket_error(L_ERROR|L_CONS, "[readline] "
                             "recv failed on fd(%d)", fd);
            return -1; /* error */
        }
    }

    *ptr = 0;
    return n;
}
