#include "radutil.h"
#include "radsql.h"
#include "dbutil.hpp"

int DBUtil::Init(const char* conffile, const char* dbsection, const char* logsection)
{
    SQL_INST* sqlinst = NULL;

    // free previous instance first
    Free();

    // init sql module
    rlm_sql_init();

    // read conf file
    CONF_SECTION *conf;
    conf = conf_read(__FILE__, __LINE__, conffile, NULL);
    if(!conf) {
        radlog(L_CONS|L_ERROR, "[DBUtil::DBUtil] can not read '%s'", conffile);
        rlm_sql_destroy();
        return -1;
    }

    // get db section
    CONF_SECTION* sqlconf = cf_section_sub_find(conf, dbsection);
    if(!sqlconf) {
        radlog(L_CONS|L_ERROR, "[DBUtil::DBUtil] can not find sub section '%s'", dbsection);
        cf_section_free(&conf);
        rlm_sql_destroy();
        return -1;
    }

    // get log section if present
    CONF_SECTION* logconf = NULL;
    if(logsection) {
        logconf = cf_section_sub_find(conf, logsection);
        if(!logconf) {
            radlog(L_CONS|L_WARN, "[DBUtil::DBUtil] can not find sub section '%s', "
                   "no logging parameters would be applied to rlm_sql. This may be "
                   "a problem when using rlm_sql with dynamic loading sql_drivers",
                   logsection);
        }
    }

    // get sql instance
    if(rlm_sql_instantiate(sqlconf, &sqlinst, logconf) != 0) {
        radlog(L_CONS|L_ERROR, "[DBUtil::DBUtil] can not instantiate sql instance");
        cf_section_free(&conf);
        rlm_sql_destroy();
        return -1;
    }

    // free conf section
    cf_section_free(&conf);

    dbHandle_ = sqlinst;

    return 0;
}

void DBUtil::Free()
{
    SQL_INST* sqlinst = (SQL_INST*)dbHandle_;
    if(sqlinst) {
        // detach sql instance
        rlm_sql_detach(sqlinst);

        // destroy sql module
        rlm_sql_destroy();

        // clear inst
        dbHandle_ = 0;
    }
}

bool DBUtil::Valid() const
{
    return (dbHandle_ != 0);
}

DBUtil::DBUtil(const char* conffile, const char* dbsection, const char* logsection)
    : dbHandle_(0)
{
    Init(conffile, dbsection, logsection);
}

DBUtil::~DBUtil()
{
    Free();
}

static
int SelectFirstRowInternal(SQL_INST* sqlinst, const char* sqlbuf, DBUtil::Row& row)
{
    SQLSOCK* sqlsock = sql_get_socket(sqlinst);
    if(sqlsock == NULL) {
        return -1;
    }

    if(rlm_sql_select_query(sqlsock, sqlinst, sqlbuf) != 0) {
        sql_release_socket(sqlinst, sqlsock);
        return -1;
    }

    if(rlm_sql_fetch_row(sqlsock, sqlinst) != 0) {
        rlm_sql_finish_select_query(sqlsock, sqlinst);
        sql_release_socket(sqlinst, sqlsock);
        return -1;
    }

    if(sqlsock->row == NULL) {
        rlm_sql_finish_select_query(sqlsock, sqlinst);
        sql_release_socket(sqlinst, sqlsock);
        return 0;
    }

    int colcnt = rlm_sql_num_fields(sqlsock, sqlinst);
    row.clear();
    for(int i=0; i<colcnt; ++i)
        row.push_back(sqlsock->row[i]);
    rlm_sql_finish_select_query(sqlsock, sqlinst);
    sql_release_socket(sqlinst, sqlsock);
    return 1;
}

static
int SelectLastRowInternal(SQL_INST* sqlinst, const char* sqlbuf, DBUtil::Row& row)
{
    SQLSOCK* sqlsock = sql_get_socket(sqlinst);
    if(sqlsock == NULL) {
        return -1;
    }

    if(rlm_sql_select_query(sqlsock, sqlinst, sqlbuf) != 0) {
        sql_release_socket(sqlinst, sqlsock);
        return -1;
    }

    int colcnt = rlm_sql_num_fields(sqlsock, sqlinst);
    int rownum = 0;
    row.clear();
    for(;;) {
        if(rlm_sql_fetch_row(sqlsock, sqlinst) != 0) {
            rownum = -1;
            break;
        }

        if(sqlsock->row == NULL) {
            break;
        }

        ++rownum;
        row.clear();
        for(int i=0; i<colcnt; ++i)
            row.push_back(sqlsock->row[i]);
    }

    rlm_sql_finish_select_query(sqlsock, sqlinst);
    sql_release_socket(sqlinst, sqlsock);
    return rownum;
}

int DBUtil::SelectFirstRow(Row& row, const char* sql, ...)
{
    SQL_INST* sqlinst = (SQL_INST*)dbHandle_;
    if(sqlinst == 0) {
        radlog(L_CONS|L_ERROR, "[DBUtil::SelectFirstRow] invalid instance");
        return -1;
    }

    char sqlbuf[4096];
    va_list args;
    va_start(args, sql);
    vsNprintf(sqlbuf, sizeof(sqlbuf), sql, args);
    va_end(args);

    return SelectFirstRowInternal(sqlinst, sqlbuf, row);
}

