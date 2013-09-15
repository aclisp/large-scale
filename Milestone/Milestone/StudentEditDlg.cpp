#include "stdafx.h"
#include "StudentEditDlg.h"
#include "Util.h"
#include "SqlLite.h"
#include "CalendarClassDataObj.h"
#include "ClassDlg.h"


StudentEditDlg::StudentEditDlg( wxWindow* parent, const wxString& studentName )
:
StudentEditDlgGui( parent ), _studentName(studentName)
{
	wxListCtrl *pLc = m_lClass;
	pLc->AppendColumn(L"日期");
	pLc->AppendColumn(L"时间");
	pLc->AppendColumn(L"教师");
	pLc->AppendColumn(L"请假");
	pLc->AppendColumn(L"收费");
	pLc->AppendColumn(L"类型");
	pLc->AppendColumn(L"集体课名");
	pLc->AppendColumn(L"详细信息");
	pLc->AppendColumn(L"课时记录");

	if (_studentName.empty())
	{
		SetSize(GetSize().GetWidth() - m_lClass->GetSize().GetWidth(), GetSize().GetHeight());
	}
}


void StudentEditDlg::OnClass( wxListEvent& event )
{
	long item = event.GetIndex();
	wxListCtrl *pLc = m_lClass;
	
	wxDateTime classDate;
	classDate.ParseISODate(pLc->GetItemText(item, 0));
	int classNumber = GetClassNumberByStr(pLc->GetItemText(item, 1));
	wxString teacherName = pLc->GetItemText(item, 2);

	CalendarClassDataObj c;
	if (OP_SUCCESS == c.loadByPrimaryKey(classDate, classNumber, teacherName))
	{
		ClassDlg d(this, ClassDlg::CALENDAR);
		d.SetClassValue(c);

		// Prevent crazy input!
		d.m_classDate->Disable();
		d.m_classNumber->Disable();
		d.m_teacherName->SetEditable(false);
		if (classDate >= wxDateTime::Today()) d.m_deleteClass->Enable(c.m_oneShot > 0);

		if (d.ShowModal() == wxID_OK)
		{
			if (d.m_deleteClass->GetValue())
			{
				CalendarClassDataObj::deleteByPrimaryKey(classDate, classNumber, teacherName);
				showClass();
				if (item>=pLc->GetItemCount()) --item;
				pLc->EnsureVisible(item);
			}
			else
			{
				d.GetClassValue(&c);
				c.updateByPrimaryKey(classDate, classNumber, teacherName);
				showClass();
				pLc->SetItemState(item, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
				pLc->SetItemState(item, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED);
				pLc->EnsureVisible(item);
				pLc->SetFocus();				
			}
		}
	}
}


void StudentEditDlg::showClass()
{
	wxString studentName = m_name->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
	if (studentName.empty()) return;
	wxListCtrl *pLc = m_lClass;
	pLc->DeleteAllItems();
	
	SqlLiteStatement st("SELECT * FROM TBL_CALENDAR_CLASS_STUDENT WHERE \
						STUDENT_NAME = :studentName ORDER BY CLASS_DATE DESC LIMIT 0, 100");
	SqlLiteBind(st, "studentName", studentName);
	for (int row = 0; SQLITE_ROW == sqlite3_step(st); ++row)
	{
		int col = 0;
		wxString classDate = SqlLiteColumnStr(st, col++);
		int classNumber = SqlLiteColumnInt(st, col++);
		wxString teacherName = SqlLiteColumnStr(st, col++);
		col++;
		int leave = SqlLiteColumnInt(st, col++);
		int classFee = SqlLiteColumnInt(st, col++);
		wxString studentMemo = SqlLiteColumnStr(st, col++);
		wxString className;
		wxString classMemo;
		int oneShot = 0;
		int practice = 0;

		SqlLiteStatement st("SELECT * FROM TBL_CALENDAR_CLASS WHERE \
							CLASS_DATE = :classDate AND \
							CLASS_NUMBER = :classNumber AND \
							TEACHER_NAME = :teacherName");
		SqlLiteBind(st, "classDate", classDate);
		SqlLiteBind(st, "classNumber", classNumber);
		SqlLiteBind(st, "teacherName", teacherName);
		if (SQLITE_ROW == sqlite3_step(st))
		{
			col = 3;
			className = SqlLiteColumnStr(st, col++);
			practice = SqlLiteColumnInt(st, col++);
			oneShot = SqlLiteColumnInt(st, col++);
			classMemo = SqlLiteColumnStr(st, col++);
		}

		col = 0;
		pLc->InsertItem(row, wxEmptyString);
		if (classDate < wxDateTime::Today().FormatISODate()) 
			pLc->SetItemBackgroundColour(row, wxColor(234, 234, 234));
		if (oneShot) pLc->SetItemTextColour(row, *wxBLUE);
		if (leave) pLc->SetItemTextColour(row, *wxRED);
		pLc->SetItem(row, col, classDate); col++;
		pLc->SetItem(row, col, CLASS_NUMBER_STR[classNumber]); col++;
		pLc->SetItem(row, col, teacherName); col++;
		pLc->SetItem(row, col, CLASS_STUDENT_LEAVE_STR[leave]); col++;
		pLc->SetItem(row, col, wxString::Format("%d", classFee)); col++;
		pLc->SetItem(row, col, CLASS_PRACTICE_STR[practice]); col++;
		pLc->SetItem(row, col, className); col++;
		pLc->SetItem(row, col, classMemo); col++;
		pLc->SetItem(row, col, studentMemo); col++;
	}
}
