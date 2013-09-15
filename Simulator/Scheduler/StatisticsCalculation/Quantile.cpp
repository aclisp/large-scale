//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC5A940175.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC5A940175.cm

//## begin module%36AC5A940175.cp preserve=yes
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
//	File Name: Quantile.cpp
//
//	Description: Implementation file for the Quantile class.
//
//	Author: Eugenio Pacheli Alves
//
//	Creation Date: Apr/14/99
//
//	Visual Source Safe $Revision: 9 $
//
//	Last check in $Date: 10/04/99 2:51p $
//
//	********************************************************************************
//## end module%36AC5A940175.cp

//## Module: Quantile%36AC5A940175; Pseudo Package body
//## Source file: T:\StatisticsCalculation\Quantile.cpp

//## begin module%36AC5A940175.additionalIncludes preserve=no
//## end module%36AC5A940175.additionalIncludes

//## begin module%36AC5A940175.includes preserve=yes

#include "MemLeak.h"
#include <assert.h>	  // define the assert macro

//## end module%36AC5A940175.includes

// Quantile
#include "Quantile.h"
//## begin module%36AC5A940175.additionalDeclarations preserve=yes

namespace sch {

//## end module%36AC5A940175.additionalDeclarations


// Class Quantile 





//## Operation: Quantile%36B700850272
//	********************************************************************************
//
//	Name: Quantile(Float quantileThreshold)
//
//	Description: Non-default constructor - requires the p value to calculate the p-quantile (the value below which 100p percent of the
//	distribution lies).
//
//	Input parameters: Float quantileThreshold;   //p value
//
//
//	Returns: none
//
//	Exceptions: SchException, with one of the following error codes:
//	               SCH_ALLOCATION_ERROR  = could not allocate pMarkers array
//	               SCH_INVALID_PARAMETER = quantileThreshold  is less than 0 or greater than 1
//
//
//	********************************************************************************
Quantile::Quantile (Float quantileThreshold)
  //## begin Quantile::Quantile%36B700850272.hasinit preserve=no
      : numMarkers(5),
        numSamples(0),
        threshold(quantileThreshold)
  //## end Quantile::Quantile%36B700850272.hasinit
  //## begin Quantile::Quantile%36B700850272.initialization preserve=yes
  //## end Quantile::Quantile%36B700850272.initialization
{
  //## begin Quantile::Quantile%36B700850272.body preserve=yes

	RetCode rc = SCH_SUCCESS;   // used to provide a single point for raising exception

	//
	// threshold must range between 0 and 100
	//
	if ((threshold < 0) || (threshold > 1))
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (rc == SCH_SUCCESS)
	{
		//
		// allocate pMarkers array
		//
		pMarkers = new Marker[numMarkers];   // allocate array
		if (pMarkers == NULL)
		{
			rc = SCH_ALLOCATION_ERROR;       // could not create array
		}
		else
		{
			memset (pMarkers, 0, numMarkers * sizeof (Marker)); // set all values to 0

			//
			// initialize the increment of desired positions and 
			// calculate initial desired positions
			// for quantiles, values depend upon threshold and their position
			//
			pMarkers[0].incDesiredPos = 0;
			pMarkers[1].incDesiredPos = threshold / 2;
			pMarkers[2].incDesiredPos = threshold;
			pMarkers[3].incDesiredPos = (1 + threshold) / 2;
			pMarkers[4].incDesiredPos = 1;

			pMarkers[0].desiredPos = 1;
			pMarkers[1].desiredPos = 1 + (2 * threshold);
			pMarkers[2].desiredPos = 1 + (4 * threshold);
			pMarkers[3].desiredPos = 3 + (2 * threshold);
			pMarkers[4].desiredPos = 5;
		}
	}

	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);     // an error occurred, abort object construction
	}

	return;

  //## end Quantile::Quantile%36B700850272.body
}



