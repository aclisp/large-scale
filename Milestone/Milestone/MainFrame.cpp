#include "stdafx.h"
#include "MainFrame.h"
#include "SqlLite.h"
#include "CalendarClassDataObj.h"
#include "RegularClassDataObj.h"
#include "StudentDlg.h"
#include "TeacherDlg.h"
#include "StatDlg.h"
#include "ClassDlg.h"
#include "AdminDlg.h"


#define TIMER_ID_BACKUP 10001
#define TIMER_BACKUP_INTERVAL 3600*1000


MainFrame::MainFrame( wxWindow* parent )
:
MainFrameGui( parent )
{
	m_studentDlg = NULL;
	m_teacherDlg = NULL;
	m_pBackup = NULL;
	m_pBackupDatabase = NULL;
	m_timerBackup = new wxTimer(this, TIMER_ID_BACKUP);
	Bind(wxEVT_TIMER, &MainFrame::OnTimerBackup, this, TIMER_ID_BACKUP);
	if (!wxConfig::Get()->ReadBool("ReadOnly", true))
	{
		m_timerBackup->Start(TIMER_BACKUP_INTERVAL, wxTIMER_CONTINUOUS);
	}
	else SetTitle(GetTitle()+L"（只读模式）");

	m_gCClass->ShowScrollbars(wxSHOW_SB_DEFAULT, wxSHOW_SB_NEVER);
	m_gCClass->SetRowLabelSize(45);
	m_gCClass->SetDefaultColSize(47);

	updateConfirmationButton();
	updateDateText();
	updateGrid();
	saveGrid();

	SetIcon(wxIcon("IDI_MAINAPP"));
	SetTitle(GetTitle()+L"（未登录）");
	Maximize();
}


MainFrame::~MainFrame()
{
	if (m_pBackupDatabase && m_pBackup)
	{
		//int rc;
		//do {
		//	rc = sqlite3_backup_step(m_pBackup, 5);
		//} while ( rc==SQLITE_OK || rc==SQLITE_BUSY || rc==SQLITE_LOCKED );
		sqlite3_backup_step(m_pBackup, -1);
		sqlite3_backup_finish(m_pBackup);
	}
	sqlite3_close(m_pBackupDatabase);
}


void MainFrame::updateGrid()
{
	clearGrid();
	updateGridCols();
	loadClassVector();

	wxVector<CalendarClassDataObj>::iterator iter;
	for (iter = m_classVector.begin(); iter != m_classVector.end(); ++iter)
	{
		wxASSERT(iter->m_classDate == m_calendar1->GetDate());
		this->setCellValue(*iter);
	}
}


void MainFrame::saveGrid()
{
	wxDateTime date = m_calendar1->GetDate();
	if (IsLockCalendarClass(date)) return;
	if (date < wxDateTime::Today()) return;
	if (date > wxDateTime::Today()+wxDateSpan::Months(3)) return;
	if (wxConfig::Get()->ReadBool("ReadOnly", true)) return;

	SqlLiteTransaction tr(TRANSACTIONAL);
	wxVector<CalendarClassDataObj>::const_iterator iter;
	for (iter = m_classVector.begin(); iter != m_classVector.end(); ++iter)
	{
		const CalendarClassDataObj& c = *iter;
		if (c._saveFlag)
		{
			CalendarClassDataObj temp;
			if (OP_SUCCESS == temp.loadByPrimaryKey(c.m_classDate, c.m_classNumber, c.m_teacherName))
			{
				temp = c;
				temp.updateByPrimaryKey(c.m_classDate, c.m_classNumber, c.m_teacherName, TRANSACTION_SCOPE);
			}
			else
			{
				temp = c;
				temp.insertThisObj(TRANSACTION_SCOPE);
			}
		}
	}
	tr.commit();
}


void MainFrame::updateDateText()
{
	m_staticTextToday->SetLabelText(wxDateTime::Today().FormatISODate());
	m_staticTextThisday->SetLabelText(CLASS_WEEKDAY_STR[wxDateTime::Today().GetWeekDay()]);
	m_staticTextCalendar->SetLabelText(m_calendar1->GetDate().FormatISODate());
	m_staticTextWeekday->SetLabelText(CLASS_WEEKDAY_STR[m_calendar1->GetDate().GetWeekDay()]);
}


