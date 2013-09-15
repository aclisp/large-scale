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
//	File Name: Logger.h
//
//	Description: Header file for the Logger class.
//
//	Author: Eugenio Pacheli Alves
//
//	Creation Date: Mar/22/99
//
//	Visual Source Safe $Revision: 7 $
//
//	Last check in $Date: 10/20/99 12:20p $
//
//	********************************************************************************
//## end module%36AC467C024A.cp

//## Module: Logger%36AC467C024A; Pseudo Package specification
//## Source file: T:\TraceAndLog\Logger.h

#ifndef Logger_h
#define Logger_h 1

//## begin module%36AC467C024A.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36AC467C024A.additionalIncludes

//## begin module%36AC467C024A.includes preserve=yes
#include <fstream>    // iostreams template classes that manipulate external files
#include <string>	  // template class that implements a string container
//## end module%36AC467C024A.includes

// FileSupport
#include "FileSupport.h"
// LogLevel
#include "LogLevel.h"
//## begin module%36AC467C024A.additionalDeclarations preserve=yes

using namespace std;

namespace sch {

//## end module%36AC467C024A.additionalDeclarations


//## begin Logger%36AC467C024A.preface preserve=yes
//## end Logger%36AC467C024A.preface

//## Class: Logger%36AC467C024A
//	********************************************************************************
//
//	This class supports logging facility. Log displays informational, warning, and error messages to the end user. The log message may
//	be displayed in the standard output and/or recorded in a log file. This class is static since it just needs a single instance and
//	should be accessed from anywhere in the code.
//
//	Each logger file has a maximum size set when a Logger object is instantiated. When the log file is full, another file is created to
//	resume recording logs. Again, when the second log file is full, a third file is created.And so on, until the maximum number of
//	files (supplied at construction time) is reached. Finally when the last file is full, recording wraps to the first file, erasing
//	its previous contents. This allows for a large amount of log information to be recorded. Those files are named by adding a numbered
//	suffix to the supplied file name; e.g., if the supplied file name is "logger.trc" then the actual names are "logger1.log",
//	"logger2.log", "logger3.log", and so on.
//
//	This class implements the Singleton pattern, which ensures that the class has only one instance by:
//	  - providing a global point of access to the class - the instance() operation,
//	  - protecting its constructor to ensure that only one instance is created (see description of the instance() operation).
//
//	********************************************************************************
//## Category: TraceAndLog%369E33F80313
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%36C15F6802EB;LogLevel { -> }

class Logger : public FileSupport  //## Inherits: <unnamed>%3769489202FF
{
  //## begin Logger%36AC467C024A.initialDeclarations preserve=yes
  //## end Logger%36AC467C024A.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: init%36B60350025C
      RetCode init (const string& fileName, bool bStdOutput, Uint logMaxFile = 1, Uint LogFileSize = MAX_VALUE_UINT);

      //## Operation: instance%36D3ECDD03BD
      static Logger * instance ();

      //## Operation: isActive%36B6050C0106
      bool isActive ();

      //## Operation: log%36B6043F00ED
      RetCode log (LogLevel level, bool bTime, const string& logMsg);

      //## Operation: setActive%36B604F0025A
      void setActive (bool bMode);

      //## Operation: clear%376A70F90023
      static void clear ();

    // Additional Public Declarations
      //## begin Logger%36AC467C024A.public preserve=yes
      //## end Logger%36AC467C024A.public

  protected:
    //## Constructors (generated)
      Logger();

    //## Destructor (generated)
      ~Logger();

    // Additional Protected Declarations
      //## begin Logger%36AC467C024A.protected preserve=yes
      //## end Logger%36AC467C024A.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: bActive%36B6027F032E
      //	TRUE if logging is turned on.
      //## begin Logger::bActive%36B6027F032E.attr preserve=no  private: bool {UA} true
      bool bActive;
      //## end Logger::bActive%36B6027F032E.attr

      //## Attribute: bStdOut%36B602A30163
      //	TRUE if output to standard output is enabled.
      //## begin Logger::bStdOut%36B602A30163.attr preserve=no  private: bool {UA} false
      bool bStdOut;
      //## end Logger::bStdOut%36B602A30163.attr

