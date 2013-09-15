///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "TeacherDlgGui.h"

///////////////////////////////////////////////////////////////////////////

TeacherDlgGui::TeacherDlgGui( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button1 = new wxButton( this, wxID_ANY, wxT("新增教师"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_button1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_button7 = new wxButton( this, wxID_ANY, wxT("删除教师"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_button7, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	mainSizer->Add( bSizer5, 0, wxALIGN_RIGHT, 5 );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	m_listCtrl1 = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL );
	m_listCtrl1->SetFont( wxFont( 12, 70, 90, 90, false, wxEmptyString ) );
	
	mainSizer->Add( m_listCtrl1, 1, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( mainSizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TeacherDlgGui::OnClickAddTeacher ), NULL, this );
	m_button7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TeacherDlgGui::OnClickDelTeacher ), NULL, this );
	m_listCtrl1->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( TeacherDlgGui::OnListItemActivated ), NULL, this );
}

TeacherDlgGui::~TeacherDlgGui()
{
	// Disconnect Events
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TeacherDlgGui::OnClickAddTeacher ), NULL, this );
	m_button7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TeacherDlgGui::OnClickDelTeacher ), NULL, this );
	m_listCtrl1->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( TeacherDlgGui::OnListItemActivated ), NULL, this );
	
}

