//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%37037F930023.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%37037F930023.cm

//## begin module%37037F930023.cp preserve=yes
//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: Common
//
//	File Name: SchException.h
//
//	Description: Definition of return codes and exceptions used in C++ Scheduler.
//
//	Author: Eugenio Pacheli Alves
//
//	Creation Date: Apr/05/99
//
//	Visual Source Safe $Revision: 9 $
//
//	Last check in $Date: 26/05/99 8:34 $
//
//	********************************************************************************
//## end module%37037F930023.cp

//## Module: SchException%37037F930023; Pseudo Package specification
//## Source file: T:\Common\SchException.h

#ifndef SchException_h
#define SchException_h 1

//## begin module%37037F930023.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%37037F930023.additionalIncludes

//## begin module%37037F930023.includes preserve=yes
#include <string>	  // template class that implements a string container
#include <exception>  // functions that control exception handling

//------------------------------------------------------------------------------------------------
// eliminate the files generated below by Rose since "exception" is not really a class we created
//------------------------------------------------------------------------------------------------
#if 0
//## end module%37037F930023.includes

// exception
#include "exception.h"
//## begin module%37037F930023.additionalDeclarations preserve=yes
#endif
using namespace std;
namespace sch {

//********************************************************************************
//
// Definition of return codes used in C++ Scheduler
//
//********************************************************************************

#define SCH_SUCCESS 0

//
// Generic error codes (ranges from 1 to 999 decimal)
//

// file access
#define SCH_INVALID_FILE_NAME        1
#define SCH_ERROR_CREATING_FILE      2
#define SCH_FILE_ALREADY_OPEN	     3
#define SCH_FILE_NOT_OPEN	         4
#define SCH_ERROR_READING_FROM_FILE  5
#define SCH_ERROR_WRITING_TO_FILE    6

// memory allocation
#define SCH_ALLOCATION_ERROR         10

// incorrect operation
#define SCH_INVALID_FUNCTION		 20
#define SCH_INVALID_PARAMETER		 21
#define SCH_INVALID_RANGE			 22
#define SCH_REJECTION_METHOD_FAILED	 23

//
// Error codes specific for Simulation Management (ranges from 1000 to 1999 decimal)
//

	// 	SCH_EMPTY_EVENT_LIST : Does not exist process to activate in the Event List.
#define SCH_EMPTY_EVENT_LIST					1000

	// 	SCH_ERROR_IN_FIBER_CREATION : (WINNT only) This error can be associated with the max number of fibers that the
	//  system virtual memory can support. Try change the /STACK parameter to the minimum value allowed:
	//  /STACK = 65536: 4096.  Note that with this value the max value of process is 31250.
#define SCH_ERROR_IN_FIBER_CREATION				1001

	// 	SCH_IMMORTAL_THREAD : (WINNT only) A unsuccessfuly try was done to destroy one thread. (problems in TerminateThread 
	//  or CloseHandle)
#define SCH_IMMORTAL_THREAD						1002

	// 	SCH_INVALID_DESTRUCTOR : A destructor for a SimProcess subtype was called for a
	//  process in Passivate, Scheduled, or Active state.
#define SCH_INVALID_DESTRUCTOR					1003

	// 	SCH_INVALID_ENVIRONMENT : A effort was done to use a simulation primitive outside the simulator environment. 
#define SCH_INVALID_ENVIRONMENT					1004

	// 	SCH_INVALID_LIST : This error may occur because two reasons:
	// 1. It was tried to remove a process that does not exist in the associated list
	// 2. The process suplied as parameter in a clause does not exist in the associated list 
#define SCH_INVALID_LIST						1005

	//	SCH_INVALID_STATE:	A action was tried with a process in a incompatible state.
#define SCH_INVALID_STATE						1006

	//	SCH_SIMULATION_ALREADY_RUNNING:	The startSimulationManager function was called with the simulation process
	//  already running.
#define SCH_SIMULATION_ALREADY_RUNNING			1007

	// 	SCH_THREAD_NOT_CREATED : (WINNT only) A unsuccessfuly try was done to creat a thread. (problems in _beginthreadex) 
#define SCH_THREAD_NOT_CREATED					1008

