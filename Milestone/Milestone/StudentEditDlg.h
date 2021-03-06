﻿#ifndef __StudentEditDlg__
#define __StudentEditDlg__

/**
@file
Subclass of StudentEditDlgGui, which is generated by wxFormBuilder.
*/

#include "StudentDlgGui.h"

//// end generated include

/** Implementing StudentEditDlgGui */
class StudentEditDlg : public StudentEditDlgGui
{
	protected:
		// Handlers for StudentEditDlgGui events.
		void OnClass( wxListEvent& event );

		const wxString _studentName;

	public:
		/** Constructor */
		StudentEditDlg( wxWindow* parent, const wxString& studentName );
	//// end generated class members
		void showClass();	
};

#endif // __StudentEditDlg__
