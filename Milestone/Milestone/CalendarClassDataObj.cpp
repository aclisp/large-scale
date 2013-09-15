#include "stdafx.h"
#include "CalendarClassDataObj.h"
#include "SqlLite.h"
#include "RegularClassDataObj.h"


CalendarClassDataObj::Student::Student()
{
	m_leave = 0;
	m_classFee = 0;
}


CalendarClassDataObj::CalendarClassDataObj()
{
	m_classNumber = 0;
	m_practice = 0;
	m_oneShot = 0;
	_saveFlag = 0;
}


int CalendarClassDataObj::loadByPrimaryKey(const wxDateTime& classDate, int classNumber, const wxString& teacherName)
{
	SqlLiteStatement st("SELECT * FROM TBL_CALENDAR_CLASS WHERE \
						CLASS_DATE = :classDate AND \
						CLASS_NUMBER = :classNumber AND \
						TEACHER_NAME = :teacherName \
						");
	SqlLiteBind(st, "classDate", classDate);
	SqlLiteBind(st, "classNumber", classNumber);
	SqlLiteBind(st, "teacherName", teacherName);
	if (SQLITE_ROW == sqlite3_step(st))
	{
		int col = 0;
		m_classDate = SqlLiteColumnDate(st, col++); wxASSERT(m_classDate == classDate);
		m_classNumber = SqlLiteColumnInt(st, col++); wxASSERT(m_classNumber == classNumber);
		m_teacherName = SqlLiteColumnStr(st, col++); wxASSERT(m_teacherName == teacherName);
		m_className = SqlLiteColumnStr(st, col++);
		m_practice = SqlLiteColumnInt(st, col++);
		m_oneShot = SqlLiteColumnInt(st, col++);
		m_memo = SqlLiteColumnStr(st, col++);

		m_studentVector.clear();

		SqlLiteStatement st("SELECT * FROM TBL_CALENDAR_CLASS_STUDENT WHERE \
							CLASS_DATE = :classDate AND \
							CLASS_NUMBER = :classNumber AND \
							TEACHER_NAME = :teacherName \
							");
		SqlLiteBind(st, "classDate", classDate);
		SqlLiteBind(st, "classNumber", classNumber);
		SqlLiteBind(st, "teacherName", teacherName);
		for (int row = 0; SQLITE_ROW == sqlite3_step(st); row++)
		{
			Student stud;
			col = 3;
			stud.m_studentName = SqlLiteColumnStr(st, col++);
			stud.m_leave = SqlLiteColumnInt(st, col++);
			stud.m_classFee = SqlLiteColumnInt(st, col++);
			stud.m_memo = SqlLiteColumnStr(st, col++);

			m_studentVector.push_back(stud);
		}

		wxASSERT(!m_studentVector.empty());
		return 0;
	}

	return DB_NOTFOUND;
}


