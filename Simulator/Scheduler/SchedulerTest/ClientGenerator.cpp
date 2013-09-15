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
//	File Name: ClientGenerator.cpp
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
#include "Bank.h"
#include "ClientGenerator.h"
#include "Simulation.h"




using namespace sch;






//	********************************************************************************
//
//	Operator Name: void bodyProcess(void)
//
//	Description: Implements the Client Generator process.
//
//	Parameters: none
//
//	Returns: none
//
//
//	********************************************************************************
void ClientGenerator::bodyProcess ()
{

	Float newArrival;
	Clause position;					// defines the type of clause condition
	RetCode result;
	SingleSimulationManager *pSimBank = SingleSimulationManager::instance();
	Client *pNewClient ;
 	// string nameProc;                 // Cause memory leak !!!
	Int clientCounter = 1;				// Client counter
	char  clientNumber[20];

	
	for ( ; ; ) {									// repeat for ever

		// calculates a new arrival time following pArrivalTime distribution
		if ((result = pArrivalTime->nextValue(&newArrival))!= SCH_SUCCESS )
			errorTreatment( getProcessName(), result);

		// wait until arrival time
		position.setClause ( DelayTime,  NULL, newArrival, After );
		if ((result = pSimBank->hold(position)) != SCH_SUCCESS) 
			errorTreatment( getProcessName(), result);

		// saves statistics
		if ((result = statTimeBetweenArrival.nextSample(newArrival)) != SCH_SUCCESS )
			errorTreatment( getProcessName(), result);

		if ((result = pTimeBetweenArrival->nextSample (newArrival)) != SCH_SUCCESS) 
			errorTreatment( getProcessName(), result);

		if ((result = pQuanTimeBetweenArrival->nextSample (newArrival)) != SCH_SUCCESS) 
			errorTreatment( getProcessName(), result);



		// Start  and activate the new Client
		pNewClient = new Client;
		sprintf( clientNumber,"Client%d", clientCounter++);
		//nameProc = "Client";
		//nameProc += clientNumber;					// build the client name
		if ((result = pSimBank->startProcess(*pNewClient, clientNumber, true))!= SCH_SUCCESS )
			errorTreatment( getProcessName(), result);
		if ((result = pSimBank->activate(*pNewClient))!= SCH_SUCCESS )
			errorTreatment( getProcessName(), result);

	}	// loop for ever 



}


