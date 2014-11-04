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
#endif

char* sock_peer_name(int fd, char* addrbuf, int* port)
{
    struct sockaddr_in sockinfo;
    struct in_addr     address;
    socklen_t          sockinfosize = sizeof(sockinfo);

    // Get the peer address for the client socket
    getpeername(fd, (struct sockaddr *)&sockinfo, &sockinfosize);
    memcpy(&address, &sockinfo.sin_addr, sizeof(address));
    if(port) *port = sockinfo.sin_port;

    return ip_ntoa(addrbuf, address.s_addr);
}
