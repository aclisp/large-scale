#include "stdafx.h"
#include "RegularClassDataObj.h"
#include "SqlLite.h"
#include "CalendarClassDataObj.h"


RegularClassDataObj::Student::Student()
{
	m_classFee = 0;
}


RegularClassDataObj::RegularClassDataObj()
{
	m_classNumber = 0;
	m_classWeekday = 0;
	m_practice = 0;
}


int RegularClassDataObj::loadByPrimaryKey(int classNumber, int classWeekday, const wxString& teacherName)
{
	SqlLiteStatement st("SELECT * FROM TBL_REGULAR_CLASS WHERE \
						CLASS_NUMBER = :classNumber AND \
						CLASS_WEEKDAY = :classWeekday AND \
						TEACHER_NAME = :teacherName \
						");
	SqlLiteBind(st, "classNumber", classNumber);
	SqlLiteBind(st, "classWeekday", classWeekday);
	SqlLiteBind(st, "teacherName", teacherName);
	if (SQLITE_ROW == sqlite3_step(st))
	{
		int col = 0;
		m_classNumber = SqlLiteColumnInt(st, col++); wxASSERT(m_classNumber == classNumber);
		m_classWeekday = SqlLiteColumnInt(st, col++); wxASSERT(m_classWeekday == classWeekday);
		m_teacherName = SqlLiteColumnStr(st, col++); wxASSERT(m_teacherName == teacherName);
		m_className = SqlLiteColumnStr(st, col++);
		m_practice = SqlLiteColumnInt(st, col++);
		m_memo = SqlLiteColumnStr(st, col++);

		m_studentVector.clear();

		SqlLiteStatement st("SELECT * FROM TBL_REGULAR_CLASS_STUDENT WHERE \
							CLASS_NUMBER = :classNumber AND \
							CLASS_WEEKDAY = :classWeekday AND \
							TEACHER_NAME = :teacherName \
							");
		SqlLiteBind(st, "classNumber", classNumber);
		SqlLiteBind(st, "classWeekday", classWeekday);
		SqlLiteBind(st, "teacherName", teacherName);
		for (int row = 0; SQLITE_ROW == sqlite3_step(st); row++)
		{
			Student stud;
			col = 3;
			stud.m_studentName = SqlLiteColumnStr(st, col++);
			stud.m_classFee = SqlLiteColumnInt(st, col++);
			stud.m_memo = SqlLiteColumnStr(st, col++);

			m_studentVector.push_back(stud);
		}

		wxASSERT(!m_studentVector.empty());
		return 0;
	}

	return DB_NOTFOUND;
}


