///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "StatDlgGui.h"

///////////////////////////////////////////////////////////////////////////

StatDlgGui::StatDlgGui( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	wxString m_statTypeChoices[] = { wxT("按上课教师统计"), wxT("按上课学生统计") };
	int m_statTypeNChoices = sizeof( m_statTypeChoices ) / sizeof( wxString );
	m_statType = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_statTypeNChoices, m_statTypeChoices, 0 );
	m_statType->SetSelection( 0 );
	bSizer2->Add( m_statType, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString m_statMonthChoices[] = { wxT("一月"), wxT("二月"), wxT("三月"), wxT("四月"), wxT("五月"), wxT("六月"), wxT("七月"), wxT("八月"), wxT("九月"), wxT("十月"), wxT("十一月"), wxT("十二月") };
	int m_statMonthNChoices = sizeof( m_statMonthChoices ) / sizeof( wxString );
	m_statMonth = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_statMonthNChoices, m_statMonthChoices, 0 );
	m_statMonth->SetSelection( 0 );
	bSizer2->Add( m_statMonth, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("日期从"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer2->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_dateFrom = new wxDatePickerCtrl( this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN );
	bSizer2->Add( m_dateFrom, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("到"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer2->Add( m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_dateTo = new wxDatePickerCtrl( this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN );
	bSizer2->Add( m_dateTo, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_button1 = new wxButton( this, wxID_ANY, wxT("开始统计"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_button1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	mainSizer->Add( bSizer2, 0, wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	m_listCtrl1 = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	mainSizer->Add( m_listCtrl1, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("合计"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer3->Add( m_staticText3, 0, wxALL, 5 );
	
	m_staticTextTotal = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 20,-1 ), 0 );
	m_staticTextTotal->Wrap( -1 );
	bSizer3->Add( m_staticTextTotal, 0, wxALL, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("节课时"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer3->Add( m_staticText5, 0, wxALL, 5 );
	
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("注：集体课时未纳入统计范围"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer3->Add( m_staticText6, 0, wxALL, 5 );
	
	
	mainSizer->Add( bSizer3, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( mainSizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_statType->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( StatDlgGui::OnChoiceType ), NULL, this );
	m_statMonth->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( StatDlgGui::OnChoiceMonth ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StatDlgGui::OnClickStat ), NULL, this );
}

StatDlgGui::~StatDlgGui()
{
	// Disconnect Events
	m_statType->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( StatDlgGui::OnChoiceType ), NULL, this );
	m_statMonth->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( StatDlgGui::OnChoiceMonth ), NULL, this );
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( StatDlgGui::OnClickStat ), NULL, this );
	
}
