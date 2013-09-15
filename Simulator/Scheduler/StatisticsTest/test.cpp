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
//	File Name: Test.cpp
//
//	Description: This module implements the first Statistics test.
//               The test objectives consist in use the Continuous Histogram.
//	Author:	 Helio Azevedo
//
//	Creation Date:	14/05/99
//
//	Visual Source Safe $Revision: 6 $
//
//	Last check in $Date: 10/04/99 2:52p $
//
//	********************************************************************************

// include files
#include "MemLeak.h"
#include <iostream>
#include <fstream>
#include "statscalc.h"
#include "RandomGen.h"
#include <assert.h>




// constants

const Int    MAXSAMPLES = 10000;					// max number of samples



using namespace std;
using namespace sch;






//
//*			 global  Variables
//
ofstream outFileA ("resultA.txt");
ofstream outFileB ("resultB.txt");
const Int intervals = 10; 


//	Handle several math errors caused by abnormal operation in
//  Function Evaluator package. When this happens, _matherr only 
//  reports the error and follows the normal processing.

Int _matherr( struct _exception *except )
{
	cout << endl << "ERROR IN:" << 	except->name << " with " << except->arg1 << endl;

    return 0;    /*  use the default actions */    
}



//	********************************************************************************
//	Main : void errorTreatment(void)
//	Description: Informs a error occur. Note that this function does not returns.
//	Parameters:error : error code
//	Returns: none
//	********************************************************************************

void errorTreatment(RetCode error)
{

	cout << "\n\n Error:" << error << endl;
	exit (error);	// finish execution

}

void printResults (ofstream &outFile, ContinuousHistogram * pHisto, string name)
{
	Int cont;
	Int y;
	RetCode result;

 	// print the continuous histograma result
	outFile << "\n\n\n\t\t" << name << "\t Samples=" <<  MAXSAMPLES << "\n";
	

	outFile << endl << endl << "Continuous Histogram results:" << endl;
	outFile << "\t Interval Width = " << pHisto->getIntervalWidth() << endl;
	outFile << "\t Maximum Sample = " << pHisto->getMaxSample() << endl;
	outFile << "\t Maximum Value = " << pHisto->getMaxValue() << endl;
	outFile << "\t Minimum Sample = " << pHisto->getMinSample() << endl;
	outFile << "\t Minimum Value = " << pHisto->getMinValue() << endl;
	outFile << "\t Number of Intervals = " << pHisto->getNumIntervals() << endl;
	outFile << "\t Number of Samples = " << pHisto->getNumSamples() << endl;
	outFile << "\nHistogram Plotting\n";
	for (cont = 0; cont < intervals ; cont++) {
		if ((result=pHisto->getResult(cont, &y)) != SCH_SUCCESS) errorTreatment(result);
		outFile << "\t i = " << cont;
		outFile << "\t y_axis = " << y << endl;
	}
}


void printResults (ofstream &outFile, CumulativeHistogram * pHisto, string name)
{
	Int cont;
	Float x,y;
	RetCode result;

 	// print the histograma result
	outFile << "\n\n\n\t\t" << name << "\t Samples=" <<  MAXSAMPLES << "\n";
	

	outFile << endl << endl << "Histogram results with Cumulative:" << endl;
	for (cont = 0; cont <= intervals ; cont++) {
		if ((result=pHisto->getCumulativeResult(cont, &x, &y)) != SCH_SUCCESS)
			outFile << "\t i = " << cont << "  Error " << result << " in getCumulativeResult()" << endl;
		else {
			outFile << "\t i = " << cont;
			outFile << "\t x_axis = " << x;
			outFile << "\t y_axis = " << y << endl;
		}
	}
	outFile << endl << endl << "Histogram results with Quantiles:" << endl;
	for (cont = 0; cont < intervals ; cont++) {

		if ((pHisto->getDerivedResult(cont, &x, &y)) != SCH_SUCCESS)
			outFile << "\t i = " << cont << "  Error " << result << " in getDerivedResult()" << endl;
		else {
			outFile << "\t i = " << cont;
			outFile << "\t x_axis = " << x;
			outFile << "\t y_axis = " << ((Float)(cont+1)/intervals);
			outFile << "\t deviates = " << y << endl;
		}
	}
}


void printResults (ofstream &outFile, DescriptiveStatistic * pDesc, string name)
{

 	// print the result
	outFile << "\n\n\n\t\t" << name << "\t Samples=" <<  MAXSAMPLES << "\n";
	
	outFile << endl << endl << "Descriptive Statistic results:" << endl;
	outFile << "\t Standard Deviation = " << pDesc->getStdDev() << endl;
	outFile << "\t Variance = " << pDesc->getVariance() << endl;
}


