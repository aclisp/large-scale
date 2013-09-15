//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC467C024A.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC467C024A.cm

//## begin module%36AC467C024A.cp preserve=yes
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
//	File Name: Logger.cpp
//
//	Description: Implementation file for the Logger class.
//
//	Author: Eugenio Pacheli Alves
//
//	Creation Date: Mar/22/99
//
//	Visual Source Safe $Revision: 5 $
//
//	Last check in $Date: 29/06/99 15:55 $
//
//	********************************************************************************
//## end module%36AC467C024A.cp

//## Module: Logger%36AC467C024A; Pseudo Package body
//## Source file: T:\TraceAndLog\Logger.cpp

//## begin module%36AC467C024A.additionalIncludes preserve=no
//## end module%36AC467C024A.additionalIncludes
#include "MemLeak.h"
//## begin module%36AC467C024A.includes preserve=yes
#include <iostream>	  // iostreams objects that manipulate the standard streams
#include <time.h>	  // convertion between various time and date formats
//## end module%36AC467C024A.includes

// Logger
#include "Logger.h"
//## begin module%36AC467C024A.additionalDeclarations preserve=yes

using namespace std;

namespace sch {

//## end module%36AC467C024A.additionalDeclarations


// Class Logger 



//## begin Logger::pInstance%36D3ED7E00B2.attr preserve=no  private: static Logger * {UA} NULL
Logger *Logger::pInstance = NULL;
//## end Logger::pInstance%36D3ED7E00B2.attr


//## Other Operations (implementation)
//## Operation: init%36B60350025C
//	********************************************************************************
//
//	Name: RetCode init(const string& fileName, bool bStdOutput, Uint logMaxFile  = 1, Uint logFileSize = MAX_VALUE_UINT)
//
//	Description: Initiates logging; bActive is set to TRUE and log file is opened.
//
//	Input parameters: const string& fileName; //name of log file
//	                  bool bStdOutput;        //TRUE = send log messages to standard output; FALSE otherwise
//	                  Uint logMaxFile;             //maximum number of log files
//	                  Uint logFileSize;            //maximum size in bytes of each log file
//
//	Returns: SUCCESS
//	         SCH_INVALID_FILE_NAME	   - invalid log file name
//	         SCH_ERROR_CREATING_FILE   - error opening log file
//	         SCH_FILE_ALREADY_OPEN     - called again before end()
//	         SCH_ERROR_WRITING_TO_FILE - error writing opening banner
//
//	Remarks: The log file name must be provided. The Logger class does not have a public constructor since it is a singleton. This
//	function performs the initialization tasks, therefore it should be called at the very beginning of the client application.
//	The  logFileSize parameter defines a optional maximum file size. When the file is full, another file is created to resume file
//	writting. Again, when the second file is full, a third file is created. And so on, until the maximum number of files (supplied by
//	logMaxFile) is reached. Finally when the last file is full, recording wraps to the first file, erasing its previous contents. This
//	allows for a large amount of information to be recorded. If the default values are used the system allows the presence of only one
//	file with unlimited size.
//
//	********************************************************************************
RetCode Logger::init (const string& fileName, bool bStdOutput, Uint logMaxFile, Uint LogFileSize)
{
  //## begin Logger::init%36B60350025C.body preserve=yes

	RetCode rc = SCH_SUCCESS;

	//
	// init() must be called just once. If called twice, the log file will already be open
	//
	if (active())
	{
		//
		// log file already open
		//
		return SCH_FILE_ALREADY_OPEN;
	}

	bStdOut = bStdOutput;                     // save into object attribute

	return FileSupport::init( fileName, true, logMaxFile, LogFileSize);

  //## end Logger::init%36B60350025C.body
}

//## Operation: log%36B6043F00ED
//	********************************************************************************
//
//	Name: RetCode log(LogLevel level, bool bTime, const string& logMsg)
//
//	Description: Records the log message in the log file.
//
//	Input parameters: LogLevel level;       //level of the log message
//	                  bool bTime;           //TRUE = records the timestamp; FALSE = otherwise
//	                  const string& logMsg; //log message to be recorded
//
//	Returns: SCH_SUCCESS
//	         SCH_ERROR_WRITING_TO_FILE
//	         SCH_INVALID_LOG_LEVEL
//
//	********************************************************************************
RetCode Logger::log (LogLevel level, bool bTime, const string& logMsg)
{
  //## begin Logger::log%36B6043F00ED.body preserve=yes

	//
	// is logging turned ON?
	//
	if (bActive == true)
	{
		string fullMsg;  // aux string to hold whole log message
		
		//
		// determine log level string
		//
		switch (level)
		{
			case Informative:
				fullMsg = "I ";
				break;

			case Warning:
				fullMsg = "W ";
				break;

			case Error:
				fullMsg = "E ";
				break;

			default:
				return SCH_INVALID_LOG_LEVEL;
				break;
		}

		//
		// write timestamp if enabled
		//
		if (bTime == true)
		{
			time_t lTime;				 // holds current system time
			time (&lTime);				 // get current system time
			fullMsg += ctime (&lTime);   // display UNIX-style time
		}

		fullMsg += logMsg + '\n';

		//
		// write log info on standard output if enabled
		//
		if (bStdOut)
		{
				cout << fullMsg;
		}

		//
		// append log message into file
		//
		return write (fullMsg);         // write into log file
		

	} // logging turned on

	return SCH_SUCCESS;

  //## end Logger::log%36B6043F00ED.body
}

// Additional Declarations
  //## begin Logger%36AC467C024A.declarations preserve=yes
  //## end Logger%36AC467C024A.declarations

//## begin module%36AC467C024A.epilog preserve=yes

} // namespace sch

//## end module%36AC467C024A.epilog