void MainFrame::updateConfirmationButton()
{
	wxDateTime date = m_calendar1->GetDate();
	m_button3->Show(date <= wxDateTime::Today());

	wxString lockBy = SqlLiteQueryForString("SELECT LOCK_BY FROM TBL_CALENDAR_CLASS_LOCK WHERE \
											LOCK_DATE = '"+date.FormatISODate()+"'");
	m_button3->SetValue(!lockBy.empty());
	if (lockBy.empty()) m_button3->SetLabel(L"确认课表");
	else m_button3->SetLabel(lockBy+L"\n已确认");
}


void MainFrame::updateGridCols()
{
	// remember position, and clear all existing columns
	int row = m_gCClass->GetGridCursorRow();
	int col = m_gCClass->GetGridCursorCol();
	int totalCol = m_gCClass->GetNumberCols();
	if (totalCol) m_gCClass->DeleteCols(0, totalCol);

	// get all teacher name
	SqlLiteStatement st("SELECT TEACHER_NAME FROM TBL_TEACHER \
						ORDER BY TEACHER_NAME COLLATE pinyin LIMIT 0,100");
	for (int row = 0; SQLITE_ROW == sqlite3_step(st); row++)
	{
		wxString teacherName = SqlLiteColumnStr(st,	LEFTMOST_COLUMN);
		m_gCClass->AppendCols();
		m_gCClass->SetColLabelValue(row, teacherName);
	}

	// restore position
	if (row>=0 && col>=0) 
	{
		m_gCClass->GoToCell(row, col);
		m_gCClass->SelectBlock(row, col, row, col);
	}
	else if (m_gCClass->GetNumberCols())
	{
		m_gCClass->GoToCell(0, 0);
	}
}


void MainFrame::clearGrid()
{
	m_classVector.clear();
	m_gCClass->ClearGrid();
	m_gCClass->ClearSelection();
}


void MainFrame::loadClassVector()
{
	wxDateTime date = m_calendar1->GetDate();

	// First we load from TBL_CALENDAR_CLASS
	CalendarClassDataObj::loadVectorByDate(m_classVector, date);

	// Then we load from TBL_REGULAR_CLASS

	// Here we need to know if the date of Class is locked:
	if (IsLockCalendarClass(date)) return;
	if (date < wxDateTime::Today()) return;

	wxVector<RegularClassDataObj> tempVector;
	RegularClassDataObj::loadVectorByDate(tempVector, date);
	wxVector<RegularClassDataObj>::iterator iter;
	for (iter = tempVector.begin() ; iter != tempVector.end(); ++iter)
	{
		RegularClassDataObj *pRegularClass = iter;
		CalendarClassDataObj *pCalendarClass = this->findCalendarClass(pRegularClass->m_classNumber, pRegularClass->m_teacherName);
		if (pCalendarClass == NULL)
		{
			// 2012-9-20 Check the student would change class to another teacher
			wxVector<RegularClassDataObj::Student>& studVec = pRegularClass->m_studentVector;
			wxVector<RegularClassDataObj::Student>::iterator iter;
			for (iter = studVec.begin(); iter != studVec.end(); )
			{
				if (this->existClassStudent(pRegularClass->m_classNumber, iter->m_studentName))
					iter = studVec.erase(iter);
				else
					++iter;
			}

			if (!studVec.empty())
			{
				CalendarClassDataObj classFromRegular(date, *pRegularClass);
				m_classVector.push_back(classFromRegular);
			}
		}
		else
		{
			// We do merge student only!
			pCalendarClass->mergeWith(date, *pRegularClass, CalendarClassDataObj::MERGE_STUDENT);
		}
	}
}


CalendarClassDataObj* MainFrame::findCalendarClass(int classNumber, const wxString& teacherName)
{
	wxVector<CalendarClassDataObj>::iterator iter;
	for (iter = m_classVector.begin(); iter != m_classVector.end(); ++iter)
	{
		CalendarClassDataObj* pCalendarClass = iter;
		if (pCalendarClass->m_classNumber == classNumber && pCalendarClass->m_teacherName == teacherName)
			return pCalendarClass;
	}
	return NULL;
}


CalendarClassDataObj* MainFrame::existClassStudent(int classNumber, const wxString& studentName)
{
	wxVector<CalendarClassDataObj>::iterator iter;
	for (iter = m_classVector.begin(); iter != m_classVector.end(); ++iter)
	{
		CalendarClassDataObj* pCalendarClass = iter;
		if (pCalendarClass->m_classNumber == classNumber && pCalendarClass->findStudent(studentName))
			return pCalendarClass;
	}
	return NULL;
}