//## Other Operations (implementation)
//## Operation: getQuantile%36B715D70158
//	********************************************************************************
//
//	Name:Float  getQuantile()
//
//	Description: Returns the current value of pQuantile, which is the height of the third marker (the middle marker, since quantile
//	computations always use 5 markers).
//
//	Output parameters: none
//
//	Returns: value of the corresponding pQuantile
//	         SIM_HUGE_VAL - so far less than 5 samples were inserted in the sequence
//
//	Remarks: the first five observations are used to initialize the markers, so this function is only valid from the 6th sample on. SIM_
//	HUGE_VAL is returned if less than 6 samples were inserted in the sequence, so no action other than ignoring the returned value is
//	required from the caller in this case.
//
//	********************************************************************************
Float Quantile::getQuantile ()
{
  //## begin Quantile::getQuantile%36B715D70158.body preserve=yes

	assert (pMarkers);

	//
	// should be called only after 6th sample is inserted
	//
	return (numSamples < 6) ? SIM_HUGE_VAL : pMarkers[2].height;

  //## end Quantile::getQuantile%36B715D70158.body
}

//## Operation: nextSample%36B700DE02C0
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
//	Remarks: the first numMarkers (5 for quantile computations) samples are sorted and used to initialize the markers values. Actual
//	computations start from the (numMarkers+1)th sample (6th sample for quantile computations).
//
//	********************************************************************************
RetCode Quantile::nextSample (Float sample)
{
  //## begin Quantile::nextSample%36B700DE02C0.body preserve=yes
							   
	Marker *pThis,					// points to (i)th   marker for faster computations
		   *pPrev,					// points to (i-1)th marker for faster computations
		   *pNext;					// points to (i+1)th marker for faster computations
	Int     numIntervals = numMarkers - 1;	// aux int var for faster computations
	RetCode rc = SCH_SUCCESS;  

	assert (pMarkers);


	//
	// the only thing to do with the first numMarker (m) samples is to sort them
	// and initialize the markers
	//
	if (numSamples < numMarkers)
	{
		//
		// fill pMarkers array sequentially and sort it when the 
		// (m)th sample is inserted
		//
		pThis = &(pMarkers[numSamples]);              // pos in array = (i - 1)
		numSamples++;								  // numSamples = i
		pThis->height   = sample;                     // q(i) = x(i)
		pThis->position = numSamples;                 // n(i) = i

		//
		// did we get all m samples?
		//
		if (numSamples == numMarkers)
		{
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
		Int i;			// loop control
		numSamples++;

		if (sample < pMarkers[0].height)					// sample < q(1)
		{
			k = 1;
			pMarkers[0].height = sample;
		}
		else if (sample >= pMarkers[numIntervals].height)	// sample >= q(m)
		{
			k = numIntervals;
			if (sample > pMarkers[numIntervals].height)		// sample > q(m)
			{
				pMarkers[numIntervals].height = sample;
			}
		}
		else
		{
			while (sample >= pMarkers[k].height)	  // q(k-1) <= sample < q(k)
			{
				k++;
			}
		}

		//
		// increment positions of markers (k+1) through m
		//
		for (i = k; i < numMarkers; i++)  // i=k instead of i=k+1 since array is 0-based
		{
			pMarkers[i].position++;
		}

		//
		// update desired positions for all markers
		//
		for (i = 0, pThis = pMarkers; i < numMarkers; i++, pThis++)
		{
			pThis->desiredPos += pThis->incDesiredPos;
		}

		//
		// adjust heights of markers 2 thru (m-1) if necessary
		// (loop thru 1 to (m-2) since array is 0-based)
		//
		pThis = &pMarkers[1];		     // points to this marker
		pPrev = &pMarkers[0];			 // points to prev marker
		pNext = &pMarkers[2];			 // points to next marker
		for (i = 1; i < numIntervals; i++, pThis++, pPrev++, pNext++)
		{
			Int   deltaSign;
			Double delta = pThis->desiredPos - pThis->position;
			Double desiredHeight;

			if (((delta >= +1) && ((pNext->position - pThis->position) > +1)) || 
				((delta <= -1) && ((pPrev->position - pThis->position) < -1)))
			{
				deltaSign = (delta > 0) ? +1 : -1;  // = +1 if delta is positive; -1 otherwise

				desiredHeight = parabolic (i, deltaSign);   // from parabolic formula

				if ((pPrev->height < desiredHeight) && 
					(desiredHeight < pNext->height))
				{
					pThis->height = desiredHeight;
				}
				else
				{
					//
					// use linear formula
					//
					pThis->height = linear (i, deltaSign);  // from linear formula
				}
				pThis->position += deltaSign;
			}
		} // for i = 2 to (m-1)
	} // else

	return (rc);

  //## end Quantile::nextSample%36B700DE02C0.body
}

//## Operation: reset%374C035E0393
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
RetCode Quantile::reset ()
{
  //## begin Quantile::reset%374C035E0393.body preserve=yes

	// resets attributes variables

	numSamples = 0;

	if (pMarkers == NULL)
		return SCH_ALLOCATION_ERROR;       // the array has not been created
	else
	{
		memset (pMarkers, 0, numMarkers * sizeof (Marker)); // set all values to 0

		//
		// initialize the increment of desired positions and 
		// calculate initial desired positions
		// for quantiles, values depend upon threshold and their position
		//
		pMarkers[0].incDesiredPos = 0;
		pMarkers[1].incDesiredPos = threshold / 2;
		pMarkers[2].incDesiredPos = threshold;
		pMarkers[3].incDesiredPos = (1 + threshold) / 2;
		pMarkers[4].incDesiredPos = 1;

		pMarkers[0].desiredPos = 1;
		pMarkers[1].desiredPos = 1 + (2 * threshold);
		pMarkers[2].desiredPos = 1 + (4 * threshold);
		pMarkers[3].desiredPos = 3 + (2 * threshold);
		pMarkers[4].desiredPos = 5;
	}


	return SCH_SUCCESS;

  //## end Quantile::reset%374C035E0393.body
}

// Additional Declarations
  //## begin Quantile%36AC5A940175.declarations preserve=yes
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
void Quantile::selectionSort (void)
{
	Double   bestValue;
	Int     i, j, bestJ;
	Marker *pI, *pJ;

	for (i = 0, pI = pMarkers; i < numMarkers - 1; i++, pI++)
	{
		bestValue = pI->height;
		bestJ = i;
		for (j = i + 1, pJ = pI + 1; j < numMarkers; j++, pJ++)
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
//	Name: Float parabolic (Int i, Int deltaSign)
//
//	Description: Calculates a new marker height using the parabolic formula.
//
//	Input parameters: Int i;         //position in pMarker according to marker position
//                    Int deltaSign; //left or right shift (-1 or +1 respectively)
//
//	Returns: the calculated marker height
//
//	********************************************************************************
Double Quantile::parabolic (Int i, Int deltaSign)
{
	assert ((deltaSign == +1) || (deltaSign == -1)); // deltaSign must be +1 or -1
	assert ((i > 0) && (i < numMarkers - 1));		 // i = 2...(m-1) but array is 0-based

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
	a  = (Double) deltaSign / (Double) (pNextMarker->position - pPrevMarker->position);
	result = pThisMarker->height + (a * b);


	return (result);
}

//	********************************************************************************
//
//	Name: Float linear (Int i, Int deltaSign)
//
//	Description: Calculates a new marker height using the linear formula.
//
//	Input parameters: Int i;         //position in pMarker according to marker position
//                    Int deltaSign; //left or right shift (-1 or +1 respectively)
//
//	Returns: the calculated marker height
//
//	********************************************************************************
Double Quantile::linear (Int i, Int deltaSign)
{
	assert ((deltaSign == +1) || (deltaSign == -1));  // deltaSign must be +1 or -1
	assert ((i > 0) && (i < numMarkers - 1));		  // i = 2...(m-1) but array is 0-based

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

  //## end Quantile%36AC5A940175.declarations
//## begin module%36AC5A940175.epilog preserve=yes

} // namespace sch

//## end module%36AC5A940175.epilog
