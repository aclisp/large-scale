//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%369E34A700DA.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%369E34A700DA.cm

//## begin module%369E34A700DA.cp preserve=yes
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
//	File Name: SimulationManager.h
//
//	Description:
//
//	Author:	  Hélio Azevedo
//
//	Creation Date: 03/99
//
//	Visual Source Safe $Revision: 7 $
//
//	Last check in $Date: 4/06/99 13:45 $
//
//	********************************************************************************
//## end module%369E34A700DA.cp

//## Module: SimulationManager%369E34A700DA; Pseudo Package specification
//## Source file: T:\SimulationManagement\SimulationManager.h

#ifndef SimulationManager_h
#define SimulationManager_h 1

//## begin module%369E34A700DA.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%369E34A700DA.additionalIncludes

//## begin module%369E34A700DA.includes preserve=yes
#include <memory>	// (STL) for defining several templates that allocate and free storage for various container classes
//## end module%369E34A700DA.includes

// Statistics
#include "Statistics.h"
// ControlProcess
#include "ControlProcess.h"
// PositionType
#include "PositionType.h"
// ClauseType
#include "ClauseType.h"
// Clause
#include "Clause.h"
// SimProcess
#include "SimProcess.h"
// ProcessList
#include "ProcessList.h"


//## begin module%369E34A700DA.additionalDeclarations preserve=yes

using namespace std;
namespace sch  {

//## end module%369E34A700DA.additionalDeclarations


//## begin SimulationManager%369E34A700DA.preface preserve=yes
//## end SimulationManager%369E34A700DA.preface

//## Class: SimulationManager%369E34A700DA
//	********************************************************************************
//	 Simulation Management class supplies a set of functions for process-oriented simulation.   It controls:
//	- the Event List
//	- the Passive List
//	- beginning and ending of the simulation
//	- process initialization
//
//	This class has the public interface for the client application of Simulation Management.
//
//	********************************************************************************
//## Category: SimulationManagement%369E33A90107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%36B1F04F0361;Clause { -> }
//## Uses: <unnamed>%37011A0D039C;ClauseType { -> }
//## Uses: <unnamed>%37011A110045;PositionType { -> }
//## Uses: <unnamed>%371F737801BE;Statistics { -> }
//## Uses: <unnamed>%3731DE3D02FC;SimProcess { -> F}

class SimulationManager 
{
  //## begin SimulationManager%369E34A700DA.initialDeclarations preserve=yes
  //## end SimulationManager%369E34A700DA.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: SimulationManager%36FF7B0B0131
      SimulationManager ();

    //## Destructor (specified)
      //## Operation: ~SimulationManager%3729B761039B
      virtual ~SimulationManager ();


    //## Other Operations (specified)
      //## Operation: startSimulationManager%36ADEBC101B6
      RetCode startSimulationManager (SimProcess& mainProcess);

      //## Operation: endSimulationManager%36ADECA20278
      RetCode endSimulationManager ();

      //## Operation: isRunning%3724670802BA
      bool isRunning ();

      //## Operation: getSystemSimulationTime%36ADECEE0137
      Double getSystemSimulationTime () const;

      //## Operation: getStatistics%371F53FB007D
      RetCode getStatistics (Statistics& statResp);

      //## Operation: startProcess%36ADEC7E0276
      RetCode startProcess (SimProcess& newProcess, string name, bool destroy = false);

      //## Operation: terminateProcess%36D16E0400FD
      RetCode terminateProcess (SimProcess &targetProcess);

      //## Operation: terminateProcess%3729DD2503E3
      RetCode terminateProcess ();

      //## Operation: activate%36ADEC05018C
      RetCode activate (SimProcess& targetProcess);

      //## Operation: passivate%36ADED120188
      RetCode passivate ();

      //## Operation: passivate%3737135901ED
      RetCode passivate (SimProcess& targetProcess);

      //## Operation: schedule%36ADED1F00DD
      RetCode schedule (SimProcess& targetProcess, const Clause& position);

      //## Operation: hold%36ADED0702A5
      RetCode hold (const Clause& position);

    // Additional Public Declarations
      //## begin SimulationManager%369E34A700DA.public preserve=yes
      //## end SimulationManager%369E34A700DA.public

  protected:
    // Additional Protected Declarations
      //## begin SimulationManager%369E34A700DA.protected preserve=yes
      //## end SimulationManager%369E34A700DA.protected

  private:

    //## Other Operations (specified)
      //## Operation: updateActiveProcess%370CB53903A6
      RetCode updateActiveProcess (SimProcess * &pNewActive);

      //## Operation: countTerminateProc%371F7D8900A1
      void countTerminateProc ();

    // Data Members for Class Attributes

      //## Attribute: systemSimulationTime%36ADEB6B0285
      //	Indicates the current system simulation time.
      //## begin SimulationManager::systemSimulationTime%36ADEB6B0285.attr preserve=no  private: Double {UA} 0.0
      Double systemSimulationTime;
      //## end SimulationManager::systemSimulationTime%36ADEB6B0285.attr

