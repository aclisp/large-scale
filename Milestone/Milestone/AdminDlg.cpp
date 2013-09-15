#include "stdafx.h"
#include "AdminDlg.h"


AdminDlg::AdminDlg( wxWindow* parent )
:
AdminDlgGui( parent )
{
	wxListCtrl *pLc = m_listCtrl1;
	pLc->AppendColumn(L"管理员姓名");
	pLc->AppendColumn(L"操作权限");
	pLc->AppendColumn(L"管理员密码");
	pLc->AppendColumn(L"登录时间");
	pLc->AppendColumn(L"管理员电话");
	pLc->AppendColumn(L"备注");
}


void AdminDlg::OnButtonClickAdd( wxCommandEvent& event )
{
// TODO: Implement OnButtonClickAdd
}


void AdminDlg::OnButtonClickDel( wxCommandEvent& event )
{
// TODO: Implement OnButtonClickDel
}


void AdminDlg::OnButtonClickUpdate( wxCommandEvent& event )
{
// TODO: Implement OnButtonClickUpdate
}


void AdminDlg::query()
{
}


void AdminDlg::select(const wxString& adminName)
{
}
