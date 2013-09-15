/* Compile:
cl /nologo /LDd /W3 /GX /GF /MTd /Zi /O2 /Ob2 /Ferlm_sql_msado /I../proj sql_msado.cpp ../proj/lib/radutil.a ws2_32.lib /DRAD_DLL
 */

#include <comdef.h>
#include <radutil/radutil.h>
#include <radsql/radsql.h>

#pragma warning (disable: 4146)
#import  <C:\Program Files\Common Files\System\ado\msado15.dll> \
         no_namespace rename("EOF", "EndOfFile")


typedef struct rlm_sql_msado_sock {
    /* _xxxPtr is type of _com_ptr_t */
    _ConnectionPtr *sock;
    _RecordsetPtr  *rset;
    int     affected_rows_cache;
    int     num_fields_cache;
    int     *field_size_cache;
    char	**results;
    int     end_of_records;
} rlm_sql_msado_sock;

#define	MAX_DATASTR_LEN	64

static int sql_dump_error(const _com_error &e, const char* who, const char* what)
{
    radlog(L_ERROR|L_CONS, "[rlm_sql_msado] %s: %s [%s] %s: (%08X) %s",
           who, what, (const char*)e.Source(), (const char*)e.Description(),
           e.Error(), e.ErrorMessage());

    return 0;
}

static int sql_close(SQLSOCK *sqlsocket, SQL_CONFIG *config)
{
	rlm_sql_msado_sock *msado_sock = (rlm_sql_msado_sock*)sqlsocket->conn;
    rad_assert(msado_sock != NULL);

    if(msado_sock->rset) {
        try {
        if((*msado_sock->rset) != NULL && (*msado_sock->rset)->State == adStateOpen)
            (*msado_sock->rset)->Close(); }
        catch(const _com_error &e) {
            sql_dump_error(e, "close", "Recordset.State & Recordset.Close()");
        }
        delete msado_sock->rset;
        msado_sock->rset = NULL;
    }

    if(msado_sock->sock) {
        try {
        if((*msado_sock->sock) != NULL && (*msado_sock->sock)->State == adStateOpen)
            (*msado_sock->sock)->Close(); }
        catch(const _com_error &e) {
            sql_dump_error(e, "close", "Connection.State & Connection.Close()");
        }
        delete msado_sock->sock;
        msado_sock->sock = NULL;
    }

    CoUninitialize();

    return 0;
}

static int sql_init_socket(SQLSOCK *sqlsocket, SQL_CONFIG *config)
{
	rlm_sql_msado_sock *msado_sock;

    rad_assert(sqlsocket != NULL);
	if (!sqlsocket->conn) {
		sqlsocket->conn = (rlm_sql_msado_sock *)rad_malloc(sizeof(rlm_sql_msado_sock));
		if (!sqlsocket->conn) {
			return -1;
		}
	}
    memset(sqlsocket->conn, 0, sizeof(rlm_sql_msado_sock));

	msado_sock = (rlm_sql_msado_sock*)sqlsocket->conn;

    /*
      Typically, the COM library is initialized on a thread only once.
      Subsequent calls to CoInitialize or CoInitializeEx on the same thread
      will succeed, as long as they do not attempt to change the concurrency
      model, but will return S_FALSE. To close the COM library gracefully,
      each successful call to CoInitialize or CoInitializeEx, including those
      that return S_FALSE, must be balanced by a corresponding call to
      CoUninitialize.
    */
    HRESULT hr;
    if(FAILED(hr = CoInitialize(NULL))) {
        _com_error e(hr);
        radlog(L_ERROR|L_CONS, "[rlm_sql_msado] init_socket: "
               "FAILED CoInitialize(NULL): (%08X) %s",
               e.Error(), e.ErrorMessage());
        return -1;
    }

    msado_sock->sock = new _ConnectionPtr;
    if(FAILED(hr = (*msado_sock->sock).CreateInstance(__uuidof(Connection)))) {
        _com_error e(hr);
        radlog(L_ERROR|L_CONS, "[rlm_sql_msado] init_socket: "
               "FAILED CreateInstance(__uuidof(Connection)): (%08X) %s",
               e.Error(), e.ErrorMessage());
        sql_close(sqlsocket, config);
        return -1;
    }

    /* config->sql_db: http://www.codeproject.com/database/connectionstrings.asp */
    try {
        (*msado_sock->sock)->Open(config->sql_db, "", "", adConnectUnspecified);
    } catch(const _com_error& e) {
        sql_dump_error(e, "init_socket", "Connection.Open()");
        sql_close(sqlsocket, config);
        return -1;
    }

    return 0;
}

