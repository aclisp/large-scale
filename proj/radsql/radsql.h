/* -*- mode: c -*- */

/* Author:   Huanghao@cellcom
 * Date:     2005/10/08 18:42:56
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RAD_SQL_H__
#define __RAD_SQL_H__

#include <radutil/radplatform.h>
#include <radutil/radconffile.h>

#if defined(RAD_OS_WIN32)
  #define _WIN32_WINNT 0x0400 /* for TryEnterCriticalSection */
  #include <windows.h> /* for mutex */
  #define HAVE_WIN32THREAD_H
#else
  #define HAVE_PTHREAD_H
#endif

#ifdef HAVE_PTHREAD_H
  #include <pthread.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* SQL Defines */
#define MAX_QUERY_LEN           4096
#define SQLTRACELEVEL           1000

/* SQL Errors */
#define SQL_DOWN                1 /* for re-connect */

/* SQL Limits */
#define MAX_SQL_SOCKS           256

/**
 * + 2005/08/18 Huanghao
 * When we load sqldrivers dynamically, the logging parameters inside
 * DLL(Shared Object?) would be what it is in libradutil.lib. So
 * logging parameters of main program, which come from configuration
 * files, have no effect on sqldrivers.
 *
 * This is what I do not want, so comes LOG_CONFIG below and the user
 * could call rlm_sql_set_logging_parameter whenever he wants.
 *
 * the rlm_sql_instantiate function can take another CONF_SECTION
 * about logging. If it's not NULL, rlm_sql_set_logging_parameter
 * would be called automatically immediately after sqldriver is
 * linked and loaded.
 */
typedef struct log_config {
    int   log_debug_flag;
    int   log_level;
    int   log_dest;
    char* log_file;
    int   log_millisec;
} LOG_CONFIG;

typedef struct sql_config {
    char   *sql_driver;
    char   *sql_server;
    char   *sql_port;
    char   *sql_login;
    char   *sql_password;
    char   *sql_db;
    int     sqltrace;
    int     sqltracelevel;
    int     num_sql_socks;
    int     connect_failure_retry_delay;
    char   *allowed_chars;

    /* individual driver config */
    void   *localcfg;

} SQL_CONFIG;

typedef char** SQL_ROW;

typedef struct sql_socket {
    int     id;
#if defined(HAVE_PTHREAD_H)
    pthread_mutex_t mutex;
#elif defined(HAVE_WIN32THREAD_H)
    CRITICAL_SECTION crit;
#endif
    int     inuse;
    struct sql_socket *next;
    enum { sockconnected, sockunconnected } state;

    void    *conn;
    SQL_ROW row;
} SQLSOCK;

typedef struct rlm_sql_module_t {
    const char *name;
    int (*sql_init_socket)   (SQLSOCK *sqlsocket, SQL_CONFIG *config);
    int (*sql_destroy_socket)(SQLSOCK *sqlsocket, SQL_CONFIG *config);
    int (*sql_query)         (SQLSOCK *sqlsocket, SQL_CONFIG *config, const char *query);
    int (*sql_select_query)  (SQLSOCK *sqlsocket, SQL_CONFIG *config, const char *query);
    int (*sql_store_result)  (SQLSOCK *sqlsocket, SQL_CONFIG *config);
    int (*sql_num_fields)    (SQLSOCK *sqlsocket, SQL_CONFIG *config);
    int (*sql_num_rows)      (SQLSOCK *sqlsocket, SQL_CONFIG *config);
    int (*sql_fetch_row)     (SQLSOCK *sqlsocket, SQL_CONFIG *config);
    int (*sql_free_result)   (SQLSOCK *sqlsocket, SQL_CONFIG *config);
    char *(*sql_error)       (SQLSOCK *sqlsocket, SQL_CONFIG *config);
    int (*sql_close)         (SQLSOCK *sqlsocket, SQL_CONFIG *config);
    int (*sql_finish_query)  (SQLSOCK *sqlsocket, SQL_CONFIG *config);
    int (*sql_finish_select_query)(SQLSOCK *sqlsocket, SQL_CONFIG *config);
    int (*sql_affected_rows) (SQLSOCK *sqlsocket, SQL_CONFIG *config);
    void (*set_logging_parameter)(LOG_CONFIG *logconfig);
} rlm_sql_module_t;

typedef struct sql_inst {
    time_t      connect_after;
    SQLSOCK     *sqlpool;
    SQLSOCK     *last_used;
    SQL_CONFIG  *config;
    LOG_CONFIG  *logconfig;

    void* handle; /* + 2005/08/18 Huanghao, @see sql.c#rlm_sql_instantiate */
    rlm_sql_module_t *module;
} SQL_INST;

int rlm_sql_init(void);
int rlm_sql_instantiate(CONF_SECTION * conf, SQL_INST **instance, CONF_SECTION * logging);
int rlm_sql_detach(SQL_INST *instance);
int rlm_sql_destroy(void);
int sql_escape_func(char *out, size_t outlen, const char *in);

SQLSOCK *sql_get_socket(SQL_INST * inst);
int     sql_release_socket(SQL_INST * inst, SQLSOCK * sqlsocket);

#define rlm_sql_num_fields(sqlsocket, inst) \
    (inst->module->sql_num_fields)(sqlsocket, inst->config)
#define rlm_sql_finish_select_query(sqlsocket, inst) \
    (inst->module->sql_finish_select_query)(sqlsocket, inst->config)
#define rlm_sql_finish_query(sqlsocket, inst) \
    (inst->module->sql_finish_query)(sqlsocket, inst->config)
#define rlm_sql_affected_rows(sqlsocket, inst) \
    (inst->module->sql_affected_rows)(sqlsocket, inst->config)
#define rlm_sql_set_logging_parameter(logconfig, inst) \
    (inst->module->set_logging_parameter)(logconfig)

int rlm_sql_fetch_row(SQLSOCK *sqlsocket, SQL_INST *inst);
int rlm_sql_query(SQLSOCK *sqlsocket, SQL_INST *inst, const char *query);
int rlm_sql_select_query(SQLSOCK *sqlsocket, SQL_INST *inst, const char *query);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RAD_SQL_H__ */
