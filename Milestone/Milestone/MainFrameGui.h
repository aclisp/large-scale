///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __MAINFRAMEGUI_H__
#define __MAINFRAMEGUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/calctrl.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/tglbtn.h>
#include <wx/stattext.h>
#include <wx/grid.h>
#include <wx/panel.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameGui
///////////////////////////////////////////////////////////////////////////////
class MainFrameGui : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menubar1;
		wxMenu* m_menu1;
		wxMenu* m_menu2;
		wxPanel* m_panel1;
		wxCalendarCtrl* m_calendar1;
		wxStaticLine* m_staticline2;
		wxButton* m_button1;
		wxButton* m_button2;
		wxButton* m_button4;
		wxButton* m_button5;
		wxButton* m_button6;
		wxToggleButton* m_button3;
		wxStaticText* m_staticText3;
		wxStaticText* m_staticTextToday;
		wxStaticText* m_staticTextThisday;
		wxStaticText* m_staticText9;
		wxStaticText* m_staticText5;
		wxStaticText* m_staticTextCalendar;
		wxStaticText* m_staticTextWeekday;
		wxStaticText* m_staticText7;
		wxStaticLine* m_staticline1;
		wxGrid* m_gCClass;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMenuAdmin( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCalendarSelChanged( wxCalendarEvent& event ) { event.Skip(); }
		virtual void OnClickStudent( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClickTeacher( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClickRainlendar( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClickStat( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClickConfirm( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCalendarClass( wxGridEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrameGui( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("岭南艺术培训中心 管理系统"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1024,730 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFrameGui();
	
};

#endif //__MAINFRAMEGUI_H__
