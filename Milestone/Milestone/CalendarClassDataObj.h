#ifndef CalendarClassDataObj_INCLUDED
#define CalendarClassDataObj_INCLUDED


#include "Util.h"


struct RegularClassDataObj;


struct CalendarClassDataObj
{
	wxDateTime m_classDate;
	int m_classNumber;
	wxString m_teacherName;
	wxString m_className;
	int m_practice;
	int m_oneShot;
	wxString m_memo;

	struct Student
	{
		wxString m_studentName;
		int m_leave;
		int m_classFee;
		wxString m_memo;

		Student();
	};

	wxVector<Student> m_studentVector;
	int _saveFlag; // Used by MainFrame::saveGrid()

	CalendarClassDataObj();

	int loadByPrimaryKey(const wxDateTime& classDate, int classNumber, const wxString& teacherName);
	int insertThisObj(int trans = TRANSACTIONAL);
	static int deleteByPrimaryKey(const wxDateTime& classDate, int classNumber, const wxString& teacherName, int trans = TRANSACTIONAL);
	int updateByPrimaryKey(const wxDateTime& classDate, int classNumber, const wxString& teacherName, int trans = TRANSACTIONAL);

	static void loadVectorByDate(wxVector<CalendarClassDataObj>& vec, const wxDateTime& date);
	Student* findStudent(const wxString& studentName);
	CalendarClassDataObj(const wxDateTime& date, const RegularClassDataObj& regularClass);
	enum MergeType { MERGE_STUDENT, MERGE_COMPLETE };
	void mergeWith(const wxDateTime& date, const RegularClassDataObj& regularClass, MergeType mergeType);
};


#endif // CalendarClassDataObj_INCLUDED