int RegularClassDataObj::insertThisObj(int trans)
{
	if (m_studentVector.empty())
	{
		ErrorMessageBox(L"�޷�������ʱ��", L"ѧ���б���û��ȷ���Ͽε�ѧ����");
		return DB_ERROR;
	}

	SqlLiteTransaction tr(trans);

	// Added on 2012-8-11 to prevent ��ʱ��ͻ 
	// Added on 2012-8-12 ����Ƿ���ȷ�ϲ������޸Ŀ�ʱ
	wxDateTime startDay = wxDateTime::Today();
	if (IsLockCalendarClass(startDay)) startDay += wxDateSpan::Day();
	wxDateTime endDay = wxDateTime::Today() + wxDateSpan::Months(3);
	for (wxDateTime d = startDay; d < endDay; d += wxDateSpan::Day())
	{
		if (d.GetWeekDay() == m_classWeekday)
		{
			CalendarClassDataObj calendarClass;
			if (OP_SUCCESS == calendarClass.loadByPrimaryKey(d, m_classNumber, m_teacherName))
			{
				ErrorMessageBox(L"�޷�������ʱ��", L"��ʦ��"+m_teacherName+"���ڡ�"+d.FormatISODate()+" "+CLASS_NUMBER_STR[m_classNumber]+"���Ѿ����ſ�ʱ��");
				return DB_ERROR;
			}
		}
	}
	endDay = wxDateTime::Today() + wxDateSpan::Months(1);
	for (wxDateTime d = startDay; d < endDay; d += wxDateSpan::Day())
	{
		if (d.GetWeekDay() == m_classWeekday)
		{
			CalendarClassDataObj classFromRegular(d, *this);
			classFromRegular.insertThisObj(TRANSACTION_SCOPE);
		}
	}

	SqlLiteStatement st("INSERT INTO TBL_REGULAR_CLASS VALUES ( \
						:classNumber, \
						:classWeekday, \
						:teacherName, \
						:className, \
						:practice, \
						:memo)");
	SqlLiteBind(st, "classNumber", m_classNumber);
	SqlLiteBind(st, "classWeekday", m_classWeekday);
	SqlLiteBind(st, "teacherName", m_teacherName);
	SqlLiteBind(st, "className", m_className);
	SqlLiteBind(st, "practice", m_practice);
	SqlLiteBind(st, "memo", m_memo);
	if (SQLITE_DONE != sqlite3_step(st))
	{
		ErrorMessageBox(L"�޷�������ʱ��", SqlLiteErrMsg());
		return DB_ERROR;
	}

	unsigned int insertError = 0;
	wxVector<RegularClassDataObj::Student>::iterator iter;
	for (iter = m_studentVector.begin(); iter != m_studentVector.end(); ++iter)
	{
		const RegularClassDataObj::Student& stud = *iter;
		SqlLiteStatement st("INSERT INTO TBL_REGULAR_CLASS_STUDENT VALUES ( \
							:classNumber, \
							:classWeekday, \
							:teacherName, \
							:studentName, \
							:classFee, \
							:memo)");
		SqlLiteBind(st, "classNumber", m_classNumber);
		SqlLiteBind(st, "classWeekday", m_classWeekday);
		SqlLiteBind(st, "teacherName", m_teacherName);
		SqlLiteBind(st, "studentName", stud.m_studentName);
		SqlLiteBind(st, "classFee", stud.m_classFee);
		SqlLiteBind(st, "memo", stud.m_memo);
		if (SQLITE_DONE != sqlite3_step(st))
		{
			ErrorMessageBox(L"�޷�������ʱ��"+stud.m_studentName+"����", SqlLiteErrMsg());
			insertError++;
		}
	}
	if (insertError == m_studentVector.size()) return DB_ERROR;
		
	tr.commit();
	return 0;
}


int RegularClassDataObj::deleteByPrimaryKey(int classNumber, int classWeekday, const wxString& teacherName, int trans)
{
	SqlLiteTransaction tr(trans);

	// Added on 2012-8-11 to ͬ��ɾ�� 
	// Added on 2012-8-12 ����Ƿ���ȷ�ϲ������޸Ŀ�ʱ
	wxDateTime startDay = wxDateTime::Today();
	if (IsLockCalendarClass(startDay)) startDay += wxDateSpan::Day();
	wxDateTime endDay = wxDateTime::Today() + wxDateSpan::Months(3);
	for (wxDateTime d = startDay; d < endDay; d += wxDateSpan::Day())
	{
		if (d.GetWeekDay() == classWeekday)
		{
			CalendarClassDataObj calendarClass;
			if (OP_SUCCESS == calendarClass.loadByPrimaryKey(d, classNumber, teacherName))
			{
				CalendarClassDataObj::deleteByPrimaryKey(d, classNumber, teacherName, TRANSACTION_SCOPE);
			}
		}
	}
	
	{
		SqlLiteStatement st("DELETE FROM TBL_REGULAR_CLASS_STUDENT WHERE \
							CLASS_NUMBER = :classNumber AND \
							CLASS_WEEKDAY = :classWeekday AND \
							TEACHER_NAME = :teacherName");
		SqlLiteBind(st, "classNumber", classNumber);
		SqlLiteBind(st, "classWeekday", classWeekday);
		SqlLiteBind(st, "teacherName", teacherName);
		if (SQLITE_DONE != sqlite3_step(st))
		{
			ErrorMessageBox(L"�޷�ɾ����ʱѧ����", SqlLiteErrMsg());
			return DB_ERROR;
		}
	}

	SqlLiteStatement st("DELETE FROM TBL_REGULAR_CLASS WHERE \
						CLASS_NUMBER = :classNumber AND \
						CLASS_WEEKDAY = :classWeekday AND \
						TEACHER_NAME = :teacherName");
	SqlLiteBind(st, "classNumber", classNumber);
	SqlLiteBind(st, "classWeekday", classWeekday);
	SqlLiteBind(st, "teacherName", teacherName);
	if (SQLITE_DONE != sqlite3_step(st))
	{
		ErrorMessageBox(L"�޷�ɾ����ʱ��", SqlLiteErrMsg());
		return DB_ERROR;
	}

	tr.commit();
	return 0;
}