void printResults (ofstream &outFile, DiscreteHistogram * pHisto, string name)
{
	Int cont;
	Int y;
	RetCode result;

 	// print the continuous histograma result
	outFile << "\n\n\n\t\t" << name << "\t Samples=" <<  MAXSAMPLES << "\n";
	

	outFile << endl << endl << "Discrete Histogram results:" << endl;
	outFile << "\t Interval Width = " << pHisto->getIntervalWidth() << endl;
	outFile << "\t Maximum Sample = " << pHisto->getMaxSample() << endl;
	outFile << "\t Maximum Value = " << pHisto->getMaxValue() << endl;
	outFile << "\t Minimum Sample = " << pHisto->getMinSample() << endl;
	outFile << "\t Minimum Value = " << pHisto->getMinValue() << endl;
	outFile << "\t Number of Intervals = " << pHisto->getNumIntervals() << endl;
	outFile << "\t Number of Samples = " << pHisto->getNumSamples() << endl;
	outFile << "\nHistogram Plotting\n";
	for (cont = 0; cont < intervals ; cont++) {
		if ((result=pHisto->getResult(cont, &y)) != SCH_SUCCESS) errorTreatment(result);
		outFile << "\t i = " << cont;
		outFile << "\t y_axis = " << y << endl;
	}
}

void printResults (ofstream &outFile, Mean * pMean, string name)
{

 	// print the result
	outFile << "\n\n\n\t\t" << name << "\t Samples=" <<  MAXSAMPLES << "\n";
	
	outFile << endl << endl << "Mean results:" << endl;
	outFile << "\t Mean = " << pMean->getMean() << endl;
	outFile << "\t Maximum = " << pMean->getMax() << endl;
	outFile << "\t Minimum = " << pMean->getMin() << endl;
	outFile << "\t Number of Samples = " << pMean->getNumSamples() << endl;
	outFile << "\t Sum = " << pMean->getSum() << endl;
}


void printResults (ofstream &outFile, Quantile * pQuantile, string name)
{

 	// print the result
	outFile << "\n\n\n\t\t" << name << "\t Samples=" <<  MAXSAMPLES << "\n";
	
	outFile << endl << endl << "Quantile results:" << endl;
	outFile << "\t Quantile = " << pQuantile->getQuantile() << endl;
	outFile << "\t Maximum = " << pQuantile->getMax() << endl;
	outFile << "\t Minimum = " << pQuantile->getMin() << endl;
	outFile << "\t Number of Samples = " << pQuantile->getNumSamples() << endl;
	outFile << "\t Threshold = " << pQuantile->getThreshold() << endl;
}



void printResults (ofstream &outFile, WindowedAverage *pWindow, string name)
{

 	// print the result
	outFile << "\n\n\n\t\t" << name << "\t Samples=" <<  MAXSAMPLES << "\n";
	
	outFile << endl << endl << "Windowed Avarage results:" << endl;
	outFile << "\t Windowed Average = " << pWindow->getWindowedAverage() << endl;
}



//
//*			BEGIN OF MAIN  FUNCTION
//

