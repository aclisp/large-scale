//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%370E56FC0001.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%370E56FC0001.cm

//## begin module%370E56FC0001.cp preserve=yes
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
//	File Name: WindowedAverage.cpp
//
//	Description: Implementation file for the WindowedAverage class.
//
//	Author: Eugenio Pacheli Alves
//
//	Creation Date: Apr/12/99
//
//	Visual Source Safe $Revision: 4 $
//
//	Last check in $Date: 16/06/99 9:30 $
//
//	********************************************************************************
//## end module%370E56FC0001.cp

//## Module: WindowedAverage%370E56FC0001; Pseudo Package body
//## Source file: T:\StatisticsCalculation\WindowedAverage.cpp

//## begin module%370E56FC0001.additionalIncludes preserve=no
//## end module%370E56FC0001.additionalIncludes

//## begin module%370E56FC0001.includes preserve=yes

#include "MemLeak.h"
#include <assert.h>	  // define the assert macro

//## end module%370E56FC0001.includes

// WindowedAverage
#include "WindowedAverage.h"
//## begin module%370E56FC0001.additionalDeclarations preserve=yes

using namespace std;

namespace sch {

//## end module%370E56FC0001.additionalDeclarations


// Class WindowedAverage 






//## Operation: WindowedAverage%370E60FC01EC
//	********************************************************************************
//
//	Name: WindowedAverage(Int winSize)
//
//	Description: Non-default constructor - requires the window for calculating the windowed average.
//
//	Input parameters: Int winSize;  //window for calculating the windowed average
//
//	Returns: none
//
//	Exceptions: SchException, with one of the following error codes:
//	               SCH_ALLOCATION_ERROR = could not allocate pLastSamples array or pMean object
//	               SCH_INVALID_PARAMETER = winSize parameter = 0
//
//	Remarks: winSize must be non-zero.
//
//	********************************************************************************
WindowedAverage::WindowedAverage (Int winSize)
  //## begin WindowedAverage::WindowedAverage%370E60FC01EC.hasinit preserve=no
      : headWA(0),
        numWA(0),
        pLastSamples(NULL),
        windowSize(winSize)
  //## end WindowedAverage::WindowedAverage%370E60FC01EC.hasinit
  //## begin WindowedAverage::WindowedAverage%370E60FC01EC.initialization preserve=yes
  //## end WindowedAverage::WindowedAverage%370E60FC01EC.initialization
{
  //## begin WindowedAverage::WindowedAverage%370E60FC01EC.body preserve=yes

	RetCode rc = SCH_SUCCESS;   // used to provide a single point for raising exception

	//
	// allocate pLastSamples[windowSize] array
	//
	if (windowSize == 0)
	{
		rc = SCH_INVALID_PARAMETER;           // window size must be non-zero
	}
	else
	{
		pLastSamples = new Float[windowSize]; // allocate array
		if (pLastSamples == NULL)
		{
			rc = SCH_ALLOCATION_ERROR;        // could not create array
		}
	}

	//
	// allocate Mean object
	//
	if (rc == SCH_SUCCESS)
	{
		pMean = new Mean();
		if (pMean == NULL)
		{
			rc = SCH_ALLOCATION_ERROR;        // could not create object
		}
	}

	//
	// error?
	//
	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);              // an error occurred, abort object construction
	}

	return;

  //## end WindowedAverage::WindowedAverage%370E60FC01EC.body
}



//## Other Operations (implementation)
//## Operation: getWindowedAverage%370E5768009C
//	********************************************************************************
//
//	Name: Float getWindowedAverage (void)
//
//	Description: Returns the current windowed average of the sequence, which is calculated from the values stored in the pLastSamples
//	array.
//
//	Parameters: none
//
//	Returns: the calculated windowed average.
//	         SIM_HUGE_VAL - Does not exist samples yet.
//
//	********************************************************************************
Float WindowedAverage::getWindowedAverage ()
{
  //## begin WindowedAverage::getWindowedAverage%370E5768009C.body preserve=yes

	assert (pLastSamples != NULL);
	
	// 
	// the windowed average is the sum of all elements in pLastSamples array divided by 
	// the number of samples in the array. Since all elements are accounted in the sum we
	// don't care where headWA points to - just start from the first element and add
	// each subsequent element until all numWA elements are added
	//
	Int    i = 0;
	Float  windowedAverage = 0.0f;	        // current windowed average reckoned
	Float *pCurrentSample  = pLastSamples;  // points to current element; start from first

	while (i++ < numWA)
	{
		windowedAverage += *pCurrentSample++; // add current sample and advance pointer to next sample
	}

	// divide sum reckoned above by the number of elements in array
	windowedAverage  = (numWA == 0) ? SIM_HUGE_VAL : ( windowedAverage / numWA);  
	return (windowedAverage);

  //## end WindowedAverage::getWindowedAverage%370E5768009C.body
}

//## Operation: nextSample%370E58D40190
//	********************************************************************************
//
//	Name: RetCode nextSample (Float sample)
//
//	Description: Includes the sample both in the sequence maintained by Mean class (recalculating its values) and pLastSamples array.
//
//	Input parameters: Float sample;  //next sample in the sequence
//
//	Returns: SCH_SUCCESS
//
//	Remarks: this operation is virtual since derived classes may have their own way of handling insertion of samples in the sequence.
//
//	********************************************************************************
RetCode WindowedAverage::nextSample (Float sample)
{
  //## begin WindowedAverage::nextSample%370E58D40190.body preserve=yes

	RetCode rc = SCH_SUCCESS;

	assert (pLastSamples != NULL);

	//
	// insert this sample in pLastSamples array
	//
	pLastSamples[headWA] = sample;  // headWA indexes the insertion point

	//
	// increment counter if buffer not full
	//
	if (numWA < windowSize)
	{
		numWA++;
	}

	//
	// advance head index one position (to the next insertion point). The pLastSamples
	// array is actually a circular buffer, so the head wraps to the first element
	// after reaching the last one. A very efficient way of incrementing indexes in 
	// circular buffers is to increment the pointer and then divide it by the maximum
	// allowed value. The remainder of such division is the next, incremented position
	//
	headWA = (headWA + 1) % windowSize;

	//
	// calculate new mean
	//
	rc = pMean->nextSample (sample);

	return (rc);

  //## end WindowedAverage::nextSample%370E58D40190.body
}

//## Operation: reset%374C034D035D
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
RetCode WindowedAverage::reset ()
{
  //## begin WindowedAverage::reset%374C034D035D.body preserve=yes

	// resets attributes variables
	headWA = 0;
	numWA = 0;

	if (pMean == NULL)
		return SCH_ALLOCATION_ERROR;        // the object has not been created
	else 
		return pMean->reset();

  //## end WindowedAverage::reset%374C034D035D.body
}

// Additional Declarations
  //## begin WindowedAverage%370E56FC0001.declarations preserve=yes
  //## end WindowedAverage%370E56FC0001.declarations

//## begin module%370E56FC0001.epilog preserve=yes

} // namespace sch

//## end module%370E56FC0001.epilog