int RegularClassDataObj::updateByPrimaryKey(int classNumber, int classWeekday, const wxString& teacherName, int trans)
{
	if (m_studentVector.empty())
	{
		ErrorMessageBox(L"�޷����¿�ʱ��", L"ѧ���б���û��ȷ���Ͽε�ѧ����");
		return DB_ERROR;
	}

	SqlLiteTransaction tr(trans);

	// Added on 2012-8-11 to ͬ������ 
	// Added on 2012-8-12 ����Ƿ���ȷ�ϲ������޸Ŀ�ʱ
	wxDateTime startDay = wxDateTime::Today();
	if (IsLockCalendarClass(startDay)) startDay += wxDateSpan::Day();
	wxDateTime endDay = wxDateTime::Today() + wxDateSpan::Months(3);
	for (wxDateTime d = startDay; d < endDay; d += wxDateSpan::Day())
	{
		if (d.GetWeekDay() == classWeekday)
		{
			CalendarClassDataObj calendarClass;
			if (OP_SUCCESS == calendarClass.loadByPrimaryKey(d, classNumber, teacherName))
			{
				calendarClass.mergeWith(d, *this, CalendarClassDataObj::MERGE_COMPLETE);
				calendarClass.updateByPrimaryKey(d, classNumber, teacherName, TRANSACTION_SCOPE);
			}
		}
	}

	{
		SqlLiteStatement st("DELETE FROM TBL_REGULAR_CLASS_STUDENT WHERE \
							CLASS_NUMBER = :classNumber AND \
							CLASS_WEEKDAY = :classWeekday AND \
							TEACHER_NAME = :teacherName");
		SqlLiteBind(st, "classNumber", classNumber);
		SqlLiteBind(st, "classWeekday", classWeekday);
		SqlLiteBind(st, "teacherName", teacherName);
		if (SQLITE_DONE != sqlite3_step(st))
		{
			ErrorMessageBox(L"�޷����¿�ʱ������ʧ�ܡ�", SqlLiteErrMsg());
			return DB_ERROR;
		}
	}

	SqlLiteStatement st("UPDATE TBL_REGULAR_CLASS SET \
						CLASS_NUMBER = :classNumber, \
						CLASS_WEEKDAY = :classWeekday, \
						TEACHER_NAME = :teacherName, \
						CLASS_NAME = :className, \
						PRACTICE = :practice, \
						MEMO = :memo \
						WHERE \
						CLASS_NUMBER = :classNumberInput AND \
						CLASS_WEEKDAY = :classWeekdayInput AND \
						TEACHER_NAME = :teacherNameInput");
	SqlLiteBind(st, "classNumber", m_classNumber);
	SqlLiteBind(st, "classWeekday", m_classWeekday);
	SqlLiteBind(st, "teacherName", m_teacherName);
	SqlLiteBind(st, "className", m_className);
	SqlLiteBind(st, "practice", m_practice);
	SqlLiteBind(st, "memo", m_memo);
	SqlLiteBind(st, "classNumberInput", classNumber);
	SqlLiteBind(st, "classWeekdayInput", classWeekday);
	SqlLiteBind(st, "teacherNameInput", teacherName);
	if (SQLITE_DONE != sqlite3_step(st))
	{
		ErrorMessageBox(L"�޷����¿�ʱ��", SqlLiteErrMsg());
		return DB_ERROR;
	}

	unsigned int insertError = 0;
	wxVector<RegularClassDataObj::Student>::iterator iter;
	for (iter = m_studentVector.begin(); iter != m_studentVector.end(); ++iter)
	{
		const RegularClassDataObj::Student& stud = *iter;
		SqlLiteStatement st("INSERT INTO TBL_REGULAR_CLASS_STUDENT VALUES ( \
							:classNumber, \
							:classWeekday, \
							:teacherName, \
							:studentName, \
							:classFee, \
							:memo)");
		SqlLiteBind(st, "classNumber", m_classNumber);
		SqlLiteBind(st, "classWeekday", m_classWeekday);
		SqlLiteBind(st, "teacherName", m_teacherName);
		SqlLiteBind(st, "studentName", stud.m_studentName);
		SqlLiteBind(st, "classFee", stud.m_classFee);
		SqlLiteBind(st, "memo", stud.m_memo);
		if (SQLITE_DONE != sqlite3_step(st))
		{
			ErrorMessageBox(L"�޷����¿�ʱ��"+stud.m_studentName+"����", SqlLiteErrMsg());
			insertError++;
		}
	}
	if (insertError == m_studentVector.size()) return DB_ERROR;

	tr.commit();
	return 0;
}


