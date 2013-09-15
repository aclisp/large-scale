//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%38DB88D1003D.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%38DB88D1003D.cm

//## begin module%38DB88D1003D.cp preserve=no
//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: RandomVariableGeneration
//
//	File Name: TruncatedWeibullGen.cpp
//
//	Description:
//
//	Author:
//
//	Creation Date:
//
//	Visual Source Safe $Revision: $
//
//	Last check in $Date: $
//
//	********************************************************************************
//## end module%38DB88D1003D.cp

//## Module: TruncatedWeibullGen%38DB88D1003D; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\TruncatedWeibullGen.cpp

//## begin module%38DB88D1003D.additionalIncludes preserve=no
//## end module%38DB88D1003D.additionalIncludes

//## begin module%38DB88D1003D.includes preserve=yes
#include "MemLeak.h"
#include <math.h>
#include <float.h>
//## end module%38DB88D1003D.includes

// TruncatedWeibullGen
#include "TruncatedWeibullGen.h"
//## begin module%38DB88D1003D.additionalDeclarations preserve=yes
namespace sch {
//## end module%38DB88D1003D.additionalDeclarations


// Class TruncatedWeibullGen 



//## Operation: TruncatedWeibullGen%38DB88D1003F
//	********************************************************************************
//
//	Name: TruncatedWeibullGen(Ulong seed, Float lambdaValue, Float bValue)
//
//	Description: Non-default constructor - requires the initial seed, the lambda and b  to generate the random variables. Requires
//	limits xMin and xMax for truncation via rejection method.
//
//	Input parameters: Ulong seed;  //seed to begin the random number generation
//	                  Float lambdaValue
//	                  Float bValue;   // The value used to obtain the exponent value to calculate the Weibull distribution.
//	                  Float xMin;   // lower limit of trunctation
//	                  Float xMax;  // upper limit of truncation
//
//	Returns:  none
//
//	Remarks: Invokes WeibullGen constructor, that in turn invokes NegExpGen constructor passing the values of the follow parameters:
//	seed and mean.
//	         Arguments must satisfy: 0 <= seed <= MAX_WORD; lambdaValue >0;  bValue > 0; and 0 < xMin < xMax.
//
//	********************************************************************************
TruncatedWeibullGen::TruncatedWeibullGen (Ulong seed, Float lambdaValue, Float bValue, Float xMin, Float xMax)
  //## begin TruncatedWeibullGen::TruncatedWeibullGen%38DB88D1003F.hasinit preserve=no
      : xMaximum(xMax),
        xMinimum(xMin)
  //## end TruncatedWeibullGen::TruncatedWeibullGen%38DB88D1003F.hasinit
  //## begin TruncatedWeibullGen::TruncatedWeibullGen%38DB88D1003F.initialization preserve=yes
  , WeibullGen ( seed,  lambdaValue,  bValue)
  //## end TruncatedWeibullGen::TruncatedWeibullGen%38DB88D1003F.initialization
{
  //## begin TruncatedWeibullGen::TruncatedWeibullGen%38DB88D1003F.body preserve=yes
	RetCode rc = SCH_SUCCESS;

	Float pMin;
	Float pMax;

	// If Pr(xMinimimum < x <= xMaximum)==0 , then the rejection method will cause an infinite loop in nextValue().
	if (xMinimum >= xMaximum || xMaximum <= 0)
	{
		rc = SCH_INVALID_PARAMETER;
	}

	// If  Pr(xMinimimum < x <= xMaximum) < 1/1000 then rejection method will be very inefficient.
	pMin = 1 - exp( -lambdaValue* (Float) pow(xMinimum, bValue) );
	pMax = 1 - exp( -lambdaValue* (Float) pow(xMaximum, bValue) );
	if (pMax -  pMin <= 0.001)
	{
		rc = SCH_INVALID_PARAMETER;
	}


	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);
	}
 
  //## end TruncatedWeibullGen::TruncatedWeibullGen%38DB88D1003F.body
}



//## Other Operations (implementation)
//## Operation: nextValue%38DB88D10045
//	********************************************************************************
//
//	Name: RetCode nextValue(Float *pValue)
//
//	Description: Returns the next value of the truncated Weibull distribution.
//
//	Output parameters: Float *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode TruncatedWeibullGen::nextValue (Float *pValue)
{
  //## begin TruncatedWeibullGen::nextValue%38DB88D10045.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Float number;
	
	do
	rc = WeibullGen::nextValue(&number);
	while( (rc == SCH_SUCCESS) && ((number < xMinimum) || (number > xMaximum)) );
	
	
	if (rc == SCH_SUCCESS)
	{
		*pValue = number;
	}
	
	return rc;
  //## end TruncatedWeibullGen::nextValue%38DB88D10045.body
}

// Additional Declarations
  //## begin TruncatedWeibullGen%38DB88D1003D.declarations preserve=yes
  //## end TruncatedWeibullGen%38DB88D1003D.declarations

//## begin module%38DB88D1003D.epilog preserve=yes
} // namespace sch
//## end module%38DB88D1003D.epilog
