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
//	File Name: SimulationSystem.cpp
//
//	Description:
//
//	Author:	  Hélio Azevedo
//
//	Creation Date:	May/07/99
//
//	Visual Source Safe $Revision: 4 $
//
//	Last check in $Date: 16/06/99 9:28 $
//
//	********************************************************************************

// includes
#include "MemLeak.h"
#include <iostream>
#include "Simulation.h"
#include "SimulationSystem.h"
#include "Bank.h"
#include "Attendant.h"
#include "ClientGenerator.h"





using namespace sch;
//	********************************************************************************
//
//	Main : void printQuantile(Quantile *pQuant , string id)
//
//	Description: Print the histrogram.
//
//	Parameters:none
//
//	Returns: none
//
//
//	********************************************************************************

void printQuantile(Quantile *pQuant , string id)
{
	char  tmpMsg[200];
	RetCode rc = SCH_SUCCESS;
	Logger *pLogger = Logger::instance();

	
	sprintf (tmpMsg, "Quantile results from  %s",  id.data());
	pLogger->log (Informative, true, tmpMsg );

	sprintf (tmpMsg, "#samples = %02d, min = %02f, max = %02f, Threshold = %02f, \n   quantile = %02f\n", 			  
			pQuant->getNumSamples(),				  
			pQuant->getMin(), pQuant->getMax(),  
			pQuant->getThreshold(), pQuant->getQuantile());	      
	pLogger->log (Informative, false, tmpMsg);


}


//	********************************************************************************
//
//	Main : void printHistogram(ContinuousHistogram *pHisto , string id)
//
//	Description: Print the histrogram.
//
//	Parameters:none
//
//	Returns: none
//
//
//	********************************************************************************

void printHistogram(CumulativeHistogram *pHisto , string id)
{
	Float x;
	Float y;
	char  tmpMsg[200];
	RetCode rc = SCH_SUCCESS;
	Logger *pLogger = Logger::instance();


	//
	// now loop thru histogram intervals and print results
	//
	sprintf (tmpMsg, "Cumulative Histogram results from  %s",  id.data());
	pLogger->log (Informative, true, tmpMsg );

	sprintf (tmpMsg, "Cumulative Results; #intervals = %02d", pHisto->getNumIntervals());
	pLogger->log (Informative, false, tmpMsg );
	pLogger->log(Informative, false, "Interval\tx\ty");
	for (Int i = 0; (i <= pHisto->getNumIntervals()) && (rc == SCH_SUCCESS); i++) {
		if ((rc = pHisto->getCumulativeResult (i, &x, &y)) == SCH_SUCCESS) {
			sprintf (tmpMsg, "%02d\t%02f\t%02f", i, x, y);
			pLogger->log (Informative, false, tmpMsg );
		}
	}

	pLogger->log (Informative, false, "Derived Results");
	pLogger->log(Informative, false, "Interval\tx\ty");
	for (Int j = 0; (j < pHisto->getNumIntervals()) && (rc == SCH_SUCCESS); j++) {
		if ( pHisto->getDerivedResult (j, &x, &y) == SCH_SUCCESS) {
			sprintf (tmpMsg, "%02d\t%02f\t%02f", j, x, y);
			pLogger->log (Informative, false, tmpMsg );
			}
	}

	pLogger->log(Informative, false, "\n");
}

//	********************************************************************************
//
//	Main : void printDescriptiveStatistics(void)
//
//	Description: Print the descriptive statistics.
//
//	Parameters:none
//
//	Returns: none
//
//
//	********************************************************************************

void printDescriptiveStatistics(DescriptiveStatistic *pDes, string id)
{
	char  tmpMsg[200];
	Logger *pLogger = Logger::instance();

	sprintf (tmpMsg, "Descriptive Statistic results from  %s",  id.data());
	pLogger->log (Informative, true, tmpMsg );

	sprintf (tmpMsg, "#samples = %02d, min = %02f, max = %02f, sum = %02f, \n   mean = %02f, variance = %02f, sd = %02f \n", 			  
			pDes->pMean->getNumSamples(),				  
			pDes->pMean->getMin(), pDes->pMean->getMax(),  
			pDes->pMean->getSum(), pDes->pMean->getMean(), 
			pDes->getVariance(), pDes->getStdDev());	      
	pLogger->log (Informative, false, tmpMsg );

}

//	********************************************************************************
//
//	Main : void printStatistics(void)
//
//	Description: Print the system statistics.
//
//	Parameters:none
//
//	Returns: none
//
//
//	********************************************************************************

void printStatistics(void)
{
	printDescriptiveStatistics (&totalClientTime, "Total Client Time");
	printDescriptiveStatistics (&attendantLazyTime, "Attendant Lazy Time");
	printDescriptiveStatistics (&statTimeBetweenArrival, "Time Between Arrival");
	printDescriptiveStatistics (&statServiceTime, "Service Time");
	printDescriptiveStatistics (&statSizeQueue, "Size Queue for Attendance");
	printHistogram (pTimeBetweenArrival, "Time Between Arrival");
	printHistogram (pServiceTime, "Service Time");
	printHistogram(pSizeQueue, "Size of Queue");
	printHistogram(pTotalClientTime, "Total Client Time");
	printQuantile (pQuanTimeBetweenArrival, "TimeBetweenArrival");
	printQuantile (pQuanServiceTime, "ServiceTime");
	printQuantile (pQuanAttendantLazyTime, "AttendantLazyTime");
	printQuantile(pQuanSizeQueue, "Size of Queue");

}





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
void SimulationSystem::bodyProcess ()
{
	RetCode result;
	SingleSimulationManager *pSimBank = SingleSimulationManager::instance();

	// Start Attendant process in passive state

	if ((result = pSimBank->startProcess(attendant,"Attendant", false))!= SCH_SUCCESS ) 
		errorTreatment( getProcessName(), result);

	// activate Attendant
	if ((result = pSimBank->activate(attendant)) != SCH_SUCCESS ) 
		errorTreatment( getProcessName(), result);

	// Start ClientGenerator
	if ((result = pSimBank->startProcess(clientGenerator,"ClientGenerator", false))!= SCH_SUCCESS )
		errorTreatment( getProcessName(), result);

	// activate ClientGenerator
	if ((result = pSimBank->activate(clientGenerator)) != SCH_SUCCESS )
		errorTreatment( getProcessName(), result);

	// wait generation of total number of client
	if ((result = pSimBank->passivate()) != SCH_SUCCESS ) 
		errorTreatment( getProcessName(), result);


	// at the end of clients  print statistics and finish the simulation
	printStatistics();
	pSimBank->endSimulationManager();		

}


