//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36B6DD7000B6.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36B6DD7000B6.cm

//## begin module%36B6DD7000B6.cp preserve=yes
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
//	File Name: DescriptiveStatistic.cpp
//
//	Description: Implementation file for the DescriptiveStatistic class.
//
//	Author: Eugenio Pacheli Alves
//
//	Creation Date: Apr/12/99
//
//	Visual Source Safe $Revision: 8 $
//
//	Last check in $Date: 8/03/99 1:36p $
//
//	********************************************************************************
//## end module%36B6DD7000B6.cp

//## Module: DescriptiveStatistic%36B6DD7000B6; Pseudo Package body
//## Source file: T:\StatisticsCalculation\DescriptiveStatistic.cpp

//## begin module%36B6DD7000B6.additionalIncludes preserve=no
//## end module%36B6DD7000B6.additionalIncludes

//## begin module%36B6DD7000B6.includes preserve=yes

#include "MemLeak.h"
#include <assert.h>	  // define the assert macro

//## end module%36B6DD7000B6.includes

// DescriptiveStatistic
#include "DescriptiveStatistic.h"
//## begin module%36B6DD7000B6.additionalDeclarations preserve=yes

using namespace std;

namespace sch {

//## end module%36B6DD7000B6.additionalDeclarations


// Class DescriptiveStatistic 



//## Operation: DescriptiveStatistic%3713428B038F
//	********************************************************************************
//
//	Name: DescriptiveStatistics(void)
//
//	Description: Default constructor to allocate pMean object.
//
//	Parameters: none
//
//	Returns: none
//
//	Exceptions: SchException, with one of the following error codes:
//	               SCH_ALLOCATION_ERROR = could not allocate pMean object
//
//	********************************************************************************
DescriptiveStatistic::DescriptiveStatistic ()
  //## begin DescriptiveStatistic::DescriptiveStatistic%3713428B038F.hasinit preserve=no
      : varianceValue(0.0)
  //## end DescriptiveStatistic::DescriptiveStatistic%3713428B038F.hasinit
  //## begin DescriptiveStatistic::DescriptiveStatistic%3713428B038F.initialization preserve=yes
  //## end DescriptiveStatistic::DescriptiveStatistic%3713428B038F.initialization
{
  //## begin DescriptiveStatistic::DescriptiveStatistic%3713428B038F.body preserve=yes

	pMean = new Mean();
	if (pMean == 0)
	{
		throw SchException (SCH_ALLOCATION_ERROR);	   // could not allocate object
	}
	return;

  //## end DescriptiveStatistic::DescriptiveStatistic%3713428B038F.body
}



//## Other Operations (implementation)
//## Operation: nextSample%36B743DD0312
//	********************************************************************************
//
//	Name: RetCode nextSample (Float sample)
//
//	Description: Includes the sample in the sequence and recalculates the variance, as well as invokes Mean::nextSample() operation
//	which recalculates minimum, maximum, and mean.
//
//	Input parameters: Float sample;  //next sample in the sequence
//
//	Returns: SCH_SUCCESS
//
//	Remarks: this operation is virtual since derived classes may have their own way of handling insertion of samples in the sequence.
//
//	********************************************************************************
RetCode DescriptiveStatistic::nextSample (Float sample)
{
  //## begin DescriptiveStatistic::nextSample%36B743DD0312.body preserve=yes

	RetCode rc = SCH_SUCCESS;

	assert (pMean != NULL);
	Double   numSamples = (Double) (pMean->getNumSamples() + 1);   // account this sample

	//
	// calculate new variance (based on last mean)
	// Note that all calculations are made in Double to attend the desired precision
	//
	if 	(numSamples == 1)									// starting at second sample free the calculation of the variance
		varianceValue = 0.0;
	else {						  							// at least two samples are needed	for calculations
		Double delta = (Double)(sample - pMean->getMean());	// mean calculated last time pMean->nextSample() was invoked

		varianceValue += (delta * delta) / numSamples;
		varianceValue *= ((numSamples - 1.0) / numSamples);
	}
	//
	// calculate new mean
	//
	rc = pMean->nextSample (sample);

	return (rc);

  //## end DescriptiveStatistic::nextSample%36B743DD0312.body
}

//## Operation: reset%374C03550318
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
RetCode DescriptiveStatistic::reset ()
{
  //## begin DescriptiveStatistic::reset%374C03550318.body preserve=yes

	// resets attributes variables
	varianceValue = SIM_HUGE_VAL;

	if (pMean == NULL)
		return SCH_ALLOCATION_ERROR;        // the objetct has not been created	!!!
	else 
		return pMean->reset();

  //## end DescriptiveStatistic::reset%374C03550318.body
}

// Additional Declarations
  //## begin DescriptiveStatistic%36B6DD7000B6.declarations preserve=yes
  //## end DescriptiveStatistic%36B6DD7000B6.declarations

//## begin module%36B6DD7000B6.epilog preserve=yes

} // namespace sch

//## end module%36B6DD7000B6.epilog
