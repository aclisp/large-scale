#include "stdafx.h"
#include "MainApp.h"
#include "MainFrame.h"
#include "SqlLite.h"


#include "StudentDataObj.h"


IMPLEMENT_APP(MainApp);


void DbTest()
{
	// SqlLiteStatement st("SELECT * FROM TBL_XXX");
	StudentDataObj stud;
	wxASSERT(OP_SUCCESS == stud.loadByName(L"³ÌêÑä¿"));
	wxMessageBox(stud.m_phone);
	stud.deleteByName(L"ÖÓÎÄÌ©");
}


void Md5Test()
{
	wxLogMessage(Md5Digest(L"»Æå°"));
}


bool MainApp::OnInit()
{
	SqlLiteOpen();

	// TODO: 
	// DbTest();
	// Md5Test();

	MainFrame* pMainFrame = new MainFrame(NULL);
	SetTopWindow(pMainFrame);
	pMainFrame->Show();
	return true;
}


int MainApp::OnExit()
{
	SqlLiteClose();

	return 0;
}


void MainApp::OnUnhandledException()
{
	wxString what;
	try { throw; } catch (std::exception& e) 
	{
		what.Printf("std::exception of type \"%s\", what() = \"%s\"",
                    typeid(e).name(), e.what());
	} catch (...)
	{
		what = "unknown exception";
	}
	wxMessageOutputBest().Printf("*** Caught unhandled %s; terminating\n", what);
	throw;
}
