///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __CLASSDLGGUI_H__
#define __CLASSDLGGUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/radiobox.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/listctrl.h>
#include <wx/statline.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class ClassDlgGui
///////////////////////////////////////////////////////////////////////////////
class ClassDlgGui : public wxDialog 
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
		wxStaticText* m_staticText12;
		wxStaticText* m_staticText14;
		wxStaticText* m_staticText9;
		wxTextCtrl* m_studentName;
		wxStaticText* m_staticText11;
		wxTextCtrl* m_classFee;
		wxStaticText* m_staticText10;
		wxCheckBox* m_leave;
		wxStaticText* m_staticText13;
		wxTextCtrl* m_studentMemo;
		wxStaticText* m_staticText8;
		wxButton* m_button6;
		wxButton* m_button7;
		wxStaticLine* m_staticline1;
		wxButton* m_buttonOk;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClickAddStudent( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClickDelStudent( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnListStudentActivated( wxListEvent& event ) { event.Skip(); }
		virtual void OnListStudentSelected( wxListEvent& event ) { event.Skip(); }
		
	
	public:
		wxDatePickerCtrl* m_classDate;
		wxChoice* m_classWeekday;
		wxChoice* m_classNumber;
		wxTextCtrl* m_teacherName;
		wxTextCtrl* m_className;
		wxRadioBox* m_practice;
		wxRadioBox* m_oneShot;
		wxTextCtrl* m_classMemo;
		wxCheckBox* m_deleteClass;
		wxListCtrl* m_lStudentList;
		
		ClassDlgGui( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("课时信息"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~ClassDlgGui();
	
};

#endif //__CLASSDLGGUI_H__
