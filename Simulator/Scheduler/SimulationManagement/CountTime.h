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
//	File Name: CountTime.h
//
//	Description:
//
//	Author:	Hélio Azevedo
//
//	Creation Date:	 03/99
//
//	Visual Source Safe $Revision: 3 $
//
//	Last check in $Date: 13/05/99 9:24 $
//
//	********************************************************************************
//## end module%371CC7540338.cp

//## Module: CountTime%371CC7540338; Pseudo Package specification
//## Source file: T:\SimulationManagement\CountTime.h

#ifndef CountTime_h
#define CountTime_h 1

//## begin module%371CC7540338.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%371CC7540338.additionalIncludes

//## begin module%371CC7540338.includes preserve=yes
#include <windows.h>


//## end module%371CC7540338.includes

//## begin module%371CC7540338.additionalDeclarations preserve=yes
namespace sch  {


//## end module%371CC7540338.additionalDeclarations


//## begin CountTime%371CC7540338.preface preserve=yes
//## end CountTime%371CC7540338.preface

//## Class: CountTime%371CC7540338
//	********************************************************************************
//
//	Responsible for calculation of total thread execution time. The objective of this class is to determine how long a thread uses to
//	perform its task. Basically, this class adds the elapsed kernel (operating system code) and user times (application code) to obtain
//	the total execution time. To obtain the result in:
//	seconds:           divide by 10,000,000  (10^7)
//	miliseconds:     divide by 10,000  (10^4)
//	microseconds: divide by 10
//	nanoseconds:  multipy by 100
//
//	This class is plataform dependent.
//
//	********************************************************************************
//## Category: SimulationManagement%369E33A90107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CountTime 
{
  //## begin CountTime%371CC7540338.initialDeclarations preserve=yes
  //## end CountTime%371CC7540338.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: startCount%371CC8DB0394
      void startCount ();

      //## Operation: getCount%371CC8E60209
      Int64 getCount ();

      //## Operation: calculateCount%371F5E6803C4
      Int64 calculateCount ();

      //## Operation: calculateCount%371F78260167
      Int64 calculateCount (HANDLE threadHandle);

    // Additional Public Declarations
      //## begin CountTime%371CC7540338.public preserve=yes
      //## end CountTime%371CC7540338.public

  protected:

    //## Other Operations (specified)
      //## Operation: fileTimeToQuad%371CC8ED035E
      Int64 fileTimeToQuad (FILETIME *pFileTime);

    // Additional Protected Declarations
      //## begin CountTime%371CC7540338.protected preserve=yes
      //## end CountTime%371CC7540338.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: ftUserTimeStart%371F09180012
      //	Stores the start time  wasted with user code of the  user application by the thread.
      //## begin CountTime::ftUserTimeStart%371F09180012.attr preserve=no  private: FILETIME {UA} 0
      FILETIME ftUserTimeStart;
      //## end CountTime::ftUserTimeStart%371F09180012.attr

      //## Attribute: ftKernelTimeStart%371F09C002EF
      //	Stores the start time  wasted with OS code of the user application by the thread.
      //## begin CountTime::ftKernelTimeStart%371F09C002EF.attr preserve=no  private: FILETIME {UA} 0
      FILETIME ftKernelTimeStart;
      //## end CountTime::ftKernelTimeStart%371F09C002EF.attr

      //## Attribute: qwTotalTimeElapsed%371F0D7200C8
      //	Stores the total time wasted with kernel and user code of user application.
      //## begin CountTime::qwTotalTimeElapsed%371F0D7200C8.attr preserve=no  private: Int64 {UA} 0
      Int64 qwTotalTimeElapsed;
      //## end CountTime::qwTotalTimeElapsed%371F0D7200C8.attr

    // Additional Private Declarations
      //## begin CountTime%371CC7540338.private preserve=yes
      //## end CountTime%371CC7540338.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CountTime%371CC7540338.implementation preserve=yes
      //## end CountTime%371CC7540338.implementation

};

//## begin CountTime%371CC7540338.postscript preserve=yes
//## end CountTime%371CC7540338.postscript

// Class CountTime 


//## Other Operations (inline)
//## Operation: getCount%371CC8E60209
//	********************************************************************************
//
//	Name:Int64 getCount (void)
//
//	Description: Returns the last elapsed time calculated. The period of elapsed time is defined as the time between the startCount
//	function and the last calculateCount function called.
//
//
//	Parameters: none
//
//	Returns: Ulong = elapsed time ( miliseconds) )
//
//	********************************************************************************
inline Int64 CountTime::getCount ()
{
  //## begin CountTime::getCount%371CC8E60209.body preserve=yes
	return qwTotalTimeElapsed;
  //## end CountTime::getCount%371CC8E60209.body
}

//## begin module%371CC7540338.epilog preserve=yes
}		// close namespace
//## end module%371CC7540338.epilog


#endif