      //## Attribute: pInstance%36D3ED7E00B2
      //	Static pointer to the unique instance of this class.
      //## begin Logger::pInstance%36D3ED7E00B2.attr preserve=no  private: static Logger * {UA} NULL
      static Logger *pInstance;
      //## end Logger::pInstance%36D3ED7E00B2.attr

    // Additional Private Declarations
      //## begin Logger%36AC467C024A.private preserve=yes
      //## end Logger%36AC467C024A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin Logger%36AC467C024A.implementation preserve=yes
	  RetCode writeLog (const string& logMsg);
      //## end Logger%36AC467C024A.implementation
};

//## begin Logger%36AC467C024A.postscript preserve=yes
//## end Logger%36AC467C024A.postscript

// Class Logger 

inline Logger::Logger()
  //## begin Logger::Logger%36AC467C024A_const.hasinit preserve=no
      : bActive(true),
        bStdOut(false)
  //## end Logger::Logger%36AC467C024A_const.hasinit
  //## begin Logger::Logger%36AC467C024A_const.initialization preserve=yes
  //## end Logger::Logger%36AC467C024A_const.initialization
{
  //## begin Logger::Logger%36AC467C024A_const.body preserve=yes
  //## end Logger::Logger%36AC467C024A_const.body
}


inline Logger::~Logger()
{
  //## begin Logger::~Logger%36AC467C024A_dest.body preserve=yes
  //## end Logger::~Logger%36AC467C024A_dest.body
}



//## Other Operations (inline)
//## Operation: instance%36D3ECDD03BD
//	********************************************************************************
//
//	Name: Logger * instance(void)
//
//	Description: Returns the unique instance of this class. This operator uses lazy initialization: it creates the instance when it is
//	first accessed, stores it in pInstance attribute and returns it. Later accesses to instance() just return the value of pInstance.
//	This implementation, combined to the protected constructor, ensures that this class is created and initialized before (and only at
//	that time) its first use.
//
//	Parameters: none
//
//	Returns: the value of the pInstance attribute
//
//	Remarks: this operator is static since it is the global point of access to this class (clients access Logger exclusively through
//	this operator).
//
//	********************************************************************************
inline Logger * Logger::instance ()
{
  //## begin Logger::instance%36D3ECDD03BD.body preserve=yes

	if (pInstance == NULL)
	{
		pInstance = new Logger;
	}
	return (pInstance);

  //## end Logger::instance%36D3ECDD03BD.body
}

//## Operation: isActive%36B6050C0106
//	********************************************************************************
//
//	Name: bool isActive(void)
//
//	Description: Indicates whether logging is enabled or not.
//
//	Parameters: none
//
//	Returns: TRUE = logging is enabled
//	         FALSE = otherwise
//
//	********************************************************************************
inline bool Logger::isActive ()
{
  //## begin Logger::isActive%36B6050C0106.body preserve=yes
	
	return (bActive);

  //## end Logger::isActive%36B6050C0106.body
}

//## Operation: setActive%36B604F0025A
//	********************************************************************************
//
//	Name: void setActive(bool bMode)
//
//	Description: Sets the message logging task to the passed mode (ON/OFF).
//
//	Input parameters: bool bMode;  //TRUE = ON; FALSE = OFF
//
//	Returns: none
//
//	********************************************************************************
inline void Logger::setActive (bool bMode)
{
  //## begin Logger::setActive%36B604F0025A.body preserve=yes

	bActive = bMode;

  //## end Logger::setActive%36B604F0025A.body
}

//## Operation: clear%376A70F90023
//	********************************************************************************
//
//	Name:void clear(void)
//
//	Description: Makes a orderly destruction for the Singleton Logger.
//
//	Parameters: none
//
//	Returns: None
//
//	Remarks: this operator is static since it is the global point to destroy  this singleton.
//
//	********************************************************************************
inline void Logger::clear ()
{
  //## begin Logger::clear%376A70F90023.body preserve=yes
	
	if (pInstance != NULL)
	{
		delete pInstance;
		pInstance = NULL;
	}


  //## end Logger::clear%376A70F90023.body
}

//## begin module%36AC467C024A.epilog preserve=yes

} // namespace sch

//## end module%36AC467C024A.epilog


#endif
