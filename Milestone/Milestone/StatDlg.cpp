#include "stdafx.h"
#include "StatDlg.h"
#include "SqlLite.h"
#include "Util.h"


StatDlg::StatDlg( wxWindow* parent )
:
StatDlgGui( parent )
{
	wxDateTime startMonth = wxDateTime::Today()-wxDateSpan::Month();
	m_statMonth->SetSelection(startMonth.GetMonth());
	
	wxDateTime dateFrom(1, startMonth.GetMonth(), startMonth.GetYear());
	wxDateTime dateTo = dateFrom + wxDateSpan::Month() - wxDateSpan::Day();
	m_dateFrom->SetValue(dateFrom);
	m_dateTo->SetValue(dateTo);	
	
	wxListCtrl *pLc = m_listCtrl1;
	pLc->AppendColumn(L"教师姓名");
	pLc->AppendColumn(L"收费标准");
	pLc->AppendColumn(L"课时类型");
	pLc->AppendColumn(L"课时（节）");
}


void StatDlg::OnChoiceType( wxCommandEvent& event )
{
	wxListCtrl *pLc = m_listCtrl1;
	pLc->DeleteAllItems();
	m_staticTextTotal->SetLabelText("");
	
	pLc->DeleteAllColumns();
	if (event.GetSelection() == 0)
	{
		pLc->AppendColumn(L"教师姓名");
		pLc->AppendColumn(L"收费标准");
		pLc->AppendColumn(L"课时类型");
		pLc->AppendColumn(L"课时（节）");		
	}
	else
	{
		pLc->AppendColumn(L"学生姓名");
		pLc->AppendColumn(L"收费标准");
		pLc->AppendColumn(L"课时类型");
		pLc->AppendColumn(L"课时（节）");		
	}
}


void StatDlg::OnChoiceMonth( wxCommandEvent& event )
{
	wxDateTime startMonth(wxDateTime::Today().GetDay(), (wxDateTime::Month)event.GetSelection(), wxDateTime::Today().GetYear(), 0, 0, 0, 0);
	
	wxDateTime dateFrom(1, startMonth.GetMonth(), startMonth.GetYear());
	wxDateTime dateTo = dateFrom + wxDateSpan::Month() - wxDateSpan::Day();
	m_dateFrom->SetValue(dateFrom);
	m_dateTo->SetValue(dateTo);	
}


void StatDlg::OnClickStat( wxCommandEvent& )
{
	wxListCtrl *pLc = m_listCtrl1;
	pLc->DeleteAllItems();
	m_staticTextTotal->SetLabelText("");

	wxString sql;
	if (m_statType->GetSelection() == 0)
	{
		sql = "select teacher_name,class_fee,practice,count(*) from (				\
			  		select class_date,class_number,teacher_name,practice,class_fee	\
			  		from v_calendar_class where leave=0 and class_name=''			\
					and class_date >= :dateFrom and class_date <= :dateTo			\
			  		group by class_date,class_number,teacher_name having count(*)=1	\
			  	) group by teacher_name,class_fee,practice							\
			    order by teacher_name collate pinyin,class_fee desc,practice		"
			  ;
	}
	else
	{
		sql = "select student_name,class_fee,practice,count(*) from (				\
			  		select class_date,class_number,student_name,practice,class_fee	\
			    	from v_calendar_class where leave=0 and class_name=''			\
					and class_date >= :dateFrom and class_date <= :dateTo			\
			  		group by class_date,class_number,teacher_name having count(*)=1	\
			   ) group by student_name,class_fee,practice							\
			    order by student_name collate pinyin,class_fee desc,practice		"
			  ;
	}
	SqlLiteStatement st(sql);
	SqlLiteBind(st, "dateFrom", m_dateFrom->GetValue());
	SqlLiteBind(st, "dateTo", m_dateTo->GetValue());
	bool paintColor = false;
	int totalCount = 0;
	for (int row = 0; SQLITE_ROW == sqlite3_step(st); ++row)
	{
		int col = LEFTMOST_COLUMN;
		wxString name = SqlLiteColumnStr(st, col++);
		int fee = SqlLiteColumnInt(st, col++);
		int practice = SqlLiteColumnInt(st, col++);
		int count = SqlLiteColumnInt(st, col++);

		totalCount += count;

		pLc->InsertItem(row, wxEmptyString);
		
		// 同名记录设置同样颜色
		if (row > 0)
		{
			wxString lastRowName = pLc->GetItemText(row-1);
			if (lastRowName != name) 
			{
				paintColor = !paintColor;
				if (paintColor)	pLc->SetItemBackgroundColour(row, wxColour(241,241,241));
			}
			else pLc->SetItemBackgroundColour(row, pLc->GetItemBackgroundColour(row-1));
		}

		col = 0;
		pLc->SetItem(row, col, name); col++;
		pLc->SetItem(row, col, wxString::Format("%d", fee)); col++;
		pLc->SetItem(row, col, CLASS_PRACTICE_STR[practice]); col++;
		pLc->SetItem(row, col, wxString::Format("%d", count)); col++;
	}
	m_staticTextTotal->SetLabelText(wxString::Format("%d", totalCount));
}