int CalendarClassDataObj::insertThisObj(int trans)
{
	wxASSERT(m_classDate.IsValid());
	if (m_studentVector.empty())
	{
		ErrorMessageBox(L"无法新增课时。", L"学生列表中没有确认上课的学生。");
		return DB_ERROR;
	}
	SqlLiteTransaction tr(trans);
	SqlLiteStatement st("INSERT INTO TBL_CALENDAR_CLASS VALUES ( \
						:classDate, \
						:classNumber, \
						:teacherName, \
						:className, \
						:practice, \
						:oneShot, \
						:memo)");
	SqlLiteBind(st, "classDate", m_classDate);
	SqlLiteBind(st, "classNumber", m_classNumber);
	SqlLiteBind(st, "teacherName", m_teacherName);
	SqlLiteBind(st, "className", m_className);
	SqlLiteBind(st, "practice", m_practice);
	SqlLiteBind(st, "oneShot", m_oneShot);
	SqlLiteBind(st, "memo", m_memo);
	if (SQLITE_DONE != sqlite3_step(st))
	{
		ErrorMessageBox(L"无法新增课时。", SqlLiteErrMsg());
		return DB_ERROR;
	}

	unsigned int insertError = 0;
	wxVector<CalendarClassDataObj::Student>::iterator iter;
	for (iter = m_studentVector.begin(); iter != m_studentVector.end(); ++iter)
	{
		const CalendarClassDataObj::Student& stud = *iter;
		SqlLiteStatement st("INSERT INTO TBL_CALENDAR_CLASS_STUDENT VALUES ( \
							:classDate, \
							:classNumber, \
							:teacherName, \
							:studentName, \
							:leave, \
							:classFee, \
							:memo)");
		SqlLiteBind(st, "classDate", m_classDate);
		SqlLiteBind(st, "classNumber", m_classNumber);
		SqlLiteBind(st, "teacherName", m_teacherName);
		SqlLiteBind(st, "studentName", stud.m_studentName);
		SqlLiteBind(st, "leave", stud.m_leave);
		SqlLiteBind(st, "classFee", stud.m_classFee);
		SqlLiteBind(st, "memo", stud.m_memo);
		if (SQLITE_DONE != sqlite3_step(st))
		{
			ErrorMessageBox(L"无法新增课时【"+stud.m_studentName+"】。", SqlLiteErrMsg());
			insertError++;
		}
	}
	if (insertError == m_studentVector.size()) return DB_ERROR;
		
	tr.commit();
	return 0;
}


int CalendarClassDataObj::deleteByPrimaryKey(const wxDateTime& classDate, int classNumber, const wxString& teacherName, int trans)
{
	wxASSERT(classDate.IsValid());
	SqlLiteTransaction tr(trans);
	
	{
		SqlLiteStatement st("DELETE FROM TBL_CALENDAR_CLASS_STUDENT WHERE \
							CLASS_DATE = :classDate AND \
							CLASS_NUMBER = :classNumber AND \
							TEACHER_NAME = :teacherName");
		SqlLiteBind(st, "classDate", classDate);
		SqlLiteBind(st, "classNumber", classNumber);
		SqlLiteBind(st, "teacherName", teacherName);
		if (SQLITE_DONE != sqlite3_step(st))
		{
			ErrorMessageBox(L"无法删除课时学生。", SqlLiteErrMsg());
			return DB_ERROR;
		}
	}

	SqlLiteStatement st("DELETE FROM TBL_CALENDAR_CLASS WHERE \
						CLASS_DATE = :classDate AND \
						CLASS_NUMBER = :classNumber AND \
						TEACHER_NAME = :teacherName");
	SqlLiteBind(st, "classDate", classDate);
	SqlLiteBind(st, "classNumber", classNumber);
	SqlLiteBind(st, "teacherName", teacherName);
	if (SQLITE_DONE != sqlite3_step(st))
	{
		ErrorMessageBox(L"无法删除课时。", SqlLiteErrMsg());
		return DB_ERROR;
	}

	tr.commit();
	return 0;
}


