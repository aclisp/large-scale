#include "stdafx.h"
#include "Util.h"
#include "SqlLite.h"
#include "md5.h"


const wxString DATA_DIR = L"D:\\data_lnysksgl\\";
const int OP_SUCCESS = 0;
const int DB_ERROR = 100; // Database error
const int DB_NOTFOUND = 110; // Can not load data from database
const int FS_ERROR = 200; // File system error


const wxString CLASS_PRACTICE_STR[] = {
	L"S", 
	L"P"
};
const wxString CLASS_NUMBER_STR[] = {
	L"9:00",
	L"9:45",
	L"10:30",
	L"11:15",
	L"12:00",
	L"12:45",
	L"13:30",
	L"14:15",
	L"15:00",
	L"15:45",
	L"16:30",
	L"17:15",
	L"18:00",
	L"18:45",
	L"19:30",
	L"20:15"
};
const wxString CLASS_WEEKDAY_STR[] = {
	L"星期日",
	L"星期一",
	L"星期二",
	L"星期三",
	L"星期四",
	L"星期五",
	L"星期六"
};
const wxString CLASS_MEMOERROR_STR = 
	L"临时加课与常规课时冲突";
const wxString CLASS_STUDENT_LEAVE_STR[] = {
	L"", 
	L"△"
};


void ErrorMessageBox(const wxString& message, const wxString& detail)
{
	// TODO: use more friendly message dialog expand the details
	wxMessageBox(message+"\n\n"+detail, L"错误", wxOK|wxCENTRE|wxICON_ERROR);
}


int IsLockCalendarClass(const wxDateTime& date)
{
	return SqlLiteQueryForInt("SELECT COUNT(*) FROM TBL_CALENDAR_CLASS_LOCK \
								WHERE LOCK_DATE = '"+date.FormatISODate()+"'");
}


int StringToUInt(const wxString& uintStr)
{
	int resultInt;
	long tempInt;
	if (uintStr.ToLong(&tempInt)) resultInt = tempInt;
	else resultInt = 0;
	if (resultInt < 0) resultInt = 0;
	return resultInt;
}


int GetClassNumberByStr(const wxString& classNumberStr)
{
	for (int i=0; i<16; ++i)
	{
		if (CLASS_NUMBER_STR[i] == classNumberStr)
			return i;
	}
	return -1;
}


wxString Md5Digest(const wxString& input)
{
	unsigned char md5Buf[MD5_DIGEST_LENGTH];
	wxScopedCharBuffer iBuf = input.utf8_str();
	MD5(md5Buf, reinterpret_cast<unsigned char*>(iBuf.data()), iBuf.length());
	wxString ret;
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
		ret += wxString::Format("%02x", md5Buf[i]);
	return ret;
}
