/* Compile:
cl /nologo /LDd /W3 /GF /MTd /Zi /O2 /Ob2 /Ferlm_sql_sybase /I../proj /I../site/sybase/include sql_sybase.c ../proj/lib/radutil.a ws2_32.lib ../site/sybase/lib/libct.lib ../site/sybase/lib/libcs.lib /DRAD_DLL
 */

#include <radutil/radutil.h>
#include <radsql/radsql.h>

#include <ctpublic.h>

typedef struct rlm_sql_sybase_sock {
	CS_CONTEXT	*context;
	CS_CONNECTION	*connection;
	CS_COMMAND	*command;
	char		**results;
	int		id;
	int		in_use;
	struct timeval	tv;
    int     num_fields_cache;
    CS_INT *field_size_cache;
    int     affected_rows_cache;
} rlm_sql_sybase_sock;

#define	MAX_DATASTR_LEN	64


/************************************************************************
* Handler for server messages. Client-Library will call this
* routine when it receives a message from the server.
************************************************************************/

static CS_RETCODE CS_PUBLIC
servermsg_callback(cp, chp, msgp)
CS_CONTEXT         *cp;
CS_CONNECTION      *chp;
CS_SERVERMSG       *msgp;
{
    char mod[1024];

    /*
    ** Print the server and procedure names if supplied.
    */
    if (msgp->svrnlen > 0 && msgp->proclen > 0)
        sNprintf(mod, sizeof(mod), "Server(%s) Notice of Procedure(%s)",
                 msgp->svrname, msgp->proc);
    else if (msgp->svrnlen > 0)
        sNprintf(mod, sizeof(mod), "Server(%s) Notice",
                 msgp->svrname);
    else if (msgp->proclen > 0)
        sNprintf(mod, sizeof(mod), "Server Notice of Procedure(%s)",
                 msgp->proc);
    else
        sNprintf(mod, sizeof(mod), "Server Notice");



    /*
    ** Print the message info.
    */
    radlog(L_ERROR+1, "[rlm_sql_sybase] [%s] "
           "number(%ld) severity(%ld) state(%ld) line(%ld) %s",
           mod,
           (long)msgp->msgnumber, (long)msgp->severity,
           (long)msgp->state, (long)msgp->line,
           msgp->text);

    /*
    ** Server message callbacks must return CS_SUCCEED.
    */
    return (CS_SUCCEED);
}

/************************************************************************
*  Client-Library error handler.
************************************************************************/

static CS_RETCODE CS_PUBLIC
clientmsg_callback(context, conn, emsgp)
CS_CONTEXT         *context;
CS_CONNECTION      *conn;
CS_CLIENTMSG       *emsgp;
{
    /*
    ** Error number: Print the error's severity, number, origin, and
    ** layer. These four numbers uniquely identify the error.
    */
    radlog(L_ERROR+1, "[rlm_sql_sybase] [Client Library] "
           "layer(%ld) origin(%ld) severity(%ld) number(%ld) %s",
           (long)CS_LAYER(emsgp->msgnumber),
           (long)CS_ORIGIN(emsgp->msgnumber),
           (long)CS_SEVERITY(emsgp->msgnumber),
           (long)CS_NUMBER(emsgp->msgnumber),
           emsgp->msgstring);

    if (emsgp->osstringlen > 0)
        radlog(L_ERROR+1, "[rlm_sql_sybase] [Operating System] number(%ld) %s",
               (long)emsgp->osnumber, emsgp->osstring);

    return (CS_SUCCEED);
}

/************************************************************************
*  CS-Library error handler. This function will be invoked
*  when CS-Library has detected an error.
************************************************************************/

