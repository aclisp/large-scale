///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __STATDLGGUI_H__
#define __STATDLGGUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/choice.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/listctrl.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class StatDlgGui
///////////////////////////////////////////////////////////////////////////////
class StatDlgGui : public wxDialog 
{
	private:
	
	protected:
		wxChoice* m_statType;
		wxChoice* m_statMonth;
		wxStaticText* m_staticText1;
		wxDatePickerCtrl* m_dateFrom;
		wxStaticText* m_staticText2;
		wxDatePickerCtrl* m_dateTo;
		wxButton* m_button1;
		wxStaticLine* m_staticline1;
		wxListCtrl* m_listCtrl1;
		wxStaticText* m_staticText3;
		wxStaticText* m_staticTextTotal;
		wxStaticText* m_staticText5;
		wxStaticText* m_staticText6;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnChoiceType( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnChoiceMonth( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClickStat( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		StatDlgGui( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("月结统计"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 640,640 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~StatDlgGui();
	
};

#endif //__STATDLGGUI_H__
