#ifndef RegularClassDataObj_INCLUDED
#define RegularClassDataObj_INCLUDED


#include "Util.h"


struct RegularClassDataObj
{
	int m_classNumber;
	int m_classWeekday;
	wxString m_teacherName;
	wxString m_className;
	int m_practice;
	wxString m_memo;

	struct Student
	{
		wxString m_studentName;
		int m_classFee;
		wxString m_memo;

		Student();
	};

	wxVector<Student> m_studentVector;

	RegularClassDataObj();

	int loadByPrimaryKey(int classNumber, int classWeekday, const wxString& teacherName);
	int insertThisObj(int trans = TRANSACTIONAL);
	static int deleteByPrimaryKey(int classNumber, int classWeekday, const wxString& teacherName, int trans = TRANSACTIONAL);
	int updateByPrimaryKey(int classNumber, int classWeekday, const wxString& teacherName, int trans = TRANSACTIONAL);

	static void loadVectorByDate(wxVector<RegularClassDataObj>& vec, const wxDateTime& date);
};


#endif // RegularClassDataObj_INCLUDED
