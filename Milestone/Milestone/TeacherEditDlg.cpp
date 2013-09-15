#include "stdafx.h"
#include "TeacherEditDlg.h"
#include "Util.h"
#include "SqlLite.h"
#include "ClassDlg.h"
#include "RegularClassDataObj.h"


TeacherEditDlg::TeacherEditDlg( wxWindow* parent, const wxString& teacherName )
:
TeacherEditDlgGui( parent ), _teacherName(teacherName)
{
	if (_teacherName.empty())
	{
		SetTitle(L"教师信息");
		SetSize(GetSize().GetWidth() - m_gClass->GetSize().GetWidth(), GetSize().GetHeight());
	}
}


void TeacherEditDlg::OnClass( wxGridEvent& event )
{
	wxASSERT(!_teacherName.empty());

	int row = event.GetRow();
	int col = event.GetCol();
	m_gClass->GoToCell(row, col);
	m_gClass->SelectBlock(row, col, row, col);

	RegularClassDataObj c;
	c.m_classNumber = row;
	c.m_classWeekday = col;
	c.m_teacherName = _teacherName;
	c.m_practice = 0;
	bool exist = (OP_SUCCESS == c.loadByPrimaryKey(row, col, _teacherName));

	ClassDlg d(this, ClassDlg::REGULAR);
	d.SetClassValue(c);
	// Prevent crazy input!
	d.m_classNumber->Disable();
	d.m_classWeekday->Disable();
	d.m_teacherName->SetEditable(false);
	d.m_deleteClass->Enable(exist);

	if (wxID_OK == d.ShowModal())
	{
		if (d.m_deleteClass->GetValue())
		{
			RegularClassDataObj::deleteByPrimaryKey(row, col, _teacherName);
		}
		else
		{
			d.GetClassValue(&c);
			if (exist) c.updateByPrimaryKey(row, col, _teacherName);
			else c.insertThisObj();
		}
		showClass();
		m_gClass->GoToCell(row, col);
		m_gClass->SelectBlock(row, col, row, col);
	}
}


void TeacherEditDlg::showClass()
{
	wxString teacherName = m_name->GetValue().Trim(FROM_LEFT).Trim(FROM_RIGHT);
	if (teacherName.IsEmpty())
		return;

	m_gClass->ClearGrid();
	m_gClass->ClearSelection();

	SqlLiteStatement st("SELECT * FROM TBL_REGULAR_CLASS WHERE TEACHER_NAME = :teacherName");
	SqlLiteBind(st, "teacherName", teacherName);
	for (int row = 0; SQLITE_ROW == sqlite3_step(st); ++row)
	{
		int col = LEFTMOST_COLUMN;
		int classNumber = SqlLiteColumnInt(st, col++);
		int classWeekday = SqlLiteColumnInt(st, col++);
		col++;
		wxString className = SqlLiteColumnStr(st, col++);
		int practice = SqlLiteColumnInt(st, col++);
		wxString classMemo = SqlLiteColumnStr(st, col++);
		
		wxVector<wxString> studentNameVector;
		SqlLiteStatement st("SELECT STUDENT_NAME FROM TBL_REGULAR_CLASS_STUDENT WHERE \
							CLASS_NUMBER = :classNumber AND \
							CLASS_WEEKDAY = :classWeekday AND \
							TEACHER_NAME = :teacherName");
		SqlLiteBind(st, "classNumber", classNumber);
		SqlLiteBind(st, "classWeekday", classWeekday);
		SqlLiteBind(st, "teacherName", teacherName);
		for (int row = 0; SQLITE_ROW == sqlite3_step(st); ++row)
		{
			wxString studentName = SqlLiteColumnStr(st, LEFTMOST_COLUMN);
			studentNameVector.push_back(studentName);
		}
		wxASSERT(!studentNameVector.empty());

		wxString cell;
		if (studentNameVector.size() == 1)
		{
			cell += studentNameVector[0];
			cell += CLASS_PRACTICE_STR[practice];
		}
		else
		{
			cell += className;
			cell += CLASS_PRACTICE_STR[practice];
		}

		// 第二行显示课时详细信息
		if (!classMemo.IsEmpty())
		{
			cell += L"\n";
			cell += classMemo;
		}	

		m_gClass->SetCellValue(classNumber, classWeekday, cell);
	}	
}
