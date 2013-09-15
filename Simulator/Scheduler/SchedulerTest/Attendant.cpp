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
//	File Name: Attendant.cpp
//
//	Description:
//
//	Author:	  Hélio Azevedo
//
//	Creation Date:	May/07/99
//
//	Visual Source Safe $Revision: 1 $
//
//	Last check in $Date: 13/05/99 9:32 $
//
//	********************************************************************************

// includes
#include "MemLeak.h"
#include <list>
#include <iostream>

#include "Bank.h"
#include "Attendant.h"
#include "Simulation.h"




using namespace sch;
using namespace std;




//	********************************************************************************
//
//	Operator Name: void bodyProcess(void)
//
//	Description: Implements the simulation process.
//
//	Parameters:none
//
//	Returns: none
//
//
//	********************************************************************************
void Attendant::bodyProcess ()
{
	Clause position;					// defines the type of clause condition
	Double lazyTime;
	Float serviceTime;
   	RetCode result;
	Client *pClient;
	SingleSimulationManager *pSimBank = SingleSimulationManager::instance();

	
	while ( maxClients > 0) {									// repeat for ever

		// is the list empty ?
		if (listClient.empty())	{		
			lazyTime = pSimBank->getSystemSimulationTime();

			// if empty => wait client arrival
			if ((result = pSimBank->passivate()	) != SCH_SUCCESS )
				errorTreatment( getProcessName(), result);

			// update statistics associated with lazy time
			lazyTime = pSimBank->getSystemSimulationTime() - lazyTime; 
			if ((result = attendantLazyTime.nextSample(lazyTime)) != SCH_SUCCESS )
				errorTreatment( getProcessName(), result);

			if ((result = pQuanAttendantLazyTime->nextSample(lazyTime)) != SCH_SUCCESS )
				errorTreatment( getProcessName(), result);

		}

		// obtain the head client
		pClient =  (Client *) listClient.front();			// saves the first client at queue
		listClient.pop_front();					// remove from the list

		// wait for the attendance time
		 if ((result = pAttendanceTime->nextValue( &serviceTime)) != SCH_SUCCESS)
				errorTreatment( getProcessName(), result);

		position.setClause ( DelayTime,  NULL, serviceTime, After );
		if ((result = pSimBank->hold(position)) != SCH_SUCCESS) 
			errorTreatment( getProcessName(), result);

		// updates ServiceTime statistics
		if ((result = statServiceTime.nextSample(serviceTime)) != SCH_SUCCESS)
			errorTreatment( getProcessName(), result);

		if ((result = pServiceTime->nextSample (serviceTime)) != SCH_SUCCESS) 
			errorTreatment( getProcessName(), result);

		if ((result = pQuanServiceTime->nextSample (serviceTime)) != SCH_SUCCESS) 
			errorTreatment( getProcessName(), result);


		// activates the Client at end of attendance
		if ((result = pSimBank->activate(*pClient)) != SCH_SUCCESS) 
			errorTreatment( getProcessName(), result);

		// decrements total number of clients
		maxClients--;


	}	// end of while



	// All client has been attended, then activate SimulationSystem to conclude the simulation
	if ((result = pSimBank->activate(simulationSystem)) != SCH_SUCCESS )
		errorTreatment( getProcessName(), result);



}


