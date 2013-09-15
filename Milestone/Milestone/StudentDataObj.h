#ifndef StudentDataObj_INCLUDED
#define StudentDataObj_INCLUDED


struct StudentDataObj
{
	wxString m_name;
	wxDateTime m_signonDate;
	int m_signoff;
	wxDateTime m_signoffDate;
	wxString m_phone;
	wxString m_phone2;
	wxString m_address;
	wxDateTime m_birthDate;
	int m_age;
	wxString m_specialty;
	wxString m_memo;

	static const wxString SIGNOFF_STR[];

	StudentDataObj();

	int loadByName(const wxString& name);
	int insertThisObj();
	static int deleteByName(const wxString& name);
	int updateByName(const wxString& name);
};


#endif // StudentDataObj_INCLUDED
