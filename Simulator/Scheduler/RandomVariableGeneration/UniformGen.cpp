//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%373711FF0283.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%373711FF0283.cm

//## begin module%373711FF0283.cp preserve=yes
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
//	File Name: UniformGen.cpp
//
//	Description: Implementation file for the UniformGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 7 $
//
//	Last check in $Date: 17/05/99 16:35 $
//
//	********************************************************************************
//## end module%373711FF0283.cp

//## Module: UniformGen%373711FF0283; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\UniformGen.cpp

//## begin module%373711FF0283.additionalIncludes preserve=no
//## end module%373711FF0283.additionalIncludes

//## begin module%373711FF0283.includes preserve=yes
//## end module%373711FF0283.includes

// UniformGen
#include "MemLeak.h"
#include "UniformGen.h"
//## begin module%373711FF0283.additionalDeclarations preserve=yes
namespace sch {
//## end module%373711FF0283.additionalDeclarations


// Class UniformGen 

//## Operation: UniformGen%3737125C02CD
//	********************************************************************************
//
//	Name: UniformGen(Ulong seed = 78562, Float minVal = 0, Float maxVal = 1)
//
//	Description: Non-default constructor - requires the initial seed and range to generate the random variables.
//
//	Input parameters: Ulong seed;       //seed to begin the random number generation
//	                  Float minVal = 0; //lower boundary of the range
//	                  Float maxVal = 1; //upper boundary of the range
//
//	Returns: none
//
//	Remarks: must invoke the ContinuousDistribution constructor passing the values of the follow parameters: seed, minVal and maxVal.
//	         The seed value must be: 0 <= seed <= MAX_WORD; and minVal < maxVal
//
//	********************************************************************************
UniformGen::UniformGen (Ulong seed, Float minVal, Float maxVal)
  //## begin UniformGen::UniformGen%3737125C02CD.hasinit preserve=no
  //## end UniformGen::UniformGen%3737125C02CD.hasinit
  //## begin UniformGen::UniformGen%3737125C02CD.initialization preserve=yes
	: ContinuousDistribution (seed, minVal, maxVal)
  //## end UniformGen::UniformGen%3737125C02CD.initialization
{
  //## begin UniformGen::UniformGen%3737125C02CD.body preserve=yes
  //## end UniformGen::UniformGen%3737125C02CD.body
}



//## Other Operations (implementation)
//## Operation: nextValue%373712F001C1
//	********************************************************************************
//
//	Name: RetCode nextValue(Float *pValue)
//
//	Description: Returns the next value of the Uniform distribution.
//
//	Output parameters: Float *pValue;  //next number generated.
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode UniformGen::nextValue (Float *pValue)
{
  //## begin UniformGen::nextValue%373712F001C1.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Float number;

	rc = getNextValue (&number);
	if (rc == SCH_SUCCESS)
	{
	 	*pValue = number;
	}
	
	return rc;
  //## end UniformGen::nextValue%373712F001C1.body
}

// Additional Declarations
  //## begin UniformGen%373711FF0283.declarations preserve=yes
  //## end UniformGen%373711FF0283.declarations

//## begin module%373711FF0283.epilog preserve=yes
} // namespace sch
//## end module%373711FF0283.epilog
