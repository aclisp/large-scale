//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%376945E303CA.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%376945E303CA.cm

//## begin module%376945E303CA.cp preserve=yes
//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: Trace and Log
//
//	File Name: FileSupport.cpp
//
//	Description:   This class offers support for files treatment. 
//
//	Author:	 Hélio Azevedo
//
//	Creation Date: Jun/18/99
//
//	Visual Source Safe $Revision: 4 $
//
//	Last check in $Date: 10/04/99 2:53p $
//
//	********************************************************************************
//## end module%376945E303CA.cp

//## Module: FileSupport%376945E303CA; Pseudo Package body
//## Source file: T:\TraceAndLog\FileSupport.cpp

//## begin module%376945E303CA.additionalIncludes preserve=no
//## end module%376945E303CA.additionalIncludes

//## begin module%376945E303CA.includes preserve=yes
//## end module%376945E303CA.includes
#include "MemLeak.h"
// FileSupport
#include "FileSupport.h"
//## begin module%376945E303CA.additionalDeclarations preserve=yes

using namespace std;

namespace sch {

	const Int END_BANNER_SIZE = 13;					// define the size of the end banner


//## end module%376945E303CA.additionalDeclarations


// Class FileSupport 









//## Operation: FileSupport%376946440293
//	********************************************************************************
//
//	Name: FileSupport (const string &fileName, bool bBanner, Uint maxFile  = 1, Uint fileSize = MAX_VALUE_UINT)
//
//	Description: Non-default constructor - requires information about the file name, presence of banner, and opcionally  information
//	about maximum number of files and file size.
//
//	Input parameters: const string& fileName;  //name of the new  file
//	                  bool bBanner;            // indicates if banner is need
//	                  Uint maxFile;              //maximum number of files
//	                  Uint fileSize;               //maximum size in bytes of each file
//
//	Returns: none
//
//	Exceptions: SchException, with one of the following error codes:
//	               SCH_INVALID_FILE_NAME     - file name not supplied
//	               SCH_ERROR_CREATING_FILE   - error opening file
//
//	Remarks:
//	The banner is inserted  at the beginnig and the end of the file. The banner has the following format :
//	--- <bannerType> at <week day> <month> <day> <hour>:<min>:<sec> <year> [Sequential number = <number>]
//	 Ex: --- ENDED at Mon Mar 29 08:30:54 1999
//	The sequential number will only be presented in the beginning banner.
//
//	The file name effectivaly used in all the operations is composed from three parts:  <file_base_name><number>.<ext>
//	1. <file_base_name> : file name supplied as parameter without the extension.
//	2. <number> : File sequence number that grows until the maxFile parameter. If maxFile is equal one this field is empty.
//	3. <ext>: The extension supplied on the fileName parameter. If fileName does not have extension this field is empty on the actual
//	file name.
//
//	********************************************************************************
FileSupport::FileSupport (const string& fileName, bool bBanner, Uint maxFile, Uint fileSize)
  //## begin FileSupport::FileSupport%376946440293.hasinit preserve=no
      : curSize(0),
        fileNum(1),
        maxSize(MAX_VALUE_UINT),
        maxFileNum(1),
        pFile(NULL),
        banner(false),
        seqFileNum(1)
  //## end FileSupport::FileSupport%376946440293.hasinit
  //## begin FileSupport::FileSupport%376946440293.initialization preserve=yes
  //## end FileSupport::FileSupport%376946440293.initialization
{
  //## begin FileSupport::FileSupport%376946440293.body preserve=yes

	RetCode rc = SCH_SUCCESS;

	rc = init (fileName, bBanner, maxFile, fileSize);

	if (rc != SCH_SUCCESS)
	{
		//
		// error opening file - raise an exception passing the cause along
		//
		throw SchException (rc);
	}

  //## end FileSupport::FileSupport%376946440293.body
}


//## Operation: ~FileSupport%376947A401F9
//	********************************************************************************
//
//	Name: ~FileSupport()
//
//	Description: Destructor - closes the file currently open.
//
//	Input parameters: none
//
//	Returns: none
//
//	********************************************************************************
FileSupport::~FileSupport ()
{
  //## begin FileSupport::~FileSupport%376947A401F9.body preserve=yes

	close();					// close current open file

  //## end FileSupport::~FileSupport%376947A401F9.body
}



//## Other Operations (implementation)
//## Operation: write%376946C202B2
//	********************************************************************************
//
//	Name: RetCode write(const string& msg)
//
//	Description: Writes the message into file considering the strategy of consider several files working in a round_robin fashion.
//
//	Input Parameters:  const string& msg; //message to be recorded
//
//	Returns: SCH_SUCCESS
//	           SCH_FILE_NOT_OPEN
//	           SCH_ERROR_WRITING_TO_FILE
//
//	********************************************************************************
RetCode FileSupport::write (const string& msg)
{
  //## begin FileSupport::write%376946C202B2.body preserve=yes

 	string  bannerMsg;				// aux string to hold banner message
	time_t  lTime;					// holds current system time
	RetCode rc = SCH_SUCCESS;
	const char *pStr = msg.c_str();
	Uint	stringSize;					// actual string size considering new line expansion
	char	tmpString[60] = "";


	//
	// make sure file is open
	//
	if (pFile == NULL)
		return SCH_FILE_NOT_OPEN;
	
	//
	// write string and banner	in the format
	// --- <bannerType> at <week day> <month> <day> <hour>:<min>:<sec> <year> 
	// Ex: --- ENDED at Mon Mar 29 08:30:54 1999 
	//
	try
	{
		//
		// update string size
		//
		stringSize= msg.size ();						// add message size
		// Considering that the file is text type the effective file size must consider
		// that for the Microsoft universe a line feed is expanded ascarriage
		// return and line feed (so one more char must be taken in account).
		while (*pStr != '\0')					
			if (*pStr++ == '\n') stringSize++;


		//
		// current trace file full?
		//
		if ( ((banner)?(curSize + END_BANNER_SIZE) : curSize)  >  (maxSize - stringSize))
		{
			curSize = 0;
			//
			// yes, move to the next one
			//
 			fileNum = (fileNum % maxFileNum) + 1;    // next number, wrap if last

			//
			// close current file and open next one by invoking open() method
			// if requested by user, record closing banner before closing old file and 
			// write opening banner in the new one
			//
			if (banner) {
				time (&lTime);
				bannerMsg = "\n--- ENDED at ";
				bannerMsg += ctime (&lTime);
				bannerMsg += '\n';

			// write end banner into  file
			// Note that we do not verify error code in this case. Actually we assume that
			// does not make up for stop the execution in the final banner writting.
				writeString (bannerMsg);
			}

			if ((rc = open ()) == SCH_SUCCESS && banner)
			{

				sprintf (tmpString, "\nSequential File Number = %ld --- STARTED at ",
								seqFileNum);  
				bannerMsg = tmpString;
				bannerMsg += ctime (&lTime);
				bannerMsg += '\n';
				rc = writeString (bannerMsg);
			}
		}

		//
		// finally write  info into file
		//
		if (rc == SCH_SUCCESS)
		{
			rc = writeString (msg);  // write into trace file
		}

	}  // try block  

	catch (std::exception)
	{
		rc = SCH_ERROR_WRITING_TO_FILE;
	}

	return (rc);

  //## end FileSupport::write%376946C202B2.body
}

//## Operation: open%3769466C0132
//	********************************************************************************
//
//	Name: RetCode open()
//
//	Description: Closes the current open file and opens a new file following the sequence established by maxFile and fileSize
//	parameters supplied at construction time.
//
//	Input parameters: none
//
//	Returns: SCH_SUCCESS
//	         SCH_ERROR_CREATING_FILE - error opening trace file
//
//	********************************************************************************
RetCode FileSupport::open ()
{
  //## begin FileSupport::open%3769466C0132.body preserve=yes

	string actualFileName;
	RetCode rc = SCH_SUCCESS;


	if (pFile != NULL)	{								// there is a file open - close it
		//
		// close trace file and delete ofstream object (regardless of previous I/O status)
		//
		pFile->close ();
		delete pFile;
		pFile = NULL;									// cleanup pointer for reuse
		seqFileNum ++;									// updates sequential file number
	}

	try
	{
		actualFileName = GetActualName ();
		pFile = new ofstream (actualFileName.c_str());  // create trace file
//		pFile = new ofstream (actualFileName.c_str(), ios::binary);  // create trace file
		if (pFile == NULL)								// could not create file
			rc = SCH_ERROR_CREATING_FILE;
	}
	catch (std::exception)
	{
		rc = SCH_ERROR_CREATING_FILE;
	}

	return (rc);


  //## end FileSupport::open%3769466C0132.body
}

//## Operation: GetActualName%376A97240376
//	********************************************************************************
//
//	Name:  const string GetActualName()
//
//	Description: Builds the full-qualified file name, which format is
//	<base_file_name><number>.<ext>
//
//	Input parameters: none
//
//	Returns: the value of the string with the actual file name
//
//	********************************************************************************
const string FileSupport::GetActualName ()
{
  //## begin FileSupport::GetActualName%376A97240376.body preserve=yes
	string baseName;
	string extension;
	char strNum[15] = "";    // trace file number, one digit only
	Uint pos;

	// obtain string representing the sequential file number when user need more than 1 file
	if (maxFileNum != 1)
			sprintf (strNum, "%d", fileNum);  
	
	// search for dot in the original file name
	pos = name.find_last_of('.');

	if (pos == MAX_VALUE_UINT)					// char not found
		return  name + strNum;
	else {
#if defined(ALPHAEV67_OSF50_CXX62)
		// Replaced in UNIX version since I could not figure-out why it kept insisting
		// on instantiating the template with "unsigned long", which is unavailable
		char temp[2048];
		strncpy(temp, name.c_str(), pos);
		temp[pos] = '\0';
		baseName = temp;
		
		strncpy(temp, (name.c_str())+pos+1, name.size());
		temp[name.size()] = '\0';
		extension = temp;
#else
		baseName.assign(name, 0, pos);
		extension.assign(name, pos+1, name.size());
#endif
		return  baseName + strNum + '.' + extension;  //  file name is "<baseName><#>.<ext>"
	}



  //## end FileSupport::GetActualName%376A97240376.body
}

//## Operation: writeString%376AA056021D
//	********************************************************************************
//
//	Name: RetCode writeString(const string& msg)
//
//	Description: Writes the message into file and checks I/O stream status.
//
//	Input Parameters:  const string& msg; //message to be recorded
//
//	Returns: SCH_SUCCESS
//	          SCH_ERROR_WRITING_TO_FILE
//
//	********************************************************************************
RetCode FileSupport::writeString (const string& msg)
{
  //## begin FileSupport::writeString%376AA056021D.body preserve=yes
	const char *pStr = msg.c_str();

	//
	// check stream state
	//
	if (pFile->good() == 0)         // stream state = not good
		return SCH_ERROR_WRITING_TO_FILE;

	//
	// write trace into file
	//
	*pFile << msg;
	pFile->flush ();                // force data to be written to disk

	//
	// check I/O stream status
	//
	if (pFile->fail() != 0)         // I/O error (either badbit or failbit error flag set)
	{
		pFile->clear();		        // clear the error-state flags
		return SCH_ERROR_WRITING_TO_FILE;
	}
	
	//
	// update file size
	//
	curSize += msg.size ();						// add message size
	// Considering that the file is text type the effective file size must consider
	// that for the Microsoft universe a line feed is expanded ascarriage
	// return and line feed (so one more char must be taken in account).
	while (*pStr != '\0')					
		if (*pStr++ == '\n') curSize++;



	return SCH_SUCCESS;

  //## end FileSupport::writeString%376AA056021D.body
}

//## Operation: init%37723336012A
//	********************************************************************************
//
//	Name: RetCode init(const string& fileName,  bool bBanner, Uint maxFile  = 1, Uint fileSize = MAX_VALUE_UINT)
//
//	Description: Initiates FileSupport class. This method is only used by derived classes that are singleton. It requires information
//	about the file name, presence of banner, and opcionally  information about maximum number of files and file size.
//
//	Input parameters: const string& fileName; //name of log file
//	                  bool bBanner;          //  indicates if banner is need
//	                  Uint maxFile;             // maximum number of  files
//	                  Uint fileSize;              // maximum size in bytes of each  file
//
//	Returns: SUCCESS
//	         SCH_INVALID_FILE_NAME	   - invalid log file name
//	         SCH_ERROR_CREATING_FILE   - error opening log file
//
//	Remarks:
//	The banner is inserted  at the beginnig and the end of the file. The banner has the following format :
//	--- <bannerType> at <week day> <month> <day> <hour>:<min>:<sec> <year>
//	 Ex: --- ENDED at Mon Mar 29 08:30:54 1999
//
//	The file name effectivaly used in all the operations is composed from three parts:  <file_base_name><number>.<ext>
//	1. <file_base_name> : file name supplied as parameter without the extension.
//	2. <number> : File sequence number that grows until the maxFile parameter. If maxFile is equal one this field is empty.
//	3. <ext>: The extension supplied on the fileName parameter. If fileName does not have extension this field is empty on the actual
//	file name.
//	********************************************************************************
RetCode FileSupport::init (const string& fileName, bool bBanner, Uint maxFile, Uint fileSize)
{
  //## begin FileSupport::init%37723336012A.body preserve=yes

   	string  bannerMsg;							// aux string to hold banner message
	time_t  lTime;								// holds current system time
	RetCode rc = SCH_SUCCESS;
	char tmpString[60] = "";

	pFile = NULL; 
    curSize = 0;
	fileNum = 1;

	name = fileName;
	banner = bBanner;
	maxFileNum = maxFile;
	maxSize = fileSize;

	if (name.size()	== 0)					// file name supplied?
		return SCH_INVALID_FILE_NAME;
							
	rc = open ();							// open file
	if (rc != SCH_SUCCESS) return rc;

	if ( banner) {							// write banner if requested
		sprintf (tmpString, "\nSequential File Number = %ld --- STARTED at ",seqFileNum);  
		bannerMsg = tmpString;
		time (&lTime);
		bannerMsg += ctime (&lTime);
		bannerMsg += '\n';
		rc = write (bannerMsg);
	}

	return rc;


  //## end FileSupport::init%37723336012A.body
}

//## Operation: close%37724041026B
//	********************************************************************************
//
//	Name: RetCode close(void)
//
//	Description: Terminates FileSupport operations. The  file is closed.
//
//	Parameters: none
//
//	Returns: SCH_SUCCESS
//	         SCH_ERROR_WRITING_TO_FILE - error writing closing eventual banner - the log file is closed anyway
//
//	Remarks: This function performs the cleanup tasks, therefore it should be called at the very end of the client application.
//
//	********************************************************************************
RetCode FileSupport::close ()
{
  //## begin FileSupport::close%37724041026B.body preserve=yes

 	string  bannerMsg = "\n--- ENDED at ";		// aux string to hold banner message
	time_t  lTime;								// holds current system time
	RetCode rc = SCH_SUCCESS;


	if (pFile != NULL)	{								// there is a file open - close it
		if (banner) {
			time (&lTime);
			bannerMsg += ctime (&lTime);
			bannerMsg += '\n';
			rc = writeString (bannerMsg);
		}
		//
		// close trace file and delete ofstream object (regardless of previous I/O status)
		//
		pFile->close ();
		delete pFile;
		pFile = NULL;									// cleanup pointer for reuse
	}

	return rc;

  //## end FileSupport::close%37724041026B.body
}

// Additional Declarations
  //## begin FileSupport%376945E303CA.declarations preserve=yes
  //## end FileSupport%376945E303CA.declarations

//## begin module%376945E303CA.epilog preserve=yes

} // namespace sch

//## end module%376945E303CA.epilog
