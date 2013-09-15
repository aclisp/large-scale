#include "RAD_DBPool.h"

int RAD_DBPool::Init(const char* conffile, const char* dbsection, const char* logsection)
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
        radlog(L_CONS|L_ERROR, "[RAD_DBPool::RAD_DBPool] can not read '%s'", conffile);
        rlm_sql_destroy();
        return -1;
    }

    // get db section, support name1 and name2
    char* dbsec_name2;
    if((dbsec_name2 = strchr(dbsection, ' '))) {
        dbsec_name2[0] = '\0';
        ++dbsec_name2;
    }
    CONF_SECTION* sqlconf = cf_section_sub_find(conf, dbsection);
    if(!sqlconf) {
        radlog(L_CONS|L_ERROR, "[RAD_DBPool::RAD_DBPool] can not find sub section '%s'", dbsection);
        cf_section_free(&conf);
        rlm_sql_destroy();
        return -1;
    }
    if(dbsec_name2) {
        do {
            char* name2 = cf_section_name2(sqlconf);
            if(name2 && strcmp(name2, dbsec_name2) == 0)
                break;
        } while((sqlconf = cf_subsection_find_next(conf, sqlconf, dbsection)));
        if(!sqlconf) {
            radlog(L_CONS|L_ERROR, "[RAD_DBPool::RAD_DBPool] can not find sub section '%s %s'", dbsection, dbsec_name2);
            cf_section_free(&conf);
            rlm_sql_destroy();
            return -1;
        }
    }

    // get log section if present
    CONF_SECTION* logconf = NULL;
    if(logsection) {
        logconf = cf_section_sub_find(conf, logsection);
        if(!logconf) {
            radlog(L_CONS|L_WARN, "[RAD_DBPool::RAD_DBPool] can not find sub section '%s', "
                   "no logging parameters would be applied to rlm_sql. This may be "
                   "a problem when using rlm_sql with dynamic loading sql_drivers",
                   logsection);
        }
    }

    // get sql instance
    if(rlm_sql_instantiate(sqlconf, &sqlinst, logconf) != 0) {
        radlog(L_CONS|L_ERROR, "[RAD_DBPool::RAD_DBPool] can not instantiate sql instance");
        cf_section_free(&conf);
        rlm_sql_destroy();
        return -1;
    }

    // free conf section
    cf_section_free(&conf);

    dbHandle_ = sqlinst;

    return 0;
}

void RAD_DBPool::Free()
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

bool RAD_DBPool::Valid() const
{
    return (dbHandle_ != 0);
}

RAD_DBPool::RAD_DBPool(const char* conffile, const char* dbsection, const char* logsection)
    : RAD_Object("RAD_DBPool"), dbHandle_(0)
{
    Init(conffile, dbsection, logsection);
}

RAD_DBPool::~RAD_DBPool()
{
    Free();
}

static
int SelectFirstRowInternal(SQL_INST* sqlinst, const char* sqlbuf, RAD_DBPool::Row& row)
{
    SQLSOCK* sqlsock = sql_get_socket(sqlinst);
    if(sqlsock == NULL) {
        return -1;
    }

    if(rlm_sql_select_query(sqlsock, sqlinst, sqlbuf) != 0) {
        radlog(L_WARN, "[SelectFirstRow] FAIL [SQL] %s", sqlbuf);
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
int SelectLastRowInternal(SQL_INST* sqlinst, const char* sqlbuf, RAD_DBPool::Row& row)
{
    SQLSOCK* sqlsock = sql_get_socket(sqlinst);
    if(sqlsock == NULL) {
        return -1;
    }

    if(rlm_sql_select_query(sqlsock, sqlinst, sqlbuf) != 0) {
        radlog(L_WARN, "[SelectLastRow] FAIL [SQL] %s", sqlbuf);
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

int RAD_DBPool::SelectFirstRow(Row& row, const char* sql, ...)
{
    SQL_INST* sqlinst = (SQL_INST*)dbHandle_;
    if(sqlinst == 0) {
        radlog(L_CONS|L_ERROR, "[RAD_DBPool::SelectFirstRow] invalid instance");
        return -1;
    }

    char sqlbuf[4096];
    va_list args;
    va_start(args, sql);
    vsNprintf(sqlbuf, sizeof(sqlbuf), sql, args);
    va_end(args);

    return SelectFirstRowInternal(sqlinst, sqlbuf, row);
}

int RAD_DBPool::SelectLastRow (Row& row, const char* sql, ...)
{
    SQL_INST* sqlinst = (SQL_INST*)dbHandle_;
    if(sqlinst == 0) {
        radlog(L_CONS|L_ERROR, "[RAD_DBPool::SelectLastRow] invalid instance");
        return -1;
    }

    char sqlbuf[4096];
    va_list args;
    va_start(args, sql);
    vsNprintf(sqlbuf, sizeof(sqlbuf), sql, args);
    va_end(args);

    return SelectLastRowInternal(sqlinst, sqlbuf, row);
}

int RAD_DBPool::QueryForInt(const char* sql, ...)
{
    SQL_INST* sqlinst = (SQL_INST*)dbHandle_;
    if(sqlinst == 0) {
        radlog(L_CONS|L_ERROR, "[RAD_DBPool::QueryForInt] invalid instance");
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

int RAD_DBPool::QueryForString(char* buf, size_t buflen, const char* sql, ...)
{
    SQL_INST* sqlinst = (SQL_INST*)dbHandle_;
    if(sqlinst == 0) {
        radlog(L_CONS|L_ERROR, "[RAD_DBPool::QueryForString] invalid instance");
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

int RAD_DBPool::Query(RowSet& rset, const char* sql, ...)
{
    SQL_INST* sqlinst = (SQL_INST*)dbHandle_;
    if(sqlinst == 0) {
        radlog(L_CONS|L_ERROR, "[RAD_DBPool::Query] invalid instance");
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
        radlog(L_WARN, "[Query] FAIL [SQL] %s", sqlbuf);
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

int RAD_DBPool::Execute(const char* sql, ...)
{
    SQL_INST* sqlinst = (SQL_INST*)dbHandle_;
    if(sqlinst == 0) {
        radlog(L_CONS|L_ERROR, "[RAD_DBPool::Execute] invalid instance");
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
        radlog(L_WARN, "[Execute] FAIL [SQL] %s", sqlbuf);
        sql_release_socket(sqlinst, sqlsock);
        return -1;
    }

    int result  = rlm_sql_affected_rows(sqlsock, sqlinst);

    rlm_sql_finish_query(sqlsock, sqlinst);
    sql_release_socket(sqlinst, sqlsock);
    return result;
}
