#include "stdafx.h"
#include "StudentDataObj.h"
#include "SqlLite.h"
#include "Util.h"


const wxString StudentDataObj::SIGNOFF_STR[] = {L"正常", L"休学"};


StudentDataObj::StudentDataObj()
{
	m_signoff = 0;
	m_age = 0;
}


int StudentDataObj::loadByName(const wxString& name)
{
	SqlLiteStatement st("SELECT * FROM TBL_STUDENT WHERE STUDENT_NAME = :studentName");
	SqlLiteBind(st, "studentName", name);
	if (SQLITE_ROW == sqlite3_step(st))
	{
		int col = 0;
		m_name = SqlLiteColumnStr(st, col++); wxASSERT(m_name == name);
		m_signonDate = SqlLiteColumnDate(st, col++);
		m_signoff = SqlLiteColumnInt(st, col++);
		m_signoffDate = SqlLiteColumnDate(st, col++);
		m_phone = SqlLiteColumnStr(st, col++);
		m_phone2 = SqlLiteColumnStr(st, col++);
		m_address = SqlLiteColumnStr(st, col++);
		m_birthDate = SqlLiteColumnDate(st, col++);
		m_age = SqlLiteColumnInt(st, col++);
		m_specialty = SqlLiteColumnStr(st, col++);
		m_memo = SqlLiteColumnStr(st, col++);
		return 0;
	}

	return DB_NOTFOUND;
}


int StudentDataObj::insertThisObj()
{
	SqlLiteStatement st("INSERT INTO TBL_STUDENT VALUES (:studentName, \
		                                                 :signonDate,  \
														 :signoff,     \
														 :signoffDate, \
														 :phone,       \
														 :phone2,      \
														 :address,     \
														 :birthDate,   \
														 :age,         \
														 :specialty,   \
														 :memo         \
														 )");
	SqlLiteBind(st, "studentName", m_name);
	SqlLiteBind(st, "signonDate", m_signonDate);
	SqlLiteBind(st, "signoff", m_signoff);
	SqlLiteBind(st, "signoffDate", m_signoffDate);
	SqlLiteBind(st, "phone", m_phone);
	SqlLiteBind(st, "phone2", m_phone2);
	SqlLiteBind(st, "address", m_address);
	SqlLiteBind(st, "birthDate", m_birthDate);
	SqlLiteBind(st, "age", m_age);
	SqlLiteBind(st, "specialty", m_specialty);
	SqlLiteBind(st, "memo", m_memo);
	if (SQLITE_DONE == sqlite3_step(st)) return 0;

	ErrorMessageBox(L"无法新增学员【"+m_name+L"】。", SqlLiteErrMsg());
	return DB_ERROR;
}


int StudentDataObj::deleteByName(const wxString& name)
{
	SqlLiteStatement st("DELETE FROM TBL_STUDENT WHERE STUDENT_NAME = :studentName");
	SqlLiteBind(st, "studentName", name);
	if (SQLITE_DONE == sqlite3_step(st)) return 0;

	ErrorMessageBox(L"无法删除学员【"+name+L"】。", SqlLiteErrMsg());
	return DB_ERROR;
}


int StudentDataObj::updateByName(const wxString& name)
{
	SqlLiteStatement st("UPDATE TBL_STUDENT SET STUDENT_NAME = :studentName, \
												SIGNON_DATE = :signonDate, \
												SIGNOFF = :signoff, \
												SIGNOFF_DATE= :signoffDate, \
												PHONE = :phone, \
												PHONE2 = :phone2, \
												ADDRESS = :address, \
												BIRTH_DATE = :birthDate, \
												AGE = :age, \
												SPECIALTY = :specialty, \
												MEMO = :memo \
								WHERE STUDENT_NAME = :studentNameInput \
		");
	SqlLiteBind(st, "studentName", m_name);
	SqlLiteBind(st, "signonDate", m_signonDate);
	SqlLiteBind(st, "signoff", m_signoff);
	SqlLiteBind(st, "signoffDate", m_signoffDate);
	SqlLiteBind(st, "phone", m_phone);
	SqlLiteBind(st, "phone2", m_phone2);
	SqlLiteBind(st, "address", m_address);
	SqlLiteBind(st, "birthDate", m_birthDate);
	SqlLiteBind(st, "age", m_age);
	SqlLiteBind(st, "specialty", m_specialty);
	SqlLiteBind(st, "memo", m_memo);
	SqlLiteBind(st, "studentNameInput", name);
	if (SQLITE_DONE == sqlite3_step(st)) return 0;

	ErrorMessageBox(L"无法更新学员【"+name+L"】。", SqlLiteErrMsg());
	return DB_ERROR;
}
