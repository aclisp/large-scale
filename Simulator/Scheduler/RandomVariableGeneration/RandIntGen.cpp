//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC502E0316.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC502E0316.cm

//## begin module%36AC502E0316.cp preserve=yes
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
//	File Name: RandIntGen.cpp
//
//	Description: Implementation file for the RandIntGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 9 $
//
//	Last check in $Date: 7/28/99 10:46a $
//
//	********************************************************************************
//## end module%36AC502E0316.cp

//## Module: RandIntGen%36AC502E0316; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\RandIntGen.cpp

//## begin module%36AC502E0316.additionalIncludes preserve=no
//## end module%36AC502E0316.additionalIncludes

//## begin module%36AC502E0316.includes preserve=yes
#include "MemLeak.h"
#include <math.h>
//## end module%36AC502E0316.includes

// RandIntGen
#include "RandIntGen.h"
//## begin module%36AC502E0316.additionalDeclarations preserve=yes
namespace sch {
//## end module%36AC502E0316.additionalDeclarations


// Class RandIntGen 



//## Operation: RandIntGen%36B6E391017D
//	********************************************************************************
//
//	Name: RandIntGen(Ulong seed, Ulong minVal, Ulong maxVal)
//
//	Description: Non-default constructor - requires the initial seed, the minVal and the maxVal to generate the random variables.
//
//	Input parameters: Ulong seed;   //seed to begin the random number generation
//	                  Ulong minVal; //lower boundary of the range
//	                  Ulong maxVal; //upper boundary of the range
//
//	Returns: none
//
//	Remarks: must invoke the DiscreteDistribution constructor passing the value of the seed parameter.
//	         The seed value must be: 0 <= seed <= MAX_WORD; minVal >= 0; maxVal >= 0; and minVal <= maxVal
//
//	********************************************************************************
RandIntGen::RandIntGen (Ulong seed, Ulong minVal, Ulong maxVal)
  //## begin RandIntGen::RandIntGen%36B6E391017D.hasinit preserve=no
      : maxValue(maxVal),
        minValue(minVal)
  //## end RandIntGen::RandIntGen%36B6E391017D.hasinit
  //## begin RandIntGen::RandIntGen%36B6E391017D.initialization preserve=yes
	, DiscreteDistribution (seed)
  //## end RandIntGen::RandIntGen%36B6E391017D.initialization
{
  //## begin RandIntGen::RandIntGen%36B6E391017D.body preserve=yes
	RetCode rc = SCH_SUCCESS;

	if ((minValue < 0) || (maxValue < 0) || (minValue > maxValue))
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);
	}
  //## end RandIntGen::RandIntGen%36B6E391017D.body
}



//## Other Operations (implementation)
//## Operation: nextValue%36B6E370017F
//	********************************************************************************
//
//	Name: RetCode nextValue(Int *pValue)
//
//	Description: Returns the next value of the RandInt distribution.
//
//	Output parameters: Int *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode RandIntGen::nextValue (Int *pValue)
{
  //## begin RandIntGen::nextValue%36B6E370017F.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Float number;
	Float result;
	Int   randInt; 

	// limits the value of number to:  0 <= number < 1
	// otherwise the returned value will be greater than maxValue
	do	
		rc = getNextValue (&number);
	while (number == 1.0f);

	if (rc == SCH_SUCCESS)
	{
		result = (maxValue - minValue + 1) * number;
		randInt = (Int) result;
		randInt += minValue;
		*pValue = randInt;
	}
	
	return rc;
  //## end RandIntGen::nextValue%36B6E370017F.body
}

//## Operation: redefine%374EEE7B0336
//	********************************************************************************
//
//	Name:  RetCode redefine(Ulong minVal, Ulong maxVal)
//
//	Description:  Redefines the parameters suplied at construction time. As the  constructor, requires the , the minVal and the maxVal
//	to generate the random variables.
//
//	Input parameters:Ulong minVal; //lower boundary of the range
//	                  Ulong maxVal; //upper boundary of the range
//
//	Returns: SCH_SUCCESS
//	                error code in case of error
//
//	Remarks: The parameters must be: ; minVal >= 0; maxVal >= 0; and minVal <= maxVal
//
//	********************************************************************************
RetCode RandIntGen::redefine (Ulong minVal, Ulong maxVal)
{
  //## begin RandIntGen::redefine%374EEE7B0336.body preserve=yes

	if ((minVal < 0) || (maxVal < 0) || (minVal > maxVal))
		return SCH_INVALID_PARAMETER;

	maxValue = maxVal;
	minValue = minVal;

	return SCH_SUCCESS;

  //## end RandIntGen::redefine%374EEE7B0336.body
}

// Additional Declarations
  //## begin RandIntGen%36AC502E0316.declarations preserve=yes
  //## end RandIntGen%36AC502E0316.declarations

//## begin module%36AC502E0316.epilog preserve=yes
} // namespace sch 
//## end module%36AC502E0316.epilog
