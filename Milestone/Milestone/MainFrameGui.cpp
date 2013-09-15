///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "MainFrameGui.h"

///////////////////////////////////////////////////////////////////////////

MainFrameGui::MainFrameGui( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_menubar1 = new wxMenuBar( 0 );
	m_menu1 = new wxMenu();
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("登录...(&L)") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem1 );
	
	wxMenuItem* m_menuItem2;
	m_menuItem2 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("修改密码...(&P)") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem2 );
	
	wxMenuItem* m_menuItem3;
	m_menuItem3 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("配置管理员...(&A)") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem3 );
	
	wxMenuItem* m_menuItem4;
	m_menuItem4 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("查看日志...(&M)") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem4 );
	
	wxMenuItem* m_menuItem5;
	m_menuItem5 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("数据审核...(&D)") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem5 );
	
	m_menubar1->Append( m_menu1, wxT("高级功能(&F)") ); 
	
	m_menu2 = new wxMenu();
	wxMenuItem* m_menuItem6;
	m_menuItem6 = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("只显示有课的教师") ) + wxT('\t') + wxT("Ctrl+T"), wxEmptyString, wxITEM_CHECK );
	m_menu2->Append( m_menuItem6 );
	
	m_menubar1->Append( m_menu2, wxT("选项设置(&O)") ); 
	
	this->SetMenuBar( m_menubar1 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_calendar1 = new wxCalendarCtrl( m_panel1, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxCAL_SHOW_HOLIDAYS );
	bSizer5->Add( m_calendar1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	bSizer31->Add( bSizer5, 0, wxEXPAND, 5 );
	
	m_staticline2 = new wxStaticLine( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer31->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button1 = new wxButton( m_panel1, wxID_ANY, wxT("学员管理"), wxDefaultPosition, wxSize( 72,72 ), 0 );
	bSizer6->Add( m_button1, 0, wxALL, 5 );
	
	m_button2 = new wxButton( m_panel1, wxID_ANY, wxT("教师管理"), wxDefaultPosition, wxSize( 72,72 ), 0 );
	bSizer6->Add( m_button2, 0, wxALL, 5 );
	
	m_button4 = new wxButton( m_panel1, wxID_ANY, wxT("闹钟提醒"), wxDefaultPosition, wxSize( 72,72 ), 0 );
	bSizer6->Add( m_button4, 0, wxALL, 5 );
	
	m_button5 = new wxButton( m_panel1, wxID_ANY, wxT("月结统计"), wxDefaultPosition, wxSize( 72,72 ), 0 );
	bSizer6->Add( m_button5, 0, wxALL, 5 );
	
	m_button6 = new wxButton( m_panel1, wxID_ANY, wxT("学费管理"), wxDefaultPosition, wxSize( 72,72 ), 0 );
	bSizer6->Add( m_button6, 0, wxALL, 5 );
	
	
	bSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button3 = new wxToggleButton( m_panel1, wxID_ANY, wxT("确认课表"), wxDefaultPosition, wxSize( 72,72 ), 0 );
	bSizer6->Add( m_button3, 0, wxALL, 5 );
	
	
	bSizer7->Add( bSizer6, 0, wxEXPAND, 5 );
	
	
	bSizer7->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText3 = new wxStaticText( m_panel1, wxID_ANY, wxT("今日"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer1->Add( m_staticText3, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	m_staticTextToday = new wxStaticText( m_panel1, wxID_ANY, wxT("0000-00-00"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextToday->Wrap( -1 );
	m_staticTextToday->SetForegroundColour( wxColour( 255, 0, 0 ) );
	
	fgSizer1->Add( m_staticTextToday, 0, wxALL, 5 );
	
	m_staticTextThisday = new wxStaticText( m_panel1, wxID_ANY, wxT("星期一"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextThisday->Wrap( -1 );
	m_staticTextThisday->SetForegroundColour( wxColour( 255, 0, 0 ) );
	
	fgSizer1->Add( m_staticTextThisday, 0, wxALL, 5 );
	
	m_staticText9 = new wxStaticText( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer1->Add( m_staticText9, 0, wxALL, 5 );
	
	m_staticText5 = new wxStaticText( m_panel1, wxID_ANY, wxT("正在显示"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer1->Add( m_staticText5, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_staticTextCalendar = new wxStaticText( m_panel1, wxID_ANY, wxT("0000-00-00"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextCalendar->Wrap( -1 );
	m_staticTextCalendar->SetForegroundColour( wxColour( 0, 0, 255 ) );
	
	fgSizer1->Add( m_staticTextCalendar, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_staticTextWeekday = new wxStaticText( m_panel1, wxID_ANY, wxT("星期一"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextWeekday->Wrap( -1 );
	m_staticTextWeekday->SetForegroundColour( wxColour( 0, 0, 255 ) );
	
	fgSizer1->Add( m_staticTextWeekday, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_staticText7 = new wxStaticText( m_panel1, wxID_ANY, wxT("的课表"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer1->Add( m_staticText7, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer7->Add( fgSizer1, 0, 0, 5 );
	
	
	bSizer31->Add( bSizer7, 1, wxEXPAND, 5 );
	
	
	bSizer2->Add( bSizer31, 0, wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer2->Add( m_staticline1, 0, wxALL|wxEXPAND, 5 );
	
	m_gCClass = new wxGrid( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	m_gCClass->CreateGrid( 17, 12 );
	m_gCClass->EnableEditing( false );
	m_gCClass->EnableGridLines( true );
	m_gCClass->EnableDragGridSize( false );
	m_gCClass->SetMargins( 0, 0 );
	
	// Columns
	m_gCClass->EnableDragColMove( false );
	m_gCClass->EnableDragColSize( false );
	m_gCClass->SetColLabelSize( 30 );
	m_gCClass->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gCClass->SetRowSize( 0, 27 );
	m_gCClass->SetRowSize( 1, 27 );
	m_gCClass->SetRowSize( 2, 27 );
	m_gCClass->SetRowSize( 3, 27 );
	m_gCClass->SetRowSize( 4, 27 );
	m_gCClass->SetRowSize( 5, 27 );
	m_gCClass->SetRowSize( 6, 27 );
	m_gCClass->SetRowSize( 7, 27 );
	m_gCClass->SetRowSize( 8, 27 );
	m_gCClass->SetRowSize( 9, 27 );
	m_gCClass->SetRowSize( 10, 27 );
	m_gCClass->SetRowSize( 11, 27 );
	m_gCClass->SetRowSize( 12, 27 );
	m_gCClass->SetRowSize( 13, 27 );
	m_gCClass->SetRowSize( 14, 27 );
	m_gCClass->SetRowSize( 15, 27 );
	m_gCClass->SetRowSize( 16, 17 );
	m_gCClass->EnableDragRowSize( false );
	m_gCClass->SetRowLabelSize( 80 );
	m_gCClass->SetRowLabelValue( 0, wxT("9:00") );
	m_gCClass->SetRowLabelValue( 1, wxT("9:45") );
	m_gCClass->SetRowLabelValue( 2, wxT("10:30") );
	m_gCClass->SetRowLabelValue( 3, wxT("11:15") );
	m_gCClass->SetRowLabelValue( 4, wxT("12:00") );
	m_gCClass->SetRowLabelValue( 5, wxT("12:45") );
	m_gCClass->SetRowLabelValue( 6, wxT("13:30") );
	m_gCClass->SetRowLabelValue( 7, wxT("14:15") );
	m_gCClass->SetRowLabelValue( 8, wxT("15:00") );
	m_gCClass->SetRowLabelValue( 9, wxT("15:45") );
	m_gCClass->SetRowLabelValue( 10, wxT("16:30") );
	m_gCClass->SetRowLabelValue( 11, wxT("17:15") );
	m_gCClass->SetRowLabelValue( 12, wxT("18:00") );
	m_gCClass->SetRowLabelValue( 13, wxT("18:45") );
	m_gCClass->SetRowLabelValue( 14, wxT("19:30") );
	m_gCClass->SetRowLabelValue( 15, wxT("20:15") );
	m_gCClass->SetRowLabelValue( 16, wxT("-") );
	m_gCClass->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gCClass->SetDefaultCellFont( wxFont( 9, 70, 90, 90, false, wxEmptyString ) );
	m_gCClass->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	bSizer2->Add( m_gCClass, 0, wxALL, 5 );
	
	
	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();
	bSizer2->Fit( m_panel1 );
	bSizer3->Add( m_panel1, 1, wxEXPAND | wxALL, 0 );
	
	
	this->SetSizer( bSizer3 );
	this->Layout();
	
	// Connect Events
	this->Connect( m_menuItem3->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameGui::OnMenuAdmin ) );
	m_calendar1->Connect( wxEVT_CALENDAR_SEL_CHANGED, wxCalendarEventHandler( MainFrameGui::OnCalendarSelChanged ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameGui::OnClickStudent ), NULL, this );
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameGui::OnClickTeacher ), NULL, this );
	m_button4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameGui::OnClickRainlendar ), NULL, this );
	m_button5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameGui::OnClickStat ), NULL, this );
	m_button3->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( MainFrameGui::OnClickConfirm ), NULL, this );
	m_gCClass->Connect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( MainFrameGui::OnCalendarClass ), NULL, this );
}

MainFrameGui::~MainFrameGui()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameGui::OnMenuAdmin ) );
	m_calendar1->Disconnect( wxEVT_CALENDAR_SEL_CHANGED, wxCalendarEventHandler( MainFrameGui::OnCalendarSelChanged ), NULL, this );
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameGui::OnClickStudent ), NULL, this );
	m_button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameGui::OnClickTeacher ), NULL, this );
	m_button4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameGui::OnClickRainlendar ), NULL, this );
	m_button5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameGui::OnClickStat ), NULL, this );
	m_button3->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( MainFrameGui::OnClickConfirm ), NULL, this );
	m_gCClass->Disconnect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( MainFrameGui::OnCalendarClass ), NULL, this );
	
}
