#include "radutil.h"
#include "radtest.h"
#include <string.h>
#if defined(RAD_OS_WIN32)
  #include <winsock2.h>
#else
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
#endif

RAD_TEST(strRepl2)
{
    char sql[256];

    static const char* sqlconf = "SELECT * FROM accts "
        "WHERE update_time < ${END_DATE} AND update_time >= ${START_DATE} "
        "AND user = ${USER} AND rownum < ${ROWNUM}";

    static const char* sqlins = "INSERT INTO accts VALUES (${CUR_DATE}) "
        "WHERE user = ${USER}";

    char start_date[16] = "'20050714'";
    char end_date[16] = "'20050919'";
    char user[8] = "'Alice'";
    char rownum[8] = "100";

    const LRAD_NAME_MAP repls[] = {
        { "${START_DATE}", start_date },
        { "${END_DATE}",   end_date   },
        { "${CUR_DATE}",   "SYSDATE"  },
        { "${ROWNUM}",     rownum     },
        { "${USER}",       user       },
        { NULL, NULL }
    };

    rad_assert(strcmp(strRepl(sql, sizeof(sql), sqlconf, repls),
                      "SELECT * FROM accts "
                      "WHERE update_time < '20050919' AND update_time >= '20050714' "
                      "AND user = 'Alice' AND rownum < 100") == 0);

    strscpy(user, "'Jack'");
    strscpy(end_date, "'20050920'");
    sNprintf(rownum, sizeof(rownum), "%d", 36);

    rad_assert(strcmp(strRepl(sql, sizeof(sql), sqlconf, repls),
                      "SELECT * FROM accts "
                      "WHERE update_time < '20050920' AND update_time >= '20050714' "
                      "AND user = 'Jack' AND rownum < 36") == 0);

    strscpy(user, "Neo");

    rad_assert(strcmp(strRepl(sql, sizeof(sql), sqlins, repls),
                      "INSERT INTO accts VALUES (SYSDATE) WHERE user = Neo") == 0);
}

RAD_TEST(strRepl)
{
    char buf[256];

    static const LRAD_NAME_MAP repls[] = {
        { "${DATE}",    "YYYYMMDD" }, /* order is important! */
        { "${TIME}",    "HH24MISS" },
        { "${NAME}",    "Dick"     },
        { "",           "break"    },
        { "${WEEKDAY}", "Sunday"   },
        { "",           "break"    },
        { "${MONTH}",   "Jan."     },
        { NULL, NULL },
    };

    rad_assert(strcmp(strRepl(buf, sizeof(buf),
                              "abcdefg",
                              repls),
                      "abcdefg"
                   ) == 0);
    rad_assert(strcmp(strRepl(buf, sizeof(buf),
                              "abc${DATE}de${TIME}fg",
                              repls),
                      "abcYYYYMMDDdeHH24MISSfg"
                   ) == 0);
    rad_assert(strcmp(strRepl(buf, sizeof(buf),
                              "Hello, ${NAME}! Good for ${WEEKDAY} in ${MONTH}.",
                              repls),
                      "Hello, Dick! Good for Sunday in Jan.."
                   ) == 0);
    rad_assert(strcmp(strRepl(buf, sizeof(buf),
                              "${NAME}${WEEKDAY}${MONTH}${DATE}${TIME}",
                              repls),
                      "DickSundayJan.YYYYMMDDHH24MISS"
                   ) == 0);
    rad_assert(strcmp(strRepl(buf, sizeof(buf),
                              "${NAME}${WEEKDAY}${MONTH}${DTE}${TIME}",
                              repls),
                      "DickSundayJan.${DTE}HH24MISS"
                   ) == 0);

    static const LRAD_NAME_MAP repls_null[] = {
        { NULL, NULL }
    };

    rad_assert(strcmp(strRepl(buf, sizeof(buf), "abc${DATE}de${TIME}fg", repls_null),
                      "abc${DATE}de${TIME}fg") == 0);

    rad_assert(strcmp(strRepl(buf, 16,
                              "${NAME}${WEEKDAY}${MONTH}${DTE}${TIME}",
                              repls),
                      "DickSundayJan.$"
                   ) == 0);

    rad_assert(strcmp(strRepl(buf, sizeof(buf),
                              "${NAME} ${NAME} ${NAME} ${TIME}",
                              repls),
                      "Dick Dick Dick HH24MISS"
                   ) == 0);
}

#if 0

RAD_TEST(ip_hostname)
{
    char buf[256];

    rad_assert(strcmp(ip_hostname(buf, sizeof(buf),
                                  inet_addr("61.182.213.238")),
                      "61.182.213.238") == 0);
    rad_assert(strcmp(ip_hostname(buf, sizeof(buf),
                                  inet_addr("127.0.0.1")),
                      "127.0.0.1") == 0);
    rad_assert(strcmp(ip_hostname(buf, sizeof(buf),
                                  inet_addr("192.168.7.25")),
                      "192.168.7.25") == 0);
    librad_dodns = 1;
    rad_assert(strcmp(ip_hostname(buf, sizeof(buf),
                                  inet_addr("192.168.7.25")),
                      "huanghao") == 0);
    rad_assert(strcmp(ip_hostname(buf, sizeof(buf),
                                  inet_addr("127.0.0.1")),
                      "huanghao") == 0);
    rad_assert(strcmp(ip_hostname(buf, sizeof(buf),
                                  inet_addr("61.182.213.238")),
                      "61.182.213.238") == 0);
    librad_dodns = 0;
}

RAD_TEST(ip_getaddr)
{
    rad_assert(ip_getaddr("192.168.7.25")
               == inet_addr("192.168.7.25"));
    rad_assert(ip_getaddr("127.0.0.1")
               == inet_addr("127.0.0.1"));
    rad_assert(ip_getaddr("localhost")
               == inet_addr("127.0.0.1"));
    rad_assert(ip_getaddr("newsmth.org")
               == inet_addr("61.182.213.238"));
}

#endif

RAD_TEST(trim_adjust2)
{
    char buf[] = "";
    char* p = buf;
    int len = trim_adjust(&p, " ");
    rad_assert(p[0] == '\0');
    rad_assert(len == 0);
}

RAD_TEST(trim_adjust)
{
    char buf[] = " \t \n Za ku \t \r\n ";

    char* p = buf;
    int len = trim_adjust(&p, " \t\r\n");
    rad_assert(strcmp(p, "Za ku") == 0);
    rad_assert(len == 5);

    len = trim_adjust(&p, " \t\r\n");
    rad_assert(strcmp(p, "Za ku") == 0);
    rad_assert(len == 5);
}
