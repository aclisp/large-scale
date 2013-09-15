/* -*- mode: c -*- */

/* Author:   Huanghao@cellcom
 * Date:     2005/10/09 18:22:04
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RAD_SOCK_H__
#define __RAD_SOCK_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void socket_init();
void socket_clean();
void log_socket_error(int level, const char* fmt, ...);

int tcp_connect(const char *host, int port);
int tcp_listen(const char *host, int port, int backlog);

struct sockaddr_in;
int udp_connect(const char *host, int port,  /* IN  */
                struct sockaddr_in* servaddr /* OUT */);
int udp_bind(const char *host, int port);

char* sock_peer_name(int fd, char* addrbuf, int* port);

int recvn(int fd, void *vptr, size_t n);
int sendn(int fd, const void *vptr, size_t n);
int readline(int fd, char* ptr, size_t maxlen, char delim);

int	send_email(const char *smtp_server, const char *smtp_helo, const char *smtp_email,
               const char *mailto, const char *mailsubject, const char *mailbody);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RAD_SOCK_H__ */
