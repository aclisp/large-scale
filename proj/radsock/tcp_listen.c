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

int tcp_listen(const char *host, int port, int backlog)
{
    SOCKET listenfd;
    struct sockaddr_in servaddr;
    char ipbuf[16];

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        log_socket_error(L_ERROR|L_CONS, "[tcp_listen] "
                         "socket() failed for %s, %d",
                         host, port);
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if(strcmp(host, "*") == 0)
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        servaddr.sin_addr.s_addr = ip_getaddr(host);

    if(servaddr.sin_addr.s_addr == htonl(INADDR_NONE)) {
        radlog(L_ERROR|L_CONS, "[tcp_listen] "
               "invalid host %s", host);
        closesocket(listenfd);
        return -1;
    }

    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
        log_socket_error(L_ERROR|L_CONS, "[tcp_listen] "
                         "bind() failed for %s (%s), %d",
                         host, ip_ntoa(ipbuf, servaddr.sin_addr.s_addr), port);
        closesocket(listenfd);
        return -1;
    }

    if (listen(listenfd, backlog) == SOCKET_ERROR) {
        log_socket_error(L_ERROR|L_CONS, "[tcp_listen] "
                         "listen() failed for %s (%s), %d",
                         host, ip_ntoa(ipbuf, servaddr.sin_addr.s_addr), port);
        closesocket(listenfd);
        return -1;
    }

    DEBUG("[tcp_listen] %s (%s:%d) [%d], got listenfd(%d)", host,
          ip_ntoa(ipbuf, servaddr.sin_addr.s_addr), port, backlog, listenfd);

    return listenfd;
}
