#include "stdafx.h"
#include "SqlLite.h"
#include "Util.h"
#include "pinyin.h"


sqlite3* SqlLiteHandle = NULL;


int SqlLiteOpen()
{
	if (SqlLiteHandle != NULL)
		return 0;

	wxString dataDir = DATA_DIR;
	if (!wxConfig::Get()->Read("DataDir", &dataDir))
		wxConfig::Get()->Write("DataDir", dataDir);

	if (!wxDir::Exists(dataDir))
		if (!wxDir::Make(dataDir))
		{
			ErrorMessageBox(L"没有数据目录。", "False returned by wxDir::Make "+dataDir);
			return FS_ERROR;
		}

	bool openReadOnly = true;
	if (!wxConfig::Get()->Read("ReadOnly", &openReadOnly))
		wxConfig::Get()->Write("ReadOnly", openReadOnly);

	if (SQLITE_OK != sqlite3_open_v2((dataDir+"Milestone.bin").utf8_str(), &SqlLiteHandle, 
	                                openReadOnly?SQLITE_OPEN_READONLY:SQLITE_OPEN_READWRITE, NULL))
	{
		if (SqlLiteHandle == NULL)
			ErrorMessageBox(L"分配内存失败。", "SqlLiteHandle is NULL after sqlite3_open_v2");
		else
			ErrorMessageBox(L"打开数据文件失败。", SqlLiteErrMsg());
		sqlite3_close(SqlLiteHandle);
		return DB_ERROR;
	}

	int sqliteReturn;
	int configResult;
	sqliteReturn = sqlite3_db_config(SqlLiteHandle, SQLITE_DBCONFIG_ENABLE_FKEY, 1, &configResult);
	wxASSERT(sqliteReturn == SQLITE_OK);
	wxASSERT(configResult == 1);
	sqliteReturn = sqlite3_db_config(SqlLiteHandle, SQLITE_DBCONFIG_ENABLE_TRIGGER, 1, &configResult);
	wxASSERT(sqliteReturn == SQLITE_OK);
	wxASSERT(configResult == 1);
	sqlite3_extended_result_codes(SqlLiteHandle, 1);
	sqlite3_create_collation(SqlLiteHandle, "pinyin", SQLITE_UTF8, NULL, pinyin_cmp_utf8);

	return 0;
}


void SqlLiteClose()
{
	sqlite3_close(SqlLiteHandle);
	SqlLiteHandle = NULL;
}


SqlLiteStatement::SqlLiteStatement(const wxString& sql)
{
	if (SQLITE_OK != sqlite3_prepare_v2(SqlLiteHandle, sql.utf8_str(), -1, &_st, NULL))
		throw std::runtime_error("SqlLiteStatement prepare failure: "+sql);
}


SqlLiteStatement::~SqlLiteStatement()
{
	sqlite3_finalize(_st);
}


void SqlLiteBind(sqlite3_stmt* st, const wxString& id, int val)
{
	int index = sqlite3_bind_parameter_index(st, (":"+id).utf8_str());
	if (index == 0) throw std::runtime_error("SqlLiteBind no matching parameter is found: "+id);

	if (SQLITE_OK != sqlite3_bind_int(st, index, val)) 
		throw std::runtime_error("sqlite3_bind_int failure: "+SqlLiteErrMsg());
}


void SqlLiteBind(sqlite3_stmt* st, const wxString& id, const wxString& val)
{
	int index = sqlite3_bind_parameter_index(st, (":"+id).utf8_str());
	if (index == 0) throw std::runtime_error("SqlLiteBind no matching parameter is found: "+id);

	if (SQLITE_OK != sqlite3_bind_text(st, index, val.utf8_str(), -1, SQLITE_TRANSIENT)) 
		throw std::runtime_error("sqlite3_bind_text failure: "+SqlLiteErrMsg());
}


