//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC5042027E.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC5042027E.cm

//## begin module%36AC5042027E.cp preserve=yes
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
//	File Name: ErlangGen.cpp
//
//	Description: Implementation file for the ErlangGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 8 $
//
//	Last check in $Date: 8/24/99 9:48a $
//
//	********************************************************************************
//## end module%36AC5042027E.cp

//## Module: ErlangGen%36AC5042027E; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\ErlangGen.cpp

//## begin module%36AC5042027E.additionalIncludes preserve=no
//## end module%36AC5042027E.additionalIncludes

//## begin module%36AC5042027E.includes preserve=yes
#include "MemLeak.h"
#include <math.h>
#include <float.h>
//## end module%36AC5042027E.includes

// ErlangGen
#include "ErlangGen.h"
//## begin module%36AC5042027E.additionalDeclarations preserve=yes
namespace sch {
//## end module%36AC5042027E.additionalDeclarations


// Class ErlangGen 



//## Operation: ErlangGen%36B6E1C80144
//	********************************************************************************
//
//	Name: ErlangGen(Ulong seed, Float lambdaValue, Ulong kValue)
//
//	Description: Non-default constructor - requires the initial seed, the lambda value and the k value to generate the random variables.
//
//	Input parameters: Ulong seed;   //seed to begin the random number generation
//	                  Float lambdaValue;   //lambda value
//	                  Ulong kValue; //k value
//
//	Returns: none
//
//	Remarks: must invoke the ContinuousDistribution constructor passing the value of the seed parameter.
//	         The seed value must be: 0 <= seed <= MAX_WORD;lambdaValue > 0; and kValue > 0
//
//	********************************************************************************
ErlangGen::ErlangGen (Ulong seed, Float lambdaValue, Ulong kValue)
  //## begin ErlangGen::ErlangGen%36B6E1C80144.hasinit preserve=no
      : k(kValue),
        lambda(lambdaValue)
  //## end ErlangGen::ErlangGen%36B6E1C80144.hasinit
  //## begin ErlangGen::ErlangGen%36B6E1C80144.initialization preserve=yes
  , ContinuousDistribution (seed)
  //## end ErlangGen::ErlangGen%36B6E1C80144.initialization
{
  //## begin ErlangGen::ErlangGen%36B6E1C80144.body preserve=yes
	RetCode rc = SCH_SUCCESS;

	if ((k <= 0) || (lambda <= 0))
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);
	}
  //## end ErlangGen::ErlangGen%36B6E1C80144.body
}



//## Other Operations (implementation)
//## Operation: nextValue%36B6E1880047
//	********************************************************************************
//
//	Name: RetCode nextValue(Float *pValue)
//
//	Description: Returns the next value of the Erlang distribution.
//
//	Output parameters: Float *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode ErlangGen::nextValue (Float *pValue)
{
  //## begin ErlangGen::nextValue%36B6E1880047.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Ulong i;
	Float number;
	Double erlang;
	Double mean;
	Double multiplication;

	multiplication = 1;
#if defined(WIN32)
	mean = _chgsign(1 / lambda); 
#else
	mean = (Double)-1.0 / lambda; 
#endif
	for (i=0; i<k; i++)
	{
		// limits the value of number to:  0 < number <= 1
		// otherwise  it can occur a exception at log(multiplication)
		do	
			rc = getNextValue (&number);
		while (number == 0.0f);


		if (rc != SCH_SUCCESS)
		{
			break;
		}
		multiplication *= number;
	}
	if (rc == SCH_SUCCESS)
	{
		erlang = mean * log(multiplication);
		*pValue = (Float) erlang;
	}

	return rc;	
  //## end ErlangGen::nextValue%36B6E1880047.body
}

// Additional Declarations
  //## begin ErlangGen%36AC5042027E.declarations preserve=yes
  //## end ErlangGen%36AC5042027E.declarations

//## begin module%36AC5042027E.epilog preserve=yes
} //namespace sch
//## end module%36AC5042027E.epilog