int CalendarClassDataObj::updateByPrimaryKey(const wxDateTime& classDate, int classNumber, const wxString& teacherName, int trans)
{
	wxASSERT(m_classDate.IsValid());
	wxASSERT(classDate.IsValid());
	if (m_studentVector.empty())
	{
		ErrorMessageBox(L"无法更新课时。", L"学生列表中没有确认上课的学生。");
		return DB_ERROR;
	}
	SqlLiteTransaction tr(trans);

	{
		SqlLiteStatement st("DELETE FROM TBL_CALENDAR_CLASS_STUDENT WHERE \
							CLASS_DATE = :classDate AND \
							CLASS_NUMBER = :classNumber AND \
							TEACHER_NAME = :teacherName");
		SqlLiteBind(st, "classDate", classDate);
		SqlLiteBind(st, "classNumber", classNumber);
		SqlLiteBind(st, "teacherName", teacherName);
		if (SQLITE_DONE != sqlite3_step(st))
		{
			ErrorMessageBox(L"无法更新课时，重置失败。", SqlLiteErrMsg());
			return DB_ERROR;
		}
	}

	SqlLiteStatement st("UPDATE TBL_CALENDAR_CLASS SET \
						CLASS_DATE = :classDate, \
						CLASS_NUMBER = :classNumber, \
						TEACHER_NAME = :teacherName, \
						CLASS_NAME = :className, \
						PRACTICE = :practice, \
						ONESHOT = :oneShot, \
						MEMO = :memo \
						WHERE \
						CLASS_DATE = :classDateInput AND \
						CLASS_NUMBER = :classNumberInput AND \
						TEACHER_NAME = :teacherNameInput");
	SqlLiteBind(st, "classDate", m_classDate);
	SqlLiteBind(st, "classNumber", m_classNumber);
	SqlLiteBind(st, "teacherName", m_teacherName);
	SqlLiteBind(st, "className", m_className);
	SqlLiteBind(st, "practice", m_practice);
	SqlLiteBind(st, "oneShot", m_oneShot);
	SqlLiteBind(st, "memo", m_memo);
	SqlLiteBind(st, "classDateInput", classDate);
	SqlLiteBind(st, "classNumberInput", classNumber);
	SqlLiteBind(st, "teacherNameInput", teacherName);
	if (SQLITE_DONE != sqlite3_step(st))
	{
		ErrorMessageBox(L"无法更新课时。", SqlLiteErrMsg());
		return DB_ERROR;
	}

	unsigned int insertError = 0;
	wxVector<CalendarClassDataObj::Student>::iterator iter;
	for (iter = m_studentVector.begin(); iter != m_studentVector.end(); ++iter)
	{
		const CalendarClassDataObj::Student& stud = *iter;
		SqlLiteStatement st("INSERT INTO TBL_CALENDAR_CLASS_STUDENT VALUES ( \
							:classDate, \
							:classNumber, \
							:teacherName, \
							:studentName, \
							:leave, \
							:classFee, \
							:memo)");
		SqlLiteBind(st, "classDate", m_classDate);
		SqlLiteBind(st, "classNumber", m_classNumber);
		SqlLiteBind(st, "teacherName", m_teacherName);
		SqlLiteBind(st, "studentName", stud.m_studentName);
		SqlLiteBind(st, "leave", stud.m_leave);
		SqlLiteBind(st, "classFee", stud.m_classFee);
		SqlLiteBind(st, "memo", stud.m_memo);
		if (SQLITE_DONE != sqlite3_step(st))
		{
			ErrorMessageBox(L"无法更新课时【"+stud.m_studentName+"】。", SqlLiteErrMsg());
			insertError++;
		}
	}
	if (insertError == m_studentVector.size()) return DB_ERROR;

	tr.commit();
	return 0;
}


void CalendarClassDataObj::loadVectorByDate(wxVector<CalendarClassDataObj>& vec, const wxDateTime& date)
{
	vec.clear();
	wxASSERT(date.IsValid());

	SqlLiteStatement st("SELECT * FROM TBL_CALENDAR_CLASS WHERE CLASS_DATE = :date");
	SqlLiteBind(st, "date", date);
	for (int row = 0; SQLITE_ROW == sqlite3_step(st); row++)
	{
		CalendarClassDataObj oneCalendarClass;
		int col = LEFTMOST_COLUMN;
		oneCalendarClass.m_classDate = SqlLiteColumnDate(st, col++); wxASSERT(oneCalendarClass.m_classDate == date);
		oneCalendarClass.m_classNumber = SqlLiteColumnInt(st, col++);
		oneCalendarClass.m_teacherName = SqlLiteColumnStr(st, col++);
		oneCalendarClass.m_className = SqlLiteColumnStr(st, col++);
		oneCalendarClass.m_practice = SqlLiteColumnInt(st, col++);
		oneCalendarClass.m_oneShot = SqlLiteColumnInt(st, col++);
		oneCalendarClass.m_memo = SqlLiteColumnStr(st, col++);

		SqlLiteStatement st("SELECT * FROM TBL_CALENDAR_CLASS_STUDENT \
							WHERE CLASS_DATE   = :date AND \
							      CLASS_NUMBER = :classNumber AND \
								  TEACHER_NAME = :teacherName");
		SqlLiteBind(st, "date", date);
		SqlLiteBind(st, "classNumber", oneCalendarClass.m_classNumber);
		SqlLiteBind(st, "teacherName", oneCalendarClass.m_teacherName);
		for (int row = 0; SQLITE_ROW == sqlite3_step(st); row++)
		{
			CalendarClassDataObj::Student stud;
			col = 3;
			stud.m_studentName = SqlLiteColumnStr(st, col++);
			stud.m_leave = SqlLiteColumnInt(st, col++);
			stud.m_classFee = SqlLiteColumnInt(st, col++);
			stud.m_memo = SqlLiteColumnStr(st, col++);

			oneCalendarClass.m_studentVector.push_back(stud);
		}

		wxASSERT(!oneCalendarClass.m_studentVector.empty());
		vec.push_back(oneCalendarClass);
	}
}


