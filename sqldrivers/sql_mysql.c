/* Compile:
cl /nologo /LDd /W3 /GF /MTd /Zi /O2 /Ob2 /Ferlm_sql_mysql /I../proj /I../site/mysql/include sql_mysql.c ../proj/lib/radutil.a ws2_32.lib ../site/mysql/lib/opt/libmysql.lib /DRAD_DLL
 */

#include <radutil/radutil.h>
#include <radsql/radsql.h>

#include <errmsg.h>
#include <mysql.h>

typedef struct rlm_sql_mysql_sock {
	MYSQL conn;
	MYSQL *sock;
	MYSQL_RES *result;
	SQL_ROW row;
} rlm_sql_mysql_sock;


/*************************************************************************
 *
 *	Function: sql_close
 *
 *	Purpose: database specific close. Closes an open database
 *               connection
 *
 *************************************************************************/
static int sql_close(SQLSOCK * sqlsocket, SQL_CONFIG *config)
{
	rlm_sql_mysql_sock *mysql_sock = sqlsocket->conn;
    rad_assert(mysql_sock != NULL);

	if (mysql_sock && mysql_sock->sock) {
		mysql_close(mysql_sock->sock);
		mysql_sock->sock = NULL;
	}

	return 0;
}


/*************************************************************************
 *
 *	Function: sql_init_socket
 *
 *	Purpose: Establish connection to the db
 *
 *************************************************************************/
static int sql_init_socket(SQLSOCK *sqlsocket, SQL_CONFIG *config)
{
	rlm_sql_mysql_sock *mysql_sock;

    rad_assert(sqlsocket != NULL);
	if (!sqlsocket->conn) {
		sqlsocket->conn = (rlm_sql_mysql_sock *)rad_malloc(sizeof(rlm_sql_mysql_sock));
		if (!sqlsocket->conn) {
			return -1;
		}
	}
    memset(sqlsocket->conn, 0, sizeof(rlm_sql_mysql_sock));

	mysql_sock = sqlsocket->conn;

	mysql_init(&(mysql_sock->conn));
	if (!(mysql_sock->sock = mysql_real_connect(&(mysql_sock->conn),
						    config->sql_server,
						    config->sql_login,
						    config->sql_password,
						    config->sql_db,
						    atoi(config->sql_port),
						    NULL,
						    CLIENT_FOUND_ROWS))) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_mysql] init_socket: "
               "Couldn't connect socket to MySQL server %s@%s:%s",
               config->sql_login, config->sql_server, config->sql_db);
		radlog(L_ERROR|L_CONS, "[rlm_sql_mysql] init_socket: "
               "Mysql error '%s'", mysql_error(&mysql_sock->conn));
        sql_close(sqlsocket, config);
		return -1;
	}

	return 0;
}


/*************************************************************************
 *
 *	Function: sql_destroy_socket
 *
 *	Purpose: Free socket and any private connection data
 *
 *************************************************************************/
static int sql_destroy_socket(SQLSOCK *sqlsocket, SQL_CONFIG *config)
{
	rad_free(sqlsocket->conn);
	sqlsocket->conn = NULL;
	return 0;
}


/*************************************************************************
 *
 *	Function: sql_error
 *
 *	Purpose: database specific error. Returns error associated with
 *               connection
 *
 *************************************************************************/
static char *sql_error(SQLSOCK * sqlsocket, SQL_CONFIG *config)
{
	rlm_sql_mysql_sock *mysql_sock = sqlsocket->conn;

	if (mysql_sock == NULL || mysql_sock->sock == NULL) {
		return "[rlm_sql_mysql] error: no connection to db";
	}
	return (char*)mysql_error(mysql_sock->sock);
}


/*************************************************************************
 *
 *	Function: sql_check_error
 *
 *	Purpose: check the error to see if the server is down
 *
 *************************************************************************/
