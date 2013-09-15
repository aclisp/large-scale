//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%370DE9B8005D.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%370DE9B8005D.cm

//## begin module%370DE9B8005D.cp preserve=yes
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
//	File Name: ControlProcess.h
//
//	Description:
//
//	Author:	 Hélio Azevedo
//
//	Creation Date:	 03/99
//
//	Visual Source Safe $Revision: 5 $
//
//	Last check in $Date: 14/07/99 8:59 $
//
//	********************************************************************************
//## end module%370DE9B8005D.cp

//## Module: ControlProcess%370DE9B8005D; Pseudo Package specification
//## Source file: T:\SimulationManagement\ControlProcess.h

#ifndef ControlProcess_h
#define ControlProcess_h 1

//## begin module%370DE9B8005D.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%370DE9B8005D.additionalIncludes

//## begin module%370DE9B8005D.includes preserve=yes
#if defined(ALPHAEV67_OSF50_CXX62)
#	define schThreadId_type pthread_t *
#	include <pthread.h>
#elif defined(WIN32)
#	define schThreadId_type unsigned
#	include <windows.h>
#	include <winbase.h>
#	include <process.h>	// needed for _beginthreadex		
#endif

#include <assert.h>
#include <memory>		// (STL) for defining several templates that allocate and free storage for various container classes
//## end module%370DE9B8005D.includes

// Statistics
#include "Statistics.h"
// CountTime
#include "CountTime.h"

//class SimProcess;
//class SimulationManager;

//## begin module%370DE9B8005D.additionalDeclarations preserve=yes
using namespace std;
namespace sch  {

class SimulationManager;
class SimProcess;
//## end module%370DE9B8005D.additionalDeclarations


//## begin ControlProcess%370DE9B8005D.preface preserve=yes
//## end ControlProcess%370DE9B8005D.preface

//## Class: ControlProcess%370DE9B8005D
//	********************************************************************************
//
//	This  class contain the thread used as a parent for all simulation processes create at any simulation run. A dead of this thread
//	causes the finish of the simulation.
//
//	Remarks: Thisclass is platform-dependent - the context depends on the platform.
//
//	********************************************************************************
//## Category: SimulationManagement%369E33A90107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%371F795802C5;Statistics { -> }

class ControlProcess 
{
  //## begin ControlProcess%370DE9B8005D.initialDeclarations preserve=yes
  //## end ControlProcess%370DE9B8005D.initialDeclarations

  public:
    // Additional Public Declarations
      //## begin ControlProcess%370DE9B8005D.public preserve=yes
      //## end ControlProcess%370DE9B8005D.public

  protected:
    //## Constructors (specified)
      //## Operation: ControlProcess%370DF7780148
      ControlProcess ();

    // Additional Protected Declarations
      //## begin ControlProcess%370DE9B8005D.protected preserve=yes
      //## end ControlProcess%370DE9B8005D.protected

  private:
    //## Destructor (specified)
      //## Operation: ~ControlProcess%370DF5FA0239
      virtual ~ControlProcess ();


    //## Other Operations (specified)
      //## Operation: startControlProcess%370E0EF401CF
      RetCode startControlProcess (SimulationManager *pSimulationManager, SimProcess& mainProcess);

      //## Operation: terminateControlProcess%37122DEB0112
      RetCode terminateControlProcess ();

      //## Operation: schedulerThread%370DF5C9030B
      static unsigned WINAPI schedulerThread (LPVOID pControlProcess);

      //## Operation: setClientFiberId%3711FDA50339
      void setClientFiberId (LPVOID fiberIdent);

      //## Operation: getFiberId%371253D5000A
      LPVOID getFiberId ();

      //## Operation: isEnvironmentValid%37132FA502E0
      bool isEnvironmentValid (SimProcess *current);

      //## Operation: createContext%3716279A0017
      RetCode createContext (SimProcess& simProcess, Ulong process, string name, bool destroy);

      //## Operation: goToControlProcess%37177A14032B
      RetCode goToControlProcess ();

      //## Operation: getElapsedTime%371F729A0038
      Int64 getElapsedTime ();

      //## Operation: getFatalError%3727460A0261
      RetCode getFatalError ();

      //## Operation: setFatalError%372746160150
      void setFatalError (RetCode error);

      //## Operation: killProcess%378B6C700234
      RetCode killProcess (SimProcess& simProcess);

    // Data Members for Class Attributes

      //## Attribute: schedulerHandle%370DF7FC0062
      //## begin ControlProcess::schedulerHandle%370DF7FC0062.attr preserve=no  private: HANDLE {UA} NULL
      HANDLE schedulerHandle;
      //## end ControlProcess::schedulerHandle%370DF7FC0062.attr

      //## Attribute: clientFiberId%370E163A0259
      //	Maintain the fiber id of the requesting service  fiber.
      //## begin ControlProcess::clientFiberId%370E163A0259.attr preserve=no  private: LPVOID {UA} 
      LPVOID clientFiberId;
      //## end ControlProcess::clientFiberId%370E163A0259.attr

      //## Attribute: fiberId%370E16D901C1
      //	Maintain the fiber id generated by  ConvertFiberToThread function.
      //	.
      //## begin ControlProcess::fiberId%370E16D901C1.attr preserve=no  private: LPVOID {UA} 
      LPVOID fiberId;
      //## end ControlProcess::fiberId%370E16D901C1.attr

