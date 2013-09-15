///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __STUDENTDLGGUI_H__
#define __STUDENTDLGGUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/srchctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/listctrl.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/choice.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class StudentDlgGui
///////////////////////////////////////////////////////////////////////////////
class StudentDlgGui : public wxDialog 
{
	private:
	
	protected:
		wxSearchCtrl* m_searchCtrl1;
		wxButton* m_button1;
		wxButton* m_button2;
		wxStaticLine* m_staticline1;
		wxListCtrl* m_listCtrl1;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnSearch( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClickAddStudent( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClickDelStudent( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListItemActivated( wxListEvent& event ) { event.Skip(); }
		
	
	public:
		
		StudentDlgGui( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("学员管理"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,640 ), long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER ); 
		~StudentDlgGui();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class StudentEditDlgGui
///////////////////////////////////////////////////////////////////////////////
class StudentEditDlgGui : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxStaticText* m_staticText6;
		wxStaticText* m_staticText2;
		wxStaticText* m_staticText7;
		wxStaticText* m_staticText8;
		wxStaticText* m_staticText9;
		wxStaticText* m_staticText10;
		wxStaticText* m_staticText3;
		wxStaticText* m_staticText4;
		wxStaticText* m_staticText5;
		wxStaticText* m_staticText11;
		wxStaticText* m_staticText12;
		wxListCtrl* m_lClass;
		wxStaticLine* m_staticline2;
		wxButton* m_button3;
		wxButton* m_button4;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClass( wxListEvent& event ) { event.Skip(); }
		
	
	public:
		wxTextCtrl* m_name;
		wxTextCtrl* m_age;
		wxTextCtrl* m_phone;
		wxTextCtrl* m_phone2;
		wxTextCtrl* m_address;
		wxTextCtrl* m_specialty;
		wxDatePickerCtrl* m_birthDate;
		wxDatePickerCtrl* m_signonDate;
		wxChoice* m_signoff;
		wxDatePickerCtrl* m_signoffDate;
		wxTextCtrl* m_memo;
		
		StudentEditDlgGui( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("学员信息"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~StudentEditDlgGui();
	
};

#endif //__STUDENTDLGGUI_H__
