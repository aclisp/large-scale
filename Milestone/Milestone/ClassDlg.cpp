#include "stdafx.h"
#include "ClassDlg.h"
#include "RegularClassDataObj.h"
#include "CalendarClassDataObj.h"
#include "StudentDataObj.h"
#include "StudentEditDlg.h"


ClassDlg::ClassDlg( wxWindow* parent, ModeType mode )
:
ClassDlgGui( parent )
{
	m_studentName->SetFocus();
	wxListCtrl *pLc = m_lStudentList;

	if (mode == REGULAR) 
	{
		m_staticText1->Hide();
		m_classDate->Hide();
		m_oneShot->Disable();
		m_staticText10->Hide();
		m_leave->Hide();
		pLc->AppendColumn(L"学生姓名", wxLIST_FORMAT_LEFT, 70);
		pLc->AppendColumn(L"收费标准", wxLIST_FORMAT_LEFT, 70);
		pLc->AppendColumn(L"课时记录");
	} 
	else if (mode == CALENDAR) 
	{
		m_staticText11->SetLabelText(L"课时收费");
		m_classWeekday->Disable();
		m_oneShot->Disable();
		pLc->AppendColumn(L"学生姓名", wxLIST_FORMAT_LEFT, 70);
		pLc->AppendColumn(L"实际收费", wxLIST_FORMAT_LEFT, 70);
		pLc->AppendColumn(L"请假", wxLIST_FORMAT_LEFT, 45);
		pLc->AppendColumn(L"课时记录");
	}
}


void ClassDlg::OnClickAddStudent( wxCommandEvent& )
{
	wxString studentName = m_studentName->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
	
	// 保证该学生已存在. See RegularClassDataObj::insertThisObj
	StudentDataObj stud;
	if (studentName.empty())
	{
		ErrorMessageBox(L"学生姓名不能为空。", wxEmptyString);
		m_studentName->SetFocus();
		return;
	}	
		
	if (stud.loadByName(studentName) == DB_NOTFOUND)
	{
		ErrorMessageBox(L"没有录入学生【"+studentName+"】的信息。", wxEmptyString);
		m_studentName->SetFocus();
		return;
	}

	// 保证收费标准不为0
	int classFee = StringToUInt(m_classFee->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT));
	if (classFee < 0 || classFee > 1000)
	{
		ErrorMessageBox(L"收费标准应在【0 - 1000】之内。", wxEmptyString);
		m_classFee->SetFocus();
		return;
	}

	// 查找是否列表中已有学生
	wxListCtrl *pLc = m_lStudentList;
	long item, itemCount;
	for (item = -1, itemCount = 0; -1 != (item = pLc->GetNextItem(item)); ++itemCount) 
		if (studentName == pLc->GetItemText(item)) break;
	
	int row = itemCount;
	int col = 0;
	if (-1 == item) pLc->InsertItem(row, wxEmptyString);
	pLc->SetItem(row, col, studentName); col++;
	pLc->SetItem(row, col, wxString::Format("%d", classFee)); col++;
	if (m_leave->IsShown())
	{
		int leave = m_leave->GetValue()? 1:0;
		if (leave) pLc->SetItemTextColour(row, *wxRED);
		else pLc->SetItemTextColour(row, *wxBLACK);
		pLc->SetItem(row, col, CLASS_STUDENT_LEAVE_STR[leave]); col++;
	}
	pLc->SetItem(row, col, m_studentMemo->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT)); col++;

	// 选中
	pLc->SetItemState(row, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	pLc->SetItemState(row, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED);
	pLc->EnsureVisible(row);

	// 提示填写集体课名
	int studentCount = 0; 
	int activeCount = 0;
	for (item = -1; -1 != (item = pLc->GetNextItem(item)); ++studentCount) 
		if (pLc->GetItemText(item, 2) == CLASS_STUDENT_LEAVE_STR[0])
			++activeCount;	
	if (activeCount > 1 && m_className->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT).IsEmpty())
	{
		pLc->DeleteItem(row);
		ErrorMessageBox(L"请填写集体课名。", wxEmptyString);
		m_className->SetFocus();
	}	
}