static CS_RETCODE CS_PUBLIC
csmsg_callback(context, emsgp)
CS_CONTEXT         *context;
CS_CLIENTMSG       *emsgp;
{
    /*
    ** Print the error number and message.
    */
    radlog(L_ERROR+1, "[rlm_sql_sybase] [CS-Library] "
           "layer(%ld) origin(%ld) severity(%ld) number(%ld) %s",
           (long)CS_LAYER(emsgp->msgnumber),
           (long)CS_ORIGIN(emsgp->msgnumber),
           (long)CS_SEVERITY(emsgp->msgnumber),
           (long)CS_NUMBER(emsgp->msgnumber),
           emsgp->msgstring);

    /*
    ** Print any operating system error information.
    */
    if (emsgp->osstringlen > 0)
        radlog(L_ERROR+1, "[rlm_sql_sybase] [Operating System] number(%ld) %s",
               (long)emsgp->osnumber, emsgp->osstring);

    return (CS_SUCCEED);
}


/*************************************************************************
 *
 *	Function: sql_error
 *
 *	Purpose: database specific error. Returns error associated with
 *               connection
 *
 *************************************************************************/
static char *sql_error(SQLSOCK *sqlsocket, SQL_CONFIG *config) {
	static char	msg = '\0';
	return &msg;
}


/*************************************************************************
 *
 *	Function: sql_close
 *
 *	Purpose: database specific close. Closes an open database
 *               connection and cleans up any open handles.
 *
 *************************************************************************/
static int sql_close(SQLSOCK *sqlsocket, SQL_CONFIG *config) {

	rlm_sql_sybase_sock *sybase_sock = sqlsocket->conn;
    rad_assert(sybase_sock != NULL);

    if(sybase_sock->connection != (CS_CONNECTION *)NULL) {
        ct_close(sybase_sock->connection, CS_FORCE_CLOSE);
        ct_con_drop(sybase_sock->connection);
    }

    if (sybase_sock->context != (CS_CONTEXT *)NULL) {
        ct_exit(sybase_sock->context, CS_FORCE_EXIT);
        cs_ctx_drop(sybase_sock->context);
    }

	sybase_sock->connection = NULL;

	return 0;
}


/*************************************************************************
 *
 *	Function: sql_destroy_socket
 *
 *	Purpose: Free socket and private connection data
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
 *	Function: sql_finish_query
 *
 *	Purpose: End the query, such as freeing memory
 *
 *************************************************************************/
static int sql_finish_query(SQLSOCK *sqlsocket, SQL_CONFIG *config)
{
	rlm_sql_sybase_sock *sybase_sock = sqlsocket->conn;
    rad_assert(sybase_sock != NULL);

	ct_cancel(NULL, sybase_sock->command, CS_CANCEL_ALL);

	if (ct_cmd_drop(sybase_sock->command) != CS_SUCCEED) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] finish_query: "
               "Freeing command structure failed.");
		return -1;
	}

	return 0;
}


/*************************************************************************
 *
 *	Function: sql_num_rows
 *
 *	Purpose: database specific num_rows. Returns number of rows in
 *               query
 *
 *************************************************************************/
static int sql_num_rows(SQLSOCK *sqlsocket, SQL_CONFIG *config) {

	rlm_sql_sybase_sock *sybase_sock = sqlsocket->conn;
	int	num;
    rad_assert(sybase_sock != NULL);

	if (ct_res_info(sybase_sock->command, CS_ROW_COUNT,
                    (CS_INT *)&num, CS_UNUSED, NULL) != CS_SUCCEED) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] num_rows: "
               "Error retrieving row count %s",
               sql_error(sqlsocket, config));
		return -1;
	}
	return num;
}


/*************************************************************************
 *
 *	Function: sql_query
 *
 *	Purpose: Issue a non-SELECT query (ie: update/delete/insert) to
 *               the database.
 *
 *************************************************************************/
