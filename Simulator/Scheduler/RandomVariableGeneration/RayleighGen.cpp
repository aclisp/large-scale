//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%370E09160239.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%370E09160239.cm

//## begin module%370E09160239.cp preserve=yes
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
//	File Name: RayleighGen.cpp
//
//	Description: Implementation file for the RayleighGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 7 $
//
//	Last check in $Date: 8/24/99 9:51a $
//
//	********************************************************************************
//## end module%370E09160239.cp

//## Module: RayleighGen%370E09160239; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\RayleighGen.cpp

//## begin module%370E09160239.additionalIncludes preserve=no
//## end module%370E09160239.additionalIncludes

//## begin module%370E09160239.includes preserve=yes
#include "MemLeak.h"
#include <math.h>
#include <float.h>
//## end module%370E09160239.includes

// RayleighGen
#include "RayleighGen.h"
//## begin module%370E09160239.additionalDeclarations preserve=yes
namespace sch {
//## end module%370E09160239.additionalDeclarations


// Class RayleighGen 


//## Operation: RayleighGen%370E096502C9
//	********************************************************************************
//
//	Name: RayleighGen(Ulong seed, Float lambdaValue)
//
//	Description: Non-default constructor - requires the initial seed and the lambda value to generate the random variables.
//
//	Input parameters: Ulong seed;  //seed to begin the random number generation
//	                  Float lambdaValue; //
//
//	Returns: none
//
//	Remarks: must invoke the ContinuousDistribution constructor passing the value of the seed parameter.
//	         The seed value must be: 0 <= seed <= MAX_WORD; and lambdaValue != 0
//
//	********************************************************************************
RayleighGen::RayleighGen (Ulong seed, Float lambdaValue)
  //## begin RayleighGen::RayleighGen%370E096502C9.hasinit preserve=no
      : lambda(lambdaValue)
  //## end RayleighGen::RayleighGen%370E096502C9.hasinit
  //## begin RayleighGen::RayleighGen%370E096502C9.initialization preserve=yes
	, ContinuousDistribution (seed)
  //## end RayleighGen::RayleighGen%370E096502C9.initialization
{
  //## begin RayleighGen::RayleighGen%370E096502C9.body preserve=yes
	RetCode rc = SCH_SUCCESS;

	if (lambda == 0)
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);
	}
  //## end RayleighGen::RayleighGen%370E096502C9.body
}



//## Other Operations (implementation)
//## Operation: nextValue%370E097500D7
//	********************************************************************************
//
//	Name: RetCode nextValue(Float *pValue)
//
//	Description: Returns the next value of the Rayleigh distribution.
//
//	Output parameters: Float *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode RayleighGen::nextValue (Float *pValue)
{
  //## begin RayleighGen::nextValue%370E097500D7.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Float number;
	Float rayleigh;

	// limits the value of number to:  0 < number <= 1
	// otherwise  it can occur	a exception	 at log (number)
	do	
		rc = getNextValue (&number);
	while (number == 0.0f );

	if (rc == SCH_SUCCESS)
	{
		rayleigh = lambda * sqrt (-2 * log (number));
		*pValue = rayleigh;
	}

	return rc;
  //## end RayleighGen::nextValue%370E097500D7.body
}

// Additional Declarations
  //## begin RayleighGen%370E09160239.declarations preserve=yes
  //## end RayleighGen%370E09160239.declarations

//## begin module%370E09160239.epilog preserve=yes
} // namespace sch
//## end module%370E09160239.epilog
