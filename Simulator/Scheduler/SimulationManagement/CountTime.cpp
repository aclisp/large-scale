//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%371CC7540338.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%371CC7540338.cm

//## begin module%371CC7540338.cp preserve=yes
//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: Simulation Management
//
//	File Name: CountTime.cpp
//
//	Description:
//
//	Author:	 Hélio Azevedo
//
//	Creation Date:	 03/99
//
//	Visual Source Safe $Revision: 3 $
//
//	Last check in $Date: 13/05/99 9:23 $
//
//	********************************************************************************
//## end module%371CC7540338.cp

//## Module: CountTime%371CC7540338; Pseudo Package body
//## Source file: T:\SimulationManagement\CountTime.cpp

//## begin module%371CC7540338.additionalIncludes preserve=no
//## end module%371CC7540338.additionalIncludes

//## begin module%371CC7540338.includes preserve=yes
//## end module%371CC7540338.includes
#include "MemLeak.h"
// CountTime
#include "CountTime.h"
//## begin module%371CC7540338.additionalDeclarations preserve=yes
namespace sch  {

//## end module%371CC7540338.additionalDeclarations


// Class CountTime 






//## Other Operations (implementation)
//## Operation: startCount%371CC8DB0394
//	********************************************************************************
//
//	Name: void startCount(void)
//
//	Description: Starts counting times.
//
//
//	Input parameters: none
//
//
//	Returns: none
//
//
//	********************************************************************************
void CountTime::startCount ()
{
  //## begin CountTime::startCount%371CC8DB0394.body preserve=yes

	//	Auxiliar variable.
      FILETIME ftDummy;

	// Get starting times
#if defined(ALPHAEV67_OSF50_CXX62)
#else
	GetThreadTimes(GetCurrentThread(), &ftDummy, &ftDummy, &ftKernelTimeStart, &ftUserTimeStart);
#endif
  //## end CountTime::startCount%371CC8DB0394.body
}

//## Operation: calculateCount%371F5E6803C4
//	********************************************************************************
//
//	Name:Int64 calculateCount (void)
//
//	Description: Calculates the elapsed time since the startCount until now. This function may be called several times. The resultant
//	elapsed time can also  be recovered by GetCount function.
//	Note that GetCount function does not calculate the result, only returns the last value calculated.
//
//	Parameters: none
//
//	Returns: Ulong = elapsed time
//
//	********************************************************************************
Int64 CountTime::calculateCount ()
{
  //## begin CountTime::calculateCount%371F5E6803C4.body preserve=yes

      //	Stores the end time  wasted with  user code of the user application by the thread.
      FILETIME ftUserTimeEnd;

      //	Stores the end time  wasted with OS code of the user application by the thread.
      FILETIME ftKernelTimeEnd;

      //	Auxiliar variable.
      FILETIME ftDummy;

      //	Stores the time wasted with OS code of user application.
      Int64 qwKernelTimeElapsed;

      //	Stores the time wasted with user code of user application.
      Int64 qwUserTimeElapsed;


	// get ending times
#if defined(ALPHAEV67_OSF50_CXX62)
#else
	GetThreadTimes(GetCurrentThread(), &ftDummy, &ftDummy, &ftKernelTimeEnd, &ftUserTimeEnd);
#endif

	// get the elapsed kernel and user times by converting the start and end times
	// from FILETIMES to quad words, adn then subtract the start times from end times.
	qwKernelTimeElapsed = fileTimeToQuad(&ftKernelTimeEnd)-
							 fileTimeToQuad(&ftKernelTimeStart);

	qwUserTimeElapsed = fileTimeToQuad(&ftUserTimeEnd)-
							 fileTimeToQuad(&ftUserTimeStart);

	// Get total time duration by adding the kernel and user times.
	qwTotalTimeElapsed = qwKernelTimeElapsed + qwUserTimeElapsed; 

	return 	qwTotalTimeElapsed;


  //## end CountTime::calculateCount%371F5E6803C4.body
}

//## Operation: calculateCount%371F78260167
//	********************************************************************************
//
//	Name:Int64 calculateCount (HANDLE threadHandle)
//
//	Description: Calculates the elapsed time since the startCount until now. This function may be called several times. The resultant
//	elapsed time can also  be recovered by GetCount function.
//	Note that GetCount function does not calculate the result, only returns the last value calculated.
//
//	Parameters: threadHandle : handle to thread that must have the elapsed time calculated.
//
//	Returns: Ulong = elapsed time
//
//	********************************************************************************
Int64 CountTime::calculateCount (HANDLE threadHandle)
{
  //## begin CountTime::calculateCount%371F78260167.body preserve=yes
	
      //	Stores the end time  wasted with  user code of the user application by the thread.
      FILETIME ftUserTimeEnd;

      //	Stores the end time  wasted with OS code of the user application by the thread.
      FILETIME ftKernelTimeEnd;

      //	Auxiliar variable.
      FILETIME ftDummy;

      //	Stores the time wasted with OS code of user application.
      Int64 qwKernelTimeElapsed;

      //	Stores the time wasted with user code of user application.
      Int64 qwUserTimeElapsed;


	// get ending times
#if defined(ALPHAEV67_OSF50_CXX62)
#else
	GetThreadTimes(threadHandle, &ftDummy, &ftDummy, &ftKernelTimeEnd, &ftUserTimeEnd);
#endif

	// get the elapsed kernel and user times by converting the start and end times
	// from FILETIMES to quad words, adn then subtract the start times from end times.
	qwKernelTimeElapsed = fileTimeToQuad(&ftKernelTimeEnd)-
							 fileTimeToQuad(&ftKernelTimeStart);

	qwUserTimeElapsed = fileTimeToQuad(&ftUserTimeEnd)-
							 fileTimeToQuad(&ftUserTimeStart);
			  

	// Get total time duration by adding the kernel and user times.
	qwTotalTimeElapsed = qwKernelTimeElapsed + qwUserTimeElapsed; 

	return 	qwTotalTimeElapsed;

  //## end CountTime::calculateCount%371F78260167.body
}

//## Operation: fileTimeToQuad%371CC8ED035E
//	********************************************************************************
//
//	Name: Int64  fileTimeToQuad(PFILETIME pFileTime)
//
//	Description:Converts from FILETIME to quad word..
//
//	Parameters: pFileTime ; pointer to FileTime variable.
//
//	Returns: the converted value.
//
//	********************************************************************************
Int64 CountTime::fileTimeToQuad (FILETIME *pFileTime)
{
  //## begin CountTime::fileTimeToQuad%371CC8ED035E.body preserve=yes
	Int64 qw;

#if defined(ALPHAEV67_OSF50_CXX62)
	qw=0;
#else
	qw = pFileTime->dwHighDateTime;
	qw <<= 32;
	qw |= pFileTime->dwLowDateTime;
#endif
	return (qw);

  //## end CountTime::fileTimeToQuad%371CC8ED035E.body
}

// Additional Declarations
  //## begin CountTime%371CC7540338.declarations preserve=yes
  //## end CountTime%371CC7540338.declarations

//## begin module%371CC7540338.epilog preserve=yes
}		// end of namespace
//## end module%371CC7540338.epilog
