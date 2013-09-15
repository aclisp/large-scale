//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%3711FAB003E4.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3711FAB003E4.cm

//## begin module%3711FAB003E4.cp preserve=yes
//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: Random Variable Generation
//
//	File Name: TruncatedLogNormalGen.cpp
//
//	Description: Implementation file for the TruncatedLogNormalGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 6 $
//
//	Last check in $Date: 17/05/99 16:35 $
//
//	********************************************************************************
//## end module%3711FAB003E4.cp

//## Module: TruncatedLogNormalGen%3711FAB003E4; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\TruncatedLogNormalGen.cpp

//## begin module%3711FAB003E4.additionalIncludes preserve=no
//## end module%3711FAB003E4.additionalIncludes

//## begin module%3711FAB003E4.includes preserve=yes
#include "MemLeak.h"
#include <math.h>
//## end module%3711FAB003E4.includes

// TruncatedLogNormalGen
#include "TruncatedLogNormalGen.h"
//## begin module%3711FAB003E4.additionalDeclarations preserve=yes
namespace sch {
//## end module%3711FAB003E4.additionalDeclarations


// Class TruncatedLogNormalGen 



//## Operation: TruncatedLogNormalGen%3711FC4E0259
//	********************************************************************************
//
//	Name: TruncatedLogNormalGen(Ulong seed, Float mean, Float stdDev, Float xMin, Float xMax)
//
//	Description: Non-default constructor - requires the initial seed, the mean, the stdDev, the xMin and the xMax values to generate
//	the random variables.
//
//	Input parameters: Ulong seed;   //seed to begin the random number generation.
//	                  Float mean;   //mean
//	                  Float stdDev; //standard deviation
//	                  Float xMin;   //minimum value
//	                  Float xMax; //maximum value
//
//	Returns: none
//
//	Remarks: must invoke the LogNormal constructor passing the values of the follow parameters: seed, mean and stdDev.
//	         The seed value must be: 0 <= seed <= MAX_WORD; mean > 0; and stdDev > 0; xMax > 0; xMin > 0; and xMin < xMax
//
//	********************************************************************************
TruncatedLogNormalGen::TruncatedLogNormalGen (Ulong seed, Float mean, Float stdDev, Float xMin, Float xMax)
  //## begin TruncatedLogNormalGen::TruncatedLogNormalGen%3711FC4E0259.hasinit preserve=no
      : xMaximum(xMax),
        xMinimum(xMin)
  //## end TruncatedLogNormalGen::TruncatedLogNormalGen%3711FC4E0259.hasinit
  //## begin TruncatedLogNormalGen::TruncatedLogNormalGen%3711FC4E0259.initialization preserve=yes
	, LogNormalGen (seed, mean, stdDev)
  //## end TruncatedLogNormalGen::TruncatedLogNormalGen%3711FC4E0259.initialization
{
  //## begin TruncatedLogNormalGen::TruncatedLogNormalGen%3711FC4E0259.body preserve=yes
	RetCode rc = SCH_SUCCESS;

	Float theMedian = exp(mean);

	if ((xMinimum < 0) || (xMinimum > theMedian))	// If xmin is greater than median, then there will be too much rejection.
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if ((xMaximum <= 0) || (xMaximum < theMedian))	// If xmax is less than median, then there will be too much rejection.
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (xMinimum >= xMaximum)
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);
	}
  //## end TruncatedLogNormalGen::TruncatedLogNormalGen%3711FC4E0259.body
}



//## Other Operations (implementation)
//## Operation: nextValue%3711FC610026
//	********************************************************************************
//
//	Name: RetCode nextValue(Float *pValue)
//
//	Description: Returns the next value of the truncated LogNormal distribution.
//
//	Output parameters: Float *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode TruncatedLogNormalGen::nextValue (Float *pValue)
{
  //## begin TruncatedLogNormalGen::nextValue%3711FC610026.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Float number = xMinimum - 1.0;

	Int numIter = 0;
	const Int maxIter = 1000;

	while( (number<xMinimum) || (number>xMaximum) ) {
		rc = LogNormalGen::nextValue(&number);
		numIter++;
		if( numIter > maxIter ) {
			rc = SCH_REJECTION_METHOD_FAILED;
			break;
		}
	}

	if (rc == SCH_SUCCESS)
	{
		*pValue = number;
	}

	return rc;
  //## end TruncatedLogNormalGen::nextValue%3711FC610026.body
}

// Additional Declarations
  //## begin TruncatedLogNormalGen%3711FAB003E4.declarations preserve=yes
  //## end TruncatedLogNormalGen%3711FAB003E4.declarations

//## begin module%3711FAB003E4.epilog preserve=yes
} // namespace sch
//## end module%3711FAB003E4.epilog
