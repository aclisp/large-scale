///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __ADMINDLGGUI_H__
#define __ADMINDLGGUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/listctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class AdminDlgGui
///////////////////////////////////////////////////////////////////////////////
class AdminDlgGui : public wxDialog 
{
	private:
	
	protected:
		wxListCtrl* m_listCtrl1;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_adminName;
		wxStaticText* m_staticText2;
		wxChoice* m_adminType;
		wxStaticText* m_staticText3;
		wxTextCtrl* m_password;
		wxStaticText* m_staticText4;
		wxTextCtrl* m_phone;
		wxStaticText* m_staticText5;
		wxTextCtrl* m_memo;
		wxStaticLine* m_staticline1;
		wxButton* m_buttonAdd;
		wxButton* m_buttonDel;
		wxButton* m_buttonUpdate;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnButtonClickAdd( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickDel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickUpdate( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		AdminDlgGui( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("配置管理员"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 700,300 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~AdminDlgGui();
	
};

#endif //__ADMINDLGGUI_H__
