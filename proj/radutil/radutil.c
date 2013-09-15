#include "radutil.h"
#include <string.h>
#include <stdio.h>
#if defined(RAD_OS_WIN32)
  #include <winsock2.h>
#else
  #include <netdb.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #ifndef INADDR_NONE
  #define INADDR_NONE	0xffffffff
  #endif
#endif

char* strNcpy(char* dst, const char* src, size_t len)
{
    char* r;
    r = strncpy(dst, src, len);
#ifdef RAD_BOUND_CHECK
    rad_assert(dst[len-1] == '\0');
#endif
    dst[len-1] = '\0';
    return r;
}

char* strNcat(char* dst, const char* src, size_t len)
{
    size_t dlen = strlen(dst); /* dst should be null terminated */

    strNcpy(dst+dlen, src, len-dlen);
    return dst;
}

int sNprintf(char* str, size_t len, const char* fmt, ...)
{
    int r;
    va_list args;

    rad_assert(len > 0);

    va_start(args, fmt);

    r = vsnprintf(str, len, fmt, args);

    va_end(args);

#ifdef RAD_BOUND_CHECK
    rad_assert(r >= 0);
    rad_assert(str[r] == '\0');
#endif
    str[len-1] = '\0';

    return r;
}

int vsNprintf(char* str, size_t len, const char* fmt, va_list args)
{
    int r;

    rad_assert(len > 0);

    r = vsnprintf(str, len, fmt, args);

#ifdef RAD_BOUND_CHECK
    rad_assert(r >= 0);
    rad_assert(str[r] == '\0');
#endif
    str[len-1] = '\0';

    return r;
}

int trim_adjust(char** pcstr, const char* delims)
{
    char* begin = 0, * end = 0;
    int len = 0;

    begin = *pcstr;
    while ( strchr(delims, *begin) != NULL && *begin != '\0' ) begin ++;

    end = begin + strlen(begin) - 1;
    while ( strchr(delims, *end) != NULL && end >= begin ) end --;

    *pcstr = begin;
    len = end-begin+1;
    (*pcstr)[len] = '\0';

    return len;
}

/*
 *  Return an IP address in standard dot notation
 */
char *ip_ntoa(char *buffer, uint32_t ipaddr)
{
    ipaddr = ntohl(ipaddr);

    sprintf(buffer, "%d.%d.%d.%d",
        (ipaddr >> 24) & 0xff,
        (ipaddr >> 16) & 0xff,
        (ipaddr >>  8) & 0xff,
        (ipaddr      ) & 0xff);
    return buffer;
}

/*
 *  Return an IP address from a host
 *  name or address in dot notation.
 */
uint32_t ip_getaddr(const char *host)
{
    struct hostent  *hp;
    uint32_t     a;
#ifdef GETHOSTBYNAMERSTYLE
#if (GETHOSTBYNAMERSTYLE == SYSVSTYLE) || (GETHOSTBYNAMERSTYLE == GNUSTYLE)
    struct hostent result;
    int error;
    char buffer[2048];
#endif
#endif

    if ((a = ip_addr(host)) != htonl(INADDR_NONE))
        return a;

#ifdef GETHOSTBYNAMERSTYLE
#if GETHOSTBYNAMERSTYLE == SYSVSTYLE
    hp = gethostbyname_r(host, &result, buffer, sizeof(buffer), &error);
#elif GETHOSTBYNAMERSTYLE == GNUSTYLE
    if (gethostbyname_r(host, &result, buffer, sizeof(buffer),
                &hp, &error) != 0) {
        radlog(L_ERROR|L_CONS, "[ip_getaddr] can not gethostbyname '%s': h_errno(%d)",
               host, errno);
        return htonl(INADDR_NONE);
    }
#else
    hp = gethostbyname(host);
#endif
#else
    hp = gethostbyname(host);
#endif
    if (hp == NULL) {
        radlog(L_ERROR|L_CONS, "[ip_getaddr] can not gethostbyname '%s': h_errno(%d)",
               host, h_errno);
        return htonl(INADDR_NONE);
    }

    /*
     *  Paranoia from a Bind vulnerability.  An attacker
     *  can manipulate DNS entries to change the length of the
     *  address.  If the length isn't 4, something's wrong.
     */
    if (hp->h_length != 4) {
        radlog(L_ERROR|L_CONS, "[ip_getaddr] gethostbyname returns hostent->h_length is NOT 4",
               host);
        return htonl(INADDR_NONE);
    }

    memcpy(&a, hp->h_addr, sizeof(uint32_t));
    return a;
}

