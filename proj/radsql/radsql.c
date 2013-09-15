#include "radutil.h"
#include "radsql.h"
#include <string.h>
#include <stdio.h>
#include <stddef.h> /* for offsetof */
#include <time.h>

#if defined(RAD_OS_UNIX) || defined(RAD_OS_LINUX)
  #include <dlfcn.h>
#endif

static const char *allowed_chars = NULL;

static CONF_PARSER module_config[] = {
    {"driver",PW_TYPE_STRING_PTR,
     offsetof(SQL_CONFIG,sql_driver), NULL, "mysql"},
    {"server",PW_TYPE_STRING_PTR,
     offsetof(SQL_CONFIG,sql_server), NULL, "localhost"},
    {"port",PW_TYPE_STRING_PTR,
     offsetof(SQL_CONFIG,sql_port), NULL, ""},
    {"login", PW_TYPE_STRING_PTR,
     offsetof(SQL_CONFIG,sql_login), NULL, ""},
    {"password", PW_TYPE_STRING_PTR,
     offsetof(SQL_CONFIG,sql_password), NULL, ""},
    {"dbname", PW_TYPE_STRING_PTR,
     offsetof(SQL_CONFIG,sql_db), NULL, "radius"},
    {"sqltrace", PW_TYPE_BOOLEAN,
     offsetof(SQL_CONFIG,sqltrace), NULL, "no"},
    {"sqltracelevel", PW_TYPE_INTEGER,
     offsetof(SQL_CONFIG,sqltracelevel), NULL, Stringify(SQLTRACELEVEL)},
    {"num_sql_socks", PW_TYPE_INTEGER,
     offsetof(SQL_CONFIG,num_sql_socks), NULL, "5"},
    {"connect_failure_retry_delay", PW_TYPE_INTEGER,
     offsetof(SQL_CONFIG,connect_failure_retry_delay), NULL, "60"},
    {"safe-characters", PW_TYPE_STRING_PTR,
     offsetof(SQL_CONFIG,allowed_chars), NULL,
    "@abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-_: /"},

    {NULL, -1, 0, NULL, NULL}
};

static CONF_PARSER log_config[] = {

    {"debug_flag", PW_TYPE_INTEGER,    offsetof(LOG_CONFIG, log_debug_flag), NULL, "99"},
    {"level",      PW_TYPE_INTEGER,    offsetof(LOG_CONFIG, log_level),      NULL, "0"},
    {"dest",       PW_TYPE_INTEGER,    offsetof(LOG_CONFIG, log_dest),       NULL, "0"},
    {"file",       PW_TYPE_STRING_PTR, offsetof(LOG_CONFIG, log_file), NULL, "log/sql.log"},
    {"millisec",   PW_TYPE_BOOLEAN,    offsetof(LOG_CONFIG, log_millisec), NULL, "yes"},

    {NULL, -1, 0, NULL, NULL}
};

static int sql_init_socketpool(SQL_INST * inst);
static void sql_poolfree(SQL_INST * inst);

/***********************************************************************
 * start of main routines
 ***********************************************************************/

int rlm_sql_init(void)
{
    /*
     * FIXME:
     * We should put the sqlsocket array here once
     * the module code is reworked to not unload
     * modules on HUP.  This way we can have
     * persistant connections.  -jcarneal
     */
    return 0;
}

/*
 *  Translate the SQL queries.
 */
int sql_escape_func(char *out, size_t outlen, const char *in)
{
    int len = 0;

    rad_assert(allowed_chars != NULL);

    while (in[0]) {
        /*
         *  Only one byte left.
         */
        if (outlen <= 1) {
            break;
        }

        /*
         *  Non-printable characters get replaced with their
         *  mime-encoded equivalents.
         */
        if ((in[0] < 32) ||
            strchr(allowed_chars, *in) == NULL) {
            snprintf(out, outlen, "=%02X", (unsigned char) in[0]);
            in++;
            out += 3;
            outlen -= 3;
            len += 3;
            continue;
        }

        /*
         *  Else it's a nice character.
         */
        *out = *in;
        out++;
        in++;
        outlen--;
        len++;
    }
    *out = '\0';
    return len;
}

