//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%369E357E00ED.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%369E357E00ED.cm

//## begin module%369E357E00ED.cp preserve=yes
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
//	File Name: SimProcess.h
//
//	Description:
//
//	Author:	  Hélio Azevedo
//
//	Creation Date: 03/99
//
//	Visual Source Safe $Revision: 7 $
//
//	Last check in $Date: 10/04/99 2:47p $
//
//	********************************************************************************
//## end module%369E357E00ED.cp

//## Module: SimProcess%369E357E00ED; Pseudo Package specification
//## Source file: T:\SimulationManagement\SimProcess.h

#ifndef SimProcess_h
#define SimProcess_h 1

//## begin module%369E357E00ED.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%369E357E00ED.additionalIncludes

//## begin module%369E357E00ED.includes preserve=yes
#if defined(WIN32)
#include <windows.h>
#include <winbase.h>
#include <assert.h>
#endif

#if defined(ALPHAEV67_OSF50_CXX62)
#include <pthread.h>
#endif
//## end module%369E357E00ED.includes

// ProcessState
#include "ProcessState.h"
// SimulationManager
#include "SimulationManager.h"

//class ControlProcess;
//class ProcessList;

//## begin module%369E357E00ED.additionalDeclarations preserve=yes

// defines a namespace for the scheduler 
namespace sch  {

class ControlProcess;
class ProcessList;
class SimulationManager;
const Int STACK_THREAD = 4096;		// allocates one page for stack thread. This size

									// is inherited by all fibers created.
#if defined(ALPHAEV67_OSF50_CXX62)
	static unsigned fiberNumber = 0;
	static pthread_t fiberIdent[20];
	LPVOID ConvertThreadToFiber( LPVOID lpParameter );
	VOID SwitchToFiber( LPVOID lpFiber );
	PVOID GetCurrentFiber(VOID);
	VOID DeleteFiber(LPVOID lpfiber);
	LPVOID CreateFiber( DWORD,   void (*lpStartAddress)(void*),  LPVOID); 
	#define GetCurrentThreadId GetCurrentFiber
#endif



//## end module%369E357E00ED.additionalDeclarations


//## begin SimProcess%369E357E00ED.preface preserve=yes
//## end SimProcess%369E357E00ED.preface

//## Class: SimProcess%369E357E00ED; Abstract
//	********************************************************************************
//
//	This is an abstract class encapsulating the common behavior of all processes. The actual processes are defined by the simulation
//	application and inherit the SimProcess behavior.
//
//	Friend of Simulation Manager.
//
//	********************************************************************************
//## Category: SimulationManagement%369E33A90107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%36BAEB480235;ProcessState { -> }
//## Uses: <unnamed>%37276C940271;SimulationManager { -> }

class SimProcess 
{
  //## begin SimProcess%369E357E00ED.initialDeclarations preserve=yes
  //## end SimProcess%369E357E00ED.initialDeclarations

  public:
    //## Destructor (specified)
      //## Operation: ~SimProcess%370D0205011B
      virtual ~SimProcess ();


    //## Other Operations (specified)
      //## Operation: getActivationTime%36ADEE6703BA
      Double getActivationTime ();

      //## Operation: getState%36ADEE85033A
      ProcessState getState ();

      //## Operation: getProcessName%3728A60402E9
      string getProcessName ();

      //## Operation: getpSimulationManager%3731CE8F00FB
      SimulationManager * getpSimulationManager ();

      //## Operation: getDestroyFlag%373340C9033A
      bool getDestroyFlag ();

    // Additional Public Declarations
      //## begin SimProcess%369E357E00ED.public preserve=yes
      //## end SimProcess%369E357E00ED.public

  protected:
    //## Constructors (specified)
      //## Operation: SimProcess%36FF7ED70247
      SimProcess ();

    // Additional Protected Declarations
      //## begin SimProcess%369E357E00ED.protected preserve=yes
      //## end SimProcess%369E357E00ED.protected

