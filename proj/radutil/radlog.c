#include "radutil.h"
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#if defined(RAD_OS_UNIX) || defined(RAD_OS_LINUX)
  #include <syslog.h>
  #define HAVE_SYSLOG_H
#endif
#if defined(RAD_VC)
  #include <winsock2.h> /* for struct timeval */
#else
  #include <sys/time.h> /* for struct timeval */
#endif
#define HAVE_VSNPRINTF

int               radlog_debug_flag             = 1;
radlog_dest_t     radlog_dest                   = RADLOG_STDOUT;
char              radlog_dir             [1024] = "";
char              radlog_file            [1024] = "rad.log";
char              radlog_progname        [256]  = "radlog";
int               radlog_level_hold             = 0;
int               radlog_print_millisec         = 0;
radlog_listener_t radlog_listener               = NULL;

/*
 * Logging facility names
 */
static LRAD_NAME_NUMBER levels[] = {
    { " TRACE ",            L_TRACE },
    { " DEBUG ",            L_DEBUG },
    { " INFO  ",            L_INFO  },
    { " WARN  ",            L_WARN  },
    { " ERROR ",            L_ERROR },
    { " FATAL ",            L_FATAL },
    { NULL, 0 }
};

/*
 * Control log filename, create directory as needed.
 */