CalendarClassDataObj::Student* CalendarClassDataObj::findStudent(const wxString& studentName)
{
	wxVector<Student>::iterator iter;
	for (iter = m_studentVector.begin(); iter != m_studentVector.end(); ++iter)
	{
		if (iter->m_studentName == studentName)
			return iter;
	}
	return NULL;
}


CalendarClassDataObj::CalendarClassDataObj(const wxDateTime& date, const RegularClassDataObj& regularClass)
{
	wxASSERT(date.IsValid() && date.GetWeekDay() == regularClass.m_classWeekday);
	
	m_classDate = date;
	m_classNumber = regularClass.m_classNumber;
	m_teacherName = regularClass.m_teacherName;
	m_className = regularClass.m_className;
	m_practice = regularClass.m_practice;
	m_oneShot = 0;
	m_memo = regularClass.m_memo;
	
	wxVector<RegularClassDataObj::Student>::const_iterator iter = regularClass.m_studentVector.begin();
	for ( ; iter != regularClass.m_studentVector.end(); ++iter)
	{
		Student student;
		student.m_studentName = iter->m_studentName;      
		student.m_leave = 0;
		student.m_classFee = iter->m_classFee;
		student.m_memo = iter->m_memo;
		
		m_studentVector.push_back(student);		
	}

	_saveFlag = 1;
}


void CalendarClassDataObj::mergeWith(const wxDateTime& date, const RegularClassDataObj& regularClass, MergeType mergeType)
{
	wxASSERT(date.IsValid() && date.GetWeekDay() == regularClass.m_classWeekday);
	wxASSERT(m_classDate == date);
	wxASSERT(m_classDate.GetWeekDay() == regularClass.m_classWeekday);
	wxASSERT(m_classNumber == regularClass.m_classNumber);
	wxASSERT(m_teacherName == regularClass.m_teacherName);
	
	// Leave these things unchanged? or sync up with regular class?
	if (mergeType == MERGE_COMPLETE)
	{
		m_className = regularClass.m_className;
		m_practice = regularClass.m_practice;
		m_memo = regularClass.m_memo;
	}
	
	// Detect merge conflit and alert!!!
	if (m_oneShot)
	{
		m_memo = CLASS_MEMOERROR_STR;	
	}
	
	// Now only merge students
	wxVector<RegularClassDataObj::Student>::const_iterator iter = regularClass.m_studentVector.begin();
	for ( ; iter != regularClass.m_studentVector.end(); ++iter)
	{
		Student* pStudent = findStudent(iter->m_studentName);
		if (pStudent == NULL)
		{
			Student student;
			student.m_studentName = iter->m_studentName;
			student.m_leave = 0;
			student.m_classFee = iter->m_classFee;
			student.m_memo = iter->m_memo;
			
			m_studentVector.push_back(student);
			_saveFlag = 1;
		}
	}
}