int DBUtil::SelectLastRow (Row& row, const char* sql, ...)
{
    SQL_INST* sqlinst = (SQL_INST*)dbHandle_;
    if(sqlinst == 0) {
        radlog(L_CONS|L_ERROR, "[DBUtil::SelectLastRow] invalid instance");
        return -1;
    }

    char sqlbuf[4096];
    va_list args;
    va_start(args, sql);
    vsNprintf(sqlbuf, sizeof(sqlbuf), sql, args);
    va_end(args);

    return SelectLastRowInternal(sqlinst, sqlbuf, row);
}

int DBUtil::QueryForInt(const char* sql, ...)
{
    SQL_INST* sqlinst = (SQL_INST*)dbHandle_;
    if(sqlinst == 0) {
        radlog(L_CONS|L_ERROR, "[DBUtil::QueryForInt] invalid instance");
        return -1;
    }

    char sqlbuf[4096];
    va_list args;
    va_start(args, sql);
    vsNprintf(sqlbuf, sizeof(sqlbuf), sql, args);
    va_end(args);

    Row row;
    int result = SelectFirstRowInternal(sqlinst, sqlbuf, row);
    if(result <= 0)
        return 0;
    else
        return atoi(row[0].c_str());
}

int DBUtil::QueryForString(char* buf, size_t buflen, const char* sql, ...)
{
    SQL_INST* sqlinst = (SQL_INST*)dbHandle_;
    if(sqlinst == 0) {
        radlog(L_CONS|L_ERROR, "[DBUtil::QueryForString] invalid instance");
        return -1;
    }

    char sqlbuf[4096];
    va_list args;
    va_start(args, sql);
    vsNprintf(sqlbuf, sizeof(sqlbuf), sql, args);
    va_end(args);

    Row row;
    int result = SelectFirstRowInternal(sqlinst, sqlbuf, row);
    if(result < 0)
        strNcpy(buf, "<DBERROR>", buflen);
    else if(result == 0)
        strNcpy(buf, "<NORESULT>", buflen);
    else
        strNcpy(buf, row[0].c_str(), buflen);

    return result;
}

int DBUtil::Query(RowSet& rset, const char* sql, ...)
{
    SQL_INST* sqlinst = (SQL_INST*)dbHandle_;
    if(sqlinst == 0) {
        radlog(L_CONS|L_ERROR, "[DBUtil::Query] invalid instance");
        return -1;
    }

    char sqlbuf[4096];
    va_list args;
    va_start(args, sql);
    vsNprintf(sqlbuf, sizeof(sqlbuf), sql, args);
    va_end(args);

    SQLSOCK* sqlsock = sql_get_socket(sqlinst);
    if(sqlsock == NULL) {
        return -1;
    }

    if(rlm_sql_select_query(sqlsock, sqlinst, sqlbuf) != 0) {
        sql_release_socket(sqlinst, sqlsock);
        return -1;
    }

    int colcnt = rlm_sql_num_fields(sqlsock, sqlinst);
    int rownum = 0;
    rset.clear();
    for(;;) {
        if(rlm_sql_fetch_row(sqlsock, sqlinst) != 0) {
            rownum = -1;
            break;
        }

        if(sqlsock->row == NULL) {
            break;
        }

        ++rownum;
        Row row;
        for(int i=0; i<colcnt; ++i)
            row.push_back(sqlsock->row[i]);
        rset.push_back(row);
    }

    rlm_sql_finish_select_query(sqlsock, sqlinst);
    sql_release_socket(sqlinst, sqlsock);
    return rownum;
}

int DBUtil::Execute(const char* sql, ...)
{
    SQL_INST* sqlinst = (SQL_INST*)dbHandle_;
    if(sqlinst == 0) {
        radlog(L_CONS|L_ERROR, "[DBUtil::Execute] invalid instance");
        return -1;
    }

    char sqlbuf[4096];
    va_list args;
    va_start(args, sql);
    vsNprintf(sqlbuf, sizeof(sqlbuf), sql, args);
    va_end(args);

    SQLSOCK* sqlsock = sql_get_socket(sqlinst);
    if(sqlsock == NULL) {
        return -1;
    }

    if(rlm_sql_query(sqlsock, sqlinst, sqlbuf) != 0) {
        sql_release_socket(sqlinst, sqlsock);
        return -1;
    }

    int result  = rlm_sql_affected_rows(sqlsock, sqlinst);

    rlm_sql_finish_query(sqlsock, sqlinst);
    sql_release_socket(sqlinst, sqlsock);
    return result;
}

void DBUtil::report(std::ostream& os, const char* prefix)
{
    SQL_INST* sqlinst = (SQL_INST*)dbHandle_;
    if(sqlinst == 0) {
        radlog(L_CONS|L_ERROR, "[DBUtil::report] invalid instance");
        return;
    }

    SQLSOCK* sqlpool = sqlinst->sqlpool;
    SQLSOCK* last_used = sqlinst->last_used;

    for(SQLSOCK* sqlsock = sqlpool; sqlsock; sqlsock = sqlsock->next) {
        os << prefix << ".SqlSocket.["<<sqlsock->id<<"].InUse=" << sqlsock->inuse << "\n";
        os << prefix << ".SqlSocket.["<<sqlsock->id<<"].State=" << sqlsock->state << "\n";
    }

    if(last_used)
        os << prefix << ".SqlSocket.LastUsed=" << last_used->id << "\n";
}
