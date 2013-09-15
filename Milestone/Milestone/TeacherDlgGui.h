///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __TEACHERDLGGUI_H__
#define __TEACHERDLGGUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/listctrl.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/choice.h>
#include <wx/grid.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class TeacherDlgGui
///////////////////////////////////////////////////////////////////////////////
class TeacherDlgGui : public wxDialog 
{
	private:
	
	protected:
		wxButton* m_button1;
		wxButton* m_button7;
		wxStaticLine* m_staticline2;
		wxListCtrl* m_listCtrl1;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClickAddTeacher( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClickDelTeacher( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListItemActivated( wxListEvent& event ) { event.Skip(); }
		
	
	public:
		
		TeacherDlgGui( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("教师管理"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 640,640 ), long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER ); 
		~TeacherDlgGui();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class TeacherEditDlgGui
///////////////////////////////////////////////////////////////////////////////
class TeacherEditDlgGui : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxStaticText* m_staticText2;
		wxStaticText* m_staticText3;
		wxStaticText* m_staticText4;
		wxStaticText* m_staticText5;
		wxStaticText* m_staticText6;
		wxStaticText* m_staticText7;
		wxStaticText* m_staticText8;
		wxGrid* m_gClass;
		wxStaticLine* m_staticline2;
		wxButton* m_button5;
		wxButton* m_button6;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClass( wxGridEvent& event ) { event.Skip(); }
		
	
	public:
		wxTextCtrl* m_name;
		wxDatePickerCtrl* m_onboardDate;
		wxChoice* m_signoff;
		wxDatePickerCtrl* m_signoffDate;
		wxChoice* m_rank;
		wxChoice* m_partTime;
		wxTextCtrl* m_phone;
		wxTextCtrl* m_memo;
		
		TeacherEditDlgGui( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("教师信息与课时安排"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~TeacherEditDlgGui();
	
};

#endif //__TEACHERDLGGUI_H__