void ClassDlg::OnClickDelStudent( wxCommandEvent& )
{
	long item = -1;
	wxListCtrl *pLc = m_lStudentList;

	item = pLc->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (item > -1) 
	{
		if (pLc->DeleteItem(item)) 
		{
			if (item >= pLc->GetItemCount()) --item;
			pLc->EnsureVisible(item);
		}
	}
}


void ClassDlg::OnListStudentActivated( wxListEvent& event )
{
	long item = event.GetIndex();
	wxListCtrl *pLc = m_lStudentList;
	StudentDataObj t;
	wxString studentName = pLc->GetItemText(item);
	if (t.loadByName(studentName) == OP_SUCCESS)
	{
		StudentEditDlg d(this, studentName);
		d.m_name->SetValue(t.m_name);
		d.m_signonDate->SetValue(t.m_signonDate);
		d.m_signoff->SetSelection(t.m_signoff);
		d.m_signoffDate->SetValue(t.m_signoffDate);
		d.m_phone->SetValue(t.m_phone);
		d.m_phone2->SetValue(t.m_phone2);
		d.m_address->SetValue(t.m_address);
		d.m_birthDate->SetValue(t.m_birthDate);
		d.m_age->SetValue( wxString::Format("%d", t.m_age));
		d.m_specialty->SetValue(t.m_specialty);
		d.m_memo->SetValue(t.m_memo);
		d.showClass();

		// Prevent crazy input!
		d.m_name->SetEditable(false);
		d.m_signonDate->Disable();
		d.m_signoff->Disable();
		d.m_signoffDate->Disable();

		if (wxID_OK == d.ShowModal()) 
		{
			t.m_phone = d.m_phone->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
			t.m_phone2 = d.m_phone2->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
			t.m_address = d.m_address->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
			t.m_birthDate = d.m_birthDate->GetValue();
			t.m_age = StringToUInt(d.m_age->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT));
			t.m_specialty = d.m_specialty->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
			t.m_memo = d.m_memo->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);

			t.updateByName(studentName);
		}
	}
}


void ClassDlg::OnListStudentSelected( wxListEvent& event )
{
	long item = event.GetIndex();
	wxListCtrl *pLc = m_lStudentList;
	int col = 0;
	m_studentName->SetValue(pLc->GetItemText(item, col++));
	m_classFee->SetValue(pLc->GetItemText(item, col++));
	if (m_leave->IsShown()) m_leave->SetValue(pLc->GetItemText(item, col++) != CLASS_STUDENT_LEAVE_STR[0]);
	m_studentMemo->SetValue(pLc->GetItemText(item, col++));
}


void ClassDlg::SetClassValue(const RegularClassDataObj& c)
{
	m_classWeekday->SetSelection(c.m_classWeekday);
	m_classNumber->SetSelection(c.m_classNumber);
	m_teacherName->SetValue(c.m_teacherName);
	m_className->SetValue(c.m_className);
	m_practice->SetSelection(c.m_practice);
	m_oneShot->SetSelection(0);
	m_classMemo->SetValue(c.m_memo);

	wxListCtrl *pLc = m_lStudentList;
	pLc->DeleteAllItems();
	wxVector<RegularClassDataObj::Student>::const_iterator iter = c.m_studentVector.begin();
	for (int row = 0; iter != c.m_studentVector.end(); ++iter, ++row)
	{
		const RegularClassDataObj::Student& stud = *iter;
		int col = 0;
		pLc->InsertItem(row, wxEmptyString);
		pLc->SetItem(row, col, stud.m_studentName); col++;
		pLc->SetItem(row, col, wxString::Format("%d", stud.m_classFee)); col++;
		pLc->SetItem(row, col, stud.m_memo); col++;
	}
}