int rlm_sql_detach(SQL_INST *instance)
{
    SQL_INST *inst = instance;

    rad_assert(instance != NULL);

    if (inst->sqlpool) {
        sql_poolfree(inst);

        /* + 2005/10/09 Huanghao. Provide here as detach hook. */
        rlm_sql_set_logging_parameter(NULL, inst);
    }

    if(inst->handle) {
        /**
         * + 2005/08/18 Huanghao
         * Load sqldrivers dynamically!
         */
        if(dlclose(inst->handle))
            radlog(L_WARN, "[rlm_sql] detach: Can not unload driver \"%s\": %s",
                   inst->config->sql_driver, dlerror());
    }

    if (inst->config) {
        int i;

        /*
         *  Free up dynamically allocated string pointers.
         */
        for (i = 0; module_config[i].name != NULL; i++) {
            char **p;
            if (module_config[i].type != PW_TYPE_STRING_PTR) {
                continue;
            }

            /*
             *  Treat 'config' as an opaque array of bytes,
             *  and take the offset into it.  There's a
             *      (char*) pointer at that offset, and we want
             *  to point to it.
             */
            p = (char **) (((char *)inst->config) + module_config[i].offset);
            if (!*p) { /* nothing allocated */
                continue;
            }
            rad_free(*p);
            *p = NULL;
        }
        rad_free(inst->config);
        inst->config = NULL;
    }

    /**
     * + 2005/08/18 Huanghao
     */
    if(inst->logconfig) {
        int i;
        for (i = 0; log_config[i].name != NULL; i++) {
            char **p;
            if (log_config[i].type != PW_TYPE_STRING_PTR) {
                continue;
            }
            p = (char **) (((char *)inst->logconfig) + log_config[i].offset);
            if (!*p) { /* nothing allocated */
                continue;
            }
            rad_free(*p);
            *p = NULL;
        }
        rad_free(inst->logconfig);
        inst->logconfig = NULL;
    }

    rad_free(inst);

    return 0;
}

int rlm_sql_instantiate(CONF_SECTION * conf, SQL_INST **instance, CONF_SECTION * logging)
{
    SQL_INST *inst;

    inst = rad_malloc(sizeof(SQL_INST));
    memset(inst, 0, sizeof(SQL_INST));

    inst->config = rad_malloc(sizeof(SQL_CONFIG));
    memset(inst->config, 0, sizeof(SQL_CONFIG));

    inst->logconfig = rad_malloc(sizeof(LOG_CONFIG));
    memset(inst->logconfig, 0, sizeof(LOG_CONFIG));

    /*
     * If the configuration parameters can't be parsed, then
     * fail.
     */
    if (cf_section_parse(conf, inst->config, module_config) < 0) {
        rlm_sql_detach(inst);
        return -1;
    }

    if (inst->config->num_sql_socks > MAX_SQL_SOCKS) {
        radlog(L_ERROR|L_CONS, "[rlm_sql] instantiate: "
               "Number of sqlsockets (%d) cannot exceed MAX_SQL_SOCKS (%d)",
               inst->config->num_sql_socks, MAX_SQL_SOCKS);
        rlm_sql_detach(inst);
        return -1;
    }

    /*
     *  Sanity check for crazy people.
     */
    if (strncasecmp(inst->config->sql_driver, "rlm_sql_", 8) != 0) {
        radlog(L_ERROR|L_CONS, "[rlm_sql] instantiate: "
               "\"%s\" is NOT an SQL driver!",
               inst->config->sql_driver);
        rlm_sql_detach(inst);
        return -1;
    }
    if(inst->config->sqltracelevel < 0) {
        inst->config->sqltracelevel = 0;
    } else if(inst->config->sqltracelevel > 65535) {
        inst->config->sqltracelevel = 65535;
    }

    /**
     * + 2005/08/18 Huanghao
     * Load sqldrivers dynamically!
     */
    inst->handle = dlopen(inst->config->sql_driver, RTLD_NOW|RTLD_GLOBAL);
    if(inst->handle == NULL) {
        radlog(L_ERROR|L_CONS, "[rlm_sql] instantiate: Can not link driver \"%s\": %s",
               inst->config->sql_driver, dlerror());
        radlog(L_ERROR|L_CONS, "[rlm_sql] instantiate: Make sure it (and all its "
               "dependent libraries!) are in the search path of your system's ld");
        rlm_sql_detach(inst);
        return -1;
    }

    inst->module = (rlm_sql_module_t *) dlsym(inst->handle, inst->config->sql_driver);
    if(inst->module == NULL) {
        radlog(L_ERROR|L_CONS, "[rlm_sql] instantiate: Can not link symbol \"%s\": %s",
               inst->config->sql_driver, dlerror());
        rlm_sql_detach(inst);
        return -1;
    }

    radlog(L_INFO, "[rlm_sql] instantiate: Driver \"%s\" (module \"%s\") loaded and linked",
           inst->config->sql_driver, inst->module->name);
    radlog(L_INFO, "[rlm_sql] instantiate: Attempting to connect to %s@%s:%s/%s",
           inst->config->sql_login, inst->config->sql_server,
           inst->config->sql_port, inst->config->sql_db);

    if(logging != NULL &&
       cf_section_parse(logging, inst->logconfig, log_config) != 0) {
        rlm_sql_detach(inst);
        return -1;
    }

    /* + 2005/10/09 Huanghao. Provide here as instantiate hook. */
    rlm_sql_set_logging_parameter(inst->logconfig, inst);

    if (sql_init_socketpool(inst) < 0) {
        rlm_sql_detach(inst);
        return -1;
    }

    allowed_chars = inst->config->allowed_chars;

    *instance = inst;

    return 0;
}