static int sql_destroy_socket(SQLSOCK *sqlsocket, SQL_CONFIG *config)
{
	rad_free(sqlsocket->conn);
	sqlsocket->conn = NULL;
	return 0;
}

static int sql_finish_query(SQLSOCK * sqlsocket, SQL_CONFIG *config)
{
    return 0;
}

static int sql_query(SQLSOCK * sqlsocket, SQL_CONFIG *config, const char *querystr)
{
	rlm_sql_msado_sock *msado_sock = (rlm_sql_msado_sock*)sqlsocket->conn;
    rad_assert(msado_sock != NULL);

	if (config->sqltrace)
		radlog(config->sqltracelevel, "[rlm_sql_msado] [SQL] %s", querystr);
	if (msado_sock->sock == NULL) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_msado] query: Socket not connected");
		return SQL_DOWN;
	}

    _variant_t vRecordsAffected;
    try {
        (*msado_sock->sock)->Execute(querystr, &vRecordsAffected, adExecuteNoRecords);
    } catch(const _com_error& e) {
        sql_dump_error(e, "query", "Connection.Execute()");
        sql_finish_query(sqlsocket, config);
        return SQL_DOWN;
    }

    try {
        msado_sock->affected_rows_cache = (long)vRecordsAffected;
    } catch(const _com_error& e) {
        sql_dump_error(e, "query", "(long)vRecordsAffected");
        sql_finish_query(sqlsocket, config);
        return -1;
    }

    return 0;
}

static int sql_finish_select_query(SQLSOCK * sqlsocket, SQL_CONFIG *config)
{
    int	i=0;
    rlm_sql_msado_sock *msado_sock = (rlm_sql_msado_sock*)sqlsocket->conn;
    rad_assert(msado_sock != NULL);

    if(msado_sock->rset) {
        try {
        if((*msado_sock->rset) != NULL && (*msado_sock->rset)->State == adStateOpen)
            (*msado_sock->rset)->Close(); }
        catch(const _com_error &e) {
            sql_dump_error(e, "finish_select_query", "Recordset.State & Recordset.Close()");
        }
        delete msado_sock->rset;
        msado_sock->rset = NULL;
    }

    if(msado_sock->results) {
        while(msado_sock->results[i]) rad_free(msado_sock->results[i++]);
        rad_free(msado_sock->results);
        msado_sock->results = NULL;
    }

    if(msado_sock->num_fields_cache != 0) {
        rad_free(msado_sock->field_size_cache);
        msado_sock->field_size_cache = NULL;
        msado_sock->num_fields_cache = 0;
    }

    msado_sock->end_of_records = 0;
    return 0;
}

static int sql_num_fields(SQLSOCK *sqlsocket, SQL_CONFIG *config)
{
    int	num;

	rlm_sql_msado_sock *msado_sock = (rlm_sql_msado_sock*)sqlsocket->conn;
    rad_assert(msado_sock != NULL);

    rad_assert(msado_sock->sock != NULL);
    rad_assert(msado_sock->rset != NULL);

    try {
        num = (*msado_sock->rset)->Fields->Count;
    } catch(const _com_error &e) {
        sql_dump_error(e, "num_fields", "Recordset.Fields.Count");
        return -1;
    }

    return num;
}