      //## Attribute: totalProcess%370DD9A70167
      //	maintain the number of process created until the present moment.
      //## begin SimulationManager::totalProcess%370DD9A70167.attr preserve=no  private: Ulong {UA} 0
      Ulong totalProcess;
      //## end SimulationManager::totalProcess%370DD9A70167.attr

      //## Attribute: totalProcessTerminated%371F1C850149
      //	Maintain the number of process created until the present moment.
      //## begin SimulationManager::totalProcessTerminated%371F1C850149.attr preserve=no  private: Ulong {UA} 0
      Ulong totalProcessTerminated;
      //## end SimulationManager::totalProcessTerminated%371F1C850149.attr

      //## Attribute: running%371F7D5500ED
      //	Indicates if the simulation is still running.
      //## begin SimulationManager::running%371F7D5500ED.attr preserve=no  private: bool {UA} false
      bool running;
      //## end SimulationManager::running%371F7D5500ED.attr

    // Data Members for Associations

      //## Association: SimulationManagement::<unnamed>%36ADF41E0331
      //## Role: SimulationManager::eventList%36ADF4200081
      //	A ProcessList object that maintains processes to be scheduled for execution at specific simulation times.
      //## begin SimulationManager::eventList%36ADF4200081.role preserve=no  private: ProcessList {1 -> 1VHgAN}
      ProcessList eventList;
      //## end SimulationManager::eventList%36ADF4200081.role

      //## Association: SimulationManagement::<unnamed>%36B87F740099
      //## Role: SimulationManager::passiveList%36B87F7403C4
      //	A ProcessList object that maintains the passive processes.
      //## begin SimulationManager::passiveList%36B87F7403C4.role preserve=no  private: ProcessList {1 -> 1VHgAN}
      ProcessList passiveList;
      //## end SimulationManager::passiveList%36B87F7403C4.role

      //## Association: SimulationManagement::<unnamed>%36CAF7AC005C
      //## Role: SimulationManager::pActiveProcess%36CAF7AD0004
      //	Pointer to the active SimProcess object.
      //## begin SimulationManager::pActiveProcess%36CAF7AD0004.role preserve=no  private: SimProcess {1 -> 1RHAN}
      SimProcess *pActiveProcess;
      //## end SimulationManager::pActiveProcess%36CAF7AD0004.role

      //## Association: SimulationManagement::<unnamed>%3711DE0B01A6
      //## Role: SimulationManager::controlProcess%3711DE0C016B
      //## begin SimulationManager::controlProcess%3711DE0C016B.role preserve=no  private: ControlProcess {1 -> 1VHgAN}
      ControlProcess controlProcess;
      //## end SimulationManager::controlProcess%3711DE0C016B.role

    // Additional Private Declarations
      //## begin SimulationManager%369E34A700DA.private preserve=yes
      //## end SimulationManager%369E34A700DA.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin SimulationManager%369E34A700DA.implementation preserve=yes
      //## end SimulationManager%369E34A700DA.implementation

    friend class ControlProcess;
    friend class ProcessList;
};

//## begin SimulationManager%369E34A700DA.postscript preserve=yes
//## end SimulationManager%369E34A700DA.postscript

// Class SimulationManager 


//## Other Operations (inline)
//## Operation: isRunning%3724670802BA
//	********************************************************************************
//
//	Name:bool getStatistics(void)
//
//	Description: Returns if the simulation task is still running.
//
//	Parameters: none.
//
//	 Returns: true: the simulation task is running
//	        fase : the simulation task is not running.
//
//	********************************************************************************
inline bool SimulationManager::isRunning ()
{
  //## begin SimulationManager::isRunning%3724670802BA.body preserve=yes

	return 	 running;

  //## end SimulationManager::isRunning%3724670802BA.body
}

//## Operation: getSystemSimulationTime%36ADECEE0137
//	********************************************************************************
//
//	Name: Double getSystemSimulationTime(void)
//
//	Description: Returns the system simulation time.
//
//	Parameters: none
//
//	Returns: the value of the systemSimulationTime attribute.
//
//	********************************************************************************
inline Double SimulationManager::getSystemSimulationTime () const
{
  //## begin SimulationManager::getSystemSimulationTime%36ADECEE0137.body preserve=yes
	
	return	systemSimulationTime;
	
  //## end SimulationManager::getSystemSimulationTime%36ADECEE0137.body
}

//## Operation: countTerminateProc%371F7D8900A1
//	********************************************************************************
//
//	Name: void countTerminateProc ()getStatistics(void)
//
//	Description: increments totalProcessTerminated  for each process terminated
//
//	Parameters: none.
//
//	 Returns:none
//
//	********************************************************************************
inline void SimulationManager::countTerminateProc ()
{
  //## begin SimulationManager::countTerminateProc%371F7D8900A1.body preserve=yes

	totalProcessTerminated++;

  //## end SimulationManager::countTerminateProc%371F7D8900A1.body
}

//## begin module%369E34A700DA.epilog preserve=yes
}	// namespace sch
//## end module%369E34A700DA.epilog


#endif
