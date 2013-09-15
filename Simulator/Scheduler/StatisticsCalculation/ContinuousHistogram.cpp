//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%3744298B0396.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3744298B0396.cm

//## begin module%3744298B0396.cp preserve=yes
//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: Statistics Calculation
//
//	File Name: ContinuousHistogram.cpp
//
//	Description: Implementation file for the ContinuousHistogram class.
//
//	Author:	Hélio Azevedo
//
//	Creation Date: May/20/99
//
//	Visual Source Safe $Revision: 8 $
//
//	Last check in $Date: 16/06/99 9:28 $
//
//	********************************************************************************
//## end module%3744298B0396.cp

//## Module: ContinuousHistogram%3744298B0396; Pseudo Package body
//## Source file: T:\StatisticsCalculation\ContinuousHistogram.cpp

//## begin module%3744298B0396.additionalIncludes preserve=no
//## end module%3744298B0396.additionalIncludes

//## begin module%3744298B0396.includes preserve=yes

#include "MemLeak.h"
#include <assert.h>	  // define the assert macro

//## end module%3744298B0396.includes

// ContinuousHistogram
#include "ContinuousHistogram.h"
//## begin module%3744298B0396.additionalDeclarations preserve=yes

using namespace std;

namespace sch {

//## end module%3744298B0396.additionalDeclarations


// Class ContinuousHistogram 











//## Operation: ContinuousHistogram%37442A26021C
//	********************************************************************************
//
//	Name: ContinuousHistogram(Float minVal,Float maxVal, Int intervals)
//
//	Description: Non-default constructor - requires the lower boundary, upper boundary and number of intervals. The pValues array is
//	allocated.
//
//	Input parameters: Float minVal;     //lower boundary of the input data stream
//	                 Float maxVal;     //upper boundary of the input data stream
//	                  Int intervals;  //number of intervals in the histogram
//
//	Returns: none
//
//	Exceptions: SchException, with one of the following error codes:
//	               SCH_ALLOCATION_ERROR       = could not allocate pValues array
//	               SCH_INVALID_RANGE          = upper boundary is not greater than lower boundary
//
//	********************************************************************************
ContinuousHistogram::ContinuousHistogram (Float minVal, Float maxVal, Int intervals)
  //## begin ContinuousHistogram::ContinuousHistogram%37442A26021C.hasinit preserve=no
      : maxValue(maxVal),
        minValue(minVal),
        numIntervals(intervals),
        pValues(NULL),
        maxSample(SIM_HUGE_VAL),
        minSample(SIM_HUGE_VAL),
        numSamples(0),
        cumSum(0.0),
        cumSumOfSquares(0.0)
  //## end ContinuousHistogram::ContinuousHistogram%37442A26021C.hasinit
  //## begin ContinuousHistogram::ContinuousHistogram%37442A26021C.initialization preserve=yes
  //## end ContinuousHistogram::ContinuousHistogram%37442A26021C.initialization
{
  //## begin ContinuousHistogram::ContinuousHistogram%37442A26021C.body preserve=yes

	RetCode rc = SCH_SUCCESS;   // used to provide a single point for raising exception

	//
	// upper boundary must be greater than lower boundary
	//
	if (minValue > maxValue)
	{
		rc = SCH_INVALID_RANGE;
	}


	if (rc == SCH_SUCCESS)
	{
		//
		// calculate width of each interval
		//
		intervalWidth = (maxValue - minValue) / numIntervals;

		//
		// allocate pValues array
		//
		pValues = new Int[numIntervals];     // allocate array
		if (pValues == NULL)
		{
			rc = SCH_ALLOCATION_ERROR;       // could not create array
		}
		else
		{
			memset (pValues, 0, numIntervals * sizeof (Int)); // set all values to 0
		}
	}

	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);     // an error occurred, abort object construction
	}

	return;

  //## end ContinuousHistogram::ContinuousHistogram%37442A26021C.body
}