static int sql_select_query(SQLSOCK *sqlsocket, SQL_CONFIG *config, const char *querystr)
{
    int     i;
    int		colcount;
    char	**rowdata = NULL;
    int     *colsize;
    int     dsize;
    int     dtype;

    rlm_sql_msado_sock *msado_sock = (rlm_sql_msado_sock*)sqlsocket->conn;
    rad_assert(msado_sock != NULL);

	if (config->sqltrace)
		radlog(config->sqltracelevel, "[rlm_sql_msado] [SQL] %s", querystr);
	if (msado_sock->sock == NULL) {
		radlog(L_ERROR|L_CONS, "[rlm_sql_msado] select_query: Socket not connected");
		return SQL_DOWN;
	}

    rad_assert(msado_sock->rset == NULL);
    rad_assert(msado_sock->end_of_records == 0);
    HRESULT hr;
    msado_sock->rset = new _RecordsetPtr;
    if(FAILED(hr = (*msado_sock->rset).CreateInstance(__uuidof(Recordset)))) {
        _com_error e(hr);
        radlog(L_ERROR|L_CONS, "[rlm_sql_msado] select_query: "
               "FAILED CreateInstance(__uuidof(Recordset)): (%08X) %s",
               e.Error(), e.ErrorMessage());
        sql_finish_select_query(sqlsocket, config);
        return -1;
    }

    try {
        (*msado_sock->rset)->Open(querystr, _variant_t((IDispatch*)(*msado_sock->sock), true),
                                  adOpenForwardOnly, adLockReadOnly, adCmdText);
    } catch(const _com_error& e) {
        sql_dump_error(e, "select_query", "Recordset.Open()");
        sql_finish_select_query(sqlsocket, config);
        return SQL_DOWN;
    }

    try {
        if((*msado_sock->rset)->State != adStateOpen) {
            radlog(L_DEBUG+1, "[rlm_sql_msado] select_query: Query returned nothing");
            return 0;
        }
    } catch(const _com_error& e) {
        sql_dump_error(e, "select_query", "Recordset.State");
        sql_finish_select_query(sqlsocket, config);
        return -1;
    }

    colcount = sql_num_fields(sqlsocket, config);
    TRACE("[rlm_sql_msado] select_query: colcount %d", colcount);
    colsize = (int *)rad_malloc(sizeof(int) * colcount);

    rowdata=(char **)rad_malloc(sizeof(char *) * (colcount+1));
    memset(rowdata, 0, sizeof(char *) * (colcount+1));

    for (i=0; i < colcount; i++) {

        FieldPtr field;

        try {
            field = (*msado_sock->rset)->Fields->GetItem((long)i);
        } catch(const _com_error& e) {
            sql_dump_error(e, "select_query", "Recordset.Fields.GetItem()");
            goto fail;
        }

        try {
            dtype = field->Type;
        } catch(const _com_error& e) {
            sql_dump_error(e, "select_query", "Field.Type");
            goto fail;
        }
        dsize = MAX_DATASTR_LEN;

        switch(dtype) {
        case adChar:
        case adLongVarChar:
        case adVarChar:
            try {
                dsize = field->DefinedSize;
            } catch(const _com_error& e) {
                sql_dump_error(e, "select_query", "Field.DefinedSize");
                goto fail;
            }
            TRACE("[rlm_sql_msado] select_query: column %d, chars type %d, size %d",
                  i+1, dtype, dsize);
            if(dsize > 1024) {
                radlog(L_DEBUG+1, "[rlm_sql_msado] select_query: "
                       "column %d, chars type %d, size %d too long! truncate to 1024",
                       i+1, dtype, dsize);
                dsize = 1024;
            }
            rowdata[i] = (char*)rad_malloc(dsize+1);
            colsize[i] = dsize+1;
            break;
        case adBSTR:
        case adWChar:
        case adLongVarWChar:
        case adVarWChar:
            try {
                dsize = field->DefinedSize * 2;
            } catch(const _com_error& e) {
                sql_dump_error(e, "select_query", "Field.DefinedSize");
                goto fail;
            }
            TRACE("[rlm_sql_msado] select_query: column %d, chars type %d, size %d",
                  i+1, dtype, dsize);
            if(dsize > 1024) {
                radlog(L_DEBUG+1, "[rlm_sql_msado] select_query: "
                       "column %d, chars type %d, size %d too long! truncate to 1024",
                       i+1, dtype, dsize);
                dsize = 1024;
            }
            rowdata[i] = (char*)rad_malloc(dsize+1);
            colsize[i] = dsize+1;
            break;
        case adBigInt:
        case adBinary:
        case adBoolean:
        case adCurrency:
        case adDate:
        case adDBDate:
        case adDBTime:
        case adDBTimeStamp:
        case adDecimal:
        case adDouble:
        case adFileTime:
        case adInteger:
        case adLongVarBinary:
        case adNumeric:
        case adSingle:
        case adSmallInt:
        case adTinyInt:
        case adUnsignedBigInt:
        case adUnsignedInt:
        case adUnsignedSmallInt:
        case adUnsignedTinyInt:
        case adVarBinary:
        case adVarNumeric:
            TRACE("[rlm_sql_msado] select_query: column %d, other type %d, size %d",
                  i+1, dtype, dsize);
            rowdata[i] = (char*)rad_malloc(dsize+1);
            colsize[i] = dsize+1;
            break;
        default:
            radlog(L_WARN, "[rlm_sql_msado] select_query: "
                   "Unsupported date type (%d) at column %d",
                   dtype, i+1);
            rowdata[i] = (char*)rad_malloc(dsize+1);
            colsize[i] = dsize+1;
            break;
        }
    }

    rad_assert(msado_sock->num_fields_cache == 0);
    msado_sock->num_fields_cache = colcount;
    rad_assert(msado_sock->field_size_cache == NULL);
    msado_sock->field_size_cache = colsize;
    rad_assert(msado_sock->results == NULL);
    msado_sock->results = rowdata;

    return 0;

fail:

    rad_free(colsize);
    for(i=0; i<colcount+1; i++) rad_free(rowdata[i]);
    rad_free(rowdata);

    sql_finish_select_query(sqlsocket, config);

    return -1;
}