int rlm_sql_destroy(void)
{
    return 0;
}

/*
 * Connect to a server.  If error, set this socket's state to be
 * "sockunconnected" and set a grace period, during which we won't try
 * connecting again (to prevent unduly lagging the server and being
 * impolite to a DB server that may be having other issues).  If
 * successful in connecting, set state to sockconnected.
 * - chad
 */
static int connect_single_socket(SQLSOCK *sqlsocket, SQL_INST *inst)
{
    int rcode;
    radlog(L_DEBUG, "[rlm_sql] connect_single_socket: Attempting to connect %s #%d",
           inst->module->name, sqlsocket->id);

    rcode = (inst->module->sql_init_socket)(sqlsocket, inst->config);
    if (rcode == 0) {
        radlog(L_DEBUG, "[rlm_sql] connect_single_socket: Connected new DB handle, #%d",
               sqlsocket->id);
        sqlsocket->state = sockconnected;
        return 0;
    }

    /*
     *  Error, or SQL_DOWN.
     */
    radlog(L_WARN|L_CONS, "[rlm_sql] connect_single_socket: Failed to connect DB handle #%d",
           sqlsocket->id);
    inst->connect_after = time(NULL) + inst->config->connect_failure_retry_delay;
    sqlsocket->state = sockunconnected;
    return -1;
}

/*************************************************************************
 *
 *  Function: sql_init_socketpool
 *
 *  Purpose: Connect to the sql server, if possible
 *
 *************************************************************************/
static int sql_init_socketpool(SQL_INST * inst)
{
    int i, rcode;
    int success = 0;
    SQLSOCK *sqlsocket;

    inst->connect_after = 0;
    inst->sqlpool = NULL;

    for (i = 0; i < inst->config->num_sql_socks; i++) {
        radlog(L_DEBUG, "[rlm_sql] init_socketpool: starting %d", i);

        sqlsocket = rad_malloc(sizeof(SQLSOCK));
        sqlsocket->conn = NULL;
        sqlsocket->id = i;
        sqlsocket->state = sockunconnected;

        rcode = 0;
#if defined(HAVE_PTHREAD_H)
        rcode = pthread_mutex_init(&sqlsocket->mutex, NULL);
#elif defined(HAVE_WIN32THREAD_H)
        InitializeCriticalSection(&sqlsocket->crit); /* In low memory situations, InitializeCriticalSection can raise a STATUS_NO_MEMORY exception. */
#endif
        sqlsocket->inuse = 0;
        if (rcode != 0) {
            radlog(L_ERROR|L_CONS, "[rlm_sql] init_socketpool: "
                   "Failed to init lock: returns (%d)", rcode);
            rad_free(sqlsocket);
            return -1;
        }

        if (time(NULL) > inst->connect_after) {
            /*
             *  This sets the sqlsocket->state, and
             *  possibly also inst->connect_after
             */
            if (connect_single_socket(sqlsocket, inst) == 0) {
                success = 1;
            }
        }

        /* Add this socket to the list of sockets */
        sqlsocket->next = inst->sqlpool;
        inst->sqlpool = sqlsocket;
    }
    inst->last_used = NULL;

    if (!success) {
        radlog(L_WARN, "[rlm_sql] init_socketpool: Failed to connect to any SQL server.");
    }

    return 0;
}

