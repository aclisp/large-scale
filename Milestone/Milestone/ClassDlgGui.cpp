///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "ClassDlgGui.h"

///////////////////////////////////////////////////////////////////////////

ClassDlgGui::ClassDlgGui( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("日期"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALL, 5 );
	
	m_classDate = new wxDatePickerCtrl( this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxSize( 200,-1 ), wxDP_DROPDOWN );
	fgSizer1->Add( m_classDate, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("星期"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer1->Add( m_staticText2, 0, wxALL, 5 );
	
	wxString m_classWeekdayChoices[] = { wxT("日"), wxT("一"), wxT("二"), wxT("三"), wxT("四"), wxT("五"), wxT("六") };
	int m_classWeekdayNChoices = sizeof( m_classWeekdayChoices ) / sizeof( wxString );
	m_classWeekday = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 200,-1 ), m_classWeekdayNChoices, m_classWeekdayChoices, 0 );
	m_classWeekday->SetSelection( 0 );
	fgSizer1->Add( m_classWeekday, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("时间"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer1->Add( m_staticText3, 0, wxALL, 5 );
	
	wxString m_classNumberChoices[] = { wxT("9:00"), wxT("9:45"), wxT("10:30"), wxT("11:15"), wxT("12:00"), wxT("12:45"), wxT("13:30"), wxT("14:15"), wxT("15:00"), wxT("15:45"), wxT("16:30"), wxT("17:15"), wxT("18:00"), wxT("18:45"), wxT("19:30"), wxT("20:15") };
	int m_classNumberNChoices = sizeof( m_classNumberChoices ) / sizeof( wxString );
	m_classNumber = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_classNumberNChoices, m_classNumberChoices, 0 );
	m_classNumber->SetSelection( 0 );
	fgSizer1->Add( m_classNumber, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("教师"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer1->Add( m_staticText4, 0, wxALL, 5 );
	
	m_teacherName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_teacherName->SetMaxLength( 10 ); 
	fgSizer1->Add( m_teacherName, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("集体课名"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer1->Add( m_staticText5, 0, wxALL, 5 );
	
	m_className = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_className->SetMaxLength( 10 ); 
	fgSizer1->Add( m_className, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("课时类型"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer1->Add( m_staticText6, 0, wxALL, 5 );
	
	wxString m_practiceChoices[] = { wxT("上课（S）"), wxT("陪练（P）") };
	int m_practiceNChoices = sizeof( m_practiceChoices ) / sizeof( wxString );
	m_practice = new wxRadioBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_practiceNChoices, m_practiceChoices, 1, wxRA_SPECIFY_COLS );
	m_practice->SetSelection( 0 );
	fgSizer1->Add( m_practice, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer1->Add( m_staticText7, 0, wxALL, 5 );
	
	wxString m_oneShotChoices[] = { wxT("常规课时"), wxT("临时课时") };
	int m_oneShotNChoices = sizeof( m_oneShotChoices ) / sizeof( wxString );
	m_oneShot = new wxRadioBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_oneShotNChoices, m_oneShotChoices, 1, wxRA_SPECIFY_COLS );
	m_oneShot->SetSelection( 0 );
	fgSizer1->Add( m_oneShot, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText12 = new wxStaticText( this, wxID_ANY, wxT("详细信息"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer1->Add( m_staticText12, 0, wxALL, 5 );
	
	m_classMemo = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,100 ), wxTE_MULTILINE|wxTE_NO_VSCROLL );
	m_classMemo->SetMaxLength( 50 ); 
	fgSizer1->Add( m_classMemo, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText14 = new wxStaticText( this, wxID_ANY, wxT("删除课时"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	fgSizer1->Add( m_staticText14, 0, wxALL, 5 );
	
	m_deleteClass = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_deleteClass, 0, wxALL, 5 );
	
	
	bSizer3->Add( fgSizer1, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, wxT("学生姓名"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer2->Add( m_staticText9, 0, wxALL, 5 );
	
	m_studentName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
	m_studentName->SetMaxLength( 10 ); 
	fgSizer2->Add( m_studentName, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("收费标准"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer2->Add( m_staticText11, 0, wxALL, 5 );
	
	m_classFee = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_classFee->SetMaxLength( 3 ); 
	fgSizer2->Add( m_classFee, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText10 = new wxStaticText( this, wxID_ANY, wxT("学生请假"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	fgSizer2->Add( m_staticText10, 0, wxALL, 5 );
	
	m_leave = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_leave, 0, wxALL|wxEXPAND, 9 );
	
	m_staticText13 = new wxStaticText( this, wxID_ANY, wxT("课时记录"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer2->Add( m_staticText13, 0, wxALL, 5 );
	
	m_studentMemo = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_NO_VSCROLL );
	m_studentMemo->SetMaxLength( 50 ); 
	fgSizer2->Add( m_studentMemo, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer6->Add( fgSizer2, 1, wxEXPAND, 5 );
	
	
	bSizer4->Add( bSizer6, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("学生列表"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer8->Add( m_staticText8, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button6 = new wxButton( this, wxID_ANY, wxT("确认"), wxDefaultPosition, wxSize( 60,-1 ), 0 );
	bSizer8->Add( m_button6, 0, wxALL, 5 );
	
	m_button7 = new wxButton( this, wxID_ANY, wxT("删除"), wxDefaultPosition, wxSize( 60,-1 ), 0 );
	bSizer8->Add( m_button7, 0, wxALL, 5 );
	
	
	bSizer4->Add( bSizer8, 0, wxEXPAND, 5 );
	
	m_lStudentList = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxLC_REPORT|wxLC_SINGLE_SEL );
	bSizer4->Add( m_lStudentList, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer3->Add( bSizer4, 1, wxEXPAND, 5 );
	
	
	mainSizer->Add( bSizer3, 1, wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* buttonSizer;
	buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOk = new wxButton( this, wxID_OK, wxT("确定"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( m_buttonOk, 0, wxALL, 5 );
	
	m_buttonCancel = new wxButton( this, wxID_CANCEL, wxT("取消"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( m_buttonCancel, 0, wxALL, 5 );
	
	
	mainSizer->Add( buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_button6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ClassDlgGui::OnClickAddStudent ), NULL, this );
	m_button7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ClassDlgGui::OnClickDelStudent ), NULL, this );
	m_lStudentList->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( ClassDlgGui::OnListStudentActivated ), NULL, this );
	m_lStudentList->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( ClassDlgGui::OnListStudentSelected ), NULL, this );
}

ClassDlgGui::~ClassDlgGui()
{
	// Disconnect Events
	m_button6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ClassDlgGui::OnClickAddStudent ), NULL, this );
	m_button7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ClassDlgGui::OnClickDelStudent ), NULL, this );
	m_lStudentList->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( ClassDlgGui::OnListStudentActivated ), NULL, this );
	m_lStudentList->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( ClassDlgGui::OnListStudentSelected ), NULL, this );
	
}