void MainFrame::setCellValue(const CalendarClassDataObj& calendarClass)
{
	// 需要设置的变量
	const wxColour *cellBackgroundColor = wxWHITE;
	const wxColour *cellTextColor = wxBLACK;
	wxString cellValue;

	// 确定行和列
	int row = calendarClass.m_classNumber;
	int col = getColByTeacherName(calendarClass.m_teacherName);
	if (col == -1)
	{
		col = m_gCClass->GetNumberCols();
		m_gCClass->AppendCols();
		m_gCClass->SetColLabelValue(col, calendarClass.m_teacherName);
	}
	
	// 确定CellValue
	if (calendarClass.m_studentVector.size() == 1) // 一对一教学
	{
		const CalendarClassDataObj::Student& student = calendarClass.m_studentVector[0];
		if (student.m_leave)
		{
			cellBackgroundColor = wxWHITE;
			cellTextColor = wxRED;
			cellValue = CLASS_STUDENT_LEAVE_STR[1];
		}
		else
		{
			cellBackgroundColor = wxWHITE;
			cellTextColor = wxBLACK;
			cellValue = student.m_studentName + CLASS_PRACTICE_STR[calendarClass.m_practice];
		}
	}
	else if (calendarClass.m_studentVector.size() == 2) // 请假替补之一对一
	{
		const CalendarClassDataObj::Student& student0 = calendarClass.m_studentVector[0];
		const CalendarClassDataObj::Student& student1 = calendarClass.m_studentVector[1];
		if (student0.m_leave == 0 && student1.m_leave == 0) // 集体课
		{
			cellBackgroundColor = wxYELLOW;
			cellTextColor = wxBLACK;
			cellValue = calendarClass.m_className + CLASS_PRACTICE_STR[calendarClass.m_practice];
		}
		else if (student0.m_leave == 0 && student1.m_leave == 1)
		{
			cellBackgroundColor = wxWHITE;
			cellTextColor = wxRED;
			cellValue = student0.m_studentName + CLASS_PRACTICE_STR[calendarClass.m_practice];
		}
		else if (student0.m_leave == 1 && student1.m_leave == 0)
		{
			cellBackgroundColor = wxWHITE;
			cellTextColor = wxRED;
			cellValue = student1.m_studentName + CLASS_PRACTICE_STR[calendarClass.m_practice];
		}
		else
		{
			cellBackgroundColor = wxWHITE;
			cellTextColor = wxRED;
			cellValue = CLASS_STUDENT_LEAVE_STR[1];
		}
	}
	else if (calendarClass.m_studentVector.size() == 0) // 不可能发生
	{
		cellBackgroundColor = wxRED;
		cellTextColor = wxWHITE;
		cellValue = L"缺少学生";
	}
	else // 集体课
	{
		cellBackgroundColor = wxYELLOW;
		// 有一人请假则设成红色，所有人请假则设成三角
		unsigned int leaveCount = 0;
		wxVector<CalendarClassDataObj::Student>::const_iterator iter;
		for (iter = calendarClass.m_studentVector.begin(); iter != calendarClass.m_studentVector.end(); ++iter)
		{
			if (iter->m_leave)
			{
				++leaveCount;
			}
		}
		if (leaveCount)
			cellTextColor = wxRED;
		else
			cellTextColor = wxBLACK;
		if (leaveCount == calendarClass.m_studentVector.size())
			cellValue = CLASS_STUDENT_LEAVE_STR[1];
		else
			cellValue = calendarClass.m_className + CLASS_PRACTICE_STR[calendarClass.m_practice];
	}
		
	// 第二行显示课时详细信息
	if (!calendarClass.m_memo.IsEmpty())
	{
		cellValue += L"\n";
		cellValue += calendarClass.m_memo;
	}
	
	// 修正颜色
	if (calendarClass.m_oneShot && cellTextColor == wxBLACK)
		cellTextColor = wxBLUE;
		
	if (calendarClass.m_memo == CLASS_MEMOERROR_STR)
	{
		cellBackgroundColor = wxRED;
		cellTextColor = wxWHITE;
		cellValue = L"课时冲突";
	}
	
	// 设置值
	m_gCClass->SetCellBackgroundColour(row, col, *cellBackgroundColor);
	m_gCClass->SetCellTextColour(row, col, *cellTextColor);
	m_gCClass->SetCellValue(row, col, cellValue);
}


