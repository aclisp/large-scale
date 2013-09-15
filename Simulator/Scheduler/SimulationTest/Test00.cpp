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
//	File Name: Test00.cpp
//
//	Description: This module implements the first Simulation Management test.
//               The test objectives consist in creat a set o simulation process
//				 and observe your comportament. 
//				 The following  SimulationManager methods are exercised
//					startSimulationManager
//					endSimulationManager
//					startProcess
//					terminateProcess
//					activate
//					hold
//					passivate
//					schedule
//					isRunning
//					getSystemSimulationTime
//					getStatistics
//					getState
//					getProcessName
//
//	Author:	 Helio azevedo
//
//	Creation Date:	31/03/99
//
//	Visual Source Safe $Revision: 4 $
//
//	Last check in $Date: 10/04/99 2:49p $
//
//	********************************************************************************

// include files
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include "Simulation.h"
#include <stdio.h>
#include <memory>

// constants

const Double ATTENDANCE = 180.0 / (Double) RAND_MAX;	// max attendance time	(s)
const Double ARRIVAL = 120.0 /(Double) RAND_MAX;		// max arrival time	 (s)
const Int    NROCLIENTS = 10;							// max number of clients
const Double TOTALSIMUL = 600.0;					    // total simulation time



using namespace std;
using namespace sch;


class Bank : public SimProcess {
private:
	void bodyProcess(void);
};

class Client : public SimProcess {
private:
	void bodyProcess(void);
};

class Robber : public SimProcess {
private:
	void bodyProcess(void);
};

// declarates basic objects associated with the current simulation

Client clients [NROCLIENTS];
Robber robber;

//
// Prints statistics data associated with simulationManager object
// 
void printStatistics (SimulationManager& simul)
{
	Statistics stat;

	simul.getStatistics(stat);
	cout << "Simulation Statistics         : " << endl;
	cout << "     Simulation state        : " << ( (stat.running) ? "running" : "Stopped") << endl;

	cout << "     Simulation Time         : " << stat.simulationTime << endl;
	hex (cout);								// writes time in hex format
	cout << "     Execution Time          : ";
	// writes high double_word 
	cout << ((Ulong) (stat.timeElapsed >> 32));
	// writes low double_word
	cout << ((Ulong) (stat.timeElapsed & 0xFFFFFFFF)) << 'H' << endl;
	dec (cout);

	cout << "     Total Process Created   : " << stat.totalProcessCreated << endl;
	cout << "     Total Process Terminated: " << stat.totalProcessTerm << endl;
	cout << "     Return Code             : " << stat.fatalError << endl;
}


void Bank::bodyProcess(void) {
	Clause position;					// defines the type of clause condition
	RetCode result;
	string nameProc;
	char  clientNumber[20];
	SingleSimulationManager *SimBank = SingleSimulationManager::instance();

	// Start Clients process in passive state

	for (Int i = 0; i< NROCLIENTS; i++) {
		if ( (i % 1000) == 0 ) 
			cout << "Starting process number " << i << endl;
		sprintf( clientNumber,"%d", i );
		nameProc = "Client";
		nameProc += clientNumber;
		if ((result = SimBank->startProcess(clients[i],nameProc, false))!= SCH_SUCCESS ){
			cout << SimBank->getSystemSimulationTime() << ": " << nameProc << " ERROR: " << result << endl;
			SimBank->endSimulationManager();		// finish the simulation
		}
	}

	// activate all Clients
	for (Int i =0; i< NROCLIENTS; i++) 
		if ((result = SimBank->activate(clients[i])) != SCH_SUCCESS ){
			cout << SimBank->getSystemSimulationTime() << ": " << clients[i].getProcessName() << " ERROR: " << result << endl;
			SimBank->endSimulationManager();		// finish the simulation
		}

	// Start robber
	if ((result = SimBank->startProcess(robber,"Robber", false))!= SCH_SUCCESS ){
			cout << SimBank->getSystemSimulationTime() << ": " << "Robber" << " ERROR: " << result << endl;
 			SimBank->endSimulationManager();		// finish the simulation

	}

	// activate Robber
	if ((result = SimBank->activate(robber)) != SCH_SUCCESS ){
		cout << SimBank->getSystemSimulationTime() << ": " << robber.getProcessName() << " ERROR: " << result << endl;
		SimBank->endSimulationManager();		// finish the simulation
	}

	// finish the simulation by expired time
	position.setClause ( ActivationTime,  NULL, TOTALSIMUL, After );
	if ((result = SimBank->hold(position)) != SCH_SUCCESS) {
		cout <<  getProcessName() << " ERROR: " << result << endl;
		SimBank->endSimulationManager();		// finish the simulation
	}

	// at the end of TOTALSIMUL time finish the simulation
	SimBank->endSimulationManager();		

}



void Client::bodyProcess(void) {
	Double attendanceTime,				// rand ( 0, MAXATTENDANCE)
		   arrivalTime,					// simulationTime + rand (0, MAXARRIVAL)
		   waitingTime;
	Clause position;					// defines the type of clause condition
	RetCode result;
	SingleSimulationManager *SimBank = SingleSimulationManager::instance();

	attendanceTime =  ATTENDANCE * (Double) rand();
	arrivalTime =  ARRIVAL * (Double) rand() + SimBank->getSystemSimulationTime();
	
	position.setClause(ActivationTime, NULL, arrivalTime, After );
	if ((result = SimBank->hold(position)) != SCH_SUCCESS) {
		cout <<  SimBank->getSystemSimulationTime() << ": " << getProcessName() << " ERROR: " << result << endl;
		SimBank->endSimulationManager();		// finish the simulation
	}

	cout << SimBank->getSystemSimulationTime() << ": " << getProcessName() << " arrival at " << arrivalTime << endl;
	
	position.setClause(DelayTime, NULL, attendanceTime, After );
	if ((result = SimBank->hold(position)) != SCH_SUCCESS) {
		cout <<  SimBank->getSystemSimulationTime() << ": " << getProcessName() << " ERROR: " << result << endl;
		SimBank->endSimulationManager();		// finish the simulation
	}
	waitingTime = SimBank->getSystemSimulationTime()	-  arrivalTime;
	cout <<	SimBank->getSystemSimulationTime() << ": " << getProcessName() << " end of attendance with waiting time = " << waitingTime << endl;

}


