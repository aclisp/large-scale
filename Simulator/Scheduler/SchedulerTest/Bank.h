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
//	File Name: Bank.h
//
//	Description:  Defines all global variables used by the program.
//
//	Author:	  Hélio Azevedo
//
//	Creation Date:	May/07/99
//
//	Visual Source Safe $Revision: 3 $
//
//	Last check in $Date: 10/04/99 2:44p $
//
//	********************************************************************************


#ifndef Bank_h
#define Bank_h 1






// includes

#include <list>
#include <iostream>
#include "Attendant.h"
#include "Client.h"
#include "ClientGenerator.h"
#include "SimulationSystem.h"
#include "Simulation.h"
#include "RandomGen.h"
#include "StatsCalc.h"
#include "trclog.h"




using namespace sch;
using namespace std ;

typedef list<Client *> LISTCLIENT;


// Global Variables	 definition

extern LISTCLIENT listClient;						// define the list of clients waiting for attendance
extern Attendant attendant;							// attendant process
extern ClientGenerator clientGenerator;				// Client Generator process
extern SimulationSystem simulationSystem;			// initiate/ coordenate the simulation

extern Int maxClients;								// maximum numbers of clients <stop condition>
extern NegExpGen *pArrivalTime;					// distribuition that generates the arrival time
extern NegExpGen *pAttendanceTime;					// distribuition that generates the attendance time
extern DescriptiveStatistic totalClientTime;		// Total time spent by the client on the system 
extern DescriptiveStatistic	attendantLazyTime;		// Attendant Lazy Time
extern DescriptiveStatistic statTimeBetweenArrival; // descriptive statistics of time between arrivals
extern DescriptiveStatistic statServiceTime;
extern DescriptiveStatistic statSizeQueue;			// descriptive statistics of Size Queue
extern CumulativeHistogram *pTimeBetweenArrival;	// Result variable with time between arrivals
extern CumulativeHistogram *pServiceTime;			// Result variable with attendent service time
extern CumulativeHistogram *pSizeQueue;
extern CumulativeHistogram *pTotalClientTime;
extern Quantile *pQuanSizeQueue;
extern Quantile *pQuanTimeBetweenArrival;
extern Quantile *pQuanServiceTime;
extern Quantile *pQuanAttendantLazyTime;




// function prototypes

void errorTreatment(string name, RetCode error);

// mark end of module
#endif