int MainFrame::getColByTeacherName(const wxString& teacherName)
{
	int totalColNumber = m_gCClass->GetNumberCols();
	for (int col=0; col<totalColNumber; ++col)
	{
		wxString colName = m_gCClass->GetColLabelValue(col);
		if (colName == teacherName)
			return col;
	}
	return -1;
}


wxString MainFrame::getTeacherNameByCol(int col)
{
	return m_gCClass->GetColLabelValue(col);
}


void MainFrame::OnCalendarSelChanged( wxCalendarEvent& event )
{
	m_gCClass->EnableGridLines(event.GetDate() >= wxDateTime::Today());
	updateConfirmationButton();
	updateDateText();
	updateGrid();
	saveGrid();
}


void MainFrame::OnClickTeacher( wxCommandEvent& )
{
	if (m_teacherDlg == NULL)
	{
		m_teacherDlg = new TeacherDlg(this);
	}
	m_teacherDlg->Show();
}


void MainFrame::OnClickStudent( wxCommandEvent& )
{
	if (m_studentDlg == NULL)
	{
		m_studentDlg = new StudentDlg(this);
	}
	m_studentDlg->Show();
}


void MainFrame::OnTimerBackup(wxTimerEvent& )
{
	if (m_pBackupDatabase == NULL)
	{
		// start a backup process here
		if (SQLITE_OK == sqlite3_open((wxConfig::Get()->Read("DataDir", DATA_DIR)+"Milestone.bbk").utf8_str(), &m_pBackupDatabase) 
		   && (m_pBackup = sqlite3_backup_init(m_pBackupDatabase, "main", SqlLiteHandle, "main")) != NULL)
		{
			m_timerBackup->Start(250, wxTIMER_CONTINUOUS);
		}
		else
		{
			sqlite3_close(m_pBackupDatabase);
			m_pBackupDatabase = NULL;
		}
	}
	else
	{
		wxASSERT(m_pBackup != NULL);
		// we are in backup progress
		int rc = sqlite3_backup_step(m_pBackup, 250); // 1Mbytes/sec
		if (rc == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED)
		{
			// there are still further pages to copy
		}
		else
		{
			sqlite3_backup_finish(m_pBackup);
			sqlite3_close(m_pBackupDatabase);
			m_pBackup = NULL;
			m_pBackupDatabase = NULL;
			m_timerBackup->Start(TIMER_BACKUP_INTERVAL, wxTIMER_CONTINUOUS);
		}
	}
}


void MainFrame::OnClickRainlendar( wxCommandEvent& )
{
	wxString exeDir = wxFileName(wxStandardPaths::Get().GetExecutablePath()).GetPathWithSep();
	wxExecute(exeDir+"Rainlendar\\Rainlendar.exe");
}


void MainFrame::OnClickStat( wxCommandEvent& )
{
	StatDlg d(this);
	d.ShowModal();
}


void MainFrame::OnClickConfirm( wxCommandEvent& )
{
	wxDateTime date = m_calendar1->GetDate();
	if (IsLockCalendarClass(date)) return; // only confirm non-locked class

	// TODO:
}


void MainFrame::OnCalendarClass( wxGridEvent& event )
{
	int rc;
	int row = event.GetRow();
	int col = event.GetCol();

	if (row == 16) return; // skip dummy row
	
	m_gCClass->GoToCell(row, col);
	m_gCClass->SelectBlock(row, col, row, col);

	CalendarClassDataObj* pCalendarClass = findCalendarClass(row, getTeacherNameByCol(col));
	if (pCalendarClass == NULL)	rc = addOneShotClass(&pCalendarClass, row, col);
	else if (pCalendarClass->m_oneShot)	rc = updateOneShotClass(pCalendarClass, row, col);
	else rc = updateRegularClass(pCalendarClass, row, col);

	if (rc == CALENDAR_CLASS_DELETED)
	{
		m_gCClass->SetCellBackgroundColour(row, col, *wxWHITE);
		m_gCClass->SetCellTextColour(row, col, *wxBLACK);
		m_gCClass->SetCellValue(row, col, wxEmptyString);
	}
	else
	{
		updateGrid();
	}
}


