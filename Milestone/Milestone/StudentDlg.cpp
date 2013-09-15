#include "stdafx.h"
#include "StudentDlg.h"
#include "SqlLite.h"
#include "StudentDataObj.h"
#include "Util.h"
#include "StudentEditDlg.h"
#include "MainApp.h"
#include "MainFrame.h"


StudentDlg::StudentDlg( wxWindow* parent )
:
StudentDlgGui( parent )
{
	m_searchCtrl1->SetDescriptiveText(L"输入学员姓名或电话搜索");
	wxListCtrl *pLc = m_listCtrl1;
	pLc->AppendColumn(L"学员名");
	pLc->AppendColumn(L"年龄", wxLIST_FORMAT_LEFT, 45);
	pLc->AppendColumn(L"电话", wxLIST_FORMAT_LEFT, 110);
	pLc->AppendColumn(L"备用电话", wxLIST_FORMAT_LEFT, 110);
	pLc->AppendColumn(L"住址", wxLIST_FORMAT_LEFT, 150);
	pLc->AppendColumn(L"入学日");
	pLc->AppendColumn(L"状态", wxLIST_FORMAT_LEFT, 45);
	pLc->AppendColumn(L"爱好与特长", wxLIST_FORMAT_LEFT, 150);
	pLc->AppendColumn(L"备忘录", wxLIST_FORMAT_LEFT, 300);
	
	query(wxEmptyString);
}


void StudentDlg::OnSearch( wxCommandEvent& event )
{
	query(event.GetString().Trim(FROM_LEFT).Trim(FROM_RIGHT));
}


void StudentDlg::OnClickAddStudent( wxCommandEvent& )
{
	StudentEditDlg d(this, wxEmptyString);
	d.m_signonDate->SetValue(wxDateTime::Today());
	d.m_signoffDate->SetValue(wxInvalidDateTime);
	if (wxID_OK == d.ShowModal())
	{
		StudentDataObj t;
		t.m_name = d.m_name->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
		t.m_signonDate = d.m_signonDate->GetValue();
		t.m_signoff = d.m_signoff->GetSelection();
		t.m_signoffDate = d.m_signoffDate->GetValue();
		t.m_phone = d.m_phone->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
		t.m_phone2 = d.m_phone2->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
		t.m_address = d.m_address->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
		t.m_birthDate = d.m_birthDate->GetValue();
		t.m_age = StringToUInt(d.m_age->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT));
		t.m_specialty = d.m_specialty->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
		t.m_memo = d.m_memo->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
				
		if (!t.m_name.empty())
		{
			if (OP_SUCCESS == t.insertThisObj())
			{
				query(t.m_name);
				((MainFrame *)wxGetApp().GetTopWindow())->updateGrid();
			}
			select(t.m_name);
		}
	}
}


void StudentDlg::OnClickDelStudent( wxCommandEvent& )
{
	long item = -1;
	wxListCtrl *pLc = m_listCtrl1;
	
	item = pLc->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (item > -1)
	{
		if (OP_SUCCESS == StudentDataObj::deleteByName(pLc->GetItemText(item, 0)))
		{
			query(m_searchCtrl1->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT));
			if (item >= pLc->GetItemCount()) --item;
			pLc->EnsureVisible(item);
			((MainFrame *)wxGetApp().GetTopWindow())->updateGrid();
		}
	}
}


void StudentDlg::OnListItemActivated( wxListEvent& event )
{
	long item = event.GetIndex();
	wxListCtrl *pLc = m_listCtrl1;
	StudentDataObj t;
	wxString studentName = pLc->GetItemText(item);
	if (OP_SUCCESS == t.loadByName(studentName))
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
		d.m_age->SetValue(wxString::Format("%d", t.m_age));
		d.m_specialty->SetValue(t.m_specialty);
		d.m_memo->SetValue(t.m_memo);
		d.showClass();
		
		if (wxID_OK == d.ShowModal())
		{
			t.m_name = d.m_name->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
			t.m_signonDate = d.m_signonDate->GetValue();
			t.m_signoff = d.m_signoff->GetSelection();
			t.m_signoffDate = d.m_signoffDate->GetValue();
			t.m_phone = d.m_phone->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
			t.m_phone2 = d.m_phone2->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
			t.m_address = d.m_address->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
			t.m_birthDate = d.m_birthDate->GetValue();
			t.m_age = StringToUInt(d.m_age->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT));
			t.m_specialty = d.m_specialty->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
			t.m_memo = d.m_memo->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
			
			if (!t.m_name.empty())
			{
				if (OP_SUCCESS == t.updateByName(studentName))
				{
					query(m_searchCtrl1->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT));
				}
				select(t.m_name);
			}
		}
		
		((MainFrame *)wxGetApp().GetTopWindow())->updateGrid();
	}
}


void StudentDlg::query(const wxString& nameOrPhone)
{
	wxListCtrl *pLc = m_listCtrl1;
	pLc->DeleteAllItems();
	SqlLiteStatement st("SELECT STUDENT_NAME, \
						AGE, \
						PHONE, \
						PHONE2, \
						ADDRESS, \
						SIGNON_DATE, \
						SIGNOFF, \
						SPECIALTY, \
						MEMO \
						FROM TBL_STUDENT WHERE \
						STUDENT_NAME LIKE :input OR \
						PHONE LIKE :input OR \
						PHONE2 LIKE :input \
						ORDER BY STUDENT_NAME COLLATE pinyin \
						LIMIT 0, 100");
	SqlLiteBind(st, "input", "%"+nameOrPhone+"%");
	for (int row = 0; SQLITE_ROW == sqlite3_step(st); ++row)
	{
		int col = 0;
		pLc->InsertItem(row, wxEmptyString);
		if (row % 2) pLc->SetItemBackgroundColour(row, wxColor(234, 255, 234));
		pLc->SetItem(row, col, SqlLiteColumnStr(st, col)); col++;
		pLc->SetItem(row, col, SqlLiteColumnStr(st, col)); col++;
		pLc->SetItem(row, col, SqlLiteColumnStr(st, col)); col++;
		pLc->SetItem(row, col, SqlLiteColumnStr(st, col)); col++;
		pLc->SetItem(row, col, SqlLiteColumnStr(st, col)); col++;
		pLc->SetItem(row, col, SqlLiteColumnStr(st, col)); col++;
		pLc->SetItem(row, col, StudentDataObj::SIGNOFF_STR[SqlLiteColumnInt(st, col)]); col++;
		pLc->SetItem(row, col, SqlLiteColumnStr(st, col)); col++;
		pLc->SetItem(row, col, SqlLiteColumnStr(st, col)); col++;
	}
}


void StudentDlg::select(const wxString& studentName)
{
	wxListCtrl *pLc = m_listCtrl1;
	long item = pLc->FindItem(-1, studentName);
	if (wxNOT_FOUND != item)
	{
		pLc->SetItemState(item, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		pLc->SetItemState(item, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED);
		pLc->EnsureVisible(item);
		pLc->SetFocus();
	}
}
