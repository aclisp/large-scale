#include "stdafx.h"
#include "TeacherDlg.h"
#include "SqlLite.h"
#include "TeacherDataObj.h"
#include "TeacherEditDlg.h"
#include "Util.h"
#include "MainApp.h"
#include "MainFrame.h"


TeacherDlg::TeacherDlg( wxWindow* parent )
:
TeacherDlgGui( parent )
{
	wxListCtrl *pLc = m_listCtrl1;
	pLc->AppendColumn(L"教师姓名");
	pLc->AppendColumn(L"入职日期");
	pLc->AppendColumn(L"目前状态");
	pLc->AppendColumn(L"离职日期");
	pLc->AppendColumn(L"教师级别");
	pLc->AppendColumn(L"教师类型");
	pLc->AppendColumn(L"联系电话");
	
	query(wxEmptyString);
}


void TeacherDlg::OnClickAddTeacher( wxCommandEvent& )
{
	TeacherEditDlg d(this, wxEmptyString);
	d.m_onboardDate->SetValue(wxDateTime::Today());
	d.m_signoffDate->SetValue(wxInvalidDateTime);
	if (wxID_OK == d.ShowModal())
	{
		TeacherDataObj t;
		t.m_name = d.m_name->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
		t.m_onboardDate = d.m_onboardDate->GetValue();
		t.m_signoff = d.m_signoff->GetSelection();
		t.m_signoffDate = d.m_signoffDate->GetValue();
		t.m_rank = d.m_rank->GetSelection();
		t.m_partTime = d.m_partTime->GetSelection();
		t.m_phone = d.m_phone->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
		t.m_memo = d.m_memo->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
		if (!t.m_name.empty())
		{
			if (OP_SUCCESS == t.insertThisObj())
			{
				query(wxEmptyString);
				((MainFrame *)wxGetApp().GetTopWindow())->updateGrid();
			}
			select(t.m_name);
		}
	}
}


void TeacherDlg::OnClickDelTeacher( wxCommandEvent& )
{
	long item = -1;
	wxListCtrl *pLc = m_listCtrl1;
	
	item = pLc->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (item > -1)
	{
		if (OP_SUCCESS == TeacherDataObj::deleteByName(pLc->GetItemText(item, 0)))
		{
			query(wxEmptyString);
			if (item >= pLc->GetItemCount()) --item;
			pLc->EnsureVisible(item);
			((MainFrame *)wxGetApp().GetTopWindow())->updateGrid();
		}
	}
}


void TeacherDlg::OnListItemActivated( wxListEvent& event )
{
	long item = event.GetIndex();
	wxListCtrl *pLc = m_listCtrl1;
	TeacherDataObj t;
	wxString teacherName = pLc->GetItemText(item);
	if (OP_SUCCESS == t.loadByName(teacherName))
	{
		TeacherEditDlg d(this, teacherName);
		d.m_name->SetValue(t.m_name);
		d.m_onboardDate->SetValue(t.m_onboardDate);
		d.m_signoff->SetSelection(t.m_signoff);
		d.m_signoffDate->SetValue(t.m_signoffDate);
		d.m_rank->SetSelection(t.m_rank);
		d.m_partTime->SetSelection(t.m_partTime);
		d.m_phone->SetValue(t.m_phone);
		d.m_memo->SetValue(t.m_memo);
		d.showClass();
		
		if (wxID_OK == d.ShowModal())
		{
			t.m_name = d.m_name->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
			t.m_onboardDate = d.m_onboardDate->GetValue();
			t.m_signoff = d.m_signoff->GetSelection();
			t.m_signoffDate = d.m_signoffDate->GetValue();
			t.m_rank = d.m_rank->GetSelection();
			t.m_partTime = d.m_partTime->GetSelection();
			t.m_phone = d.m_phone->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
			t.m_memo = d.m_memo->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
			
			if (!t.m_name.empty())
			{
				if (OP_SUCCESS == t.updateByName(teacherName))
				{
					query(wxEmptyString);
				}
				select(t.m_name);
			}
		}
		
		((MainFrame *)wxGetApp().GetTopWindow())->updateGrid();
	}
}


void TeacherDlg::query(const wxString& )
{
	wxListCtrl *pLc = m_listCtrl1;
	pLc->DeleteAllItems();
	SqlLiteStatement st("SELECT * FROM TBL_TEACHER ORDER BY TEACHER_NAME COLLATE pinyin LIMIT 0,100");
	for (int row = 0; SQLITE_ROW == sqlite3_step(st); ++row)
	{
		int col = 0;
		pLc->InsertItem(row, wxEmptyString);
		if (row % 2) pLc->SetItemBackgroundColour(row, wxColor(255, 255, 234));
		pLc->SetItem(row, col, SqlLiteColumnStr(st, col)); col++;
		pLc->SetItem(row, col, SqlLiteColumnStr(st, col)); col++;
		pLc->SetItem(row, col, TeacherDataObj::SIGNOFF_STR[SqlLiteColumnInt(st, col)]); col++;
		pLc->SetItem(row, col, SqlLiteColumnStr(st, col)); col++;
		pLc->SetItem(row, col, TeacherDataObj::RANK_STR[SqlLiteColumnInt(st, col)]); col++;
		pLc->SetItem(row, col, TeacherDataObj::PARTTIME_STR[SqlLiteColumnInt(st, col)]); col++;
		pLc->SetItem(row, col, SqlLiteColumnStr(st, col)); col++;
	}
}


void TeacherDlg::select(const wxString& teacherName)
{
	wxListCtrl *pLc = m_listCtrl1;
	long item = pLc->FindItem(-1, teacherName);
	if (wxNOT_FOUND != item)
	{
		pLc->SetItemState(item, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		pLc->SetItemState(item, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED);
		pLc->EnsureVisible(item);
		pLc->SetFocus();
	}
}