int MainFrame::addOneShotClass(CalendarClassDataObj **ppOut, int row, int col)
{
	wxASSERT(ppOut != NULL && *ppOut == NULL);
	
	CalendarClassDataObj c;
	c.m_classDate = m_calendar1->GetDate();
	c.m_classNumber = row;
	c.m_teacherName = getTeacherNameByCol(col);
	c.m_practice = 0;
	c.m_oneShot = 1;
	
	ClassDlg d(this, ClassDlg::CALENDAR);
	d.SetClassValue(c);
	
	// Prevent crazy input!
	d.m_classDate->Disable();
	d.m_classNumber->Disable();
	d.m_teacherName->SetEditable(false);
	d.m_deleteClass->Disable();
	
	if (d.ShowModal() == wxID_OK)
	{
		d.GetClassValue(&c);
		if (OP_SUCCESS == c.insertThisObj())
		{
			m_classVector.push_back(c);
			*ppOut = &m_classVector.back();
			return CALENDAR_CLASS_CHANGED;
		}
	}

	return CALENDAR_CLASS_NOOP;
}


int MainFrame::updateOneShotClass(CalendarClassDataObj *pCalendarClass, int row, int col)
{
	wxASSERT(pCalendarClass != NULL && pCalendarClass->m_oneShot);
	wxASSERT(pCalendarClass->m_classDate == m_calendar1->GetDate());
	
	ClassDlg d(this, ClassDlg::CALENDAR);
	d.SetClassValue(*pCalendarClass);
	
	// Prevent crazy input!
	d.m_classDate->Disable();
	
	if (d.ShowModal() == wxID_OK)
	{
		if (d.m_deleteClass->GetValue())
		{
			if (OP_SUCCESS == CalendarClassDataObj::deleteByPrimaryKey(m_calendar1->GetDate(), row, getTeacherNameByCol(col)))
			{
				m_classVector.erase(pCalendarClass);
				return CALENDAR_CLASS_DELETED;
			}
		}
		else
		{
			CalendarClassDataObj c;
			d.GetClassValue(&c);
			wxASSERT(c.m_classDate == m_calendar1->GetDate());
			if (OP_SUCCESS == c.updateByPrimaryKey(m_calendar1->GetDate(), row, getTeacherNameByCol(col)))
			{
				*pCalendarClass = c;
				return CALENDAR_CLASS_CHANGED;
			}
		}
	}
	
	return CALENDAR_CLASS_NOOP;
}


int MainFrame::updateRegularClass(CalendarClassDataObj *pCalendarClass, int row, int col)
{
	wxASSERT(pCalendarClass != NULL && pCalendarClass->m_oneShot == 0);
	wxASSERT(pCalendarClass->m_classDate == m_calendar1->GetDate());
	
	ClassDlg d(this, ClassDlg::CALENDAR);
	d.SetClassValue(*pCalendarClass);
	
	// Prevent crazy input!
	d.m_classDate->Disable();
	d.m_classNumber->Disable();
	d.m_teacherName->SetEditable(false);
	//if (pCalendarClass->m_classDate >= wxDateTime::Today())
	//	d.m_deleteClass->Disable();
		
	if (d.ShowModal() == wxID_OK)
	{
		if (d.m_deleteClass->GetValue())
		{
			if (OP_SUCCESS == CalendarClassDataObj::deleteByPrimaryKey(m_calendar1->GetDate(), row, getTeacherNameByCol(col)))
			{
				m_classVector.erase(pCalendarClass);
				return CALENDAR_CLASS_DELETED;
			}
		}
		else
		{
			CalendarClassDataObj c;
			d.GetClassValue(&c);
			wxASSERT(c.m_classDate == m_calendar1->GetDate());
			wxASSERT(c.m_classNumber == row);
			wxASSERT(c.m_teacherName == getTeacherNameByCol(col));
			if (OP_SUCCESS == c.updateByPrimaryKey(m_calendar1->GetDate(), row, getTeacherNameByCol(col)))
			{
				*pCalendarClass = c;
				return CALENDAR_CLASS_CHANGED;
			}
		}
	}
	
	return CALENDAR_CLASS_NOOP;
}


void MainFrame::OnMenuAdmin( wxCommandEvent& )
{
	AdminDlg d(this);
	d.ShowModal();
}
