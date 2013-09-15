//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC503A0255.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC503A0255.cm

//## begin module%36AC503A0255.cp preserve=yes
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
//	File Name: NegExpGen.cpp
//
//	Description: Implementation file for the NegExpGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 8 $
//
//	Last check in $Date: 8/24/99 9:50a $
//
//	********************************************************************************
//## end module%36AC503A0255.cp

//## Module: NegExpGen%36AC503A0255; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\NegExpGen.cpp

//## begin module%36AC503A0255.additionalIncludes preserve=no
//## end module%36AC503A0255.additionalIncludes

//## begin module%36AC503A0255.includes preserve=yes
#include "MemLeak.h"
#include <math.h>
#include <float.h>
//## end module%36AC503A0255.includes

// NegExpGen
#include "NegExpGen.h"
//## begin module%36AC503A0255.additionalDeclarations preserve=yes
namespace sch {
//## end module%36AC503A0255.additionalDeclarations


// Class NegExpGen 


//## Operation: NegExpGen%36B6E5440359
//	********************************************************************************
//
//	Name: NegExpGen(Ulong seed, Float lambdaValue)
//
//	Description: Non-default constructor - requires the initial seed and the lambda to generate the random variables.
//
//	Input parameters: Ulong seed;  //seed to begin the random number generation
//	                  Float lambdaValue;  //lambda value
//
//	Returns: none
//
//	Remarks: must invoke the ContinuousDistribution constructor passing the value of the seed parameter.
//	         The seed value must be: 0 <= seed <= MAX_WORD; and lambdaValue > 0
//
//	********************************************************************************
NegExpGen::NegExpGen (Ulong seed, Float lambdaValue)
  //## begin NegExpGen::NegExpGen%36B6E5440359.hasinit preserve=no
      : lambda(lambdaValue)
  //## end NegExpGen::NegExpGen%36B6E5440359.hasinit
  //## begin NegExpGen::NegExpGen%36B6E5440359.initialization preserve=yes
  , ContinuousDistribution (seed)
  //## end NegExpGen::NegExpGen%36B6E5440359.initialization
{
  //## begin NegExpGen::NegExpGen%36B6E5440359.body preserve=yes
	RetCode rc = SCH_SUCCESS;

	if (lambda <= 0)
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);
	}
  //## end NegExpGen::NegExpGen%36B6E5440359.body
}



//## Other Operations (implementation)
//## Operation: nextValue%36B6E528007E
//	********************************************************************************
//
//	Name: RetCode nextValue(Float *pValue)
//
//	Description: Returns the next value of the Negative Exponential distribution.
//
//	Output parameters: Float *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode NegExpGen::nextValue (Float *pValue)
{
  //## begin NegExpGen::nextValue%36B6E528007E.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Float number;
	Double negExp;
	Double mean;
		   
#if defined(WIN32)
	mean = _chgsign(1 / lambda);
#else
	mean = (Double)-1.0 / lambda; 
#endif

	// limits the value of number to:  0 < number <= 1
	// otherwise  it can occur	a exception	 at log (number)
	do	
		rc = getNextValue (&number);
	while (number == 0.0f );

	if (rc == SCH_SUCCESS)
	{
		negExp = mean * log(number);
		*pValue = (Float) negExp;
	}

	return rc;	
  //## end NegExpGen::nextValue%36B6E528007E.body
}

// Additional Declarations
  //## begin NegExpGen%36AC503A0255.declarations preserve=yes
  //## end NegExpGen%36AC503A0255.declarations

//## begin module%36AC503A0255.epilog preserve=yes
} // namespace sch
//## end module%36AC503A0255.epilog