static int sql_query(SQLSOCK *sqlsocket, SQL_CONFIG *config, const char *querystr) {

	rlm_sql_sybase_sock *sybase_sock = sqlsocket->conn;

	CS_RETCODE	ret, results_ret;
	CS_INT		result_type;
    CS_INT		count;

    rad_assert(sybase_sock != NULL);

	if (config->sqltrace)
		radlog(config->sqltracelevel, "[rlm_sql_sybase] [SQL] %s", querystr);
    if (sybase_sock->connection == NULL) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] query: Socket not connected");
		return SQL_DOWN;
	}

	if (ct_cmd_alloc(sybase_sock->connection, &sybase_sock->command) != CS_SUCCEED) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] query: "
               "Unable to allocate command structure (ct_cmd_alloc()) %s",
				sql_error(sqlsocket, config));
		return -1;
	}

	if (ct_command(sybase_sock->command, CS_LANG_CMD, (char*)querystr,
                   CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] query: "
               "Unable to initiate command structure (ct_command()) %s",
				sql_error(sqlsocket, config));
        sql_finish_query(sqlsocket, config);
		return -1;
	}

	if (ct_send(sybase_sock->command) != CS_SUCCEED) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] query: "
               "Unable to send command (ct_send()) %s",
				sql_error(sqlsocket, config));
        sql_finish_query(sqlsocket, config);
		return SQL_DOWN;
	}

	/*
	** We'll make three calls to ct_results,
    ** first to get a success indicator, secondly to get a done indicator,
    ** and thirdly to get a "nothing left to handle" status.
	*/
