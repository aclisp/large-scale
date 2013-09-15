//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%370A4B8E03DF.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%370A4B8E03DF.cm

//## begin module%370A4B8E03DF.cp preserve=yes
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
//	File Name: ParetoGen.cpp
//
//	Description: Implementation file for the ParetoGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 10 $
//
//	Last check in $Date: 10/04/99 3:58p $
//
//	********************************************************************************
//## end module%370A4B8E03DF.cp

//## Module: ParetoGen%370A4B8E03DF; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\ParetoGen.cpp

//## begin module%370A4B8E03DF.additionalIncludes preserve=no
//## end module%370A4B8E03DF.additionalIncludes

//## begin module%370A4B8E03DF.includes preserve=yes
#include "MemLeak.h"
#include <math.h>
#include <float.h>
//## end module%370A4B8E03DF.includes

// ParetoGen
#include "ParetoGen.h"
//## begin module%370A4B8E03DF.additionalDeclarations preserve=yes
namespace sch {
//## end module%370A4B8E03DF.additionalDeclarations


// Class ParetoGen 



//## Operation: ParetoGen%370A4CA502A0
//	********************************************************************************
//
//	Name: ParetoGen(Ulong seed, Float alfaValue, Float kValue)
//
//	Description: Non-default constructor - requires the initial seed, the alfa and k values to generate the random variables.
//
//	Input parameters: Ulong seed;  //seed to begin the random number generation
//	                  Float alfaValue;           //The value used to obtain the exponent value to calculate the Pareto distribution
//	                  Float kValue    // Multiplier factor of the Pareto distribution.;
//
//	Returns: none
//
//	Remarks: must invoke the ContinuousDistribution constructor passing the value of the seed parameter.
//	         The seed value must be: 0 <= seed <= MAX_WORD; alfaValue >= 1; and kValue > 0
//
//	********************************************************************************
ParetoGen::ParetoGen (Ulong seed, Float alfaValue, Float kValue)
  //## begin ParetoGen::ParetoGen%370A4CA502A0.hasinit preserve=no
      : k(kValue),
        alfa(alfaValue)
  //## end ParetoGen::ParetoGen%370A4CA502A0.hasinit
  //## begin ParetoGen::ParetoGen%370A4CA502A0.initialization preserve=yes
	, ContinuousDistribution (seed)
  //## end ParetoGen::ParetoGen%370A4CA502A0.initialization
{
  //## begin ParetoGen::ParetoGen%370A4CA502A0.body preserve=yes
	RetCode rc = SCH_SUCCESS;

	if (k <= 0)
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (rc == SCH_SUCCESS)
	{
		if (alfa <= 0.0f)
		{
			rc = SCH_INVALID_PARAMETER;
		}
	}
	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);
	}
  //## end ParetoGen::ParetoGen%370A4CA502A0.body
}



//## Other Operations (implementation)
//## Operation: nextValue%370A4D090004
//	********************************************************************************
//
//	Name: RetCode nextValue(Float *pValue)
//
//	Description: Returns the next value of the Pareto distribution.
//
//	Output parameters: Float *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode ParetoGen::nextValue (Float *pValue)
{
  //## begin ParetoGen::nextValue%370A4D090004.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Float number;
	Double exponent;

	// limits the value of number to:  0 < number <= 1
	// otherwise  it can occur	a exception	 at  pow(number,-1.0/(double)alfa)
	do	
		rc = getNextValue (&number);
	while (number == 0.0f );

	  exponent = pow (number, -1.0 / (Double) alfa);
	  *pValue = exponent * k;

	return rc;	
  //## end ParetoGen::nextValue%370A4D090004.body
}

// Additional Declarations
  //## begin ParetoGen%370A4B8E03DF.declarations preserve=yes
  //## end ParetoGen%370A4B8E03DF.declarations

//## begin module%370A4B8E03DF.epilog preserve=yes
} // namespace sch
//## end module%370A4B8E03DF.epilog
