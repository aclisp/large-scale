//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36C027010384.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36C027010384.cm

//## begin module%36C027010384.cp preserve=yes
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
//	File Name: Mean.cpp
//
//	Description: Implementation file for the Mean class.
//
//	Author: Eugenio Pacheli Alves
//
//	Creation Date: Apr/08/99
//
//	Visual Source Safe $Revision: 11 $
//
//	Last check in $Date: 8/04/99 9:06a $
//
//	********************************************************************************
//## end module%36C027010384.cp

//## Module: Mean%36C027010384; Pseudo Package body
//## Source file: T:\StatisticsCalculation\Mean.cpp

//## begin module%36C027010384.additionalIncludes preserve=no
//## end module%36C027010384.additionalIncludes

//## begin module%36C027010384.includes preserve=yes
#include "MemLeak.h"
#include <math.h>
//## end module%36C027010384.includes

// Mean
#include "Mean.h"
//## begin module%36C027010384.additionalDeclarations preserve=yes

using namespace std;

namespace sch {

//## end module%36C027010384.additionalDeclarations


// Class Mean 








//## Other Operations (implementation)
//## Operation: nextSample%36C033E50311
//	********************************************************************************
//
//	Name: RetCode nextSample (Float sample)
//
//	Description: Includes the sample in the sequence and recalculates minimum, maximum and mean.
//
//	Input parameters: Float sample;  //next sample in the sequence
//
//	Returns: SCH_SUCCESS
//
//	Remarks: this operation is virtual since derived classes may have their own way of handling insertion of samples in the sequence.
//
//	********************************************************************************
RetCode Mean::nextSample (Float sample)
{
  //## begin Mean::nextSample%36C033E50311.body preserve=yes

	RetCode rc = SCH_SUCCESS;

	//
	// one more sample included in the sequence - increment counter of samples
	//
	numSamples++;

	//
	// set min and max values
	//
	if (numSamples == 1)
	{
		//
		// if this is the first sample, set min and max values equal to this sample
		//
		minValue = sample;
		maxValue = sample;
		meanValue = 0.0;						// this initialization is usefull fot the next commands
	}
	else if (sample < minValue)
	{
		//
		// this sample is less than the minimum ever
		//
		minValue = sample;
	}
	else if (sample > maxValue)
	{
		//
		// this sample is greater than the maximum ever
		// 
		maxValue = sample;
	}

	//
	// calculate new mean
	//
	Double oldSum = meanValue * (numSamples - 1);			// old sum = old mean * old total number of samples
	meanValue = (oldSum + (Double) sample) / numSamples;	  // new mean = (old sum + new sample) / new total number of samples

	return (rc);

  //## end Mean::nextSample%36C033E50311.body
}

//## Operation: operator+=%36C16BF803A4
//	********************************************************************************
//
//	Name: Mean& operator+=(const Mean& rhs)
//
//	Description: Assignment + operator for adding the mean of the rhs object to this object. The resulting mean is calculated by:
//	   (total_sum_of_this + total_sum_of_rhs) / (numSamples_of_this + numSamples_of_rhs)
//	where
//	   total_sum_of_x = x->meanValue * x->numSamples
//
//	Input parameters: const Mean& rhs;  //operand to be added to this
//
//	Returns: the reference to this object that contains the result of the sum operation.
//
//	Remarks: this->numSamples is set to (this->numSamples + rhs->numSamples).
//
//	********************************************************************************
Mean& Mean::operator += (const Mean& rhs)
{
  //## begin Mean::operator+=%36C16BF803A4.body preserve=yes

	//
	// compute sum
	//
	Double oldSum = (numSamples == 0)? 0.0 : (meanValue * numSamples);          // old sum = old mean * old total number of samples
	Double rhsSum = (rhs.numSamples == 0)? 0.0 : (rhs.meanValue * rhs.numSamples);  // rhs sum = rhs mean * rhs total number of samples

	//
	// calculate new min and max values
	//
	if (rhs.minValue < minValue)
	{
		minValue = rhs.minValue;
	}
	if (rhs.maxValue > maxValue)
	{
		maxValue = rhs.maxValue;
	}

	//
	// calculate new number of samples
	//
	numSamples += rhs.numSamples; 					// new total number of samples = old total number + rhs total number

	//
	// calculate new mean:
	// new mean = (old sum + rhs sum) / new total number of samples
	//
	meanValue = (numSamples == 0)? SIM_HUGE_VAL : ((oldSum + rhsSum) / (Double) numSamples);	    

	return (*this);

  //## end Mean::operator+=%36C16BF803A4.body
}

//## Operation: reset%374C00B00210
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
RetCode Mean::reset ()
{
  //## begin Mean::reset%374C00B00210.body preserve=yes

	// resets attributes variables
	maxValue   = SIM_HUGE_VAL;
	meanValue  = SIM_HUGE_VAL;
	minValue   = SIM_HUGE_VAL;
	numSamples = 0;

	return SCH_SUCCESS;
 
  //## end Mean::reset%374C00B00210.body
}

// Additional Declarations
  //## begin Mean%36C027010384.declarations preserve=yes
  //## end Mean%36C027010384.declarations

//## begin module%36C027010384.epilog preserve=yes

} // namespace sch

//## end module%36C027010384.epilog


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin Mean::operator+%921242928.body preserve=no

	Mean aMean;

	//
	// compute sum
	//
	Double lhsSum = lhs.meanValue * lhs.numSamples;  // lhs sum = lhs mean * lhs total number of samples
	Double rhsSum = rhs.meanValue * rhs.numSamples;  // rhs sum = rhs mean * rhs total number of samples

	//
	// calculate new min and max values
	//
	aMean.minValue = (lhs.minValue < rhs.minValue) ? lhs.minValue : rhs.minValue;
	aMean.maxValue = (lhs.maxValue > rhs.maxValue) ? lhs.maxValue : rhs.maxValue;

	//
	// calculate new number of samples
	//
	aMean.numSamples = lhs.numSamples + rhs.numSamples; 	// new total number of samples

	//
	// calculate new mean
	//
	aMean.meanValue = (lhsSum + rhsSum) / (Double) aMean.numSamples;	// new mean = (lhs sum + rhs sum) / new total number of samples

	return (aMean);

//## end Mean::operator+%921242928.body

#endif
