//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36B6DCF801E0.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36B6DCF801E0.cm

//## begin module%36B6DCF801E0.cp preserve=yes
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
//	File Name: CumulativeHistogram.cpp
//
//	Description: Implementation file for the ContinuousHistogram class.
//
//	Author:	 Eugenio Pacheli Alves / Hélio Azevedo
//
//	Creation Date:	May/20/99
//
//	Visual Source Safe $Revision: 6 $
//
//	Last check in $Date: 10/04/99 2:50p $
//
//	********************************************************************************
//## end module%36B6DCF801E0.cp

//## Module: CumulativeHistogram%36B6DCF801E0; Pseudo Package body
//## Source file: T:\StatisticsCalculation\CumulativeHistogram.cpp

//## begin module%36B6DCF801E0.additionalIncludes preserve=no
//## end module%36B6DCF801E0.additionalIncludes

//## begin module%36B6DCF801E0.includes preserve=yes

#include "MemLeak.h"
#include <assert.h>	  // define the assert macro

//## end module%36B6DCF801E0.includes

// CumulativeHistogram
#include "CumulativeHistogram.h"
//## begin module%36B6DCF801E0.additionalDeclarations preserve=yes


namespace sch {

//## end module%36B6DCF801E0.additionalDeclarations


// Class CumulativeHistogram 




//## Operation: CumulativeHistogram%36B72F2F0067
//	********************************************************************************
//
//	Name: ContinuousHistogram(Int intervals)
//
//	Description: Non-default constructor - requires the number of intervals.
//
//	Input parameters: Int intervals;  //number of intervals in the histogram
//
//	Returns: none
//
//	               SCH_ALLOCATION_ERROR  = could not allocate pMarkers array in base class
//	               SCH_INVALID_PARAMETER = intervals parameter is less than 2
//
//	Remarks: Althout there is not a strict restrition for the maximum number of intervals allowed, care should be exercised when such
//	number is chosen. Too much CPU time would be required in case of a very large number of intervals.
//
//	********************************************************************************
CumulativeHistogram::CumulativeHistogram (Int intervals)
  //## begin CumulativeHistogram::CumulativeHistogram%36B72F2F0067.hasinit preserve=no
      : numMarkers(intervals + 1),
        numSamples(0)
  //## end CumulativeHistogram::CumulativeHistogram%36B72F2F0067.hasinit
  //## begin CumulativeHistogram::CumulativeHistogram%36B72F2F0067.initialization preserve=yes
  //## end CumulativeHistogram::CumulativeHistogram%36B72F2F0067.initialization
{
  //## begin CumulativeHistogram::CumulativeHistogram%36B72F2F0067.body preserve=yes
 RetCode rc = SCH_SUCCESS;   // used to provide a single point for raising exception

	//
	// only markers greater than 3 is possible. We need the previous and posterior positions !!!
	//
	if (numMarkers < 3)  rc = SCH_INVALID_PARAMETER;


	if (rc == SCH_SUCCESS) {
		//
		// allocate pMarkers array
		//
		pMarkers = new Marker[numMarkers+1];   // allocate array. Unfortunately, the position 0 is not used
		if (pMarkers == NULL)    rc = SCH_ALLOCATION_ERROR;		// could not create array
		else {
			memset (pMarkers, 0, (numMarkers+1) * sizeof (Marker)); // set all values to 0
		}
	}


	// initialize the increment of desired positions. 
	// for histograms, values depend upon their position only.
	// Note that the increment 1 is never used.
	for (Int i = 2; i <= intervals; i++)
		pMarkers[i].incDesiredPos = (Double) (i-1) / (Double) intervals;

	if (rc != SCH_SUCCESS)    throw SchException (rc);   // an error occurred, abort object construction

  //## end CumulativeHistogram::CumulativeHistogram%36B72F2F0067.body
}



//## Other Operations (implementation)
//## Operation: getCumulativeResult%36B742F400FB
//	********************************************************************************
//
//	Name: RetCode getCumulativeResult(Int markerNumber, Float *pX, Float *pY)
//
//	Description: Returns the x-axis and y-axis values of the corresponding marker number. The x-axis suplies the markers heights and
//	the y-axis suplies the markers locations already divided by the number of samples.
//
//	Input parameters: Int markerNumber;  //first marker is 0, last is intervals
//
//	Output parameters: Float *pX;  //x-axis value
//	                   Float *pY;  //y-axis value
//
//	Returns: SCH_SUCCESS
//	         SCH_INVALID_PARAMETER - pX or pY is NULL
//	         SCH_INVALID_RANGE  - marker number is negative or above total number of markers - ignore contents of pResult
//	         SCH_INVALID_FUNCTION  - so far less than the numMarkers samples were inserted in the sequence
//
//	Remarks: the first intervals+1 observations are used to initialize the markers, so this function is only valid from this point
//	ahead.
//	The SCH_INVALID_FUNCTION error code is returned if less than intervals+1 samples were inserted in the sequence, so no action other
//	than ignoring the contents of pX and pY is required from the caller in this case.
//
//	********************************************************************************
RetCode CumulativeHistogram::getCumulativeResult (Int markerNumber, Float *pX, Float *pY)
{
  //## begin CumulativeHistogram::getCumulativeResult%36B742F400FB.body preserve=yes
	RetCode rc = SCH_SUCCESS;

	assert (pMarkers);

	markerNumber += 1;				// internally numMarkers beging at 1

	if ((pX == NULL) || (pY == NULL))
	{
		rc = SCH_INVALID_PARAMETER;
	}

	//
	// should be called only after numMarkers samples are inserted
	//
	else if (numSamples < numMarkers)
	{
		rc = SCH_INVALID_FUNCTION;
	}

	//
	// markerNumber within valid range?
	// 
	else if ((markerNumber < 1) || (markerNumber > numMarkers))
	{
		rc = SCH_INVALID_INTERVAL;				// no, return error
	}
	else
	{		
		*pX = pMarkers[markerNumber].height;
		*pY = (Float)pMarkers[markerNumber].position / numSamples;
	}

	return (rc);

  //## end CumulativeHistogram::getCumulativeResult%36B742F400FB.body
}

//## Operation: getDerivedResult%372F25DD027B
//	********************************************************************************
//
//	Name: RetCode getDerivedResult(Int markerNumber, Float *pX, Float *pY)
//
//	Description: Returns thex-axis and y-axis values of the of a  derived function obtained  from a algebraic process over the
//	CumulativeHistogram results. The x-axis suplies the originals markers heights and the y-axis suplies, for a given i, the following
//	result:( location(i+1) - location(i)) / (samples*(heights(i+1) - heights(i))).
//
//	Input parameters: Int markerNumber;  //first marker is 0, last is number of intervals - 1
//
//	Output parameters: Float *pX;  //x-axis value
//	                   Float *pY;  //y-axis value
//
//	Returns: SCH_SUCCESS
//	         SCH_INVALID_PARAMETER - pX or pY is NULL
//	         SCH_INVALID_RANGE  - marker number is negative or above total number of markers - ignore contents of pResult
//	         SCH_INVALID_FUNCTION  - so far less than the numMarkers samples were inserted in the sequence
//
//	Remarks: the first intervals+1 observations are used to initialize the markers, so this function is only valid from this point
//	ahead.
//	The SCH_INVALID_FUNCTION error code is returned if less than intervals+1 samples were inserted in the sequence, so no action other
//	than ignoring the contents of pX and pY is required from the caller in this case.
//	********************************************************************************
RetCode CumulativeHistogram::getDerivedResult (Int markerNumber, Float *pX, Float *pY)
{
  //## begin CumulativeHistogram::getDerivedResult%372F25DD027B.body preserve=yes
	RetCode rc = SCH_SUCCESS;

	assert (pMarkers);

	markerNumber += 1;				// internally numMarkers beging at 1

	if ((pX == NULL) || (pY == NULL))
	{
		rc = SCH_INVALID_PARAMETER;
	}

	//
	// should be called only after numMarkers samples are inserted
	//
	else if (numSamples < numMarkers)
	{
		rc = SCH_INVALID_FUNCTION;
	}

	//
	// markerNumber within valid range?
	// 
	else if ((markerNumber < 1) || (markerNumber >= numMarkers))
	{
		rc = SCH_INVALID_INTERVAL;				// no, return error
	}
	else
	{
	*pX = pMarkers[markerNumber+1].height;

	// obtain: position(i) - position(i-1) / (samples * ( height(i) - height(i-1))
	*pY =  pMarkers[markerNumber+1].position - pMarkers[markerNumber].position;
	*pY =  *pY / ( (Double) numSamples * (pMarkers[markerNumber+1].height - pMarkers[markerNumber].height));

	}

	return (rc);


  //## end CumulativeHistogram::getDerivedResult%372F25DD027B.body
}

//## Operation: nextSample%373B0B46022C
//	********************************************************************************
//
//	Name: RetCode nextSample (Float sample)
//
//	Description: Includes the next sample in the sequence and recalculates each marker.
//
//	Input parameters: Float sample;  //next sample in the sequence
//
//	Returns: SCH_SUCCESS
//
//	Remarks: the first intervals+1   samples are sorted and used to initialize the markers values. Actual computations start from the
//	this point ahead.
//
//	********************************************************************************
RetCode CumulativeHistogram::nextSample (Float sample)
{
  //## begin CumulativeHistogram::nextSample%373B0B46022C.body preserve=yes
	Int     numIntervals = numMarkers - 1;	// aux int var for faster computations
	RetCode rc = SCH_SUCCESS;  

	assert (pMarkers);
	//
	// the only thing to do with the first numMarker (m) samples is to sort them
	// and initialize the markers
	//
	if (numSamples < numMarkers) {
		//
		// fill pMarkers array sequentially and sort it when the 
		// (m)th sample is inserted
		//
		numSamples++;								  // numSamples = i
		pMarkers[numSamples].height   = sample;       // q(i) = x(i)
		pMarkers[numSamples].position = numSamples;   // n(i) = i

		//
		// did we get all m samples?
		//
		if (numSamples == numMarkers){
			//
			// sort the samples using the selection sort algorithm. This
			// is an efficient algorithm for very small lists and has the
			// advantage of being easy to implement and debug
			//
			selectionSort ();
		}
	}

	//
	// now true computation takes place
	//
	else
	{
		//
		// find cell k such that q(k) <= sample < q(k+1) and 
		// adjust extreme values q(1) and q(m) if necessary
		//
		Int k = 1;
		Int i;						// loop control
		numSamples++;				// update number of samples

		if (sample < pMarkers[1].height)	{				// sample < q(1)
			k = 1;
			pMarkers[1].height = sample;
		}
		else 
			if (sample >= pMarkers[numMarkers].height)	{	// sample >= q(m)		
				k = numIntervals;
				if (sample > pMarkers[numMarkers].height)	// sample > q(m)
					pMarkers[numMarkers].height = sample;
			}
			else {
				while (sample >= pMarkers[k].height) 		// q(k-1) <= sample < q(k)
					k++;
			}

		//
		// increment positions of markers k+1 through b+1
		//
		for (i = k; i <= numMarkers; i++) {
			pMarkers[i].position++;
		}

		//
		// adjust heights of markers 2 thru numIntervals if necessary
		//

		for (i = 2; i <= numIntervals; i++) {
			Int   deltaSign;
			Double desiredHeight;

			Double delta = 1.0 + (Double)(numSamples-1) * pMarkers[i].incDesiredPos;

			delta = delta -  pMarkers[i].position;
			if (((delta >= +1) && ((pMarkers[i+1].position - pMarkers[i].position) > +1)) || 
				((delta <= -1) && ((pMarkers[i-1].position - pMarkers[i].position) < -1)))	{

				deltaSign = (delta > 0) ? +1 : -1;  // = +1 if delta is positive; -1 otherwise

				desiredHeight = parabolic (i, deltaSign);   // from parabolic formula

				if ((pMarkers[i-1].height < desiredHeight) && 
					(desiredHeight < pMarkers[i+1].height)) {
					pMarkers[i].height = desiredHeight;
				}
				else 	{
					//
					// use linear formula
					//
					pMarkers[i].height = linear (i, deltaSign);  // from linear formula
				}
				pMarkers[i].position += deltaSign;
			}
		} // for i = 2 to numIntervals
	} // else

	return (rc);


  //## end CumulativeHistogram::nextSample%373B0B46022C.body
}

//## Operation: reset%374C03380276
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
RetCode CumulativeHistogram::reset ()
{
  //## begin CumulativeHistogram::reset%374C03380276.body preserve=yes

	Int intervals = numMarkers - 1;

	// resets attributes variables
	numSamples = 0;
 	if (pMarkers == NULL)
		return SCH_ALLOCATION_ERROR;		// the array has not been created
	else 
		memset (pMarkers, 0, (numMarkers+1) * sizeof (Marker)); // set all values to 0

	// initialize the increment of desired positions. 
	// for histograms, values depend upon their position only.
	// Note that the increment 1 is never used.
	for (Int i = 2; i <= intervals; i++)
		pMarkers[i].incDesiredPos = (Double) (i-1) / (Double) intervals;

	return SCH_SUCCESS;
  //## end CumulativeHistogram::reset%374C03380276.body
}

// Additional Declarations
  //## begin CumulativeHistogram%36B6DCF801E0.declarations preserve=yes

//	********************************************************************************
//
//	Name: void selectionSort (void)
//
//	Description: Sorts the elements of a list in ascending order.
//
//	Parameters: none
//
//	Returns: none
//
//	********************************************************************************
void CumulativeHistogram::selectionSort (void)
{
	Double   bestValue;
	Int     i, j, bestJ;
	Marker *pI, *pJ;

	for (i = 1, pI = &pMarkers[1]; i < numMarkers ; i++, pI++)
	{
		bestValue = pI->height;
		bestJ = i;
		for (j = i + 1, pJ = pI + 1; j <= numMarkers; j++, pJ++)
		{
		  if (pJ->height < bestValue)
		  {
			  bestValue = pJ->height;
			  bestJ = j;
		  }
		} //for j

		if (i != bestJ)
		{
			pMarkers[bestJ].height = pI->height;
			pI->height = bestValue;
		}
	} // for i

	return;
}

//	********************************************************************************
//
//	Name: Double parabolic (Int i, Int deltaSign)
//
//	Description: Calculates a new marker height using the parabolic formula.
//
//	Input parameters: Int i;         //position in pMarker according to marker position
//                    Int deltaSign; //left or right shift (-1 or +1 respectively)
//
//	Returns: the calculated marker height
//
//	********************************************************************************
Double CumulativeHistogram::parabolic (Int i, Int deltaSign)
{
	assert ((deltaSign == +1) || (deltaSign == -1)); // deltaSign must be +1 or -1
	assert ((i > 1) && (i < numMarkers ));			// i = 2... numIntervals

	Double   result, a, b, b1, b2, c, d;
	Marker *pThisMarker = &pMarkers[i],			     // points to this marker
		   *pPrevMarker = &pMarkers[i-1],			 // points to prev marker
		   *pNextMarker = &pMarkers[i+1];			 // points to next marker

	//
	// this is a large computation, so let's do it in several steps. Formula is:
	//       q(i) = q(i) + { a * b }
	//		 a  = deltaSign / { n(i+1) - n(i-1) }
	//       b  = b1 + b2
	//       b1 = { n(i) - n(i-1) + deltaSign } * c
	//       b2 = { n(i+1) - n(i) - deltaSign } * d
	//       c  = { q(i+1) - q(i) } / { n(i+1) - n(i) }
	//       d  = { q(i) - q(i-1) } / { n(i) - n(i-1) }
	//
	d  = (pThisMarker->height   - pPrevMarker->height)  /
	 	 (pThisMarker->position - pPrevMarker->position);
	c  = (pNextMarker->height   - pThisMarker->height)  /
		 (pNextMarker->position - pThisMarker->position);
	b2 = (pNextMarker->position - pThisMarker->position - deltaSign) * d;
	b1 = (pThisMarker->position - pPrevMarker->position + deltaSign) * c;
	b  = b1 + b2;
	a  = (Double) deltaSign / (Float) (pNextMarker->position - pPrevMarker->position);
	result = pThisMarker->height + (a * b);



	return (result);
}

//	********************************************************************************
//
//	Name: Double linear (Int i, Int deltaSign)
//
//	Description: Calculates a new marker height using the linear formula.
//
//	Input parameters: Int i;         //position in pMarker according to marker position
//                    Int deltaSign; //left or right shift (-1 or +1 respectively)
//
//	Returns: the calculated marker height
//
//	********************************************************************************
Double CumulativeHistogram::linear (Int i, Int deltaSign)
{
	assert ((deltaSign == +1) || (deltaSign == -1));  // deltaSign must be +1 or -1
	assert ((i > 1) && (i < numMarkers));		  // i = 2...numIntervals


	Double   result, a;
	Marker *pThisMarker  = &pMarkers[i],			  // points to this marker
		   *pDeltaMarker = &pMarkers[i + deltaSign];  // points to prev or next marker
	//
	// let's take a 2-step calculation:
	//       d    = deltaSign
	//   	 q(i) = q(i) + d * a;
	//       a    = { q(i+d) - q(i) } / { n(i+d) - n(i) }
	//
	a = (pDeltaMarker->height   - pThisMarker->height)  /
	    (Double)(pDeltaMarker->position - pThisMarker->position);

	result = pThisMarker->height + deltaSign * a;

	return (result);
}

  //## end CumulativeHistogram%36B6DCF801E0.declarations
//## begin module%36B6DCF801E0.epilog preserve=yes

} // namespace sch

//## end module%36B6DCF801E0.epilog
