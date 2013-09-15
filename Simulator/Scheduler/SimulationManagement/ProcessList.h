//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%369E35270142.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%369E35270142.cm

//## begin module%369E35270142.cp preserve=yes
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
//	File Name: ProcessList.h
//
//	Description:
//
//	Author:	 Hélio Azevedo
//
//	Creation Date: 03/99
//
//	Visual Source Safe $Revision: 6 $
//
//	Last check in $Date: 13/05/99 9:24 $
//
//	********************************************************************************
//## end module%369E35270142.cp

//## Module: ProcessList%369E35270142; Pseudo Package specification
//## Source file: T:\SimulationManagement\ProcessList.h

#ifndef ProcessList_h
#define ProcessList_h 1

//## begin module%369E35270142.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%369E35270142.additionalIncludes

//## begin module%369E35270142.includes preserve=yes
#include <memory>	// (STL) for defining several templates that allocate and free storage for various container classes
//## end module%369E35270142.includes

// Clause
#include "Clause.h"

//class SimProcess;
//class SimulationManager;

//## begin module%369E35270142.additionalDeclarations preserve=yes
using namespace std;
namespace sch  {
	class SimulationManager;		//forward reference to Simulationmanager
	class SimProcess;

//## end module%369E35270142.additionalDeclarations


//## begin ProcessList%369E35270142.preface preserve=yes
//## end ProcessList%369E35270142.preface

//## Class: ProcessList%369E35270142
//	********************************************************************************
//
//	Controls the insertion and removal of a process in the list. This class can only be instantiated by the SimulationManager class.
//
//	Friend of SimulationManager.
//
//	********************************************************************************
//## Category: SimulationManagement%369E33A90107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%36B1F05C0265;Clause { -> }

class ProcessList 
{
  //## begin ProcessList%369E35270142.initialDeclarations preserve=yes
  //## end ProcessList%369E35270142.initialDeclarations

  public:
    // Additional Public Declarations
      //## begin ProcessList%369E35270142.public preserve=yes
      //## end ProcessList%369E35270142.public

  protected:
    //## Constructors (specified)
      //## Operation: ProcessList%36E00B1B0249
      ProcessList ();

    // Additional Protected Declarations
      //## begin ProcessList%369E35270142.protected preserve=yes
      //## end ProcessList%369E35270142.protected

  private:

    //## Other Operations (specified)
      //## Operation: startProcessList%37139823014A
      void startProcessList (const SimulationManager *  pSimulationManager);

      //## Operation: insert%36ADF2140358
      RetCode insert (SimProcess& targetProcess, const Clause& position);

      //## Operation: insertHead%36ADF11701A6
      void insertHead (SimProcess& targetProcess);

      //## Operation: remove%36ADF25A01BF
      RetCode remove (SimProcess& targetProcess);

      //## Operation: removeHead%36ADF28302EA
      SimProcess * removeHead ();

      //## Operation: isEmpty%371476F3021C
      bool isEmpty ();

    // Data Members for Associations

      //## Association: SimulationManagement::<unnamed>%36ADF2EA01C6
      //## Role: ProcessList::pHead%36ADF2EC00F7
      //	Pointer to a SimProcess object that is the head of the ProcessList.
      //## begin ProcessList::pHead%36ADF2EC00F7.role preserve=no  private: SimProcess {1 -> 0..1RFHAN}
      SimProcess *pHead;
      //## end ProcessList::pHead%36ADF2EC00F7.role

      //## Association: SimulationManagement::<unnamed>%36B8882501ED
      //## Role: ProcessList::pTail%36B8882A01A4
      //	Pointer to a SimProcess object that is the tail of the ProcessList.
      //## begin ProcessList::pTail%36B8882A01A4.role preserve=no  private: SimProcess {1 -> 0..1RFHAN}
      SimProcess *pTail;
      //## end ProcessList::pTail%36B8882A01A4.role

      //## Association: SimulationManagement::<unnamed>%371C558102F2
      //## Role: ProcessList::pSimulationMan%371C5583027D
      //## begin ProcessList::pSimulationMan%371C5583027D.role preserve=no  private: SimulationManager {1 -> 1RFHAN}
      const SimulationManager *  pSimulationMan;
      //## end ProcessList::pSimulationMan%371C5583027D.role

      //## Association: SimulationManagement::<unnamed>%371C7EB5010F
      //## Role: ProcessList::pLastInserted%371C7EB80113
      //## begin ProcessList::pLastInserted%371C7EB80113.role preserve=no  private: SimProcess {1 -> 0..1RFHAN}
      SimProcess *pLastInserted;
      //## end ProcessList::pLastInserted%371C7EB80113.role

    // Additional Private Declarations
      //## begin ProcessList%369E35270142.private preserve=yes
      //## end ProcessList%369E35270142.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ProcessList%369E35270142.implementation preserve=yes
      //## end ProcessList%369E35270142.implementation

    friend class SimulationManager;
    friend class SimulationManager;
};

//## begin ProcessList%369E35270142.postscript preserve=yes
//## end ProcessList%369E35270142.postscript

// Class ProcessList 

//## Operation: ProcessList%36E00B1B0249
//	********************************************************************************
//
//	Name: ProcessList(void)
//
//	Description: Default constructor - initializes pHead and pTail pointers.
//
//	Parameters: none
//
//	Returns: none
//
//	Remarks: this constructor is protected in order to avoid it to be instantiated by a class other than SimulationManager (which is
//	friend of ProcessList).
//
//	********************************************************************************
inline ProcessList::ProcessList ()
  //## begin ProcessList::ProcessList%36E00B1B0249.hasinit preserve=no
  //## end ProcessList::ProcessList%36E00B1B0249.hasinit
  //## begin ProcessList::ProcessList%36E00B1B0249.initialization preserve=yes
   : pHead (NULL) , pTail(NULL)
  //## end ProcessList::ProcessList%36E00B1B0249.initialization
{
  //## begin ProcessList::ProcessList%36E00B1B0249.body preserve=yes
  //## end ProcessList::ProcessList%36E00B1B0249.body
}



//## Other Operations (inline)
//## Operation: startProcessList%37139823014A
//	********************************************************************************
//
//	Name: void startProcessList (const SimulationManager*  pSimulationManager)
//
//	Description:Initializes all attributes of this class allowing a new cicle of simulation.
//
//	Input parameters: SimulationManager*  pSimulationManager //Determines which is the SimulationManager that SimProcess is associated
//
//	Returns: none
//
//	********************************************************************************
inline void ProcessList::startProcessList (const SimulationManager *  pSimulationManager)
{
  //## begin ProcessList::startProcessList%37139823014A.body preserve=yes
	pLastInserted = pHead = pTail = NULL;
	pSimulationMan = pSimulationManager;

  //## end ProcessList::startProcessList%37139823014A.body
}

//## Operation: isEmpty%371476F3021C
//	********************************************************************************
//
//	Name: bool isEmpty ()
//
//	Description: Verifies if the list is empty.
//
//	Input parameters: none
//
//	Returns: true : list is empty
//	        false : there is elements in the list.
//
//	********************************************************************************
inline bool ProcessList::isEmpty ()
{
  //## begin ProcessList::isEmpty%371476F3021C.body preserve=yes
	return (pHead == NULL) ? true : false;
  //## end ProcessList::isEmpty%371476F3021C.body
}

//## begin module%369E35270142.epilog preserve=yes
}	// namespace sch

//## end module%369E35270142.epilog


#endif