results:
	/*
	** First call to ct_results,
	** we need returncode CS_SUCCEED
	** and result_type CS_CMD_SUCCEED or CS_STATUS_RESULT.
    **
    ** Note: CS_STATUS_RESULT is got when a store procedure without
    ** row result is executed.
	*/

	if ((results_ret = ct_results(sybase_sock->command, &result_type)) == CS_SUCCEED) {
        TRACE("[rlm_sql_sybase] query (step 1): result_type is %d", result_type);
		if (result_type != CS_CMD_SUCCEED && result_type != CS_STATUS_RESULT) {
			if  (result_type == CS_ROW_RESULT) {
				radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] query (step 1): "
                       "sql_query processed a query returning rows. "
                       "Use sql_select_query instead!");
			}
			radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] query (step 1): "
                   "Result failure or unexpected result type from query %s",
                   sql_error(sqlsocket, config));
            sql_finish_query(sqlsocket, config);
			return -1;
		}

        if(result_type == CS_STATUS_RESULT) { /* fetch the status  */
            while ((ret = ct_fetch(sybase_sock->command, CS_UNUSED,
                                   CS_UNUSED, CS_UNUSED, &count)) != CS_END_DATA) {
                if (ret != CS_SUCCEED) {
                    radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] query (step 1): "
                           "Failure retrieving query status %s",
                           sql_error(sqlsocket, config));
                    sql_finish_query(sqlsocket, config);
                    return -1;
                }
            }

            goto results;
        }
	}
	else {
		switch ((int) results_ret)
		{

		case CS_FAIL: /* Serious failure, sybase requires us to
                         cancel and maybe even close connection */
			radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] query (step 1): "
                   "Failure retrieving query results %s",
                   sql_error(sqlsocket, config));
            sql_finish_query(sqlsocket, config);
			if ((ret = ct_cancel(NULL, sybase_sock->command, CS_CANCEL_ALL)) == CS_FAIL) {
				radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] query (step 1): "
                       "Fail to do ct_cancel(), close connection");
				sql_close(sqlsocket, config);
			}
			return -1;
			break;

		default:
			radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] query (step 1): "
                   "Unexpected return value from ct_results() %s",
                   sql_error(sqlsocket, config));
            sql_finish_query(sqlsocket, config);
			return -1;
		}
	}


	/*
	** Second call to ct_results,
	** we need returncode CS_SUCCEED
	** and result_type CS_CMD_DONE.
	*/

	if ((results_ret = ct_results(sybase_sock->command, &result_type)) == CS_SUCCEED) {
        TRACE("[rlm_sql_sybase] query (step 2): result_type is %d", result_type);
		if (result_type != CS_CMD_DONE) {
			radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] query (step 2): "
                   "Result failure or unexpected result type from query %s",
                   sql_error(sqlsocket, config));
            sql_finish_query(sqlsocket, config);
			return -1;
		}

        /*
        ** See the Sybase manual for ct_res_info():
        **
        ** CS_ROW_COUNT: The number of rows affected by the current command.
        ** Can be called only after a ct_results() of type CS_CMD_DONE,
        ** CS_CMD_FAIL or CS_CMD_SUCCEED.
        */

        sybase_sock->affected_rows_cache = sql_num_rows(sqlsocket, config);
	}
	else {
		switch ((int) results_ret)
		{

		case CS_FAIL: /* Serious failure, sybase requires us to
                         cancel and maybe even close connection */
			radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] query (step 2): "
                   "Failure retrieving query results %s",
                   sql_error(sqlsocket, config));
            sql_finish_query(sqlsocket, config);
			if ((ret = ct_cancel(NULL, sybase_sock->command, CS_CANCEL_ALL)) == CS_FAIL) {
                radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] query (step 2): "
                       "Fail to do ct_cancel(), close connection");
				sql_close(sqlsocket, config);
            }
			return -1;
			break;

		default:
			radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] query (step 2): "
                   "Unexpected return value from ct_results() %s",
                   sql_error(sqlsocket, config));
            sql_finish_query(sqlsocket, config);
			return -1;
		}
	}


	/*
	** Third call to ct_results,
	** we need returncode CS_END_RESULTS
	** result_type will be ignored.
	*/

	results_ret = ct_results(sybase_sock->command, &result_type);

	switch ((int) results_ret)
	{

	case CS_FAIL: /* Serious failure, sybase requires us to
                     cancel and maybe even close connection */
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] query (step 3): "
               "Failure retrieving query results %s",
               sql_error(sqlsocket, config));
        sql_finish_query(sqlsocket, config);
		if ((ret = ct_cancel(NULL, sybase_sock->command, CS_CANCEL_ALL)) == CS_FAIL) {
            radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] query (step 3): "
                   "Fail to do ct_cancel(), close connection");
            sql_close(sqlsocket, config);
		}
		return -1;
		break;

	case CS_END_RESULTS:  /* This is where we want to end up */
		break;

	default:
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] query (step 3): "
               "Unexpected return value from ct_results() %s",
               sql_error(sqlsocket, config));
        sql_finish_query(sqlsocket, config);
		return -1;
		break;
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
static int sql_init_socket(SQLSOCK *sqlsocket, SQL_CONFIG *config) {

    char cmdbuf[256];
	rlm_sql_sybase_sock *sybase_sock;

    rad_assert(sqlsocket != NULL);
	if (!sqlsocket->conn) {
		sqlsocket->conn = (rlm_sql_sybase_sock *)rad_malloc(sizeof(rlm_sql_sybase_sock));
		if (!sqlsocket->conn) {
			return -1;
		}
	}
    memset(sqlsocket->conn, 0, sizeof(rlm_sql_sybase_sock));

	sybase_sock = sqlsocket->conn;

	/* Allocate a CS context structure. This should really only be
       done once, but because of the connection pooling design of
       rlm_sql, we'll have to go with one context per connection */

	if (cs_ctx_alloc(CS_VERSION_100, &sybase_sock->context) != CS_SUCCEED) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] init_socket: "
               "Unable to allocate CS context structure (cs_ctx_alloc())");
        sql_close(sqlsocket, config);
		return -1;
	}

	/* Initialize ctlib */

	if (ct_init(sybase_sock->context, CS_VERSION_100) != CS_SUCCEED) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] init_socket: "
               "Unable to initialize Client-Library (ct_init())");
        sql_close(sqlsocket, config);
		return -1;
	}

	/* Install callback functions for error-handling */

    if (cs_config(sybase_sock->context, CS_SET, CS_MESSAGE_CB,
                  (CS_VOID *)csmsg_callback, CS_UNUSED, NULL) != CS_SUCCEED) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] init_socket: "
               "Unable to install CS Library error callback");
        sql_close(sqlsocket, config);
		return -1;
	}

	if (ct_callback(sybase_sock->context, NULL, CS_SET, CS_CLIENTMSG_CB,
                    (CS_VOID *)clientmsg_callback) != CS_SUCCEED) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] init_socket: "
               "Unable to install client message callback");
        sql_close(sqlsocket, config);
		return -1;
	}

	if (ct_callback(sybase_sock->context, NULL, CS_SET, CS_SERVERMSG_CB,
                    (CS_VOID *)servermsg_callback) != CS_SUCCEED) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] init_socket: "
               "Unable to install server message callback");
        sql_close(sqlsocket, config);
		return -1;
	}

	/* Allocate a ctlib connection structure */

	if (ct_con_alloc(sybase_sock->context, &sybase_sock->connection) != CS_SUCCEED) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] init_socket: "
               "Unable to allocate connection structure (ct_con_alloc())");
        sql_close(sqlsocket, config);
		return -1;
	}

	/* Initialize inline error handling for the connection */