	// 	SCH_TIME_MUST_GROWS_UP: A user try insert a process in the EventList with its activation
	//  time lower than the system simulation time
#define SCH_TIME_MUST_GROWS_UP					1009
//
// Error codes specific for Random Variable Generation (ranges from 2000 to 2999 decimal)
//

//
// Error codes specific for Statistics Calculation (ranges from 3000 to 3999 decimal)
//
#define SCH_INVALID_INTERVAL         3000
#define SCH_INVALID_INTERVAL_WIDTH	 3001

//
// Error codes specific for Trace & Log (ranges from 4000 to 4999 decimal)
//
#define SCH_INVALID_LOG_LEVEL        4000

//
// Error codes specific for Function Evaluation (ranges from 5000 to 5999 decimal)
//


//## end module%37037F930023.additionalDeclarations


//## begin SchException%37037F930023.preface preserve=yes
//---------------------------------------------------
// Compatibility problem with the cxx UNIX compiler
//---------------------------------------------------
#if defined(ALPHAEV67_OSF50_CXX62)
class SchException : public std::exception
{
#else
//## end SchException%37037F930023.preface

//## Class: SchException%37037F930023
//	********************************************************************************
//
//	This class encapsulates exceptions generated by C++ Scheduler. It takes an error code as parameter on its constructor that may be
//	used to determine the exception cause.
//
//	This class overrides the what() operation, showing a standard message plus the specific error code.
//
//	********************************************************************************
//## Category: Common%37037F140323; Global
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3703818501A5;RetCode { -> }

class SchException : public exception  //## Inherits: <unnamed>%370381DF0032
{
  //## begin SchException%37037F930023.initialDeclarations preserve=yes
#endif
  //## end SchException%37037F930023.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: SchException%3703A8F3013C
      SchException (RetCode errorCode);


    //## Other Operations (specified)
      //## Operation: getError%3703A902038C
      RetCode getError ();

      //## Operation: what%3703AC8500B4
      virtual const char * what ();

    // Additional Public Declarations
      //## begin SchException%37037F930023.public preserve=yes
      //## end SchException%37037F930023.public

  protected:
    // Additional Protected Declarations
      //## begin SchException%37037F930023.protected preserve=yes
      //## end SchException%37037F930023.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: id%3703A6CB0029
      //	Exception code - the errorCode constructor parameter is assigned to this attribute. Accessed by getError() operation.
      //## begin SchException::id%3703A6CB0029.attr preserve=no  private: RetCode {UAC} errorCode
      const RetCode id;
      //## end SchException::id%3703A6CB0029.attr

      //## Attribute: exMsg%3703D13401A8
      //	Actual error message returned by what() operator.
      //## begin SchException::exMsg%3703D13401A8.attr preserve=no  private: string {UA} 
      string exMsg;
      //## end SchException::exMsg%3703D13401A8.attr

    // Additional Private Declarations
      //## begin SchException%37037F930023.private preserve=yes
      //## end SchException%37037F930023.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin SchException%37037F930023.implementation preserve=yes
      //## end SchException%37037F930023.implementation

};

//## begin SchException%37037F930023.postscript preserve=yes
//## end SchException%37037F930023.postscript

// Class SchException 

//## Operation: SchException%3703A8F3013C
//	********************************************************************************
//
//	Name: SchException(RetCode errorCode)
//
//	Description: Non-default constructor - requires exception code. The exception message is built based on the exception code and is
//	stored in exMsg attribute.
//
//	Input parameters: RetCode errorCode;  //exception code
//
//	Returns: none
//
//	********************************************************************************
inline SchException::SchException (RetCode errorCode)
  //## begin SchException::SchException%3703A8F3013C.hasinit preserve=no
      : id(errorCode)
  //## end SchException::SchException%3703A8F3013C.hasinit
  //## begin SchException::SchException%3703A8F3013C.initialization preserve=yes
  //## end SchException::SchException%3703A8F3013C.initialization
{
  //## begin SchException::SchException%3703A8F3013C.body preserve=yes
	
	char tmp[30];
	sprintf (tmp, "Error %d in C++ Scheduler\n", id);
	exMsg = tmp;
	return;

  //## end SchException::SchException%3703A8F3013C.body
}



//## Other Operations (inline)
//## Operation: getError%3703A902038C
//	********************************************************************************
//
//	Name: RetCode getError (void)
//
//	Description: Returns the exception code.
//
//	Parameters: none
//
//	Returns: the value of the id attribute.
//
//	********************************************************************************
inline RetCode SchException::getError ()
{
  //## begin SchException::getError%3703A902038C.body preserve=yes
	return (id);
  //## end SchException::getError%3703A902038C.body
}

//## Operation: what%3703AC8500B4
//	********************************************************************************
//
//	Name: const char * what (void)
//
//	Description: Returns the default C++ Scheduler exception message (which includes the exception code).
//
//	Parameters: none
//
//	Returns: the value of exMsg.data().
//
//	********************************************************************************
inline const char * SchException::what ()
{
  //## begin SchException::what%3703AC8500B4.body preserve=yes
	//
	// data() is the method of the string class the returns the actual string stored
	//
	return (exMsg.data());
  //## end SchException::what%3703AC8500B4.body
}

// Class SchException 



// Additional Declarations
  //## begin SchException%37037F930023.declarations preserve=yes
  //## end SchException%37037F930023.declarations

//## begin module%37037F930023.epilog preserve=yes

} // namespace sch

//## end module%37037F930023.epilog


#endif
