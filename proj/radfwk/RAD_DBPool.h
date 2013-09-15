// -*- mode: c++ -*-

/* Author:   Huanghao@cellcom
 * Date:     2005/11/30 15:03:57
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RADFWK_DBPOOL_H__
#define __RADFWK_DBPOOL_H__

#include "RAD_Object.h"

class RADFWK_API RAD_DBPool : public RAD_Object
{
public:
    virtual ~RAD_DBPool();

    typedef vector<string> Row;
    typedef list<Row> RowSet;


    RAD_DBPool(const char* conffile, const char* dbsection = "sql",
               const char* logsection = 0);
    int Init(const char* conffile, const char* dbsection = "sql",
             const char* logsection = 0);
    bool Valid() const;
    // @return ==0:no row; 1:ok; <0:error
    int SelectFirstRow(Row& row, const char* sql, ...);
    int SelectLastRow (Row& row, const char* sql, ...);
    // convenience
    int QueryForInt(const char* sql, ...);
    int QueryForString(char* buf, size_t buflen, const char* sql, ...);
    // @return >=0:result row count; <0:error
    int Query(RowSet& rset, const char* sql, ...);
    int Execute(const char* sql, ...);

private:
    RAD_DBPool(const RAD_DBPool& );
    RAD_DBPool& operator=(const RAD_DBPool& other);

    void Free();
    void* dbHandle_;
};

#endif // __RADFWK_DBPOOL_H__