/*	if (ct_diag(sybase_sock->connection, CS_INIT, CS_UNUSED, CS_UNUSED, NULL) != CS_SUCCEED) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] init_socket: "
               "Unable to initialize error handling (ct_diag())");
        sql_close(sqlsocket, config);
		return -1;
	} */

	/* Set User and Password properties for the connection */

	if (ct_con_props(sybase_sock->connection, CS_SET, CS_USERNAME, config->sql_login,
					 strlen(config->sql_login), NULL) != CS_SUCCEED) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] init_socket: "
               "Unable to set username for connection (ct_con_props()) %s",
               sql_error(sqlsocket, config));
        sql_close(sqlsocket, config);
		return -1;
	}

	if (ct_con_props(sybase_sock->connection, CS_SET, CS_PASSWORD, config->sql_password,
					strlen(config->sql_password), NULL) != CS_SUCCEED) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] init_socket: "
               "Unable to set password for connection (ct_con_props()) %s",
               sql_error(sqlsocket, config));
        sql_close(sqlsocket, config);
		return -1;
	}

	/* Establish the connection */

	if (ct_connect(sybase_sock->connection, config->sql_server,
                   strlen(config->sql_server)) != CS_SUCCEED) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] init_socket: "
               "Unable to establish connection to symbolic servername %s %s",
				config->sql_server, sql_error(sqlsocket, config));
        sql_close(sqlsocket, config);
		return -1;
	}

    /* Use the database if specified */
    if (config->sql_db[0]) {
        sNprintf(cmdbuf, sizeof(cmdbuf), "use %s", config->sql_db);
        if(sql_query(sqlsocket, config, cmdbuf) == 0)
            sql_finish_query(sqlsocket, config);
    }

	return 0;
}


/*************************************************************************
 *
 *	Function: sql_finish_select_query
 *
 *	Purpose: End the select query, such as freeing memory or result
 *
 *************************************************************************/
static int sql_finish_select_query(SQLSOCK *sqlsocket, SQL_CONFIG *config) {

	rlm_sql_sybase_sock *sybase_sock = sqlsocket->conn;
	int	i=0;
    rad_assert(sybase_sock != NULL);

    if (sybase_sock->results) {
        while(sybase_sock->results[i]) rad_free(sybase_sock->results[i++]);
        rad_free(sybase_sock->results);
        sybase_sock->results=NULL;
    }

    if(sybase_sock->num_fields_cache != 0) {
        rad_free(sybase_sock->field_size_cache);
        sybase_sock->field_size_cache = NULL;
        sybase_sock->num_fields_cache = 0;
    }

	ct_cancel(NULL, sybase_sock->command, CS_CANCEL_ALL);

	if (ct_cmd_drop(sybase_sock->command) != CS_SUCCEED) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] finish_select_query: "
               "Freeing command structure failed.");
		return -1;
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
static int sql_num_fields(SQLSOCK *sqlsocket, SQL_CONFIG *config) {

	rlm_sql_sybase_sock *sybase_sock = sqlsocket->conn;
	int	num;
    rad_assert(sybase_sock != NULL);

	if (ct_res_info(sybase_sock->command, CS_NUMDATA,
                    (CS_INT *)&num, CS_UNUSED, NULL) != CS_SUCCEED) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] num_fields: "
               "Error retrieving column count %s",
               sql_error(sqlsocket, config));
		return -1;
	}
	return num;
}


