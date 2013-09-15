//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%379CA42802FE.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%379CA42802FE.cm

//## begin module%379CA42802FE.cp preserve=yes
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
//	File Name: HighPrecisionWinAvg.cpp
//
//	Description: Implementation file for the HighPrecisionWinAvg class.
//
//	Author: Eugenio Pacheli Alves
//
//	Creation Date: Jul/26/99
//
//	Visual Source Safe $Revision: 3 $
//
//	Last check in $Date: 30/07/99 14:13 $
//
//	********************************************************************************
//## end module%379CA42802FE.cp

//## Module: HighPrecisionWinAvg%379CA42802FE; Pseudo Package body
//## Source file: T:\StatisticsCalculation\HighPrecisionWinAvg.cpp

//## begin module%379CA42802FE.additionalIncludes preserve=no
//## end module%379CA42802FE.additionalIncludes

//## begin module%379CA42802FE.includes preserve=yes

#include "MemLeak.h"
#include <assert.h>	  // defines the assert macro
#include <math.h>	  // defines HUGE_VAL

//## end module%379CA42802FE.includes

// HighPrecisionWinAvg
#include "HighPrecisionWinAvg.h"
//## begin module%379CA42802FE.additionalDeclarations preserve=yes

using namespace std;

namespace sch {

//## end module%379CA42802FE.additionalDeclarations


// Class HighPrecisionWinAvg 





//## Operation: HighPrecisionWinAvg%379CA63D020A
//	********************************************************************************
//
//	Name: HighPrecisionWinAvg(Int winSize)
//
//	Description: Non-default constructor - requires the window size for calculating the windowed average.
//
//	Input parameters: Int winSize;  //window for calculating the windowed average
//
//	Returns: none
//
//	Exceptions: SchException, with one of the following error codes:
//	               SCH_ALLOCATION_ERROR = could not allocate pLastSamples array
//	               SCH_INVALID_PARAMETER = winSize parameter = 0
//
//	Remarks: winSize must be non-zero.
//
//	********************************************************************************
HighPrecisionWinAvg::HighPrecisionWinAvg (Int winSize)
  //## begin HighPrecisionWinAvg::HighPrecisionWinAvg%379CA63D020A.hasinit preserve=no
      : headWA(0),
        numWA(0),
        pLastSamples(NULL),
        windowSize(winSize)
  //## end HighPrecisionWinAvg::HighPrecisionWinAvg%379CA63D020A.hasinit
  //## begin HighPrecisionWinAvg::HighPrecisionWinAvg%379CA63D020A.initialization preserve=yes
  //## end HighPrecisionWinAvg::HighPrecisionWinAvg%379CA63D020A.initialization
{
  //## begin HighPrecisionWinAvg::HighPrecisionWinAvg%379CA63D020A.body preserve=yes

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
		pLastSamples = new Double[windowSize]; // allocate array
		if (pLastSamples == NULL)
		{
			rc = SCH_ALLOCATION_ERROR;        // could not create array
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

  //## end HighPrecisionWinAvg::HighPrecisionWinAvg%379CA63D020A.body
}


//## Operation: ~HighPrecisionWinAvg%379CA63D0232
//	********************************************************************************
//
//	Name: ~HighPrecisionWinAvg(void)
//
//	Description: Destructor - deallocates pLastSamples array.
//
//	Parameters: none
//
//	Returns: none
//
//	Remarks: this destructor is virtual since the HighPrecisionWinAvg class may be base for other classes.
//
//	********************************************************************************
HighPrecisionWinAvg::~HighPrecisionWinAvg ()
{
  //## begin HighPrecisionWinAvg::~HighPrecisionWinAvg%379CA63D0232.body preserve=yes

	delete [] pLastSamples;   // delete array

  //## end HighPrecisionWinAvg::~HighPrecisionWinAvg%379CA63D0232.body
}



//## Other Operations (implementation)
//## Operation: getWindowedAverage%379CA63D025A
//	********************************************************************************
//
//	Name: Double getWindowedAverage (void)
//
//	Description: Returns the current windowed average of the sequence, which is calculated from the values stored in the pLastSamples
//	array.
//
//	Parameters: none
//
//	Returns: the calculated windowed average.
//	         SIM_HUGE_VAL - no sample inserted in sequence yet.
//
//	********************************************************************************
Double HighPrecisionWinAvg::getWindowedAverage ()
{
  //## begin HighPrecisionWinAvg::getWindowedAverage%379CA63D025A.body preserve=yes

	assert (pLastSamples != NULL);
	
	// 
	// the windowed average is the sum of all elements in pLastSamples array divided by 
	// the number of samples in the array. Since all elements are accounted in the sum we
	// don't care where headWA points to - just start from the first element and add
	// each subsequent element until all numWA elements are added
	//
	Int    i = 0;
	Double  windowedAverage = 0.0;			// current windowed average reckoned
	Double *pCurrentSample  = pLastSamples;	// points to current element; start from first

	while (i++ < numWA)
	{
		windowedAverage += *pCurrentSample++; // add current sample and advance pointer to next sample
	}

	//
	// divide sum reckoned above by the number of elements in array
	// if no sample has been inserted yet, then return minus infinite
	//
	if (numWA == 0)
		windowedAverage = SIM_HUGE_VAL;
	else
		windowedAverage = (windowedAverage / numWA);  

	return (windowedAverage);

  //## end HighPrecisionWinAvg::getWindowedAverage%379CA63D025A.body
}

//## Operation: nextSample%379CA63D0282
//	********************************************************************************
//
//	Name: RetCode nextSample (Double sample)
//
//	Description: Includes the sample in pLastSamples array.
//
//	Input parameters: Double sample;  //next sample in the sequence
//
//	Returns: SCH_SUCCESS
//
//	Remarks: this operation is virtual since derived classes may have their own way of handling insertion of samples in the sequence.
//
//	********************************************************************************
RetCode HighPrecisionWinAvg::nextSample (Double sample)
{
  //## begin HighPrecisionWinAvg::nextSample%379CA63D0282.body preserve=yes

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

	return (rc);

  //## end HighPrecisionWinAvg::nextSample%379CA63D0282.body
}

//## Operation: reset%379CA63D02AA
//	********************************************************************************
//
//	Name:RetCode reset (void)
//
//	Description: Reset all attributes allowing the start of a new sequence of samples.
//
//	Parameters: none
//
//	Returns: SCH_SUCCESS
//
//	Remarks: note that all old information is lost after the reset.
//
//	********************************************************************************
RetCode HighPrecisionWinAvg::reset ()
{
  //## begin HighPrecisionWinAvg::reset%379CA63D02AA.body preserve=yes

	RetCode rc = SCH_SUCCESS;

	// resets attributes variables
	headWA = 0;
	numWA = 0;

	return (rc);

  //## end HighPrecisionWinAvg::reset%379CA63D02AA.body
}

// Additional Declarations
  //## begin HighPrecisionWinAvg%379CA42802FE.declarations preserve=yes
  //## end HighPrecisionWinAvg%379CA42802FE.declarations

//## begin module%379CA42802FE.epilog preserve=yes

} // namespace sch

//## end module%379CA42802FE.epilog