//## Other Operations (implementation)
//## Operation: getResult%37442A34033F
//	********************************************************************************
//
//	Name: RetCode getResult(Int intervalNumber, Int *pResult)
//
//	Description: Returns the contents (frequency) of the corresponding interval number.
//
//	Input parameters: Int intervalNumber;  //first interval number  is zero and the last is  interval-1
//
//	Output parameters: Int *pResult;  //contents of the corresponding interval number
//
//	Returns: SCH_SUCCESS
//	         SCH_INVALID_PARAMETER - pResult is NULL
//	         SCH_INVALID_INTERVAL  - interval number is negative or above higher interval - ignore contents of pResult
//
//	********************************************************************************
RetCode ContinuousHistogram::getResult (Int intervalNumber, Int *pResult)
{
  //## begin ContinuousHistogram::getResult%37442A34033F.body preserve=yes

	RetCode rc = SCH_SUCCESS;
	
	assert (pValues);

	if (pResult == NULL)
	{
		rc = SCH_INVALID_PARAMETER;
	}

	//
	// intervalNumber within valid range?
	// 
	else if ((intervalNumber >= 0) && (intervalNumber < numIntervals))
	{
		*pResult = pValues[intervalNumber];   // yes, return interval frequency
	}
	else
	{
		rc = SCH_INVALID_INTERVAL;			  
	}

	return (rc);

  //## end ContinuousHistogram::getResult%37442A34033F.body
}

//## Operation: nextSample%37442A34035D
//	********************************************************************************
//
//	Name: RetCode nextSample (Float sample)
//
//	Description: Includes the next sample in the sequence and increments the contents of the interval which this sample falls into.
//
//	Input parameters:Float sample;  //next sample in the sequence
//
//	Returns: SCH_SUCCESS
//
//	********************************************************************************
RetCode ContinuousHistogram::nextSample (Float sample)
{
  //## begin ContinuousHistogram::nextSample%37442A34035D.body preserve=yes

	RetCode rc = SCH_SUCCESS;
	Int     index; 			   // position of pValues to be incremented

	assert (pValues);

	// update minimum and maximun samples
	if (numSamples == 0) {
		maxSample = sample;
		minSample = sample;
	}
	else {
		if (sample < minSample) minSample = sample;
		if (sample > maxSample) maxSample = sample;
	}

	numSamples++;				// increments number of samples 
	cumSum = cumSum + sample;
	cumSumOfSquares = cumSumOfSquares + sample*sample;

		

	if (sample < minValue)
	{
		//
		// account sample in first interval if sample is below the lower boundary 
		//
		index = 0;
	}
	else if (sample >= maxValue)
	{
		//
		// account sample in last interval if sample is above the higher boundary 
		//
		index = numIntervals - 1;
	}
	else
	{
		//
		// account sample in appropriate interval
		// the interval number is actually the index of pValues, which is calculated by
		//        index = (sample - minValue) div intervalWidth
		//  where div is an integer division
		//
		index = (sample - minValue) / intervalWidth;
	}

	//
	// increment contents of target interval
	//
	pValues [index]++;

	return (rc);

  //## end ContinuousHistogram::nextSample%37442A34035D.body
}

//## Operation: reset%374C031B03A1
//	********************************************************************************
//
//	Name:RetCode reset (void)
//
//	Description: Reset all attributes allowing the start of a new sequence of samples.
//
//	Parameters: none
//
//	Returns: SCH_SUCCESS
//	           Error code in case of error
//
//	Remarks:Note that all old information is lost after the reset.
//
//	********************************************************************************
RetCode ContinuousHistogram::reset ()
{
  //## begin ContinuousHistogram::reset%374C031B03A1.body preserve=yes

	// resets attributes variables
	maxSample  = SIM_HUGE_VAL;	
	minSample  = SIM_HUGE_VAL;	  
	numSamples = 0;
	cumSum = 0.0;
	cumSumOfSquares = 0.0;

	if (pValues == NULL)
		return SCH_ALLOCATION_ERROR;       // the objetct has not been created
	else
		memset (pValues, 0, numIntervals * sizeof (Int)); // set all values to 0

	return SCH_SUCCESS;

  //## end ContinuousHistogram::reset%374C031B03A1.body
}

// Additional Declarations
  //## begin ContinuousHistogram%3744298B0396.declarations preserve=yes
  //## end ContinuousHistogram%3744298B0396.declarations

//## begin module%3744298B0396.epilog preserve=yes

} // namespace sch

//## end module%3744298B0396.epilog