/*************************************************************************
 *
 *	Function: sql_select_query
 *
 *	Purpose: Issue a select query to the database
 *
 *	Note: Only the first row from queries returning several rows
 *	      will be returned by this function, consequitive rows will
 *	      be discarded.
 *
 *************************************************************************/
static int sql_select_query(SQLSOCK *sqlsocket, SQL_CONFIG *config, const char *querystr) {

	rlm_sql_sybase_sock *sybase_sock = sqlsocket->conn;

	CS_RETCODE	ret, results_ret;
	CS_INT		result_type;
	CS_DATAFMT	descriptor;
    CS_INT      dtype;
    CS_INT      dsize;

	int		colcount,i;
	char    **rowdata = NULL;
    CS_INT  *colsize;

    rad_assert(sybase_sock != NULL);

	if (config->sqltrace)
		radlog(config->sqltracelevel, "[rlm_sql_sybase] [SQL] %s", querystr);
    if (sybase_sock->connection == NULL) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] select_query: Socket not connected");
		return SQL_DOWN;
	}


	if (ct_cmd_alloc(sybase_sock->connection, &sybase_sock->command) != CS_SUCCEED) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] select_query: "
               "Unable to allocate command structure (ct_cmd_alloc()) %s",
               sql_error(sqlsocket, config));
		return -1;
	}

	if (ct_command(sybase_sock->command, CS_LANG_CMD, (char*)querystr,
                   CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] select_query: "
               "Unable to initiate command structure (ct_command()) %s",
               sql_error(sqlsocket, config));
        sql_finish_select_query(sqlsocket, config);
		return -1;
	}

	if (ct_send(sybase_sock->command) != CS_SUCCEED) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] select_query: "
               "Unable to send command (ct_send()) %s",
               sql_error(sqlsocket, config));
        sql_finish_select_query(sqlsocket, config);
		return SQL_DOWN;
	}

	results_ret = ct_results(sybase_sock->command, &result_type);

	switch (results_ret) {

	case CS_SUCCEED:

		switch (result_type) {

		case CS_ROW_RESULT:

		/*
		** Houston, we have a row.
		**
		** We set up a target buffer for the results data, and
		** associate the buffer with the results, but the actual
		** fetching takes place in sql_fetch_row. The layer above
		** MUST call sql_fetch_row and/or sql_finish_select_query
		** or this socket will be unusable and may cause segfaults
		** if reused later on.
		*/

			colcount = sql_num_fields(sqlsocket, config);
            TRACE("[rlm_sql_sybase] select_query: colcount %d", colcount);
            colsize = (CS_INT *)rad_malloc(sizeof(CS_INT) * colcount);
            /* Get number of elements in row result */


			rowdata=(char **)rad_malloc(sizeof(char *) * (colcount+1));	/* Space for pointers */
			memset(rowdata, 0, sizeof(char *) * (colcount+1));       /* NULL-pad the pointers */

			for (i=0; i < colcount; i++) {

                ret = ct_describe(sybase_sock->command, i+1, &descriptor);
                if(ret != CS_SUCCEED) {
                    radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] select_query: "
                           "ct_describe failed %s",
                           sql_error(sqlsocket, config));
                    goto fail;
                }

                dtype = descriptor.datatype;
                dsize = MAX_DATASTR_LEN;

           		/*
        		 * Use the retrieved length of dname to allocate an output
		         * buffer, and then define the output variable (but only
		         * for char/string type columns).
		         */
                switch(dtype) {
		        case CS_CHAR_TYPE:
		        case CS_VARCHAR_TYPE:
		        case CS_TEXT_TYPE:
                    dsize = descriptor.maxlength;
                    TRACE("[rlm_sql_syabse] select_query: column %d, chars type %d, size %d",
                          i+1, dtype, dsize);
                    if(dsize > 1024) {
                        radlog(L_WARN, "[rlm_sql_sybase] select_query: "
                               "column %d, chars type %d, size %d too long! truncate to 1024",
                               i+1, dtype, dsize);
                        dsize = 1024;
                    }
                    rowdata[i] = rad_malloc(dsize+1);
                    colsize[i] = dsize+1;
                    break;
		        case CS_IMAGE_TYPE:
                case CS_BINARY_TYPE:
		        case CS_VARBINARY_TYPE:
		        case CS_BIT_TYPE:
		        case CS_TINYINT_TYPE:
		        case CS_SMALLINT_TYPE:
		        case CS_INT_TYPE:
		        case CS_REAL_TYPE:
		        case CS_FLOAT_TYPE:
		        case CS_MONEY_TYPE:
		        case CS_MONEY4_TYPE:
		        case CS_DATETIME_TYPE:
		        case CS_DATETIME4_TYPE:
		        case CS_NUMERIC_TYPE:
		        case CS_DECIMAL_TYPE:
                    TRACE("[rlm_sql_syabse] select_query: column %d, other type %d, size %d",
                          i+1, dtype, dsize);
                    rowdata[i] = rad_malloc(dsize+1);
                    colsize[i] = dsize+1;
                    break;
                default:
                    radlog(L_WARN, "[rlm_sql_sybase] select_query: "
                           "Unsupported date type (%d) at column %d",
                           dtype, i+1);
			        rowdata[i]=rad_malloc(dsize+1);
                    colsize[i]=dsize+1;
			        break;
                }

			    /*
			    ** Set up the DATAFMT structure that describes our target array
			    ** and tells sybase what we want future ct_fetch calls to do.
			    */
			    descriptor.datatype = CS_CHAR_TYPE;     /* The target buffer is a string */
			    descriptor.format = CS_FMT_NULLTERM;    /* Null termination please */
			    descriptor.maxlength = dsize+1;       	/* The string arrays are this large */
			    descriptor.count = 1;			        /* Fetch one row of data */
			    descriptor.locale = NULL;		        /* Don't do NLS stuff */

				/* Associate the target buffer with the data */
				if (ct_bind(sybase_sock->command, i+1, &descriptor,
                            rowdata[i], NULL, NULL) != CS_SUCCEED) {
					radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] select_query: "
                           "ct_bind() failed %s",
                           sql_error(sqlsocket, config));
					goto fail;
				}

			}
			break;

		case CS_CMD_SUCCEED:
		case CS_CMD_DONE:

			radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] select_query: Query returned no data");
			break;

        case CS_STATUS_RESULT:

            /* Haaa..., Execute store procedure with select_query. */
            radlog(L_DEBUG+1, "[rlm_sql_sybase] select_query: Query returned status");
            colcount = 0;
            colsize = NULL;
            rowdata = NULL;
            break;

		default:

			radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] select_query: "
                   "Unexpected result type from query %s",
                   sql_error(sqlsocket, config));
			sql_finish_select_query(sqlsocket, config);
            return -1;
			break;
		}
		break;

	case CS_FAIL:

		/*
		** Serious failure, sybase requires us to cancel
		** the results and maybe even close the connection.
		*/

		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] select_query: "
               "Failure retrieving query results %s",
               sql_error(sqlsocket, config));
        sql_finish_select_query(sqlsocket, config);
		if ((ret = ct_cancel(NULL, sybase_sock->command, CS_CANCEL_ALL)) == CS_FAIL) {
			radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] select_query: "
                   "Fail to do ct_cancel(), close connection");
			sql_close(sqlsocket, config);
		}
		return -1;
		break;

	default:

		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] select_query: "
               "Unexpected return value from ct_results() %s",
               sql_error(sqlsocket, config));
        sql_finish_select_query(sqlsocket, config);
		return -1;
		break;
	}

    rad_assert(sybase_sock->num_fields_cache == 0);
    sybase_sock->num_fields_cache = colcount;
    rad_assert(sybase_sock->field_size_cache == NULL);
    sybase_sock->field_size_cache = colsize;
    rad_assert(sybase_sock->results == NULL);
    sybase_sock->results = rowdata;

	return 0;

