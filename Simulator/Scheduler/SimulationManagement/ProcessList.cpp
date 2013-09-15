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
//	File Name: ProcessList.cpp
//
//	Description:
//
//	Author:	 Hélio Azevedo
//
//	Creation Date:	03/99
//
//	Visual Source Safe $Revision: 6 $
//
//	Last check in $Date: 13/05/99 9:24 $
//
//	********************************************************************************
//## end module%369E35270142.cp

//## Module: ProcessList%369E35270142; Pseudo Package body
//## Source file: T:\SimulationManagement\ProcessList.cpp

//## begin module%369E35270142.additionalIncludes preserve=no
//## end module%369E35270142.additionalIncludes

//## begin module%369E35270142.includes preserve=yes
//## end module%369E35270142.includes
#include "MemLeak.h"
// SimProcess
#include "SimProcess.h"
// ProcessList
#include "ProcessList.h"
// SimulationManager
#include "SimulationManager.h"
//## begin module%369E35270142.additionalDeclarations preserve=yes
namespace sch  {

//## end module%369E35270142.additionalDeclarations


// Class ProcessList 









//## Other Operations (implementation)
//## Operation: insert%36ADF2140358
//	********************************************************************************
//
//	Name: RetCode insert (SimProcess& targetProcess, const Clause& position)
//
//	Description: Inserts the target process in the Process List at the position determined by the clause.
//
//	Input parameters: SimProcess& targetProcess; //process to be inserted
//	                  const Clause& position;    //position to insert the process in the Process List
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	********************************************************************************
RetCode ProcessList::insert (SimProcess& targetProcess, const Clause& position)
{
  //## begin ProcessList::insert%36ADF2140358.body preserve=yes

	SimProcess *pInsertionAhead;						// pointer to insertion point
	SimProcess *pForward, *pBackward;
	Double targetInsertionTime = position.simulationTime;  // defines the insertion simulation time
	
	assert(targetProcess.pProcessList == NULL);			// does targetProcess already belongs to another list  ?

	switch (position.clauseType) {				// what clauseType we are talking about ?
	case RefProcess:						// the insertion point is defined by a reference process
		if (position.pReferenceProcess->pProcessList != this)	
			return	SCH_INVALID_LIST;		// target is not in this list

		// updates new activation time
		targetProcess.setActivationTime( position.pReferenceProcess->getActivationTime());
	
		pInsertionAhead =					// defines the insertion point
			(position.position==After)? position.pReferenceProcess : position.pReferenceProcess->pPrior;
	
		break;

	case DelayTime:						// the insertion point is defined by a delay time
		 targetInsertionTime += pSimulationMan->getSystemSimulationTime();
		// From this point ahead the behavior is the same as ActivationTime clauseType


	case ActivationTime:					// the insertion point is defined by a activation time
			
		// verifies if time is growing up
		if ( targetInsertionTime < pSimulationMan->getSystemSimulationTime()) 
			return SCH_TIME_MUST_GROWS_UP;
		
		// updates new activation time
		targetProcess.setActivationTime( targetInsertionTime);

		// verifies if the list is empty...
		if	(pHead == NULL)	{							// is empty list ?
			pInsertionAhead = NULL;						// found the point
			break;										// switch break	 
		}

		// seach the insertion point in two directions
		pForward = pHead;
		pBackward = pTail;
		
		
		// try gains time using last insertion point
		if (pLastInserted != NULL)	{					// pLastInserted is still valid
			assert(pLastInserted->pProcessList == this);		
			if 	( pLastInserted->activationTime < targetInsertionTime)
				pForward = pLastInserted;
			else
				pBackward = pLastInserted;
		}

		// look for insertion target
		while (	pForward != pBackward ) {
			if (pForward->activationTime < targetInsertionTime) 
				pForward = pForward->pNext;
			else {										// find the point
				pInsertionAhead = pForward;
				break;
			}

			if (pBackward->activationTime > targetInsertionTime) 
				pBackward = pBackward->pPrior;
			else {										// find the point
				pInsertionAhead = pBackward;
				break;
			}
		}

		// defines the exactly position 
		if (pForward == pBackward) 						// prevent the case of pForward == pBackward
				 		pInsertionAhead = pForward;						
		

		if 	(position.position==After ) {				// must insert after the insertion point block
			while (pInsertionAhead->activationTime <= targetInsertionTime )
				if ( pInsertionAhead->pNext== NULL)				// if is the Tail 
					break;										// break while
				else
					pInsertionAhead = pInsertionAhead->pNext;
				
			if ( pInsertionAhead->pNext != NULL || (pInsertionAhead->activationTime > targetInsertionTime))
				pInsertionAhead =  pInsertionAhead->pPrior;		// go back one element
		}
		else {								// must insert before the insertion point block
			while (pInsertionAhead->activationTime >= targetInsertionTime )
				if ( pInsertionAhead->pPrior == NULL) {			// insert at Head
					pInsertionAhead = NULL;
					break;										// break while
				}
				else
					pInsertionAhead = pInsertionAhead->pPrior;
		}

		break;

	}  // end case

	// insert at defined position
	if (pInsertionAhead == NULL) 							// insert at Head
		insertHead(targetProcess);
	else {
		targetProcess.pNext = pInsertionAhead->pNext;
		targetProcess.pPrior = pInsertionAhead;
		if (pInsertionAhead->pNext != NULL)
			(pInsertionAhead->pNext)->pPrior = &targetProcess;
		else												// pInsertionAhead is the list tail
			pTail = &targetProcess;
		pInsertionAhead->pNext = &targetProcess;
	}

	pLastInserted = &targetProcess;
	targetProcess.pProcessList = this;			// saves link to current list 

	return SCH_SUCCESS;

  //## end ProcessList::insert%36ADF2140358.body
}

//## Operation: insertHead%36ADF11701A6
//	********************************************************************************
//
//	Name:void insertHead (SimProcess& targetProcess)
//
//	Description: Inserts the target process at the head of the Process List.
//
//	Input parameters: SimProcess& targetProcess;  //process to be inserted
//
//	Returns: none
//
//	********************************************************************************
void ProcessList::insertHead (SimProcess& targetProcess)
{
  //## begin ProcessList::insertHead%36ADF11701A6.body preserve=yes
	assert(targetProcess.pProcessList == NULL);			// does targetProcess already belongs to another list  ?

	if (pHead == NULL) {							// empty list
		targetProcess.pNext = targetProcess.pPrior = NULL;
		pHead = pTail = &targetProcess;
	}
	else {
		targetProcess.pNext = pHead;
		targetProcess.pPrior = NULL;
		pHead->pPrior = &targetProcess;
		pHead = &targetProcess;
	}

	targetProcess.pProcessList = this;			// saves link to current list

  //## end ProcessList::insertHead%36ADF11701A6.body
}

//## Operation: remove%36ADF25A01BF
//	********************************************************************************
//
//	Name: RetCode remove (SimProcess& targetProcess)
//
//	Description: Removes the target process from the Process List.
//
//	Input parameters: SimProcess& targetProcess;  //process to be removed
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	********************************************************************************
RetCode ProcessList::remove (SimProcess& targetProcess)
{
  //## begin ProcessList::remove%36ADF25A01BF.body preserve=yes

	if (this != targetProcess.pProcessList)			// is the correct list ?
		return	SCH_INVALID_LIST;					// target is not in this list

	if (targetProcess.pPrior == NULL )				// target process is the head of the list.
		pHead = targetProcess.pNext;
	else
		(targetProcess.pPrior)->pNext = targetProcess.pNext;

	if (targetProcess.pNext == NULL ) 				// target process is the tail of the list.
		pTail = targetProcess.pPrior;
	else
		(targetProcess.pNext)->pPrior = targetProcess.pPrior;

	if (pLastInserted == &targetProcess)			// updates the last process inserted in this list
		 pLastInserted	= NULL;

	targetProcess.pProcessList = NULL;				// broken the link with this list


	return SCH_SUCCESS;
  //## end ProcessList::remove%36ADF25A01BF.body
}

//## Operation: removeHead%36ADF28302EA
//	********************************************************************************
//
//	Name:SimProcess *  removeHead (SimProcess& targetProcess)
//
//	Description: Removes the process from the head of the Process List and returns a pointer to it.
//
//	Input parameters: none
//
//	Returns: SimProcess * : pointer to the removed head.
//	         NULL : empty list
//
//	********************************************************************************
SimProcess * ProcessList::removeHead ()
{
  //## begin ProcessList::removeHead%36ADF28302EA.body preserve=yes
	SimProcess *pTargetProcess;

	if (pHead == NULL) return NULL;				// empty list
	pTargetProcess = pHead;						// saves return value
	if (pTail == pHead) {						// only one element
		pTail = pHead = NULL;
	}
	else {
		pHead = pHead->pNext;
		pHead->pPrior = NULL;
	}

	if (pLastInserted == pTargetProcess)		// updates the last process inserted in this list
		 pLastInserted	= NULL;

	pTargetProcess->pProcessList = NULL;		// broken the link with this list

 	return pTargetProcess;
  //## end ProcessList::removeHead%36ADF28302EA.body
}

// Additional Declarations
  //## begin ProcessList%369E35270142.declarations preserve=yes
  //## end ProcessList%369E35270142.declarations

//## begin module%369E35270142.epilog preserve=yes
}	// end of sch namespace

//## end module%369E35270142.epilog
