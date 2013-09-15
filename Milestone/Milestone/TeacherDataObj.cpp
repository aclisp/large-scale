#include "stdafx.h"
#include "TeacherDataObj.h"
#include "SqlLite.h"
#include "Util.h"


const wxString TeacherDataObj::SIGNOFF_STR[] = {L"正常", L"离职"};
const wxString TeacherDataObj::RANK_STR[] = {L"初级", L"高级", L"资深A", L"资深B", L"教授"};
const wxString TeacherDataObj::PARTTIME_STR[] = {L"全职", L"兼职"};


TeacherDataObj::TeacherDataObj()
{
	m_signoff = 0;
	m_rank = 0;
	m_partTime = 0;
}


int TeacherDataObj::loadByName(const wxString& name)
{
	SqlLiteStatement st("SELECT * FROM TBL_TEACHER WHERE TEACHER_NAME = :teacherName");
	SqlLiteBind(st, "teacherName", name);
	if (SQLITE_ROW == sqlite3_step(st))
	{
		int col = 0;
		m_name = SqlLiteColumnStr(st, col++); wxASSERT(m_name == name);
		m_onboardDate = SqlLiteColumnDate(st, col++);
		m_signoff = SqlLiteColumnInt(st, col++);
		m_signoffDate = SqlLiteColumnDate(st, col++);
		m_rank = SqlLiteColumnInt(st, col++);
		m_partTime = SqlLiteColumnInt(st, col++);
		m_phone = SqlLiteColumnStr(st, col++);
		m_memo = SqlLiteColumnStr(st, col++);
		return 0;
	}

	return DB_NOTFOUND;
}


int TeacherDataObj::insertThisObj()
{
	SqlLiteStatement st("INSERT INTO TBL_TEACHER VALUES (:teacherName, \
		                                                 :onboardDate,  \
														 :signoff,     \
														 :signoffDate, \
														 :rank,       \
														 :partTime,      \
														 :phone,     \
														 :memo         \
														 )");
	SqlLiteBind(st, "teacherName", m_name);
	SqlLiteBind(st, "onboardDate", m_onboardDate);
	SqlLiteBind(st, "signoff", m_signoff);
	SqlLiteBind(st, "signoffDate", m_signoffDate);
	SqlLiteBind(st, "rank", m_rank);
	SqlLiteBind(st, "partTime", m_partTime);
	SqlLiteBind(st, "phone", m_phone);
	SqlLiteBind(st, "memo", m_memo);
	if (SQLITE_DONE == sqlite3_step(st)) return 0;

	ErrorMessageBox(L"无法新增教师【"+m_name+L"】。", SqlLiteErrMsg());
	return DB_ERROR;
}


int TeacherDataObj::deleteByName(const wxString& name)
{
	SqlLiteStatement st("DELETE FROM TBL_TEACHER WHERE TEACHER_NAME = :teacherName");
	SqlLiteBind(st, "teacherName", name);
	if (SQLITE_DONE == sqlite3_step(st)) return 0;

	ErrorMessageBox(L"无法删除教师【"+name+L"】。", SqlLiteErrMsg());
	return DB_ERROR;
}


int TeacherDataObj::updateByName(const wxString& name)
{
	SqlLiteStatement st("UPDATE TBL_TEACHER SET TEACHER_NAME = :teacherName, \
												ONBOARD_DATE = :onboardDate, \
												SIGNOFF = :signoff, \
												SIGNOFF_DATE= :signoffDate, \
												RANK = :rank, \
												PART_TIME = :partTime, \
												PHONE = :phone, \
												MEMO = :memo \
								WHERE TEACHER_NAME = :teacherNameInput \
		");
	SqlLiteBind(st, "teacherName", m_name);
	SqlLiteBind(st, "onboardDate", m_onboardDate);
	SqlLiteBind(st, "signoff", m_signoff);
	SqlLiteBind(st, "signoffDate", m_signoffDate);
	SqlLiteBind(st, "rank", m_rank);
	SqlLiteBind(st, "partTime", m_partTime);
	SqlLiteBind(st, "phone", m_phone);
	SqlLiteBind(st, "memo", m_memo);
	SqlLiteBind(st, "teacherNameInput", name);
	if (SQLITE_DONE == sqlite3_step(st)) return 0;

	ErrorMessageBox(L"无法更新教师【"+name+L"】。", SqlLiteErrMsg());
	return DB_ERROR;
}
