//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: Scheduler Test
//
//	File Name: Client.cpp
//
//	Description:
//
//	Author:	  Hélio Azevedo
//
//	Creation Date:	May/07/99
//
//	Visual Source Safe $Revision: 1 $
//
//	Last check in $Date: 13/05/99 9:33 $
//
//	********************************************************************************

// includes
#include "MemLeak.h"
#include "Simulation.h"
#include "SimulationSystem.h"
#include "Bank.h"
#include "Attendant.h"
#include "Client.h"




using namespace sch;






//	********************************************************************************
//
//	Operator Name: void bodyProcess(void)
//
//	Description: Implements the client process.
//
//	Parameters:none
//
//	Returns: none
//
//
//	********************************************************************************
void Client::bodyProcess ()
{
	Clause position;					// defines the type of clause condition
	Double totalTime;
   	RetCode result;
	Uint queueSize;
	SingleSimulationManager *pSimBank = SingleSimulationManager::instance();


	totalTime = pSimBank->getSystemSimulationTime();


	if (attendant.getState() == Passive) {		// if attendant is not busy wake up him   		
		// schedule attendant
		position.setClause ( ActivationTime,  NULL, pSimBank->getSystemSimulationTime(), Before );
		if ((result = pSimBank->schedule(attendant,position)) != SCH_SUCCESS )
			errorTreatment( getProcessName(), result);
		queueSize = 0;
		assert(listClient.empty());
		listClient.push_back(this);					// insert client in the attendant list
	}
	else {
		listClient.push_back(this);					// insert client in the attendant list
		queueSize = listClient.size();
	}
	if ((result = statSizeQueue.nextSample(queueSize)) != SCH_SUCCESS )
		errorTreatment( getProcessName(), result);
	if ((result = pSizeQueue->nextSample(queueSize)) != SCH_SUCCESS )
		errorTreatment( getProcessName(), result);
	if ((result = pQuanSizeQueue->nextSample(queueSize)) != SCH_SUCCESS )
		errorTreatment( getProcessName(), result);

	// wait end of attendance
	if ((result = pSimBank->passivate()	) != SCH_SUCCESS )
		errorTreatment( getProcessName(), result);

	// update statistics

	totalTime = pSimBank->getSystemSimulationTime() - totalTime;
	if ((result = totalClientTime.nextSample(totalTime)) != SCH_SUCCESS )
		errorTreatment( getProcessName(), result);
	if ((result = pTotalClientTime->nextSample(totalTime)) != SCH_SUCCESS )
		errorTreatment( getProcessName(), result);
}


