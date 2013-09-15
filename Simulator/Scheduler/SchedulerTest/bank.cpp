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
//	File Name: Bank.cpp
//
//	Description:  Validation test using G/G/1 and M/M/1 queueing models. The result test must be
//                compared with the example present in the book: 
//					Law Averill M., Kelton W. David; " Simulation Modeling & Analysis", Second
//                  edition, McGraw-Hill, 1991
//
//	Author:	  Hélio Azevedo
//
//	Creation Date:	May/07/99
//
//	Visual Source Safe $Revision: 4 $
//
//	Last check in $Date: 10/04/99 2:44p $
//
//	********************************************************************************
 
// includes

#include "MemLeak.h"
#include <list>
#include <iostream>

#include "SimulationSystem.h"
#include "Simulation.h"
#include "Bank.h"



using namespace sch;
using namespace std;



// global variable declarations

LISTCLIENT listClient;						// defines the list of clients waiting for attendance
Attendant attendant;						// attendant process
ClientGenerator clientGenerator;			// Client Generator process
SimulationSystem simulationSystem;          // initiate/ coordenate the simulation


Int maxClients;								// maximum numbers of clients <stop condition>
NegExpGen *pArrivalTime;					// distribuition that generates the arrival time
NegExpGen *pAttendanceTime;					// distribuition that generates the attendance time
DescriptiveStatistic totalClientTime;		// Total time spent by the client on the system 
DescriptiveStatistic attendantLazyTime;		// Attendant Lazy Time
DescriptiveStatistic statTimeBetweenArrival; // descriptive statistics of time between arrivals
DescriptiveStatistic statServiceTime;		 // descriptive statistics of attendent service time
DescriptiveStatistic statSizeQueue;			// descriptive statistics of Size Queue
CumulativeHistogram *pTimeBetweenArrival;	// Result variable with time between arrivals
CumulativeHistogram *pServiceTime;			// Result variable with attendent service time
CumulativeHistogram *pSizeQueue;
CumulativeHistogram *pTotalClientTime;
Quantile *pQuanSizeQueue;
Quantile *pQuanTimeBetweenArrival;
Quantile *pQuanServiceTime;
Quantile *pQuanAttendantLazyTime;

void printGlobalStatistics (SimulationManager& simul)
{
	Statistics stat;
	Logger *pLogger = Logger::instance();
	char  tmpMsg[200];
	Double timeElap;

	simul.getStatistics(stat);

	pLogger->log (Informative, true, "\nSimulation Statistics         :");

	if (stat.running) 
		pLogger->log (Informative, false, "     Simulation state        : Running");
	else
		pLogger->log (Informative, false, "     Simulation state        : Stopped");

	sprintf (tmpMsg, "     Simulation Time         : %02f", stat.simulationTime);  
	pLogger->log (Informative, false, tmpMsg);

	timeElap = ((Double) stat.timeElapsed) / 10000.0;					// converts in ms
	sprintf (tmpMsg, "     CPU Simulation Time (ms): %02f", timeElap);
	pLogger->log (Informative, false, tmpMsg);
	
	sprintf (tmpMsg, "     Total Process Created   : %d", stat.totalProcessCreated);  
	pLogger->log (Informative, false, tmpMsg);

	sprintf (tmpMsg, "     Total Process Terminated: %d", stat.totalProcessTerm);  
	pLogger->log (Informative, false, tmpMsg);

	sprintf (tmpMsg, "     Return Code             : %d", stat.fatalError);  
	pLogger->log (Informative, false, tmpMsg);
}



//	********************************************************************************
//	Main : void errorTreatment(void)
//	Description: Informs a error occur. Note that this function does not returns.
//	Parameters:error : error code
//	Returns: none
//	********************************************************************************

void errorTreatment( string name, RetCode error)
{
	SingleSimulationManager *pSimBank = SingleSimulationManager::instance();
	Logger *pLogger = Logger::instance();
	char  tmpMsg[200];

	sprintf (tmpMsg, "#simulation time: %02f : %s ERROR: %d \n", 
			 pSimBank->getSystemSimulationTime(), name.data(), error);
	pLogger->log (Error, true, tmpMsg);
	
	pSimBank->endSimulationManager();				// finish the simulation
}




//	********************************************************************************
//	Main : void getInputDate(void)
//	Description: get the input date. This dates defines the simulation environment.
//	Parameters:none
//	Returns: none
//	********************************************************************************

void getInputDate(void)
{
	Logger *pLogger  = Logger::instance();
	RetCode result;
	Ulong arrivalTimeSeed = 9102;
	Ulong attendanceTimeSeed = 2744;
	char  tmpMsg[200];

	maxClients = 1000000;											// max client number
	
	if ((result = pLogger->init("SimBank", true)) != SCH_SUCCESS){
		cout << "Error in getInputDate: " <<  result << endl;
		ExitProcess (result);
	}
	
	pLogger->setActive(true);


	sprintf (tmpMsg, "\n\n #Arrival Time Seed= %d   #Attendance Time Seed= %d \n\n", 
			 arrivalTimeSeed, attendanceTimeSeed);
	pLogger->log (Informative, false, tmpMsg);
	
	pArrivalTime = new NegExpGen(arrivalTimeSeed, (Float)50.0);					// arrival time with mean = 1.00					
	pAttendanceTime = new NegExpGen(attendanceTimeSeed, (Float)60.0);			// attendance time with mean 1/2


	pTimeBetweenArrival = new CumulativeHistogram (10);		// cumulative histogram with 10 intervals
	pServiceTime = new CumulativeHistogram (10);			// cumulative histogram with 10 intervals
	pSizeQueue = new CumulativeHistogram(100);
	pTotalClientTime = new CumulativeHistogram(100);

	// defines all quantiles
	pQuanTimeBetweenArrival = new Quantile (0.5);
	pQuanServiceTime = new Quantile (0.5);
	pQuanAttendantLazyTime = new Quantile (0.5);
	pQuanSizeQueue = new Quantile(0.99999);

}



//	********************************************************************************
//	Main : void main(void)
//	Description: Coodenates system execution 
//	Parameters:none
//	Returns: none
//	********************************************************************************


void main (void)
{
	DumpMemoryLeaks();

	RetCode result;
	SingleSimulationManager *SimBank = SingleSimulationManager::instance();
	Logger *pLogger = Logger::instance();

	getInputDate();							// gets input date

	char  tmpMsg[200];

	
	if ((result = SimBank->startSimulationManager(simulationSystem)) != SCH_SUCCESS){
		sprintf (tmpMsg, "Error in startSimulationManager: %d \n",result); 
		pLogger->log (Error, true, tmpMsg);
		exit (result);
	}

	// The process primary thread can not die !!! 
	// If this happens Windows-NT calls ExitProcess function and terminates the associated process. 
	// Thus, this thread  have to wait for the end of simulation before terminated.
		
	while (SimBank->isRunning())	 {
		Sleep (5000);								// wait  5s  and look again
	}

	printGlobalStatistics (*SimBank);
	SimBank->clear();
	pLogger->clear();									// close all files
	delete pArrivalTime;
	delete pAttendanceTime;
	delete pTimeBetweenArrival;
	delete pServiceTime;
	delete pQuanTimeBetweenArrival;
	delete pQuanServiceTime;
	delete pQuanAttendantLazyTime;
	delete pSizeQueue;
	delete pQuanSizeQueue;
	delete pTotalClientTime;
	exit (0);								// finish execution

}