void SqlLiteBind(sqlite3_stmt* st, const wxString& id, const wxDateTime& val, int bindDateOrTime)
{
	wxString bindStr = wxEmptyString;
	if (val.IsValid())
	{
		if (bindDateOrTime == BIND_DATE) bindStr = val.FormatISODate();
		else if (bindDateOrTime == BIND_TIME) bindStr = val.FormatISOTime();
		else bindStr = val.FormatISOCombined(' ');
	}
	SqlLiteBind(st, id, bindStr);
}


int SqlLiteColumnInt(sqlite3_stmt* st, int col)
{
	return sqlite3_column_int(st, col);
}


wxString SqlLiteColumnStr(sqlite3_stmt* st, int col)
{
	return wxString::FromUTF8(reinterpret_cast<const char*>(sqlite3_column_text(st, col)));
}


wxDateTime SqlLiteColumnDate(sqlite3_stmt* st, int col)
{
	wxDateTime dateTime;
	dateTime.ParseISODate(SqlLiteColumnStr(st, col));
	return dateTime;
}


wxDateTime SqlLiteColumnDateTime(sqlite3_stmt* st, int col)
{
	wxDateTime dateTime;
	dateTime.ParseISOCombined(SqlLiteColumnStr(st, col), ' ');
	return dateTime;
}


wxDateTime SqlLiteColumnTime(sqlite3_stmt* st, int col)
{
	wxDateTime dateTime;
	dateTime.ParseISOTime(SqlLiteColumnStr(st, col));
	return dateTime;
}


int SqlLiteQueryForInt(const wxString& sql)
{
	SqlLiteStatement st(sql);
	if (SQLITE_ROW == sqlite3_step(st))	return SqlLiteColumnInt(st, 0); 
	else return 0;
}


wxString SqlLiteQueryForString(const wxString& sql)
{
	SqlLiteStatement st(sql);
	if (SQLITE_ROW == sqlite3_step(st)) return SqlLiteColumnStr(st, 0);
	else return wxEmptyString;
}


wxDateTime SqlLiteQueryForDate(const wxString& sql)
{
	wxDateTime dateTime;
	SqlLiteStatement st(sql);
	if (SQLITE_ROW == sqlite3_step(st)) dateTime.ParseISODate(SqlLiteColumnStr(st, 0));
	return dateTime;
}


int SqlLiteUpdate(const wxString& sql)
{
	SqlLiteStatement st(sql);
	if (SQLITE_DONE != sqlite3_step(st))
	{
		ErrorMessageBox(L"修改数据失败。", SqlLiteErrMsg());
		return DB_ERROR;
	}

	return 0;
}


int SqlLiteExecute(const wxString& sql)
{
	return sqlite3_exec(SqlLiteHandle, sql.utf8_str(), NULL, NULL, NULL);
}


wxString SqlLiteErrMsg()
{
	return wxString::FromUTF8(sqlite3_errmsg(SqlLiteHandle));
}


SqlLiteTransaction::SqlLiteTransaction(int trans) : _trans(trans), _commit(0)
{
	if (_trans == TRANSACTION_SCOPE) return;

	// BEGIN not fail, but we may recover from last failed COMMIT/ROLLBACK, where autocommit == 0.
	if (sqlite3_get_autocommit(SqlLiteHandle)) SqlLiteExecute("BEGIN");
	wxASSERT(sqlite3_get_autocommit(SqlLiteHandle) == 0);
}


void SqlLiteTransaction::commit()
{
	if (_trans == TRANSACTION_SCOPE) return;

	wxASSERT(sqlite3_get_autocommit(SqlLiteHandle) == 0);
	_commit = 1;
}


SqlLiteTransaction::~SqlLiteTransaction()
{
	if (_trans == TRANSACTION_SCOPE) return;

	wxASSERT(sqlite3_get_autocommit(SqlLiteHandle) == 0);
	if (_commit) SqlLiteExecute("COMMIT");
	else SqlLiteExecute("ROLLBACK");
	wxASSERT(sqlite3_get_autocommit(SqlLiteHandle) == 1);
}
