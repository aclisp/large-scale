/* -*- mode: c -*- */

/* Author:   Huanghao@cellcom
 * Date:     2005/09/29 16:47:00
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RAD_LOG_H__
#define __RAD_LOG_H__

#include <radutil/radmacros.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define L_TRACE     512        /* 0x00000200 */
#define L_DEBUG     1024       /* 0x00000400 */
#define L_INFO      2048       /* 0x00000800 */
#define L_WARN      4096       /* 0x00001000 */
#define L_ERROR     8192       /* 0x00002000 */
#define L_FATAL     16384      /* 0x00004000 */
#define L_CONS      32768      /* 0x00008000 */

typedef enum _radlog_dest_t {
  RADLOG_FILES = 0,
  RADLOG_SYSLOG,
  RADLOG_STDOUT,
  RADLOG_STDERR,
  RADLOG_NULL
} radlog_dest_t;

typedef void (* radlog_listener_t)(int level, const char* message);

/*
 * @return  0 done
 *         -1 can not open destination to log
 */
int radlog(int lvl, const char *msg, ...);
int radlog_debug(const char *msg, ...);
int radlog_trace(const char *msg, ...);

void radlog_set_logfile(const char* filename);

/*
 * Debugging convenience.
 * 1. DEBUG[n] only print debugging messages at a radlog_debug_flag
 *    which is greater than or equal to n
 * 2. A negative radlog_debug_flag will print all debugging messages
 * 3. When it's not about to print, we do not suffer the extra
 *    computation on radlog_debug's arguments.
 * 4. TRACE[n] only print tracing messages at a radlog_debug_flag
 *    which is less than -n
 * 5. By all above, we can conclude than radlog_debug_flag of a value
 *    -1 means print all debugging messages without any tracing
 *    messages, and radlog_debug_flag of a value 0 print nothing.
 */
#define DEBUG  (radlog_debug_flag == 0) ?                           \
    0 : radlog_debug
#define DEBUG2 (radlog_debug_flag >= 0 && radlog_debug_flag <= 1) ? \
    0 : radlog_debug
#define DEBUG3 (radlog_debug_flag >= 0 && radlog_debug_flag <= 2) ? \
    0 : radlog_debug
#define DEBUG4 (radlog_debug_flag >= 0 && radlog_debug_flag <= 3) ? \
    0 : radlog_debug
#define DEBUG5 (radlog_debug_flag >= 0 && radlog_debug_flag <= 4) ? \
    0 : radlog_debug

#define TRACE  (radlog_debug_flag >= -1) ? \
    0 : radlog_trace
#define TRACE2 (radlog_debug_flag >= -2) ? \
    0 : radlog_trace
#define TRACE3 (radlog_debug_flag >= -3) ? \
    0 : radlog_trace
#define TRACE4 (radlog_debug_flag >= -4) ? \
    0 : radlog_trace
#define TRACE5 (radlog_debug_flag >= -5) ? \
    0 : radlog_trace

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*
 * Controlling parameter.
 */
RAD_EXTERN_C int               radlog_debug_flag;
RAD_EXTERN_C radlog_dest_t     radlog_dest;
RAD_EXTERN_C char              radlog_dir             [1024];
RAD_EXTERN_C char              radlog_file            [1024];
RAD_EXTERN_C char              radlog_progname        [256];
RAD_EXTERN_C int               radlog_level_hold;
RAD_EXTERN_C int               radlog_print_millisec;
RAD_EXTERN_C radlog_listener_t radlog_listener;

#endif /* __RAD_LOG_H__ */