TeacherEditDlgGui::TeacherEditDlgGui( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("姓名"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer2->Add( m_staticText1, 0, wxALL, 5 );
	
	m_name = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
	m_name->SetMaxLength( 10 ); 
	fgSizer2->Add( m_name, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("入职日期"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer2->Add( m_staticText2, 0, wxALL, 5 );
	
	m_onboardDate = new wxDatePickerCtrl( this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_ALLOWNONE|wxDP_DROPDOWN );
	fgSizer2->Add( m_onboardDate, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("目前状态"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer2->Add( m_staticText3, 0, wxALL, 5 );
	
	wxString m_signoffChoices[] = { wxT("正常"), wxT("离职") };
	int m_signoffNChoices = sizeof( m_signoffChoices ) / sizeof( wxString );
	m_signoff = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_signoffNChoices, m_signoffChoices, 0 );
	m_signoff->SetSelection( 0 );
	fgSizer2->Add( m_signoff, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("离职日期"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer2->Add( m_staticText4, 0, wxALL, 5 );
	
	m_signoffDate = new wxDatePickerCtrl( this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_ALLOWNONE|wxDP_DROPDOWN );
	fgSizer2->Add( m_signoffDate, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("级别"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer2->Add( m_staticText5, 0, wxALL, 5 );
	
	wxString m_rankChoices[] = { wxT("初级"), wxT("高级"), wxT("资深A"), wxT("资深B"), wxT("教授") };
	int m_rankNChoices = sizeof( m_rankChoices ) / sizeof( wxString );
	m_rank = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_rankNChoices, m_rankChoices, 0 );
	m_rank->SetSelection( 0 );
	fgSizer2->Add( m_rank, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("类型"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer2->Add( m_staticText6, 0, wxALL, 5 );
	
	wxString m_partTimeChoices[] = { wxT("全职"), wxT("兼职") };
	int m_partTimeNChoices = sizeof( m_partTimeChoices ) / sizeof( wxString );
	m_partTime = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_partTimeNChoices, m_partTimeChoices, 0 );
	m_partTime->SetSelection( 0 );
	fgSizer2->Add( m_partTime, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("电话"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer2->Add( m_staticText7, 0, wxALL, 5 );
	
	m_phone = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_phone->SetMaxLength( 20 ); 
	fgSizer2->Add( m_phone, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("排课时间"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer2->Add( m_staticText8, 0, wxALL, 5 );
	
	m_memo = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,100 ), wxTE_MULTILINE|wxTE_NO_VSCROLL );
	m_memo->SetMaxLength( 50 ); 
	fgSizer2->Add( m_memo, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer5->Add( fgSizer2, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	m_gClass = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	m_gClass->CreateGrid( 16, 7 );
	m_gClass->EnableEditing( false );
	m_gClass->EnableGridLines( true );
	m_gClass->EnableDragGridSize( false );
	m_gClass->SetMargins( 0, 0 );
	
	// Columns
	m_gClass->SetColSize( 0, 47 );
	m_gClass->SetColSize( 1, 47 );
	m_gClass->SetColSize( 2, 47 );
	m_gClass->SetColSize( 3, 47 );
	m_gClass->SetColSize( 4, 47 );
	m_gClass->SetColSize( 5, 47 );
	m_gClass->SetColSize( 6, 47 );
	m_gClass->EnableDragColMove( false );
	m_gClass->EnableDragColSize( false );
	m_gClass->SetColLabelSize( 30 );
	m_gClass->SetColLabelValue( 0, wxT("日") );
	m_gClass->SetColLabelValue( 1, wxT("一") );
	m_gClass->SetColLabelValue( 2, wxT("二") );
	m_gClass->SetColLabelValue( 3, wxT("三") );
	m_gClass->SetColLabelValue( 4, wxT("四") );
	m_gClass->SetColLabelValue( 5, wxT("五") );
	m_gClass->SetColLabelValue( 6, wxT("六") );
	m_gClass->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gClass->SetRowSize( 0, 26 );
	m_gClass->SetRowSize( 1, 26 );
	m_gClass->SetRowSize( 2, 26 );
	m_gClass->SetRowSize( 3, 26 );
	m_gClass->SetRowSize( 4, 26 );
	m_gClass->SetRowSize( 5, 26 );
	m_gClass->SetRowSize( 6, 26 );
	m_gClass->SetRowSize( 7, 26 );
	m_gClass->SetRowSize( 8, 26 );
	m_gClass->SetRowSize( 9, 26 );
	m_gClass->SetRowSize( 10, 26 );
	m_gClass->SetRowSize( 11, 26 );
	m_gClass->SetRowSize( 12, 26 );
	m_gClass->SetRowSize( 13, 26 );
	m_gClass->SetRowSize( 14, 26 );
	m_gClass->SetRowSize( 15, 26 );
	m_gClass->EnableDragRowSize( false );
	m_gClass->SetRowLabelSize( 45 );
	m_gClass->SetRowLabelValue( 0, wxT("9:00") );
	m_gClass->SetRowLabelValue( 1, wxT("9:45") );
	m_gClass->SetRowLabelValue( 2, wxT("10:30") );
	m_gClass->SetRowLabelValue( 3, wxT("11:15") );
	m_gClass->SetRowLabelValue( 4, wxT("12:00") );
	m_gClass->SetRowLabelValue( 5, wxT("12:45") );
	m_gClass->SetRowLabelValue( 6, wxT("13:30") );
	m_gClass->SetRowLabelValue( 7, wxT("14:15") );
	m_gClass->SetRowLabelValue( 8, wxT("15:00") );
	m_gClass->SetRowLabelValue( 9, wxT("15:45") );
	m_gClass->SetRowLabelValue( 10, wxT("16:30") );
	m_gClass->SetRowLabelValue( 11, wxT("17:15") );
	m_gClass->SetRowLabelValue( 12, wxT("18:00") );
	m_gClass->SetRowLabelValue( 13, wxT("18:45") );
	m_gClass->SetRowLabelValue( 14, wxT("19:30") );
	m_gClass->SetRowLabelValue( 15, wxT("20:15") );
	m_gClass->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gClass->SetDefaultCellFont( wxFont( 9, 70, 90, 90, false, wxEmptyString ) );
	m_gClass->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	bSizer5->Add( m_gClass, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer3->Add( bSizer5, 1, wxEXPAND, 5 );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer3->Add( m_staticline2, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button5 = new wxButton( this, wxID_OK, wxT("确定"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button5, 0, wxALL, 5 );
	
	m_button6 = new wxButton( this, wxID_CANCEL, wxT("取消"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button6, 0, wxALL, 5 );
	
	
	bSizer3->Add( bSizer4, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	this->SetSizer( bSizer3 );
	this->Layout();
	bSizer3->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_gClass->Connect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( TeacherEditDlgGui::OnClass ), NULL, this );
}

TeacherEditDlgGui::~TeacherEditDlgGui()
{
	// Disconnect Events
	m_gClass->Disconnect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( TeacherEditDlgGui::OnClass ), NULL, this );
	
}