void main (void) {  

	DumpMemoryLeaks();
 
RetCode result;
ContinuousHistogram *pConHisto;
CumulativeHistogram *pCumHisto;
DescriptiveStatistic *pDesc;
DiscreteHistogram  *pDisHisto;
Mean *pMean;
Quantile *pQuantile;
WindowedAverage *pWindow;


Float value;
Int i, intValue;

try {					// begin of try bock for exception handling

	// allocates all Statistics areas

	pConHisto = new  ContinuousHistogram(0.0, 1.0, intervals);
	pCumHisto = new  CumulativeHistogram(intervals);   
	pDesc = new  DescriptiveStatistic();
	pDisHisto = new  DiscreteHistogram(0, 10 * intervals, intervals);
	pMean = new  Mean();
	pQuantile = new  Quantile(0.5);
	pWindow = new  WindowedAverage(12);


	// suplies a uniform distribuition for the results construction
	UniformGen *pUniformGen;
	RandIntGen *pRandIntGen;

	
	// generate the samples	and print all statistical results in resultA file
	pUniformGen = new UniformGen;
	pRandIntGen = new RandIntGen (1234, 0, 100);
	for (i = 0; i < MAXSAMPLES ; i++) {	  // continuous statistics
		if ((result=pUniformGen->nextValue ( &value)) != SCH_SUCCESS) errorTreatment(result);
		if ((result=pConHisto->nextSample ( value)) != SCH_SUCCESS) errorTreatment(result);
		if ((result=pCumHisto->nextSample ( value)) != SCH_SUCCESS) errorTreatment(result);
		if ((result=pDesc->nextSample ( value)) != SCH_SUCCESS) errorTreatment(result);
		if ((result=pMean->nextSample ( value)) != SCH_SUCCESS) errorTreatment(result);
		if ((result=pQuantile->nextSample ( value)) != SCH_SUCCESS) errorTreatment(result);
		if ((result=pWindow->nextSample ( value)) != SCH_SUCCESS) errorTreatment(result);
	}
	for (i = 0; i < MAXSAMPLES ; i++) {	  // discrete statistic
		if ((result=pRandIntGen->nextValue ( &intValue)) != SCH_SUCCESS) errorTreatment(result);
		if ((result=pDisHisto->nextSample ( intValue)) != SCH_SUCCESS) errorTreatment(result);
	}
	printResults (outFileA, pConHisto, "UNIFORM DISTRIBUTION" );
	printResults (outFileA, pCumHisto, "UNIFORM DISTRIBUTION" );
	printResults (outFileA, pDesc, "UNIFORM DISTRIBUTION" );
	printResults (outFileA, pMean, "UNIFORM DISTRIBUTION" );
	printResults (outFileA, pQuantile, "UNIFORM DISTRIBUTION" );
	printResults (outFileA, pWindow, "UNIFORM DISTRIBUTION" );
	printResults (outFileA, pDisHisto, "RANDOM INTEGER DISTRIBUTION" );
	delete pUniformGen;
	delete pRandIntGen;


	// restarts all statistical variables
	if ((result=pConHisto->reset()) != SCH_SUCCESS) errorTreatment(result);
	if ((result=pCumHisto->reset()) != SCH_SUCCESS) errorTreatment(result);
	if ((result=pDesc->reset()) != SCH_SUCCESS) errorTreatment(result);
	if ((result=pMean->reset()) != SCH_SUCCESS) errorTreatment(result);
	if ((result=pQuantile->reset()) != SCH_SUCCESS) errorTreatment(result);
	if ((result=pWindow->reset()) != SCH_SUCCESS) errorTreatment(result);
	if ((result=pDisHisto->reset()) != SCH_SUCCESS) errorTreatment(result);


	// generate all the samples again. The idea is compare the results before and after
	// the reset method execution. The result of this skip is sent to outfileB file.

	pUniformGen = new UniformGen;
	pRandIntGen = new RandIntGen (1234, 0, 100);
	for (i = 0; i < MAXSAMPLES ; i++) {	  // continuous statistics
		if ((result=pUniformGen->nextValue ( &value)) != SCH_SUCCESS) errorTreatment(result);
		if ((result=pConHisto->nextSample ( value)) != SCH_SUCCESS) errorTreatment(result);
		if ((result=pCumHisto->nextSample ( value)) != SCH_SUCCESS) errorTreatment(result);
		if ((result=pDesc->nextSample ( value)) != SCH_SUCCESS) errorTreatment(result);
		if ((result=pMean->nextSample ( value)) != SCH_SUCCESS) errorTreatment(result);
		if ((result=pQuantile->nextSample ( value)) != SCH_SUCCESS) errorTreatment(result);
		if ((result=pWindow->nextSample ( value)) != SCH_SUCCESS) errorTreatment(result);
	}
	for (i = 0; i < MAXSAMPLES ; i++) {	  // discrete statistic
		if ((result=pRandIntGen->nextValue ( &intValue)) != SCH_SUCCESS) errorTreatment(result);
		if ((result=pDisHisto->nextSample ( intValue)) != SCH_SUCCESS) errorTreatment(result);
	}
	printResults (outFileB, pConHisto, "UNIFORM DISTRIBUTION" );
	printResults (outFileB, pCumHisto, "UNIFORM DISTRIBUTION" );
	printResults (outFileB, pDesc, "UNIFORM DISTRIBUTION" );
	printResults (outFileB, pMean, "UNIFORM DISTRIBUTION" );
	printResults (outFileB, pQuantile, "UNIFORM DISTRIBUTION" );
	printResults (outFileB, pWindow, "UNIFORM DISTRIBUTION" );
	printResults (outFileB, pDisHisto, "RANDOM INTEGER DISTRIBUTION" );
	delete pUniformGen;
	delete pRandIntGen;


	// free all allocated memory
	delete pConHisto;
	delete pCumHisto;   
	delete pDesc;
	delete pDisHisto;
	delete pMean;
	delete pQuantile;
	delete pWindow;


	exit (0);	// finish execution

}						// end of try block for exception handling

	// handlers for possible exceptions
		
catch (SchException& exc) {
	cout << endl << "Occur a scheduler exception number" << exc.getError()<< endl;
}

catch (exception& exc) {
	cout << endl << "Occur a STL exception because " << exc.what()<< endl;
}


		

} // end of main block
