#ifndef MainApp_INCLUDED
#define MainApp_INCLUDED


class MainApp :	public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();
	virtual void OnUnhandledException();
};


DECLARE_APP(MainApp);


#endif // MainApp_INCLUDED