/*************************************************************************
 *
 *  Function: sql_close_socket
 *
 *  Purpose: Close and free a sql sqlsocket
 *
 *************************************************************************/
static int sql_close_socket(SQL_INST *inst, SQLSOCK * sqlsocket)
{
    int rcode;
    radlog(L_DEBUG, "[rlm_sql] close_socket: Closing sqlsocket %d",
           sqlsocket->id);
    if (sqlsocket->state == sockconnected) {
        (inst->module->sql_close)(sqlsocket, inst->config);
    }
    if (inst->module->sql_destroy_socket) {
        (inst->module->sql_destroy_socket)(sqlsocket, inst->config);
    }
    rad_assert(sqlsocket->inuse == 0);
    sqlsocket->inuse = 0;
    rcode = 0;
#if defined(HAVE_PTHREAD_H)
    rcode = pthread_mutex_destroy(&sqlsocket->mutex);
#elif defined(HAVE_WIN32THREAD_H)
    DeleteCriticalSection(&sqlsocket->crit);
#endif
    if(rcode != 0) {
        radlog(L_WARN, "[rlm_sql] close_socket: "
               "Failed to destroy lock: returns (%d)", rcode);
    }
    rad_free(sqlsocket);
    return 0;
}

/*************************************************************************
 *
 *     Function: sql_poolfree
 *
 *     Purpose: Clean up and free sql pool
 *
 *************************************************************************/
static void sql_poolfree(SQL_INST * inst)
{
    SQLSOCK *cur;
    SQLSOCK *next;

    for (cur = inst->sqlpool; cur; cur = next) {
        next = cur->next;
        sql_close_socket(inst, cur);
    }

    inst->sqlpool = NULL;
}

/*************************************************************************
 *
 *  Function: sql_get_socket
 *
 *  Purpose: Return a SQL sqlsocket from the connection pool
 *
 *************************************************************************/
