#ifndef Util_INCLUDED
#define Util_INCLUDED


extern const wxString DATA_DIR;
extern const int OP_SUCCESS;
extern const int DB_ERROR;
extern const int DB_NOTFOUND;
extern const int FS_ERROR;
extern const wxString CLASS_PRACTICE_STR[];
extern const wxString CLASS_NUMBER_STR[];
extern const wxString CLASS_WEEKDAY_STR[];
extern const wxString CLASS_MEMOERROR_STR;
extern const wxString CLASS_STUDENT_LEAVE_STR[];


const int TRANSACTION_SCOPE = 0;
const int TRANSACTIONAL     = 1;
const int LEFTMOST_COLUMN   = 0;
const int BIND_DATE         = 0;
const int BIND_DATETIME     = 1;
const int BIND_TIME         = 2;
const bool FROM_LEFT        = false; // Used by wxString& Trim(bool fromRight=true)
const bool FROM_RIGHT       = true;  // Used by wxString& Trim(bool fromRight=true)


void ErrorMessageBox(const wxString& message, const wxString& detail);
int IsLockCalendarClass(const wxDateTime& date);
int StringToUInt(const wxString& uintStr);
int GetClassNumberByStr(const wxString& classNumberStr);
wxString Md5Digest(const wxString& input);


#endif // Util_INCLUDED
