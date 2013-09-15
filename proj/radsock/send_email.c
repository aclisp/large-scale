#include "radutil.h"
#include "radsock.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#if defined(RAD_OS_WIN32)
  #include <winsock2.h>
#else
  #include <errno.h>
  #include <netdb.h>
  #include <unistd.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>

  #define WSAGetLastError() (errno)
  #define WSAEINTR EINTR
  #define SOCKET_ERROR -1
  #define closesocket(s) close(s)
  typedef int SOCKET;
#endif

static    const char  *OK_220="220";
static    const char  *OK_250="250";
static    const char  *OK_251="251";
static    const char  *OK_354="354";

static int send_msg(SOCKET s, const char* fmt, ...)
{
    int retval;
    char buf[4096];

    va_list		ap;
	va_start(ap, fmt);
    vsNprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

    DEBUG("[send_email] %s", buf);

    retval = send(s, buf, strlen(buf), 0);
    if(retval == SOCKET_ERROR) {
        log_socket_error(L_ERROR|L_CONS, "[send_email] %s", buf);
        return -1;
    }

    return 0;
}

static int recv_msg(SOCKET s, const char* hint, const char* code, ...)
{
    int retval;
    char buf[256];
    const char* c = NULL;

    va_list		ap;

    DEBUG("[send_email] %s", hint);

    retval = recv(s, buf, sizeof(buf), 0);
    if(retval == SOCKET_ERROR) {
        log_socket_error(L_ERROR|L_CONS, "[send_email] %s", hint);
        return -1;
    }
    if(retval == 0) {
        radlog(L_ERROR|L_CONS, "[send_email] %s: Connection is closed by remote", hint);
        return -1;
    }

    DEBUG("[send_email] %s", buf);

    if(code) {
        va_start(ap, code);
        do {

            if(strncmp(code, buf, strlen(code)) != 0) {
                c = code;
            } else {
                c = NULL;
                break;
            }

        } while((code = va_arg(ap, const char*)));
        va_end(ap);
    }

    if(c) {
        radlog(L_ERROR|L_CONS, "[send_email] %s: Unexpected result: %s", hint, buf);
        return -1;
    }

    return 0;
}

#define SEND_MSG0(s, x) if(send_msg(s, x)) goto error
#define SEND_MSG1(s, x, x1) if(send_msg(s, x, x1)) goto error
#define SEND_MSG2(s, x, x1, x2) if(send_msg(s, x, x1, x2)) goto error
#define SEND_MSG3(s, x, x1, x2, x3) if(send_msg(s, x, x1, x2, x3)) goto error
#define SEND_MSG4(s, x, x1, x2, x3, x4) if(send_msg(s, x, x1, x2, x3, x4)) goto error
#define SEND_MSG5(s, x, x1, x2, x3, x4, x5) if(send_msg(s, x, x1, x2, x3, x4, x5)) goto error
#define SEND_MSG6(s, x, x1, x2, x3, x4, x5, x6) if(send_msg(s, x, x1, x2, x3, x4, x5, x6)) goto error
#define SEND_MSG7(s, x, x1, x2, x3, x4, x5, x6, x7) if(send_msg(s, x, x1, x2, x3, x4, x5, x6, x7)) goto error
#define SEND_MSG8(s, x, x1, x2, x3, x4, x5, x6, x7, x8) if(send_msg(s, x, x1, x2, x3, x4, x5, x6, x7, x8)) goto error
#define SEND_MSG9(s, x, x1, x2, x3, x4, x5, x6, x7, x8, x9) if(send_msg(s, x, x1, x2, x3, x4, x5, x6, x7, x8), x9) goto error

#define RECV_MSG0(s, h, x) if(recv_msg(s, h, x)) goto error
#define RECV_MSG1(s, h, x, x1) if(recv_msg(s, h, x, x1)) goto error
#define RECV_MSG2(s, h, x, x1, x2) if(recv_msg(s, h, x, x1, x2)) goto error
#define RECV_MSG3(s, h, x, x1, x2, x3) if(recv_msg(s, h, x, x1, x2, x3)) goto error
#define RECV_MSG4(s, h, x, x1, x2, x3, x4) if(recv_msg(s, h, x, x1, x2, x3, x4)) goto error
#define RECV_MSG5(s, h, x, x1, x2, x3, x4, x5) if(recv_msg(s, h, x, x1, x2, x3, x4, x5)) goto error
#define RECV_MSG6(s, h, x, x1, x2, x3, x4, x5, x6) if(recv_msg(s, h, x, x1, x2, x3, x4, x5, x6)) goto error
#define RECV_MSG7(s, h, x, x1, x2, x3, x4, x5, x6, x7) if(recv_msg(s, h, x, x1, x2, x3, x4, x5, x6, x7)) goto error
#define RECV_MSG8(s, h, x, x1, x2, x3, x4, x5, x6, x7, x8) if(recv_msg(s, h, x, x1, x2, x3, x4, x5, x6, x7, x8)) goto error
#define RECV_MSG9(s, h, x, x1, x2, x3, x4, x5, x6, x7, x8, x9) if(recv_msg(s, h, x, x1, x2, x3, x4, x5, x6, x7, x8), x9) goto error

int	send_email(const char *smtp_server, const char *smtp_helo, const char *smtp_email,
               const char *mailto, const char *mailsubject, const char *mailbody)
{
    char* p;
    char mailserv[256];
    int smtp_port;
    time_t email_time;
    struct tm local_time;
    char str_time[64];
    SOCKET s;

    /* Get port (default 25) */
    strNcpy(mailserv, smtp_server, sizeof(mailserv));
    p = strrchr(mailserv, ':');
    if(p) {
        *p = '\0';
        smtp_port = atoi(p+1);
        smtp_server = mailserv;
    } else {
        smtp_port = 25;
        smtp_server = mailserv;
    }

    DEBUG("[send_email] connecting to smtp server %s:%d", smtp_server, smtp_port);
    if((s = tcp_connect(smtp_server, smtp_port)) == -1)
        return -1;

    RECV_MSG1(s, "recv welcome", OK_220, NULL);

    if(smtp_helo && smtp_helo[0]) {
        SEND_MSG1(s, "HELO %s\r\n", smtp_helo);
        RECV_MSG1(s, "recv helo", OK_250, NULL);
    }

    SEND_MSG1(s, "MAIL FROM: <%s>\r\n", smtp_email);
    RECV_MSG1(s, "recv mail from", OK_250, NULL);

    SEND_MSG1(s, "RCPT TO: <%s>\r\n", mailto);
    /* May return 251 as well: User not local; will forward to <forward-path>. See RFC825 */
    RECV_MSG2(s, "recv rcpt to", OK_250, OK_251, NULL);

    SEND_MSG0(s, "DATA\r\n");
    RECV_MSG1(s, "recv data", OK_354, NULL);

    time(&email_time);
    localtime_r(&email_time, &local_time);
    strftime(str_time, sizeof(str_time), "%a, %d %b %Y %H:%M:%S %z", &local_time);

    SEND_MSG4(s, "From: <%s>\r\nTo: <%s>\r\nDate: %s\r\nSubject: %s\r\n\r\n",
              smtp_email, mailto, str_time, mailsubject);
    sendn(s, mailbody, strlen(mailbody));
    SEND_MSG0(s, "\r\n.\r\n");
    RECV_MSG1(s, "recv content", OK_250, NULL);

    SEND_MSG0(s, "QUIT\r\n");
    DEBUG("[send_email] ok");
    return 0;

error:
    closesocket(s);
    return -1;
}