SQLSOCK * sql_get_socket(SQL_INST * inst)
{
    SQLSOCK *cur, *start;
    int tried_to_connect = 0;
    int unconnected = 0;
    int rcode, locked;

    /*
     *  Start at the last place we left off.
     */
    start = inst->last_used;
    if (!start) start = inst->sqlpool;

    cur = start;

    rcode = 0;
    locked = 0;
    while (cur) {
#if defined(HAVE_PTHREAD_H)
        /*
         *  If this socket is in use by another thread,
         *  skip it, and try another socket.
         *
         *  If it isn't used, then grab it ourselves.
         */
        if ((rcode = pthread_mutex_trylock(&cur->mutex)) != 0) {
            goto next;
        } /* else we now have the lock */
        else {
            locked = 1;
            TRACE("[rlm_sql] get_socket: Obtained lock with handle %d", cur->id);
        }
#elif defined(HAVE_WIN32THREAD_H)
        if (!TryEnterCriticalSection(&cur->crit)) {
            goto next;
        }
        else {
            locked = 1;
            TRACE("[rlm_sql] get_socket: Obtained lock with handle %d", cur->id);
        }
#endif

        if (locked) {
            /* rad_assert(cur->inuse == 0); */
            /* We would get here on WIN32 within the same thread, so just ignore it. */
        }

        if(cur->inuse == 1) {
            if (locked) {
#if defined(HAVE_PTHREAD_H)
            if((rcode = pthread_mutex_unlock(&cur->mutex)) != 0) {
                radlog(L_FATAL|L_CONS, "[rlm_sql] get_socket: "
                       "Can not release lock with handle %d: returns (%d)",
                       cur->id, rcode);
            } else {
                TRACE("[rlm_sql] get_socket: Released lock with handle %d", cur->id);
            }
#elif defined(HAVE_WIN32THREAD_H)
            LeaveCriticalSection(&cur->crit);
            TRACE("[rlm_sql] get_socket: Released lock with handle %d", cur->id);
#endif
            }
            goto next;
        }
        else {
            rad_assert(cur->inuse == 0);
            cur->inuse = 1;
        }

        /*
         *  If we happen upon an unconnected socket, and
         *  this instance's grace period on
         *  (re)connecting has expired, then try to
         *  connect it.  This should be really rare.
         */
        if ((cur->state == sockunconnected) && (time(NULL) > inst->connect_after)) {
            radlog(L_INFO, "[rlm_sql] get_socket: "
                   "Trying to (re)connect unconnected handle %d ...",
                   cur->id);
            tried_to_connect++;
            connect_single_socket(cur, inst);
        }

        /* if we still aren't connected, ignore this handle */
        if (cur->state == sockunconnected) {
            radlog(L_DEBUG, "[rlm_sql] get_socket: "
                   "Ignoring unconnected handle %d ...",
                   cur->id);
            unconnected++;

            rad_assert(cur->inuse == 1);
            cur->inuse = 0;

#if defined(HAVE_PTHREAD_H)
            if((rcode = pthread_mutex_unlock(&cur->mutex)) != 0) {
                radlog(L_FATAL|L_CONS, "[rlm_sql] get_socket: "
                       "Can not release lock with handle %d: returns (%d)",
                       cur->id, rcode);
            } else {
                TRACE("[rlm_sql] get_socket: Released lock with handle %d", cur->id);
            }
#elif defined(HAVE_WIN32THREAD_H)
            LeaveCriticalSection(&cur->crit);
            TRACE("[rlm_sql] get_socket: Released lock with handle %d", cur->id);
#endif

            goto next;
        }

        /* should be connected, grab it */
        radlog(L_DEBUG, "[rlm_sql] get_socket: Obtained sql socket id: %d",
               cur->id);

        if (unconnected != 0 || tried_to_connect != 0) {
            radlog(L_INFO, "[rlm_sql] get_socket: "
                   "got socket %d after skipping %d unconnected handles, "
                   "tried to reconnect %d though",
                   cur->id, unconnected, tried_to_connect);
        }

        /*
         *  The socket is returned in the locked
         *  state.
         *
         *  We also remember where we left off,
         *  so that the next search can start from
         *  here.
         *
         *  Note that multiple threads MAY over-write
         *  the 'inst->last_used' variable.  This is OK,
         *  as it's a pointer only used for reading.
         */
        inst->last_used = cur->next;
        return cur;

        /* move along the list */
    next:
        cur = cur->next;

        /*
         *  Because we didnt start at the start, once we
         *  hit the end of the linklist, we should go
         *  back to the beginning and work toward the
         *  middle!
         */
        if (!cur) {
            cur = inst->sqlpool;
        }

        /*
         *  If we're at the socket we started
         */
        if (cur == start) {
            break;
        }
    }

    /* We get here if every DB handle is unconnected and
     * unconnectABLE, or in use */
    radlog(L_WARN, "[rlm_sql] get_socket: "
           "There are no DB handles to use! skipped %d, tried to connect %d",
           unconnected, tried_to_connect);
    return NULL;
}

/*************************************************************************
 *
 *  Function: sql_release_socket
 *
 *  Purpose: Frees a SQL sqlsocket back to the connection pool
 *
 *************************************************************************/
