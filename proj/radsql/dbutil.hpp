#ifndef RAD_DBUTIL_HPP
#define RAD_DBUTIL_HPP

#include <string>
#include <vector>
#include <deque>
#include <iostream>

class DBUtil
{
public:
    typedef std::vector<std::string> Row;
    typedef std::deque<Row> RowSet;
public:
    static DBUtil& Instance();
    DBUtil(const char* conffile, const char* dbsection = "sql",
           const char* logsection = 0);
    ~DBUtil();
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
    void report(std::ostream& os, const char* prefix);
private:
    void Free();
    DBUtil(const DBUtil&);
    DBUtil& operator=(const DBUtil&);
private:
    void* dbHandle_;
};

#endif//RAD_DBUTIL_HPP