void Robber::bodyProcess(void) {
	Double attendanceTime,				// rand ( 0, MAXATTENDANCE)
		   arrivalTime;					// simulationTime + rand (0, MAXARRIVAL)
	Clause position;					// defines the type of clause condition
	SimProcess *pVipClient;				// saves vip Client
	RetCode result;
	SingleSimulationManager *SimBank = SingleSimulationManager::instance();

	attendanceTime =  ATTENDANCE * (Double) rand();
	arrivalTime =  ARRIVAL * (Double) rand() + SimBank->getSystemSimulationTime();
	
	position.setClause(ActivationTime, NULL, arrivalTime, After );
	if ((result = SimBank->hold(position)) != SCH_SUCCESS) {
		cout <<  SimBank->getSystemSimulationTime() << ": " << getProcessName() << " ERROR: " << result << endl;
		SimBank->endSimulationManager();		// finish the simulation
	}
	cout << SimBank->getSystemSimulationTime() << ": " << getProcessName() << " arrival at " << arrivalTime << endl;
	

	// remove all Clients waiting for attendance
	for (Int i =0; i< NROCLIENTS; i++) 
		if ((clients[i].getState() != NonInitialized) && (clients[i].getState() != Terminated))
			if ((result = SimBank->passivate (clients[i])) != SCH_SUCCESS) {
				cout <<  SimBank->getSystemSimulationTime() << ": " << clients[i].getProcessName() << " ERROR: " << result << endl;
				SimBank->endSimulationManager();		// finish the simulation
			}
		
	
	// wait for the end of robber action
	position.setClause(DelayTime, NULL, attendanceTime, After );
	if ((result = SimBank->hold(position)) != SCH_SUCCESS) {
		cout <<  SimBank->getSystemSimulationTime() << ": " << getProcessName() << " ERROR: " << result << endl;
		SimBank->endSimulationManager();		// finish the simulation
	}
	cout <<	SimBank->getSystemSimulationTime() << ": " << getProcessName() << ": end of action with elapsed time = " << attendanceTime << endl;

	// return  all Clients to the queue
	for (Int i =0; i< NROCLIENTS; i++) 
		if ((clients[i].getState() == Passive)) {
			arrivalTime =  ARRIVAL * (Double) rand();
			position.setClause(DelayTime, NULL, arrivalTime, After );
			pVipClient = &( clients[i]);			// the last inserted client was VIP
			if ((result = SimBank->schedule (clients[i],position)) != SCH_SUCCESS) {
				cout <<  SimBank->getSystemSimulationTime() << ": " << clients[i].getProcessName() << " ERROR: " << result << endl;
				SimBank->endSimulationManager();		// finish the simulation
			}
		}

	// OOPS !!! Put the  VIP client in the first position
	cout << "Realocating the VIP Client >>>" << endl;
	cout <<  SimBank->getSystemSimulationTime() << ": " << pVipClient->getProcessName() << " old position " << pVipClient->getActivationTime() << endl;

	position.setClause(ActivationTime, NULL, SimBank->getSystemSimulationTime(), Before );
	if ((result = SimBank->schedule (*pVipClient,position)) != SCH_SUCCESS) {
		cout <<  SimBank->getSystemSimulationTime() << ": " << pVipClient->getProcessName() << " ERROR: " << result << endl;
		SimBank->endSimulationManager();		// finish the simulation
	}
	cout <<  SimBank->getSystemSimulationTime() << ": " << pVipClient->getProcessName() << " new position " << pVipClient->getActivationTime() << endl;


	// finished robber process
	SimBank->terminateProcess();

}





//
//*			BEGIN OF MAIN  FUNCTION
//


	
void main (void) {
    
	Bank campinasBank;
	RetCode result;
	SingleSimulationManager *SimBank = SingleSimulationManager::instance();


	try {					// begin of try bock for exception handling
	
		if ((result = SimBank->startSimulationManager(campinasBank)) != SCH_SUCCESS){
			cout << "Error in startSimulationManager: " <<  result << endl;
			ExitProcess (result);
		}

		// The process primary thread can not die !!! 
		// If this happens Windows-NT calls ExitProcess function and terminates the associated process. 
		// Thus, this thread  have to wait for the end of simulation before terminated.
		
		while (SimBank->isRunning())	 {
			Sleep (5000);								// wait  5s  and look again
		}

		printStatistics (*SimBank);						// final statistics

		ExitProcess (0);								// finish execution

	}						// end of try block for exception handling

	// handlers for possible exceptions, note that only exceptions occured in application 
	// context will catch here. Exceptions occured in the simulation context will be 
	// reported by the getStatistics function.
		
	catch (SchException& exc) {
		cout << endl << "Occur a scheduler exception number" << exc.getError()<< endl;
	}

	catch (exception& exc) {
		cout << endl << "Occur a STL exception because " << exc.what()<< endl;
	}


		

}