int sql_release_socket(SQL_INST * inst, SQLSOCK * sqlsocket)
{
    int rcode;

    rad_assert(sqlsocket->inuse == 1);
    sqlsocket->inuse = 0;

    rcode = 0;
#if defined(HAVE_PTHREAD_H)
    if((rcode = pthread_mutex_unlock(&sqlsocket->mutex)) != 0) {
        radlog(L_FATAL|L_CONS, "[rlm_sql] release_socket: "
               "Can not release lock with handle %d: returns (%d)",
               sqlsocket->id, rcode);
    } else {
        TRACE("[rlm_sql] release_socket: Released lock with handle %d", sqlsocket->id);
    }
#elif defined(HAVE_WIN32THREAD_H)
    LeaveCriticalSection(&sqlsocket->crit);
    TRACE("[rlm_sql] release_socket: Released lock with handle %d", sqlsocket->id);
#endif

    radlog(L_DEBUG, "[rlm_sql] release_socket: Released sql socket id: %d", sqlsocket->id);

    return 0;
}

/*************************************************************************
 *
 *  Function: rlm_sql_fetch_row
 *
 *  Purpose: call the module's sql_fetch_row and implement re-connect
 *
 *************************************************************************/
int rlm_sql_fetch_row(SQLSOCK *sqlsocket, SQL_INST *inst)
{
    int ret;

    if (sqlsocket->conn) {
        ret = (inst->module->sql_fetch_row)(sqlsocket, inst->config);
    } else {
        ret = SQL_DOWN;
    }

    if (ret == SQL_DOWN) {
        /* close the socket that failed, but only if it was open */
        if (sqlsocket->conn) {
            (inst->module->sql_close)(sqlsocket, inst->config);
        }

        /* reconnect the socket */
        if (connect_single_socket(sqlsocket, inst) < 0) {
            radlog(L_ERROR|L_CONS, "[rlm_sql] fetch_row: Reconnect failed, database down?");
            return -1;
        }

        /* retry the query on the newly connected socket */
        ret = (inst->module->sql_fetch_row)(sqlsocket, inst->config);

        if (ret) {
            radlog(L_ERROR, "[rlm_sql] fetch_row: Failed after reconnect");
            return -1;
        }
    }

    return ret;
}

/*************************************************************************
 *
 *  Function: rlm_sql_query
 *
 *  Purpose: call the module's sql_query and implement re-connect
 *
 *************************************************************************/
int rlm_sql_query(SQLSOCK *sqlsocket, SQL_INST *inst, const char *query)
{
    int ret;

    /*
     *  If there's no query, return an error.
     */
    if (!query || !*query) {
        return -1;
    }

    ret = (inst->module->sql_query)(sqlsocket, inst->config, query);

    if (ret == SQL_DOWN) {
        /* close the socket that failed */
        (inst->module->sql_close)(sqlsocket, inst->config);

        /* reconnect the socket */
        if (connect_single_socket(sqlsocket, inst) < 0) {
            radlog(L_ERROR|L_CONS, "[rlm_sql] query: Reconnect failed, database down?");
            return -1;
        }

        /* retry the query on the newly connected socket */
        ret = (inst->module->sql_query)(sqlsocket, inst->config, query);

        if (ret) {
            radlog(L_ERROR, "[rlm_sql] query: Failed after reconnect");
            return -1;
        }
    }

    return ret;
}

/*************************************************************************
 *
 *  Function: rlm_sql_select_query
 *
 *  Purpose: call the module's sql_select_query and implement re-connect
 *
 *************************************************************************/
int rlm_sql_select_query(SQLSOCK *sqlsocket, SQL_INST *inst, const char *query)
{
    int ret;

    /*
     *  If there's no query, return an error.
     */
    if (!query || !*query) {
        return -1;
    }

    ret = (inst->module->sql_select_query)(sqlsocket, inst->config, query);

    if (ret == SQL_DOWN) {
        /* close the socket that failed */
        (inst->module->sql_close)(sqlsocket, inst->config);

        /* reconnect the socket */
        if (connect_single_socket(sqlsocket, inst) < 0) {
            radlog(L_ERROR|L_CONS, "[rlm_sql] select_query: Reconnect failed, database down?");
            return -1;
        }

        /* retry the query on the newly connected socket */
        ret = (inst->module->sql_select_query)(sqlsocket, inst->config, query);

        if (ret) {
            radlog(L_ERROR, "[rlm_sql] select_query: Failed after reconnect");
            return -1;
        }
    }

    return ret;
}
