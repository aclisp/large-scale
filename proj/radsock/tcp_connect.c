#include "radutil.h"
#include "radsock.h"

#include <string.h>

#if defined(RAD_OS_WIN32)
  #include <winsock2.h>
#else
  #include <unistd.h>
  #include <errno.h>
  #include <netdb.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>

  #define WSAGetLastError() (errno)
  #define closesocket(s) close(s)
  #define SOCKET_ERROR -1
  #define INVALID_SOCKET -1
  #ifndef INADDR_NONE
  #define INADDR_NONE	0xffffffff
  #endif
  typedef int SOCKET;
#endif

int tcp_connect(const char *host, int port)
{
    SOCKET sockfd;
    struct sockaddr_in servaddr;
    char ipbuf[16];

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        log_socket_error(L_ERROR|L_CONS, "[tcp_connect] "
                         "socket() failed for %s, %d",
                         host, port);
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = ip_getaddr(host);

    if(servaddr.sin_addr.s_addr == htonl(INADDR_NONE)) {
        radlog(L_ERROR|L_CONS, "[tcp_connect] "
               "invalid host %s", host);
        closesocket(sockfd);
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
        log_socket_error(L_ERROR|L_CONS, "[tcp_connect] "
                         "connect() failed for %s (%s), %d",
                         host, ip_ntoa(ipbuf, servaddr.sin_addr.s_addr), port);
        closesocket(sockfd);
        return -1;
    }

    DEBUG("[tcp_connect] %s (%s:%d), got sockfd(%d)", host,
          ip_ntoa(ipbuf, servaddr.sin_addr.s_addr), port, sockfd);

    return sockfd;
}