/*
 *  Return an IP address from
 *  one supplied in standard dot notation.
 */
uint32_t ip_addr(const char *ip_str)
{
    struct in_addr  in;

    if (inet_aton(ip_str, &in) == 0)
        return htonl(INADDR_NONE);
    return in.s_addr;
}

/*
 *  Return a printable host name (or IP address in dot notation)
 *  for the supplied IP address.
 */

int     librad_dodns = 0;

char * ip_hostname(char *buf, size_t buflen, uint32_t ipaddr)
{
    struct      hostent *hp;
#ifdef GETHOSTBYADDRRSTYLE
#if (GETHOSTBYADDRRSTYLE == SYSVSTYLE) || (GETHOSTBYADDRRSTYLE == GNUSTYLE)
    char buffer[2048];
    struct hostent result;
    int error;
#endif
#endif

    /*
     *  No DNS: don't look up host names
     */
    if (librad_dodns == 0) {
        ip_ntoa(buf, ipaddr);
        return buf;
    }

#ifdef GETHOSTBYADDRRSTYLE
#if GETHOSTBYADDRRSTYLE == SYSVSTYLE
    hp = gethostbyaddr_r((char *)&ipaddr, sizeof(struct in_addr), AF_INET, &result, buffer, sizeof(buffer), &error);
#elif GETHOSTBYADDRRSTYLE == GNUSTYLE
    if (gethostbyaddr_r((char *)&ipaddr, sizeof(struct in_addr),
                AF_INET, &result, buffer, sizeof(buffer),
                &hp, &error) != 0) {
        hp = NULL;
    }
#else
    hp = gethostbyaddr((char *)&ipaddr, sizeof(struct in_addr), AF_INET);
#endif
#else
    hp = gethostbyaddr((char *)&ipaddr, sizeof(struct in_addr), AF_INET);
#endif
    if ((hp == NULL) ||
        (strlen((char *)hp->h_name) >= buflen)) {
        ip_ntoa(buf, ipaddr);
        return buf;
    }

    strNcpy(buf, (char *)hp->h_name, buflen);
    return buf;
}

int print_file(const char* filename, const char* format, ...)
{
    va_list args;
    FILE* file = fopen(filename, "a+");

    if(file == NULL) {
        radlog(L_ERROR|L_CONS, "[print_file] can not open file '%s'", filename);
        return -1;
    }

    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);

    fclose(file);
    return 0;
}

char* strRepl(char* dst, size_t n,
              const char* src, const LRAD_NAME_MAP* table)
{
	const LRAD_NAME_MAP *this;
    char *p,  *psrc,  *pdst;
    char *p0, *psrc0, *pdst0;
#define BUFSIZE 4096
    char dstbuf[BUFSIZE];
    char srcbuf[BUFSIZE];
    size_t ncopied, len, nleft, temp;

    strscpy(srcbuf, src);
    p0 = srcbuf;
    psrc0 = psrc = srcbuf;
    pdst0 = pdst = dstbuf;

    /* Process replacements one by one */
	for (this = table; this->name1 != NULL; this++) {

        if(this->name1[0] == '\0')
            continue;

        ncopied = 0;
        nleft = BUFSIZE-1;
        psrc = psrc0;
        pdst = pdst0;
#undef BUFSIZE

        while((p = strstr(psrc, this->name1))) {

            /* Copy src part */
            if(nleft == 0) break;
            temp = p-psrc;
            len = temp < nleft ? temp : nleft;
            memcpy(pdst, psrc, len);
            ncopied += len;
            nleft -= len;
            psrc = p+strlen(this->name1);
            pdst += len;

            /* Copy replacement part */
            if(nleft == 0) break;
            temp = strlen(this->name2);
            len = temp < nleft ? temp : nleft;
            memcpy(pdst, this->name2, len);
            ncopied += len;
            nleft -= len;
            pdst += len;
        }

        /* Copy src remains */
        if(p == NULL && nleft != 0) {
            temp = strlen(psrc);
            len = temp < nleft ? temp : nleft;
            memcpy(pdst, psrc, len);
            ncopied += len;
            pdst += len;
        }
        pdst[0] = '\0';

        /* Exchange srcbuf and dstbuf to continue, but remember which
         * buf is the replaced result */
        p0 = pdst0;
        pdst0 = psrc0;
        psrc0 = p0;
    }

    /* p0 is the final result */
    strNcpy(dst, p0, n);
    return dst;
}
