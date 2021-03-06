﻿#ifndef __StatDlg__
#define __StatDlg__

/**
@file
Subclass of StatDlgGui, which is generated by wxFormBuilder.
*/

#include "StatDlgGui.h"

//// end generated include

/** Implementing StatDlgGui */
class StatDlg : public StatDlgGui
{
	protected:
		// Handlers for StatDlgGui events.
		void OnChoiceType( wxCommandEvent& event );
		void OnChoiceMonth( wxCommandEvent& event );
		void OnClickStat( wxCommandEvent& event );
	public:
		/** Constructor */
		StatDlg( wxWindow* parent );
	//// end generated class members
	
};

#endif // __StatDlg__