fail:

    rad_free(colsize);
    for(i=0; i<colcount+1; i++) rad_free(rowdata[i]);
    rad_free(rowdata);

    sql_finish_select_query(sqlsocket, config);

    return -1;
}


/*************************************************************************
 *
 *	Function: sql_store_result
 *
 *	Purpose: database specific store_result function. Returns a result
 *               set for the query.
 *
 *************************************************************************/
static int sql_store_result(SQLSOCK *sqlsocket, SQL_CONFIG *config) {
	/*
	** Not needed for Sybase, code that may have gone here is
	** in sql_select_query and sql_fetch_row
	*/
	return 0;
}


/*************************************************************************
 *
 *	Function: sql_fetch_row
 *
 *	Purpose: database specific fetch_row. Returns a SQL_ROW struct
 *               with all the data for the query in 'sqlsocket->row'. Returns
 *		 0 on success, -1 on failure, SQL_DOWN if 'database is down'.
 *
 *************************************************************************/
static int sql_fetch_row(SQLSOCK *sqlsocket, SQL_CONFIG *config) {

	rlm_sql_sybase_sock *sybase_sock = sqlsocket->conn;
    CS_RETCODE  ret;
	CS_INT		count;
    int         colcount;
    CS_INT      *colsize;
    char        **rowdata;
    int         i=0;

    rad_assert(sybase_sock != NULL);

	sqlsocket->row = NULL;

    colcount = sybase_sock->num_fields_cache;
    colsize = sybase_sock->field_size_cache;
    rowdata = sybase_sock->results;
    if(rowdata) {
        for(i=0; i<colcount; i++)
            memset(rowdata[i], 0, colsize[i]);
    }

	ret = ct_fetch(sybase_sock->command, CS_UNUSED, CS_UNUSED, CS_UNUSED, &count);

	switch (ret) {

	case CS_FAIL:

		/*
		** Serious failure, sybase requires us to cancel
		** the results and maybe even close the connection.
		*/

		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] fetch_row: "
               "Failure fething row data %s",
               sql_error(sqlsocket, config));
		if ((ret = ct_cancel(NULL, sybase_sock->command, CS_CANCEL_ALL)) == CS_FAIL) {
			radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] fetch_row: "
                   "Fail to do ct_cancel(), close connection");
			sql_close(sqlsocket, config);
		}
		return -1;
		break;

	case CS_END_DATA:

		return 0;
		break;

	case CS_SUCCEED:

		sqlsocket->row = sybase_sock->results;
		return 0;
		break;

	case CS_ROW_FAIL:

		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] fetch_row: "
               "Recoverable failure fething row data, try again perhaps?");
		return -1;
        break;

	default:

		radlog(L_ERROR|L_CONS, "[rlm_sql_sybase] fetch_row: "
               "Unexpected returncode from ct_fetch");
		return -1;
		break;
	}

}


/*************************************************************************
 *
 *	Function: sql_free_result
 *
 *	Purpose: database specific free_result. Frees memory allocated
 *               for a result set
 *
 *************************************************************************/
static int sql_free_result(SQLSOCK *sqlsocket, SQL_CONFIG *config) {

	/*
	** Not implemented, never called from rlm_sql anyway
	** result buffer is freed in the finish_query functions.
	*/

	return 0;

}


/*************************************************************************
 *
 *	Function: sql_affected_rows
 *
 *	Purpose: Return the number of rows affected by the query (update,
 *               or insert)
 *
 *************************************************************************/
static int sql_affected_rows(SQLSOCK *sqlsocket, SQL_CONFIG *config) {

    rlm_sql_sybase_sock *sybase_sock = sqlsocket->conn;
    rad_assert(sybase_sock != NULL);

	// return sql_num_rows(sqlsocket, config);
    return sybase_sock->affected_rows_cache;
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
rlm_sql_module_t rlm_sql_sybase = {
	"rlm_sql_sybase11",
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
