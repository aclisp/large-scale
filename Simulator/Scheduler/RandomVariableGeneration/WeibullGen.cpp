//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%370E00320092.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%370E00320092.cm

//## begin module%370E00320092.cp preserve=yes
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
//	File Name: WeibullGen.cpp
//
//	Description: Implementation file for the WeibullGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 7 $
//
//	Last check in $Date: 7/06/99 14:20 $
//
//	********************************************************************************
//## end module%370E00320092.cp

//## Module: WeibullGen%370E00320092; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\WeibullGen.cpp

//## begin module%370E00320092.additionalIncludes preserve=no
//## end module%370E00320092.additionalIncludes

//## begin module%370E00320092.includes preserve=yes
#include "MemLeak.h"
#include <math.h>
#include <float.h>
//## end module%370E00320092.includes

// WeibullGen
#include "WeibullGen.h"
//## begin module%370E00320092.additionalDeclarations preserve=yes
namespace sch {
//## end module%370E00320092.additionalDeclarations


// Class WeibullGen 


//## Operation: WeibullGen%370E00B601E6
//	********************************************************************************
//
//	Name: WeibullGen(Ulong seed, Float lambdaValue, Float bValue)
//
//	Description: Non-default constructor - requires the initial seed, the lambda and b  to generate the random variables.
//
//	Input parameters: Ulong seed;  //seed to begin the random number generation
//	                  Float lambdaValue
//	                  Float bValue;   // The value used to obtain the exponent value to calculate the Weibull distribution.
//
//	Returns: none
//
//	Remarks: must invoke the NegExpGen constructor passing the values of the follow parameters: seed and mean.
//	         The seed value must be: 0 <= seed <= MAX_WORD; lambdaValue >0; and bValue > 0
//
//	********************************************************************************
WeibullGen::WeibullGen (Ulong seed, Float lambdaValue, Float bValue)
  //## begin WeibullGen::WeibullGen%370E00B601E6.hasinit preserve=no
  //## end WeibullGen::WeibullGen%370E00B601E6.hasinit
  //## begin WeibullGen::WeibullGen%370E00B601E6.initialization preserve=yes
	: NegExpGen (seed, lambdaValue)
  //## end WeibullGen::WeibullGen%370E00B601E6.initialization
{
  //## begin WeibullGen::WeibullGen%370E00B601E6.body preserve=yes
	RetCode rc = SCH_SUCCESS;

	if (bValue <= 0)
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (rc == SCH_SUCCESS)
	{
		if (lambdaValue <= 0)
		{
			rc = SCH_INVALID_PARAMETER;
		}
	}
	
	if (rc == SCH_SUCCESS)
	{
		b = (Float) (1 / bValue);
	}
	
	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);
	}
  //## end WeibullGen::WeibullGen%370E00B601E6.body
}



//## Other Operations (implementation)
//## Operation: nextValue%370E01B001E5
//	********************************************************************************
//
//	Name: RetCode nextValue(Float *pValue)
//
//	Description: Returns the next value of the Weibull distribution.
//
//	Output parameters: Float *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode WeibullGen::nextValue (Float *pValue)
{
  //## begin WeibullGen::nextValue%370E01B001E5.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Float number;
	Float weibull;

	rc = NegExpGen::nextValue (&number);
	if (rc == SCH_SUCCESS)
	{
		weibull = (Float) pow (number, b);
		*pValue = weibull;
	}

	return rc;
  //## end WeibullGen::nextValue%370E01B001E5.body
}

// Additional Declarations
  //## begin WeibullGen%370E00320092.declarations preserve=yes
  //## end WeibullGen%370E00320092.declarations

//## begin module%370E00320092.epilog preserve=yes
} // namespace sch
//## end module%370E00320092.epilog