void radlog_set_logfile(const char* filename)
{
    char namebuf[1024];
    char* dirsep;
    char* dir = namebuf;

    if(filename == NULL)
        return;

    /* Create directory and set radlog_dir */
    strncpy(namebuf, filename, sizeof(namebuf));
    namebuf[sizeof(namebuf) - 1] = '\0';

    while((dirsep = strchr(dir, '/')) != NULL) {
        *dirsep = '\0';
        mkdir(namebuf, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
        *dirsep = '/';
        dir = dirsep + 1;
    }

    dirsep = strrchr(namebuf, '/');
    if(dirsep != NULL) {
        *dirsep = '\0';
        strncpy(radlog_dir, namebuf, sizeof(radlog_dir));
        radlog_dir[sizeof(radlog_dir) - 1] = '\0';
        *dirsep = '/';
    }

    /* Do set radlog_file */
    strncpy(radlog_file, filename, sizeof(radlog_file));
    radlog_file[sizeof(radlog_file) - 1] = '\0';
}

static void convert_logfilename(const char* origin, char* converted, size_t namelen)
{
    char* p;
    int len;
    struct tm m;
    time_t now;

    len = strlen(origin);
    if(namelen < (size_t)len+10)
        goto cannot_convert;

    now = time(NULL);
    if(now == (time_t)-1)
        goto cannot_convert;

    localtime_r(&now, &m);

    p = strrchr(origin, '.');
    if(p == NULL) {
        sprintf(converted, "%s.%04d%02d%02d",
                origin, m.tm_year+1900, m.tm_mon+1, m.tm_mday);
    } else {
        len = p - origin;
        memcpy(converted, origin, len);
        sprintf(converted+len, ".%04d%02d%02d",
                m.tm_year+1900, m.tm_mon+1, m.tm_mday);
        strcat(converted, p);
    }

    return;

cannot_convert:
    strncpy(converted, origin, namelen);
    converted[namelen-1] = '\0';
}

/*
 *  Log the message to the logfile. Include the severity and
 *  a time stamp.
 */
int vradlog(int lvl, const char *fmt, va_list ap)
{
    FILE *msgfd = NULL;
    char *p;
    char buffer[8192];
    char namebuf[1024];
    int len, len0;

    /*
     *  Filter out by level_hold
     */
    if(radlog_level_hold > (lvl & ~L_CONS)) {
        return 0;
    }

    /*
     *  NOT debugging, and trying to log debug messages.
     *
     *  Throw the message away.
     */
    if ((radlog_debug_flag == 0) && ((lvl & ~L_CONS) <= L_DEBUG)) {
        return 0;
    }

    /*
     *  If we don't want any messages, then
     *  throw them away.
     */
    if (radlog_dest == RADLOG_NULL) {
        return 0;
    }

    if (radlog_dest == RADLOG_STDOUT) {
            msgfd = stdout;

    } else if (radlog_dest == RADLOG_STDERR) {
            msgfd = stderr;

    } else if (radlog_dest != RADLOG_SYSLOG) {
        /*
         *  No log file set.  It must go to stdout.
         */
        if (radlog_file[0] == '\0') {
            msgfd = stdout;

            /*
             *  Else try to open the file.
             */
        } else {
            convert_logfilename(radlog_file, namebuf, sizeof(namebuf));

            if ((msgfd = fopen(namebuf, "a+")) == NULL) {
                fprintf(stderr, "%s: Couldn't open %s for logging: %s\n",
                        radlog_progname, namebuf, strerror(errno));

                fprintf(stderr, "  (");
                vfprintf(stderr, fmt, ap);  /* the message that caused the log */
                fprintf(stderr, ")\n");
                return -1;
            }
        }
    }

#ifdef HAVE_SYSLOG_H
    if (radlog_dest == RADLOG_SYSLOG) {
        *buffer = '\0';
        len0 = 0;
        len = 0;
    } else
#endif

    if(radlog_print_millisec)
    {
        const char* s;
        struct timeval tv;
        struct tm local;
        char deflvl[8];

        gettimeofday(&tv, NULL);
        localtime_r(&tv.tv_sec, &local);

        sprintf(buffer, "%04d/%02d/%02d %02d:%02d:%02d,%03d",
                local.tm_year+1900, local.tm_mon+1, local.tm_mday,
                local.tm_hour, local.tm_min, local.tm_sec,
                (int)tv.tv_usec/1000);

        sprintf(deflvl, " L%04X ", (lvl & ~L_CONS));
        s = lrad_int2str(levels, (lvl & ~L_CONS), deflvl);

        len0 = strlen(buffer);
        strcat(buffer, s);
        len = strlen(buffer);
    }
    else
    {
        const char *s;
        time_t tv;
        struct tm local;
        char deflvl[8];

        tv = time(NULL);
        localtime_r(&tv, &local);
        sprintf(buffer, "%04d/%02d/%02d %02d:%02d:%02d",
                local.tm_year+1900, local.tm_mon+1, local.tm_mday,
                local.tm_hour, local.tm_min, local.tm_sec);

        sprintf(deflvl, " L%04X ", (lvl & ~L_CONS));
        s = lrad_int2str(levels, (lvl & ~L_CONS), deflvl);

        len0 = strlen(buffer);
        strcat(buffer, s);
        len = strlen(buffer);
    }

#ifdef HAVE_VSNPRINTF
    vsnprintf(buffer + len, sizeof(buffer) - len - 1, fmt, ap);
    buffer[sizeof(buffer) - 2] = '\0';
#else
    vsprintf(buffer + len, fmt, ap);
    if (strlen(buffer) >= sizeof(buffer) - 1) {
        fprintf(stderr, "%s: Too long is log message of %d\n",
                radlog_progname, sizeof(buffer));

        /* What can we do? */
        abort();
    }
#endif

    /*
     *  Filter out characters not in Latin-1.
     */
    for (p = buffer; *p != '\0'; p++) {
        if (*p == '\r' || *p == '\n')
            *p = ' ';
        else if (*p < 32 || (*p >= 128 && *p <= 160))
            *p = '?';
    }
    strcat(buffer, "\n");

    /*
     *   If we're debugging, for small values of debug, then
     *   we don't do timestamps.
     */
    if (radlog_debug_flag == 1) {
        p = buffer + len0;

    } else {
        /*
         *  No debugging, or lots of debugging.  Print
         *  the time stamps.
         */
        p = buffer;
    }

    /*
     *  Call logging listener
     */
    if (radlog_listener)
        radlog_listener(lvl, p);

#ifdef HAVE_SYSLOG_H
    if (radlog_dest != RADLOG_SYSLOG)
#endif
    {
        fputs(p, msgfd);
        if (msgfd == stdout) {
            fflush(stdout);
        } else if (msgfd == stderr) {
            fflush(stderr);
        } else {
            fclose(msgfd);

            if(lvl & L_CONS) {
                if((lvl & ~L_CONS) < L_WARN) {
                    fputs(p, stdout);
                    fflush(stdout);
                } else {
                    fputs(p, stderr);
                    fflush(stderr);
                }

                /*
                 * IF debugging, also to console but no flush
                 */
            } else if(radlog_debug_flag) {
                if((lvl & ~L_CONS) < L_WARN) {
                    fputs(p, stdout);
                } else {
                    fputs(p, stderr);
                }
            }
        }
    }
#ifdef HAVE_SYSLOG_H
    else {          /* it was syslog */
        lvl = (lvl & ~L_CONS);

        if(lvl > 0 && lvl <= L_DEBUG) {
            lvl = LOG_DEBUG;

        } else if(lvl > L_DEBUG && lvl <= L_INFO) {
            lvl = LOG_NOTICE;

        } else if(lvl > L_INFO && lvl <= L_WARN) {
            lvl = LOG_WARNING;

        } else if(lvl > L_WARN && lvl <= L_ERROR) {
            lvl = LOG_ERR;

        } else if(lvl > L_ERROR && lvl <= L_FATAL) {
            lvl = LOG_CRIT;

        } else if(lvl > L_FATAL) {
            lvl = LOG_ALERT;

        } else {
            lvl = LOG_EMERG;

        }

        syslog(lvl, "%s", buffer + len0); /* don't print timestamp */
    }
#endif

    return 0;
}

int radlog_debug(const char *msg, ...)
{
	va_list ap;
	int r;

	va_start(ap, msg);
	r = vradlog(L_DEBUG, msg, ap);
	va_end(ap);

	return r;
}

int radlog_trace(const char *msg, ...)
{
	va_list ap;
	int r;

	va_start(ap, msg);
	r = vradlog(L_TRACE, msg, ap);
	va_end(ap);

	return r;
}

int radlog(int lvl, const char *msg, ...)
{
	va_list ap;
	int r;

	va_start(ap, msg);
	r = vradlog(lvl, msg, ap);
	va_end(ap);

	return r;
}