  private:

    //## Other Operations (specified)
      //## Operation: initialize%370E608C0062
      void initialize (Double activeTime, ProcessState sta, LPVOID fId, sch::ControlProcess *pControl, Ulong procId, string name, bool destroy);

      //## Operation: setActivationTime%36ADEF0300ED
      void setActivationTime (Double newTime);

      //## Operation: setState%36ADEEB3012E
      RetCode setState (ProcessState newState);

      //## Operation: getFiberId%37137E2D013A
      LPVOID getFiberId ();

      //## Operation: fiberProcess%370BA862010E
      static VOID WINAPI fiberProcess (PVOID pSimProcess);

      //## Operation: switchContext%36CD6B3701A2
      RetCode switchContext (SimProcess& targetProcess);

      //## Operation: freeContext%3714B2960265
      RetCode freeContext ();

      //## Operation: bodyProcess%36ADEEF00186
      virtual void bodyProcess () = 0;

    // Data Members for Class Attributes

      //## Attribute: activationTime%36ADED67011C
      //	Activation time of the process. It depends on the process state:
      //	  - Active: equal to the system simulation time
      //	  - Scheduled: time at which the process will become active
      //	  - Passive: ignored
      //## begin SimProcess::activationTime%36ADED67011C.attr preserve=no  private: Double {UA} 
      Double activationTime;
      //## end SimProcess::activationTime%36ADED67011C.attr

      //## Attribute: state%36ADED770210
      //	Current state of the process. It can be Active, Scheduled or Passive. Every process is created in the passive state.
      //## begin SimProcess::state%36ADED770210.attr preserve=no  private: ProcessState {UA} NonInitialized
      ProcessState state;
      //## end SimProcess::state%36ADED770210.attr

      //## Attribute: fiberId%370B614502AB
      //	Maintain the fiber id associated with the simulator process.
      //## begin SimProcess::fiberId%370B614502AB.attr preserve=no  private: LPVOID {UA} 
      LPVOID fiberId;
      //## end SimProcess::fiberId%370B614502AB.attr

      //## Attribute: processId%370DDB7C0310
      //	Identifies the process inside the scheduler.
      //## begin SimProcess::processId%370DDB7C0310.attr preserve=no  private: Ulong {UA} 0
      Ulong processId;
      //## end SimProcess::processId%370DDB7C0310.attr

      //## Attribute: pNext%371B4273026F
      //	pointer to next element present at list.
      //	pointer to next element present in the SimProcess list.
      //## begin SimProcess::pNext%371B4273026F.attr preserve=no  private: SimProcess * {UA} NULL
      SimProcess *pNext;
      //## end SimProcess::pNext%371B4273026F.attr

      //## Attribute: pPrior%371B42AE0116
      //	pointer to previous element present in the SimProcess list.
      //## begin SimProcess::pPrior%371B42AE0116.attr preserve=no  private: SimProcess * {UA} NULL
      SimProcess *pPrior;
      //## end SimProcess::pPrior%371B42AE0116.attr

      //## Attribute: processName%371CC3AF0105
      //	Define a name for the process.
      //## begin SimProcess::processName%371CC3AF0105.attr preserve=no  private: string {UA} 
      string processName;
      //## end SimProcess::processName%371CC3AF0105.attr

      //## Attribute: destroyFlag%37333FE40327
      //	Indicate if the associated SimProcess class must be destroyed at theprocess  termination.
      //## begin SimProcess::destroyFlag%37333FE40327.attr preserve=no  private: bool {UA} false
      bool destroyFlag;
      //## end SimProcess::destroyFlag%37333FE40327.attr

    // Data Members for Associations

      //## Association: SimulationManagement::<unnamed>%371640A90333
      //## Role: SimProcess::pControlProcess%371640AB006E
      //## begin SimProcess::pControlProcess%371640AB006E.role preserve=no  private: ControlProcess {1 -> 1RFHAN}
      ControlProcess *pControlProcess;
      //## end SimProcess::pControlProcess%371640AB006E.role