void ClassDlg::GetClassValue(RegularClassDataObj *pc)
{
	pc->m_classNumber = m_classNumber->GetSelection();
	pc->m_classWeekday = m_classWeekday->GetSelection();
	pc->m_teacherName = m_teacherName->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
	pc->m_className = m_className->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
	pc->m_practice = m_practice->GetSelection();
	pc->m_memo = m_classMemo->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
	pc->m_studentVector.clear();

	wxListCtrl *pLc = m_lStudentList;
	long item = -1;
	while (-1 != (item = pLc->GetNextItem(item))) 
	{
		RegularClassDataObj::Student stud;
		int col = 0;
		stud.m_studentName = pLc->GetItemText(item, col++);
		stud.m_classFee = StringToUInt(pLc->GetItemText(item, col++));
		stud.m_memo = pLc->GetItemText(item, col++);

		pc->m_studentVector.push_back(stud);
	}
}


void ClassDlg::SetClassValue(const CalendarClassDataObj& c)
{
	m_classDate->SetValue(c.m_classDate);

	// 2012-8-12 to add read-only mode when confirmed by admin 
	if (IsLockCalendarClass(c.m_classDate))
	{
		m_staticText14->Hide();
		m_deleteClass->Hide();
		m_button6->Hide();
		m_button7->Hide();
		m_buttonOk->Hide();
		m_buttonCancel->Hide();
		m_staticline1->Hide();
	}
	
	m_classWeekday->SetSelection(c.m_classDate.GetWeekDay());
	m_classNumber->SetSelection(c.m_classNumber);
	m_teacherName->SetValue(c.m_teacherName);
	m_className->SetValue(c.m_className);
	m_practice->SetSelection(c.m_practice);
	m_oneShot->SetSelection(c.m_oneShot);
	m_classMemo->SetValue(c.m_memo);
	if (c.m_memo == CLASS_MEMOERROR_STR)	m_classMemo->SetForegroundColour(*wxRED);
	else m_classMemo->SetForegroundColour(*wxBLACK);

	wxListCtrl *pLc = m_lStudentList;
	pLc->DeleteAllItems();
	wxVector<CalendarClassDataObj::Student>::const_iterator iter = c.m_studentVector.begin();
	for (int row = 0; iter != c.m_studentVector.end(); ++iter, ++row)
	{
		const CalendarClassDataObj::Student& stud = *iter;
		int col = 0;
		pLc->InsertItem(row, wxEmptyString);
		pLc->SetItem(row, col, stud.m_studentName); col++;
		pLc->SetItem(row, col, wxString::Format("%d", stud.m_classFee)); col++;
		if (stud.m_leave) pLc->SetItemTextColour(row, *wxRED);
		else pLc->SetItemTextColour(row, *wxBLACK);
		pLc->SetItem(row, col, CLASS_STUDENT_LEAVE_STR[stud.m_leave]); col++;
		pLc->SetItem(row, col, stud.m_memo); col++;
	}
}


void ClassDlg::GetClassValue(CalendarClassDataObj *pc)
{
	pc->m_classDate = m_classDate->GetValue();
	pc->m_classNumber = m_classNumber->GetSelection();
	pc->m_teacherName = m_teacherName->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
	pc->m_className = m_className->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
	pc->m_practice = m_practice->GetSelection();
	pc->m_oneShot = m_oneShot->GetSelection();
	pc->m_memo = m_classMemo->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
	pc->m_studentVector.clear();

	wxListCtrl *pLc = m_lStudentList;
	long item = -1;
	while (-1 != (item = pLc->GetNextItem(item))) {
		CalendarClassDataObj::Student stud;
		stud.m_studentName = pLc->GetItemText(item, 0);
		stud.m_leave = pLc->GetItemText(item, 2) == CLASS_STUDENT_LEAVE_STR[0]? 0 : 1;
		stud.m_classFee = StringToUInt(pLc->GetItemText(item, 1));
		stud.m_memo = pLc->GetItemText(item, 3);

		pc->m_studentVector.push_back(stud);
	}
}