static int sql_check_error(int error, const char* who,
                           SQLSOCK * sqlsocket, SQL_CONFIG *config)
{
	switch(error) {
	case CR_SERVER_GONE_ERROR:
	case CR_SERVER_LOST:
	case -1:
		radlog(L_DEBUG, "[rlm_sql_mysql] %s: %d, returning SQL_DOWN", who, error);
		return SQL_DOWN;
		break;
	case 0:
		return 0;
		break;
	case CR_OUT_OF_MEMORY:
	case CR_COMMANDS_OUT_OF_SYNC:
	case CR_UNKNOWN_ERROR:
	default:
		radlog(L_ERROR|L_CONS, "[rlm_sql_mysql] %s: %d %s", who, error,
               sql_error(sqlsocket, config));
		return -1;
		break;
	}
}


/*************************************************************************
 *
 *	Function: sql_query
 *
 *	Purpose: Issue a query to the database
 *
 *************************************************************************/
static int sql_query(SQLSOCK * sqlsocket, SQL_CONFIG *config, const char *querystr)
{
	rlm_sql_mysql_sock *mysql_sock = sqlsocket->conn;
    rad_assert(mysql_sock != NULL);

	if (config->sqltrace)
		radlog(config->sqltracelevel, "[rlm_sql_mysql] [SQL] %s", querystr);
	if (mysql_sock->sock == NULL) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_mysql] query: Socket not connected");
		return SQL_DOWN;
	}

	mysql_query(mysql_sock->sock, querystr);
	return sql_check_error(mysql_errno(mysql_sock->sock), "query", sqlsocket, config);
}


/*************************************************************************
 *
 *	Function: sql_store_result
 *
 *	Purpose: database specific store_result function. Returns a result
 *               set for the query.
 *
 *************************************************************************/
static int sql_store_result(SQLSOCK * sqlsocket, SQL_CONFIG *config)
{
	rlm_sql_mysql_sock *mysql_sock = sqlsocket->conn;
    rad_assert(mysql_sock != NULL);

	if (mysql_sock->sock == NULL) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_mysql] store_result: Socket not connected");
		return SQL_DOWN;
	}
	if (!(mysql_sock->result = mysql_store_result(mysql_sock->sock))) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_mysql] store_result: Cannot get result");
		radlog(L_ERROR|L_CONS, "[rlm_sql_mysql] store_result: MYSQL Error: %s",
		       mysql_error(mysql_sock->sock));
		return sql_check_error(mysql_errno(mysql_sock->sock), "store_result", sqlsocket, config);
	}
	return 0;
}


/*************************************************************************
 *
 *	Function: sql_num_fields
 *
 *	Purpose: database specific num_fields function. Returns number
 *               of columns from query
 *
 *************************************************************************/
