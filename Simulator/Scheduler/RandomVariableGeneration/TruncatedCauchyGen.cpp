//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%3712004402C5.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3712004402C5.cm

//## begin module%3712004402C5.cp preserve=yes
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
//	File Name: TruncatedCauchyGen.cpp
//
//	Description: Implementation file for the TruncatedCauchyGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 5 $
//
//	Last check in $Date: 17/05/99 16:35 $
//
//	********************************************************************************
//## end module%3712004402C5.cp

//## Module: TruncatedCauchyGen%3712004402C5; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\TruncatedCauchyGen.cpp

//## begin module%3712004402C5.additionalIncludes preserve=no
//## end module%3712004402C5.additionalIncludes

//## begin module%3712004402C5.includes preserve=yes
//## end module%3712004402C5.includes

// TruncatedCauchyGen
#include "MemLeak.h"
#include "TruncatedCauchyGen.h"
//## begin module%3712004402C5.additionalDeclarations preserve=yes
namespace sch {
//## end module%3712004402C5.additionalDeclarations


// Class TruncatedCauchyGen 



//## Operation: TruncatedCauchyGen%371200C60286
//	********************************************************************************
//
//	Name: TruncatedCauchyGen(Ulong seed, Float alfa, Float beta, Float xMin, Float xMax)
//
//	Description: Non-default constructor - requires the initial seed, the alfa, the beta, the xMin and the xMax values to  generate the
//	random variables.
//
//	Input parameters: Ulong seed;        //seed to begin the random number generation
//	                  Float alfa;  //alfa value
//	                  Float beta; //beta value
//	                  Float xMin; //minimum value
//	                  Float xMax; //maximum value
//
//	Returns: none
//
//	Remarks: must invoke the CauchyGen constructor passing the values of the follow parameters: seed, alfa and beta.
//	         The seed value must be: 0 <= seed <= MAX_WORD; the alfa value does not have restriction; beta > 0; and xMin <= xMax
//
//	********************************************************************************
TruncatedCauchyGen::TruncatedCauchyGen (Ulong seed, Float alfa, Float beta, Float xMin, Float xMax)
  //## begin TruncatedCauchyGen::TruncatedCauchyGen%371200C60286.hasinit preserve=no
      : xMaximum(xMax),
        xMinimum(xMin)
  //## end TruncatedCauchyGen::TruncatedCauchyGen%371200C60286.hasinit
  //## begin TruncatedCauchyGen::TruncatedCauchyGen%371200C60286.initialization preserve=yes
    , CauchyGen (seed, alfa, beta)
  //## end TruncatedCauchyGen::TruncatedCauchyGen%371200C60286.initialization
{
  //## begin TruncatedCauchyGen::TruncatedCauchyGen%371200C60286.body preserve=yes
	RetCode rc = SCH_SUCCESS;

/*
	if ((xMinimum <= -beta) || (xMinimum >= (alfa+beta)))
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if ((xMaximum <= -beta) || (xMaximum >= (alfa+beta)))
	{
		rc = SCH_INVALID_PARAMETER;
	}
*/
	if (xMinimum > xMaximum)
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);
	}
  //## end TruncatedCauchyGen::TruncatedCauchyGen%371200C60286.body
}



//## Other Operations (implementation)
//## Operation: nextValue%371200D300F4
//	********************************************************************************
//
//	Name: RetCode nextValue(Float *pValue)
//
//	Description: Returns the next value of the Truncated Cauchy distribution.
//
//	Output parameters: Float *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode TruncatedCauchyGen::nextValue (Float *pValue)
{
  //## begin TruncatedCauchyGen::nextValue%371200D300F4.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Float number;

	rc = CauchyGen::nextValue(&number);
	if (rc == SCH_SUCCESS)
	{
	 	if (number < xMinimum)
		{
			number = xMinimum;
		}
		else if (number > xMaximum)
		{
			number = xMaximum;
		}
		*pValue = number;
	}

	return rc;
  //## end TruncatedCauchyGen::nextValue%371200D300F4.body
}

// Additional Declarations
  //## begin TruncatedCauchyGen%3712004402C5.declarations preserve=yes
  //## end TruncatedCauchyGen%3712004402C5.declarations

//## begin module%3712004402C5.epilog preserve=yes
} // namespace sch
//## end module%3712004402C5.epilog
