﻿#ifndef __MainFrame__
#define __MainFrame__


#include "CalendarClassDataObj.h"


/**
@file
Subclass of MainFrameGui, which is generated by wxFormBuilder.
*/

#include "MainFrameGui.h"

//// end generated include


class StudentDlg;
class TeacherDlg;


/** Implementing MainFrameGui */
class MainFrame : public MainFrameGui
{
	protected:
		// Handlers for MainFrameGui events.
		void OnTimerBackup(wxTimerEvent& event);
		void OnClickTeacher( wxCommandEvent& event );
		void OnClickStudent( wxCommandEvent& event );
		void OnClickRainlendar( wxCommandEvent& event );
		void OnClickStat( wxCommandEvent& event );
		void OnClickConfirm( wxCommandEvent& event );
		void OnCalendarSelChanged( wxCalendarEvent& event );
		void OnCalendarClass( wxGridEvent& event );
		void OnMenuAdmin( wxCommandEvent& event );

		void updateGridCols();
		void clearGrid();
		void loadClassVector();
		CalendarClassDataObj* findCalendarClass(int classNumber, const wxString& teacherName);
		CalendarClassDataObj* existClassStudent(int classNumber, const wxString& studentName);
		void setCellValue(const CalendarClassDataObj& calendarClass);
		int getColByTeacherName(const wxString& teacherName);
		wxString getTeacherNameByCol(int col);
		enum { CALENDAR_CLASS_NOOP, CALENDAR_CLASS_CHANGED, CALENDAR_CLASS_DELETED };
		int addOneShotClass(CalendarClassDataObj **ppOut, int row, int col);
		int updateOneShotClass(CalendarClassDataObj *pCalendarClass, int row, int col);
		int updateRegularClass(CalendarClassDataObj *pCalendarClass, int row, int col);

	public:
		/** Constructor */
		MainFrame( wxWindow* parent );
		~MainFrame();
	//// end generated class members
	
		void updateGrid();
		void saveGrid();
		void updateDateText();
		void updateConfirmationButton();

	private:
		// the data of Grid
		wxVector<CalendarClassDataObj> m_classVector; 
		StudentDlg* m_studentDlg;
		TeacherDlg* m_teacherDlg;
		wxTimer* m_timerBackup;
		sqlite3_backup* m_pBackup;
		sqlite3* m_pBackupDatabase;
		wxString _adminName;
};

#endif // __MainFrame__