static int sql_num_fields(SQLSOCK * sqlsocket, SQL_CONFIG *config)
{
	int     num = 0;
	rlm_sql_mysql_sock *mysql_sock = sqlsocket->conn;
    rad_assert(mysql_sock != NULL);

#if MYSQL_VERSION_ID >= 32224
	if (!(num = mysql_field_count(mysql_sock->sock))) {
#else
	if (!(num = mysql_num_fields(mysql_sock->sock))) {
#endif
		radlog(L_ERROR|L_CONS, "[rlm_sql_mysql] num_fields: No Fields");
		radlog(L_ERROR|L_CONS, "[rlm_sql_mysql] num_fields: MYSQL error: %s",
		       mysql_error(mysql_sock->sock));
	}
	return num;
}


/*************************************************************************
 *
 *	Function: sql_select_query
 *
 *	Purpose: Issue a select query to the database
 *
 *************************************************************************/
static int sql_select_query(SQLSOCK *sqlsocket, SQL_CONFIG *config,
			    const char *querystr)
{
	int ret;

	ret = sql_query(sqlsocket, config, querystr);
	if(ret)
		return ret;
	ret = sql_store_result(sqlsocket, config);
	if (ret)
		return ret;

	/* Why? Per http://www.mysql.com/doc/n/o/node_591.html,
	 * this cannot return an error.  Perhaps just to complain if no
	 * fields are found?
	 */
	sql_num_fields(sqlsocket, config);

	return ret;
}


/*************************************************************************
 *
 *	Function: sql_num_rows
 *
 *	Purpose: database specific num_rows. Returns number of rows in
 *               query
 *
 *************************************************************************/
static int sql_num_rows(SQLSOCK * sqlsocket, SQL_CONFIG *config)
{
	rlm_sql_mysql_sock *mysql_sock = sqlsocket->conn;
    rad_assert(mysql_sock != NULL);

	if (mysql_sock->result)
		return (int)mysql_num_rows(mysql_sock->result);

	return 0;
}


/*************************************************************************
 *
 *	Function: sql_fetch_row
 *
 *	Purpose: database specific fetch_row. Returns a SQL_ROW struct
 *               with all the data for the query in 'sqlsocket->row'. Returns
 *		 0 on success, -1 on failure, SQL_DOWN if database is down.
 *
 *************************************************************************/
static int sql_fetch_row(SQLSOCK * sqlsocket, SQL_CONFIG *config)
{
	rlm_sql_mysql_sock *mysql_sock = sqlsocket->conn;
    rad_assert(mysql_sock != NULL);

	/*
	 *  Check pointer before de-referencing it.
	 */
	if (!mysql_sock->result) {
		return SQL_DOWN;
	}

	sqlsocket->row = mysql_fetch_row(mysql_sock->result);

	if (sqlsocket->row == NULL) {
		return sql_check_error(mysql_errno(mysql_sock->sock), "fetch_row", sqlsocket, config);
	}
	return 0;
}


/*************************************************************************
 *
 *	Function: sql_free_result
 *
 *	Purpose: database specific free_result. Frees memory allocated
 *               for a result set
 *
 *************************************************************************/
static int sql_free_result(SQLSOCK * sqlsocket, SQL_CONFIG *config)
{
	rlm_sql_mysql_sock *mysql_sock = sqlsocket->conn;
    rad_assert(mysql_sock != NULL);

	if (mysql_sock->result) {
		mysql_free_result(mysql_sock->result);
		mysql_sock->result = NULL;
	}

	return 0;
}



/*************************************************************************
 *
 *	Function: sql_finish_query
 *
 *	Purpose: End the query, such as freeing memory
 *
 *************************************************************************/
static int sql_finish_query(SQLSOCK * sqlsocket, SQL_CONFIG *config)
{
	return 0;
}



/*************************************************************************
 *
 *	Function: sql_finish_select_query
 *
 *	Purpose: End the select query, such as freeing memory or result
 *
 *************************************************************************/
static int sql_finish_select_query(SQLSOCK * sqlsocket, SQL_CONFIG *config)
{
	sql_free_result(sqlsocket, config);

	return 0;
}


/*************************************************************************
 *
 *	Function: sql_affected_rows
 *
 *	Purpose: End the select query, such as freeing memory or result
 *
 *************************************************************************/
static int sql_affected_rows(SQLSOCK * sqlsocket, SQL_CONFIG *config)
{
	rlm_sql_mysql_sock *mysql_sock = sqlsocket->conn;
    rad_assert(mysql_sock != NULL);

	return (int)mysql_affected_rows(mysql_sock->sock);
}

static void set_logging_parameter(LOG_CONFIG *logconfig)
{
    if(logconfig) {
        radlog_debug_flag = logconfig->log_debug_flag;
        radlog_level_hold = logconfig->log_level;
        radlog_dest = (radlog_dest_t)logconfig->log_dest;
        radlog_set_logfile(logconfig->log_file);
        radlog_print_millisec = logconfig->log_millisec;

        rad_start_heap_tracking();
    } else {
        rad_stop_heap_tracking();
    }
}

/* Exported to rlm_sql */
RAD_SYMBOL
rlm_sql_module_t rlm_sql_mysql = {
	"rlm_sql_mysql41",
	sql_init_socket,
	sql_destroy_socket,
	sql_query,
	sql_select_query,
	sql_store_result,
	sql_num_fields,
	sql_num_rows,
	sql_fetch_row,
	sql_free_result,
	sql_error,
	sql_close,
	sql_finish_query,
	sql_finish_select_query,
	sql_affected_rows,
    set_logging_parameter
};