void RegularClassDataObj::loadVectorByDate(wxVector<RegularClassDataObj>& vec, const wxDateTime& date)
{
	vec.clear();
	wxASSERT(date.IsValid());

	SqlLiteStatement st("SELECT * FROM TBL_REGULAR_CLASS WHERE CLASS_WEEKDAY = :weekday");
	SqlLiteBind(st, "weekday", date.GetWeekDay());
	for (int row = 0; SQLITE_ROW == sqlite3_step(st); row++)
	{
		RegularClassDataObj oneRegularClass;
		int col = LEFTMOST_COLUMN;
		oneRegularClass.m_classNumber = SqlLiteColumnInt(st, col++); 
		oneRegularClass.m_classWeekday = SqlLiteColumnInt(st, col++); wxASSERT(oneRegularClass.m_classWeekday == date.GetWeekDay());
		oneRegularClass.m_teacherName = SqlLiteColumnStr(st, col++);
		oneRegularClass.m_className = SqlLiteColumnStr(st, col++);
		oneRegularClass.m_practice = SqlLiteColumnInt(st, col++);
		oneRegularClass.m_memo = SqlLiteColumnStr(st, col++);

		SqlLiteStatement st("SELECT * FROM TBL_REGULAR_CLASS_STUDENT \
							WHERE CLASS_NUMBER  = :classNumber AND \
							      CLASS_WEEKDAY = :weekday AND \
								  TEACHER_NAME  = :teacherName");
		SqlLiteBind(st, "classNumber", oneRegularClass.m_classNumber);
		SqlLiteBind(st, "weekday", date.GetWeekDay());
		SqlLiteBind(st, "teacherName", oneRegularClass.m_teacherName);
		for (int row = 0; SQLITE_ROW == sqlite3_step(st); row++)
		{
			RegularClassDataObj::Student stud;
			col = 3;
			stud.m_studentName = SqlLiteColumnStr(st, col++);
			stud.m_classFee = SqlLiteColumnInt(st, col++);
			stud.m_memo = SqlLiteColumnStr(st, col++);

			oneRegularClass.m_studentVector.push_back(stud);
		}

		wxASSERT(!oneRegularClass.m_studentVector.empty());
		vec.push_back(oneRegularClass);
	}
}