      //## Attribute: schThreadId%3713358E0264
      //	Stores the ID that the systems assigns to the new thread. The type of the variable depends on the plateform used to compile the
      //	code, UNIX or windows. It needs to be defined inside the include file as a #define statement.
      //## begin ControlProcess::schThreadId%3713358E0264.attr preserve=no  private: schThreadId_type {UA} 0
      schThreadId_type schThreadId;
      //## end ControlProcess::schThreadId%3713358E0264.attr

      //## Attribute: fatalError%37273C5E0030
      //	This attribute maintains a scheduler  internal error that can not be reported through trivial return function. The presence of a
      //	error signals that the scheduler execution was aborted.
      //## begin ControlProcess::fatalError%37273C5E0030.attr preserve=no  private: RetCode {UA} 
      RetCode fatalError;
      //## end ControlProcess::fatalError%37273C5E0030.attr

    // Data Members for Associations

      //## Association: SimulationManagement::<unnamed>%370DF8A300C6
      //## Role: ControlProcess::pSimulationMan%370DF8A40398
      //## begin ControlProcess::pSimulationMan%370DF8A40398.role preserve=no  private: SimulationManager {1 -> 1RFHAN}
      SimulationManager *pSimulationMan;
      //## end ControlProcess::pSimulationMan%370DF8A40398.role

      //## Association: SimulationManagement::<unnamed>%37161AC300E3
      //## Role: ControlProcess::pMainProcess%37161AC40116
      //## begin ControlProcess::pMainProcess%37161AC40116.role preserve=no  private: SimProcess {1 -> 1RFHAN}
      SimProcess *pMainProcess;
      //## end ControlProcess::pMainProcess%37161AC40116.role

      //## Association: SimulationManagement::<unnamed>%371F716E02F4
      //## Role: ControlProcess::timeElapsed%371F716F0179
      //## begin ControlProcess::timeElapsed%371F716F0179.role preserve=no  private: CountTime {1 -> 1VHgAN}
      CountTime timeElapsed;
      //## end ControlProcess::timeElapsed%371F716F0179.role

    // Additional Private Declarations
      //## begin ControlProcess%370DE9B8005D.private preserve=yes
      //## end ControlProcess%370DE9B8005D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ControlProcess%370DE9B8005D.implementation preserve=yes
      //## end ControlProcess%370DE9B8005D.implementation

    friend class SimulationManager;
    friend class SimProcess;
};

//## begin ControlProcess%370DE9B8005D.postscript preserve=yes
//## end ControlProcess%370DE9B8005D.postscript

// Class ControlProcess 


//## Other Operations (inline)
//## Operation: getFiberId%371253D5000A
//	********************************************************************************
//
//	Name: LPVOID getFiberId  ()
//
//	Description:Returns the fiber Id associate with this ControlProcess.
//
//	Input parameters: none
//
//	Returns: fiberId
//
//	Remarks: This function is platform-dependent - the context depends on the platform. Only the Simulation Manager is allowed to run
//	this function.
//
//	********************************************************************************
inline LPVOID ControlProcess::getFiberId ()
{
  //## begin ControlProcess::getFiberId%371253D5000A.body preserve=yes
		return fiberId;
  //## end ControlProcess::getFiberId%371253D5000A.body
}

//## Operation: getElapsedTime%371F729A0038
//	********************************************************************************
//
//	Name: Int64 getElapsedTime  ()
//
//	Description:Returns the elapsed time since the thread creation
//
//	Input parameters: none
//
//	Returns: the elapsed time.
//
//	Remarks: This function is platform-dependent - the context depends on the platform. Only the Simulation Manager is allowed to run
//	this function.
//
//	********************************************************************************
inline Int64 ControlProcess::getElapsedTime ()
{
  //## begin ControlProcess::getElapsedTime%371F729A0038.body preserve=yes
	return (schedulerHandle == NULL) ?				  // Already defunct  ?
		timeElapsed.getCount():
		timeElapsed.calculateCount(schedulerHandle);
		
		

  //## end ControlProcess::getElapsedTime%371F729A0038.body
}

//## Operation: getFatalError%3727460A0261
//	********************************************************************************
//
//	Name:RetCode getFatalError  ()
//
//	Description:Returns the contents of the attribute fatalError.  This attribute maintains a scheduler  internal error that can not be
//	reported through trivial return function. The presence of a error signals that the scheduler execution was aborted
//
//	Input parameters: none
//
//	Returns: the elapsed time.
//
//	Remarks: This function is platform-dependent - the context depends on the platform. Only the Simulation Manager is allowed to run
//	this function.
//
//	********************************************************************************
inline RetCode ControlProcess::getFatalError ()
{
  //## begin ControlProcess::getFatalError%3727460A0261.body preserve=yes
	return fatalError;
  //## end ControlProcess::getFatalError%3727460A0261.body
}

//## Operation: setFatalError%372746160150
//	********************************************************************************
//
//	Name: void setFatalError (error RetCode)
//
//	Description: Update the fatalError  with the thread  abort reason.
//
//	Input parameters:error = error code
//
//	Returns:none
//
//	Remarks: This function is platform-dependent - the context depends on the platform. Only the Simulation Manager is allowed to run
//	this function.
//
//	********************************************************************************
inline void ControlProcess::setFatalError (RetCode error)
{
  //## begin ControlProcess::setFatalError%372746160150.body preserve=yes
	fatalError = error;
  //## end ControlProcess::setFatalError%372746160150.body
}

//## begin module%370DE9B8005D.epilog preserve=yes
}	// namespace sch

//## end module%370DE9B8005D.epilog


#endif
