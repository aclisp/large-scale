//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36B6DCD603AD.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36B6DCD603AD.cm

//## begin module%36B6DCD603AD.cp preserve=yes
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
//	File Name: DiscreteHistogram.cpp
//
//	Description: Implementation file for the DiscreteHistogram class.
//
//	Author: Eugenio Pacheli Alves
//
//	Creation Date: Apr/12/99
//
//	Visual Source Safe $Revision: 7 $
//
//	Last check in $Date: 16/06/99 9:30 $
//
//	********************************************************************************
//## end module%36B6DCD603AD.cp

//## Module: DiscreteHistogram%36B6DCD603AD; Pseudo Package body
//## Source file: T:\StatisticsCalculation\DiscreteHistogram.cpp

//## begin module%36B6DCD603AD.additionalIncludes preserve=no
//## end module%36B6DCD603AD.additionalIncludes

//## begin module%36B6DCD603AD.includes preserve=yes

#include "MemLeak.h"
#include <assert.h>	  // define the assert macro

//## end module%36B6DCD603AD.includes

// DiscreteHistogram
#include "DiscreteHistogram.h"
//## begin module%36B6DCD603AD.additionalDeclarations preserve=yes

using namespace std;

namespace sch {

//## end module%36B6DCD603AD.additionalDeclarations


// Class DiscreteHistogram 









//## Operation: DiscreteHistogram%36B74612038D
//	********************************************************************************
//
//	Name: DiscreteHistogram(Int minVal, Int maxVal, Int intervals)
//
//	Description: Non-default constructor - requires the lower boundary, upper boundary and number of intervals. The pValues array is
//	allocated.
//
//	Input parameters: Int minVal;     //lower boundary of the input data stream
//	                  Int maxVal;     //upper boundary of the input data stream
//	                  Int intervals;  //number of intervals in the histogram
//
//	Returns: none
//
//	Exceptions: SchException, with one of the following error codes:
//	               SCH_ALLOCATION_ERROR       = could not allocate pValues array
//	               SCH_INVALID_INTERVAL_WIDTH = the remainder of (maxVal - minVal) divided by intervals is not zero
//	               SCH_INVALID_RANGE          = upper boundary is not greater than lower boundary
//
//	Remarks: the interval width must be an integer value, that is, the number of intervals must be set such that that precondition is
//	satisfied.
//
//	********************************************************************************
DiscreteHistogram::DiscreteHistogram (Int minVal, Int maxVal, Int intervals)
  //## begin DiscreteHistogram::DiscreteHistogram%36B74612038D.hasinit preserve=no
      : maxValue(maxVal),
        minValue(minVal),
        numIntervals(intervals),
        pValues(NULL),
        maxSample(MAX_VALUE_INT),
        minSample(MIN_VALUE_INT),
        numSamples(0)
  //## end DiscreteHistogram::DiscreteHistogram%36B74612038D.hasinit
  //## begin DiscreteHistogram::DiscreteHistogram%36B74612038D.initialization preserve=yes
  //## end DiscreteHistogram::DiscreteHistogram%36B74612038D.initialization
{
  //## begin DiscreteHistogram::DiscreteHistogram%36B74612038D.body preserve=yes

	RetCode rc = SCH_SUCCESS;   // used to provide a single point for raising exception

	//
	// upper boundary must be greater than lower boundary
	//
	if (minValue >= maxValue)
	{
		rc = SCH_INVALID_RANGE;
	}

	if (rc == SCH_SUCCESS)
	{
		//
		// each interval width must be an integer value
		//
		if (((maxValue - minValue) % numIntervals) != 0)
		{
			rc = SCH_INVALID_INTERVAL_WIDTH;
		}
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

  //## end DiscreteHistogram::DiscreteHistogram%36B74612038D.body
}



//## Other Operations (implementation)
//## Operation: getResult%36B7468000CE
//	********************************************************************************
//
//	Name: RetCode getResult(Int intervalNumber, Int *pResult)
//
//	Description: Returns the contents (frequency) of the corresponding interval number.
//
//	Input parameters: Int intervalNumber;  //first interval is zero
//
//	Output parameters: Int *pResult;  //contents of the corresponding interval number
//
//	Returns: SCH_SUCCESS
//	         SCH_INVALID_PARAMETER - pResult is NULL
//	         SCH_INVALID_INTERVAL  - interval number is negative or above higher interval - ignore contents of pResult
//
//	********************************************************************************
RetCode DiscreteHistogram::getResult (Int intervalNumber, Int *pResult)
{
  //## begin DiscreteHistogram::getResult%36B7468000CE.body preserve=yes

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

  //## end DiscreteHistogram::getResult%36B7468000CE.body
}

//## Operation: nextSample%36B74653034A
//	********************************************************************************
//
//	Name: RetCode nextSample (Int sample)
//
//	Description: Includes the next sample in the sequence and increments the contents of the interval which this sample falls into.
//
//	Input parameters: Int sample;  //next sample in the sequence
//
//	Returns: SCH_SUCCESS
//
//	********************************************************************************
RetCode DiscreteHistogram::nextSample (Int sample)
{
  //## begin DiscreteHistogram::nextSample%36B74653034A.body preserve=yes

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

  //## end DiscreteHistogram::nextSample%36B74653034A.body
}

//## Operation: reset%374C032A0262
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
RetCode DiscreteHistogram::reset ()
{
  //## begin DiscreteHistogram::reset%374C032A0262.body preserve=yes

	// resets attributes variables
	maxSample = MAX_VALUE_INT;
	minSample = MIN_VALUE_INT;
	numSamples= 0;

	if (pValues == NULL)
		return SCH_ALLOCATION_ERROR;       // the array has not been created
	else
		memset (pValues, 0, numIntervals * sizeof (Int)); // set all values to 0

	return SCH_SUCCESS;

  //## end DiscreteHistogram::reset%374C032A0262.body
}

// Additional Declarations
  //## begin DiscreteHistogram%36B6DCD603AD.declarations preserve=yes
  //## end DiscreteHistogram%36B6DCD603AD.declarations

//## begin module%36B6DCD603AD.epilog preserve=yes

} // namespace sch

//## end module%36B6DCD603AD.epilog