      //## Association: SimulationManagement::<unnamed>%371B76C5002A
      //## Role: SimProcess::pProcessList%371B76C80093
      //## begin SimProcess::pProcessList%371B76C80093.role preserve=no  private: ProcessList {1 -> 1RFHAN}
      ProcessList *  pProcessList;
      //## end SimProcess::pProcessList%371B76C80093.role

    // Additional Private Declarations
      //## begin SimProcess%369E357E00ED.private preserve=yes
      //## end SimProcess%369E357E00ED.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin SimProcess%369E357E00ED.implementation preserve=yes
      //## end SimProcess%369E357E00ED.implementation

    friend class ProcessList;
    friend class SimulationManager;
    friend class ControlProcess;
};

//## begin SimProcess%369E357E00ED.postscript preserve=yes
//## end SimProcess%369E357E00ED.postscript

// Class SimProcess 


//## Other Operations (inline)
//## Operation: getActivationTime%36ADEE6703BA
//	********************************************************************************
//
//	Name: Double getActivationTime(void)
//
//	Description: Returns the activation time of this process.
//
//	Parameters: none
//
//	Returns: the value of the activationTime attribute.
//
//	********************************************************************************
inline Double SimProcess::getActivationTime ()
{
  //## begin SimProcess::getActivationTime%36ADEE6703BA.body preserve=yes
	return activationTime;
  //## end SimProcess::getActivationTime%36ADEE6703BA.body
}

//## Operation: getState%36ADEE85033A
//	********************************************************************************
//
//	Name: ProcessState getState(void)
//
//	Description: Returns the current state of the process.
//
//	Parameters: none
//
//	Returns: the value of the state attribute.
//
//	********************************************************************************
inline ProcessState SimProcess::getState ()
{
  //## begin SimProcess::getState%36ADEE85033A.body preserve=yes
	return state;
  //## end SimProcess::getState%36ADEE85033A.body
}

//## Operation: getFiberId%37137E2D013A
//	********************************************************************************
//
//	Name: LPVOID getFiberId  ()
//
//	Description:Returns the fiber Id associate with this SimProcess.
//
//	Input parameters: none
//
//	Returns: fiberId
//
//	Remarks: This function is platform-dependent - the context depends on the platform. Only the Simulation Manager is allowed to run
//	this function.
//
//	********************************************************************************
inline LPVOID SimProcess::getFiberId ()
{
  //## begin SimProcess::getFiberId%37137E2D013A.body preserve=yes
	return fiberId;
  //## end SimProcess::getFiberId%37137E2D013A.body
}

//## Operation: getProcessName%3728A60402E9
//	********************************************************************************
//
//	Name:string getState(void)
//
//	Description: Returns the  process name.
//
//	Parameters: none
//
//	Returns: the value of the processName attribute.
//
//	********************************************************************************
inline string SimProcess::getProcessName ()
{
  //## begin SimProcess::getProcessName%3728A60402E9.body preserve=yes
	return 	processName;
  //## end SimProcess::getProcessName%3728A60402E9.body
}

//## Operation: getDestroyFlag%373340C9033A
//	********************************************************************************
//
//	Name: bool geDestroyFlag(void)
//
//	Description: Returns the value of destroyFlag
//
//	Parameters: none
//
//	Returns: the value of the destroyFlag attribute.
//
//	********************************************************************************
inline bool SimProcess::getDestroyFlag ()
{
  //## begin SimProcess::getDestroyFlag%373340C9033A.body preserve=yes
	return destroyFlag;
  //## end SimProcess::getDestroyFlag%373340C9033A.body
}

//## begin module%369E357E00ED.epilog preserve=yes
}	// namespace sch

//## end module%369E357E00ED.epilog


#endif
