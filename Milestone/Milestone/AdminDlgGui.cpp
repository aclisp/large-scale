///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "AdminDlgGui.h"

///////////////////////////////////////////////////////////////////////////

AdminDlgGui::AdminDlgGui( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_listCtrl1 = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( 500,-1 ), wxLC_REPORT|wxLC_SINGLE_SEL );
	bSizer3->Add( m_listCtrl1, 1, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("姓名"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALL, 5 );
	
	m_adminName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 130,-1 ), 0 );
	m_adminName->SetMaxLength( 10 ); 
	fgSizer1->Add( m_adminName, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("权限"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer1->Add( m_staticText2, 0, wxALL, 5 );
	
	wxString m_adminTypeChoices[] = { wxT("行政"), wxT("经理"), wxT("总监"), wxT("超级用户") };
	int m_adminTypeNChoices = sizeof( m_adminTypeChoices ) / sizeof( wxString );
	m_adminType = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_adminTypeNChoices, m_adminTypeChoices, 0 );
	m_adminType->SetSelection( 0 );
	fgSizer1->Add( m_adminType, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("密码"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer1->Add( m_staticText3, 0, wxALL, 5 );
	
	m_password = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	m_password->SetMaxLength( 20 ); 
	fgSizer1->Add( m_password, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("电话"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer1->Add( m_staticText4, 0, wxALL, 5 );
	
	m_phone = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_phone->SetMaxLength( 20 ); 
	fgSizer1->Add( m_phone, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("备注"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer1->Add( m_staticText5, 0, wxALL, 5 );
	
	m_memo = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_memo->SetMaxLength( 50 ); 
	fgSizer1->Add( m_memo, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer3->Add( fgSizer1, 0, wxEXPAND, 5 );
	
	
	bSizer1->Add( bSizer3, 1, wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline1, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonAdd = new wxButton( this, wxID_ANY, wxT("新增"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonAdd, 0, wxALL, 5 );
	
	m_buttonDel = new wxButton( this, wxID_ANY, wxT("删除"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonDel, 0, wxALL, 5 );
	
	m_buttonUpdate = new wxButton( this, wxID_ANY, wxT("更新"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonUpdate, 0, wxALL, 5 );
	
	
	bSizer1->Add( bSizer2, 0, wxALIGN_RIGHT, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttonAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AdminDlgGui::OnButtonClickAdd ), NULL, this );
	m_buttonDel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AdminDlgGui::OnButtonClickDel ), NULL, this );
	m_buttonUpdate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AdminDlgGui::OnButtonClickUpdate ), NULL, this );
}

AdminDlgGui::~AdminDlgGui()
{
	// Disconnect Events
	m_buttonAdd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AdminDlgGui::OnButtonClickAdd ), NULL, this );
	m_buttonDel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AdminDlgGui::OnButtonClickDel ), NULL, this );
	m_buttonUpdate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AdminDlgGui::OnButtonClickUpdate ), NULL, this );
	
}