static int sql_fetch_row(SQLSOCK * sqlsocket, SQL_CONFIG *config)
{
    int     i;
    int		colcount;
    char	**rowdata = NULL;
    int     *colsize;

    rlm_sql_msado_sock *msado_sock = (rlm_sql_msado_sock*)sqlsocket->conn;
    rad_assert(msado_sock != NULL);
    rad_assert(msado_sock->rset != NULL);
    rad_assert((*msado_sock->rset) != NULL);

    sqlsocket->row = NULL;

    try {
        if((*msado_sock->rset)->State != adStateOpen)
            return 0;
    } catch(const _com_error& e) {
        sql_dump_error(e, "fetch_row", "Recordset.State");
        return -1;
    }

    colcount = msado_sock->num_fields_cache;
    colsize = msado_sock->field_size_cache;
    rowdata = msado_sock->results;
    if(rowdata) {
        for(i=0; i<colcount; i++)
            memset(rowdata[i], 0, colsize[i]);
    }

    try {
        msado_sock->end_of_records = (*msado_sock->rset)->EndOfFile == VARIANT_TRUE ? 1 : 0;
    } catch(const _com_error& e) {
        sql_dump_error(e, "fetch_row", "Recordset.EndOfFile");
        return -1;
    }
    if(msado_sock->end_of_records)
        return 0;

    for(i=0; i<colcount; i++) {

        FieldPtr field;

        try {
            field = (*msado_sock->rset)->Fields->GetItem((long)i);
        } catch(const _com_error& e) {
            sql_dump_error(e, "fetch_row", "Recordset.Fields.GetItem()");
            return -1;
        }

        try {
            _variant_t value = field->Value;
            if(value.vt != VT_NULL)
                strNcpy(rowdata[i], (const char*)((_bstr_t)value), colsize[i]);
        } catch(const _com_error& e) {
            sql_dump_error(e, "fetch_row", "Field.Value");
            return -1;
        }
    }

    try {
        (*msado_sock->rset)->MoveNext();
    } catch(const _com_error& e) {
        sql_dump_error(e, "fetch_row", "Recordset.MoveNext()");
        return -1;
    }

    sqlsocket->row = msado_sock->results;

	return 0;
}

static int sql_affected_rows(SQLSOCK * sqlsocket, SQL_CONFIG *config)
{
    rlm_sql_msado_sock *msado_sock = (rlm_sql_msado_sock*)sqlsocket->conn;
    rad_assert(msado_sock != NULL);

    return msado_sock->affected_rows_cache;
}

static int sql_num_rows(SQLSOCK * sqlsocket, SQL_CONFIG *config)
{
    rlm_sql_msado_sock *msado_sock = (rlm_sql_msado_sock*)sqlsocket->conn;
    rad_assert(msado_sock != NULL);

    radlog(L_ERROR|L_CONS, "[rlm_sql_msado] num_rows: No support, "
           "using 'select count(*)' instead of it is always recommended.");
    return 0;
}

static int sql_store_result(SQLSOCK *sqlsocket, SQL_CONFIG *config)
{
    return 0;
}

static int sql_free_result(SQLSOCK *sqlsocket, SQL_CONFIG *config)
{
    return 0;
}

static char *sql_error(SQLSOCK *sqlsocket, SQL_CONFIG *config)
{
    static char	msg = '\0';
	return &msg;
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
RAD_EXTERN_C RAD_SYMBOL
rlm_sql_module_t rlm_sql_msado = {
	"rlm_sql_msado15",
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
