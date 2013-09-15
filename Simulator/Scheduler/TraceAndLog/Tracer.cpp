//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC46670254.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC46670254.cm

//## begin module%36AC46670254.cp preserve=yes
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
//	File Name: Tracer.cpp
//
//	Description: Implementation file for the Tracer class.
//
//	Author:	Eugenio Pacheli Alves / Hélio Azevedo
//
//	Creation Date: Jun/24/99
//
//	Visual Source Safe $Revision: 5 $
//
//	Last check in $Date: 29/06/99 15:56 $
//
//	********************************************************************************
//## end module%36AC46670254.cp

//## Module: Tracer%36AC46670254; Pseudo Package body
//## Source file: T:\TraceAndLog\Tracer.cpp

//## begin module%36AC46670254.additionalIncludes preserve=no
//## end module%36AC46670254.additionalIncludes

//## begin module%36AC46670254.includes preserve=yes
//## end module%36AC46670254.includes
#include "MemLeak.h"
// Tracer
#include "Tracer.h"
//## begin module%36AC46670254.additionalDeclarations preserve=yes

using namespace std;

namespace sch {

//## end module%36AC46670254.additionalDeclarations


// Class Tracer 



//## Operation: Tracer%36B5F71E034D
//	********************************************************************************
//
//	Name: Tracer(const string& trcFileName , Uint16 kindSet, bool bTimestamp, Uint trcMaxFile  = 1, Uint trcFileSize = MAX_VALUE_UINT)
//
//	Description: Non-default constructor - requires information about the trace file, kinds to be set and timestamp enabling.
//
//	Input parameters: const string& trcFileName;  //name of trace file
//	                  Uint16 kindSet;             //bit mask indicating kinds which have tracing enabled
//	                  bool bTimestamp;             //TRUE = enable insertion of timestamp in trace messages
//	                  Uint trcMaxFile;             //maximum number of trace files
//	                  Uint trcFileSize;            //maximum size in bytes of each trace file
//
//	Returns: none
//
//	Exceptions: SchException, with one of the following error codes:
//	               SCH_INVALID_FILE_NAME     - trace file name not supplied
//	               SCH_ERROR_CREATING_FILE   - error opening trace file
//	               SCH_ERROR_WRITING_TO_FILE - error writing opening banner
//
//	Remarks: Only the core trace file name must be provided. The actual file names include the numbered suffix.
//
//	********************************************************************************
Tracer::Tracer (const string& trcFileName, Uint16 kindSet, bool bTimestamp, Uint trcMaxFile, Uint trcFileSize)
  //## begin Tracer::Tracer%36B5F71E034D.hasinit preserve=no
      : kindMask(kindSet)
  //## end Tracer::Tracer%36B5F71E034D.hasinit
  //## begin Tracer::Tracer%36B5F71E034D.initialization preserve=yes
	,FileSupport (trcFileName, true, trcMaxFile, trcFileSize)
   
  //## end Tracer::Tracer%36B5F71E034D.initialization
{
  //## begin Tracer::Tracer%36B5F71E034D.body preserve=yes

 	RetCode rc = SCH_SUCCESS;

	//
	// if bTimestamp is enabled then the current system time is the basis for the time used in
	// trace messages. This is time 0, and all subsequent messages are relative to initTime
	//
	if (bTimestamp == true)
	{
		time (&initTime);   // timestamp enabled - get current system time
	}
	else
	{
		initTime = 0;		// timestamp not enabled - set to 0
	}


  //## end Tracer::Tracer%36B5F71E034D.body
}


//## Operation: ~Tracer%36DFC86100AC
//	********************************************************************************
//
//	Name: ~Tracer(void)
//
//	Description: Destructor - closes the trace file.
//
//	Parameters: none
//
//	Returns: none
//
//	********************************************************************************
Tracer::~Tracer ()
{
  //## begin Tracer::~Tracer%36DFC86100AC.body preserve=yes
  //## end Tracer::~Tracer%36DFC86100AC.body
}



//## Other Operations (implementation)
//## Operation: trace%36B5FA8D006C
//	********************************************************************************
//
//	Name: RetCode trace(Uint16 kind, const string& traceMsg)
//
//	Description: Records the trace message in the trace file if tracing is enabled for the passed kind.
//
//	Input parameters: Uint16 kind;            //kind of this trace message
//	                  const string& traceMsg; //message to be recorded
//
//	Returns: SCH_SUCCESS
//	         SCH_ERROR_WRITING_TO_FILE
//
//	Remarks: A trace message belongs to a single kind, therefore only one bit (the one related to this message kind) should be set in
//	the kind parameter.
//
//	********************************************************************************
RetCode Tracer::trace (Uint16 kind, const string& traceMsg)
{
  //## begin Tracer::trace%36B5FA8D006C.body preserve=yes
 
	char    tmpString[10] = "";    // for integer-to-string convertions
	RetCode rc = SCH_SUCCESS;

	//
	// is this kind turned ON? Check thru bitwise AND operation
	//
	if (kind & kindMask)
	{
		//
		// build info to be written into trace file
		//
		string fullMsg;  // aux string to hold whole trace message
		
		//
		// write timestamp if enabled
		//
		if (initTime != 0)
		{
			time_t lTime;    // holds current system time
 			time (&lTime);
			sprintf (tmpString, "%ld ", 		   // time_t is actually a typedef to long
								lTime - initTime);  
			fullMsg = tmpString;
		}

		//
		// append remainder text, format is (with timestamp)
		// timestamp kind message
		// Ex: 131 0040 Entering Trace::setKind, new Kind = 0200
		//
		sprintf (tmpString, "%04X ", kind);
		fullMsg += tmpString + traceMsg;
		

		return write (fullMsg);
	}

	return SCH_SUCCESS;

  //## end Tracer::trace%36B5FA8D006C.body
}

// Additional Declarations
  //## begin Tracer%36AC46670254.declarations preserve=yes
  //## end Tracer%36AC46670254.declarations

//## begin module%36AC46670254.epilog preserve=yes

} // namespace sch

//## end module%36AC46670254.epilog
