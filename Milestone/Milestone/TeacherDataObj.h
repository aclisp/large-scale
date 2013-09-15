#ifndef TeacherDataObj_INCLUDED
#define TeacherDataObj_INCLUDED


struct TeacherDataObj
{
	wxString m_name;
	wxDateTime m_onboardDate;
	int m_signoff;
	wxDateTime m_signoffDate;
	int m_rank;
	int m_partTime;
	wxString m_phone;
	wxString m_memo;

	static const wxString SIGNOFF_STR[];
	static const wxString RANK_STR[];
	static const wxString PARTTIME_STR[];

	TeacherDataObj();

	int loadByName(const wxString& name);
	int insertThisObj();
	static int deleteByName(const wxString& name);
	int updateByName(const wxString& name);
};


#endif // TeacherDataObj_INCLUDED
