#ifndef SqlLite_INCLUDED
#define SqlLite_INCLUDED


// The global sqlite instance, which could be accessed directly and used otherwise. 
extern sqlite3* SqlLiteHandle;


// The global wrapper functions to manipulate the instance. The wrapper functions 
// follow a general error handing strategy: 1) It returns 0 on success and other
// positive integers to indicate different failure code; 2) It handles (such as 
// gracefully release some kind of resources) and notifies the detailed failure 
// by itself.
int SqlLiteOpen();
void SqlLiteClose();


// From JdbcTemplate, to execute sql of SELECT
int SqlLiteQueryForInt(const wxString& sql);
wxString SqlLiteQueryForString(const wxString& sql);
wxDateTime SqlLiteQueryForDate(const wxString& sql);


// To execute sql of INSERT/UPDATE/DELETE, returns 0 on success.
int SqlLiteUpdate(const wxString& sql);
// To execute any arbitrary SQL, and as such the method is often used for DDL statements. 
// It is just a wrapper of sqlite3_exec, so it will return SQLITE_OK on success.
int SqlLiteExecute(const wxString& sql);


// Statement object for RAII
class SqlLiteStatement
{
public:
	SqlLiteStatement(const wxString& sql);
	~SqlLiteStatement();
	operator sqlite3_stmt* () const { return _st; }
private:
	SqlLiteStatement(const SqlLiteStatement& );
    SqlLiteStatement& operator=(const SqlLiteStatement& );
	sqlite3_stmt* _st;
};


// For Safe Transaction
class SqlLiteTransaction
{
public:
	SqlLiteTransaction(int trans);
	~SqlLiteTransaction();
	void commit();
private:
	SqlLiteTransaction(const SqlLiteTransaction& );
	SqlLiteTransaction& operator=(const SqlLiteTransaction& );
	int _trans;
	int _commit;
};


void SqlLiteBind(sqlite3_stmt* st, const wxString& id, int val);
void SqlLiteBind(sqlite3_stmt* st, const wxString& id, const wxString& val);
void SqlLiteBind(sqlite3_stmt* st, const wxString& id, const wxDateTime& val, int bindDateOrTime = 0);


// The leftmost column of the result set has the index 0. The number of columns 
// in the result can be determined using sqlite3_column_count().
int SqlLiteColumnInt(sqlite3_stmt* st, int col);
wxString SqlLiteColumnStr(sqlite3_stmt* st, int col);
wxDateTime SqlLiteColumnDate(sqlite3_stmt* st, int col);
wxDateTime SqlLiteColumnDateTime(sqlite3_stmt* st, int col);
wxDateTime SqlLiteColumnTime(sqlite3_stmt* st, int col);


wxString SqlLiteErrMsg();


#endif // SqlLite_INCLUDED
