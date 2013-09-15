//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%3753D7910126.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3753D7910126.cm

//## begin module%3753D7910126.cp preserve=yes
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
//	File Name: TruncatedParetoGen.cpp
//
//	Description:  Implementation file for the TruncatedPareto class
//
//	Author:	  Hélio Azevedo
//
//	Creation Date:	 June / 01/ 1999
//
//	Visual Source Safe $Revision: 2 $
//
//	Last check in $Date: 7/06/99 14:20 $
//
//	********************************************************************************
//## end module%3753D7910126.cp

//## Module: TruncatedParetoGen%3753D7910126; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\TruncatedParetoGen.cpp

//## begin module%3753D7910126.additionalIncludes preserve=no
//## end module%3753D7910126.additionalIncludes

//## begin module%3753D7910126.includes preserve=yes
#include "MemLeak.h"
#include <math.h>
#include <float.h>
//## end module%3753D7910126.includes

// TruncatedParetoGen
#include "TruncatedParetoGen.h"
//## begin module%3753D7910126.additionalDeclarations preserve=yes
namespace sch {

//## end module%3753D7910126.additionalDeclarations


// Class TruncatedParetoGen 



//## Operation: TruncatedParetoGen%3753D8AB02D0
//	********************************************************************************
//
//	Name: TruncatedParetoGen(Ulong seed, Float alfaValue, Float kValue, Float xMin, Float xMax)
//
//	Description: Non-default constructor - requires the initial seed, the alfa and k values , the xMin, and the xMax values to
//	generate the random variables.
//
//	Input parameters: Ulong seed;        //seed to begin the random number generation
//	                  Float alfaValue;            //The value used to obtain the exponent value to calculate the Pareto distribution
//	                  Float kValue;                // Multiplier factor of the Pareto
//	                  Float xMin; //minimum value
//	                  Float xMax; //maximum value
//
//	Returns: none
//
//	Remarks: must invoke the ParetoGen constructor passing the values of the follow parameters: seed,mean and cValue.
//	         The seed value must be: 0 <= seed <= MAX_WORD;  alfaValue >= 1; kValue > 0 ; beta > 0; and xMin <= xMax
//
//	********************************************************************************
TruncatedParetoGen::TruncatedParetoGen (Ulong seed, Float alfaValue, Float kValue, Float xMin, Float xMax)
  //## begin TruncatedParetoGen::TruncatedParetoGen%3753D8AB02D0.hasinit preserve=no
      : xMaximum(xMax),
        xMinimum(xMin)
  //## end TruncatedParetoGen::TruncatedParetoGen%3753D8AB02D0.hasinit
  //## begin TruncatedParetoGen::TruncatedParetoGen%3753D8AB02D0.initialization preserve=yes
      , ParetoGen (seed, alfaValue, kValue)
  //## end TruncatedParetoGen::TruncatedParetoGen%3753D8AB02D0.initialization
{
  //## begin TruncatedParetoGen::TruncatedParetoGen%3753D8AB02D0.body preserve=yes

	RetCode rc = SCH_SUCCESS;

	Float Pmin;
	Float Pmax;

	if ((xMinimum < kValue))
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (xMinimum >= xMaximum)
	{
		rc = SCH_INVALID_PARAMETER;
	}

	// Check that the rejection method will have efficiency better than 0.001 ; otherwise it is best to use a different truncation method than rejection,
	Pmin =  1 - pow(kValue/xMinimum, alfaValue );
	Pmax =  1 - pow(kValue/xMaximum, alfaValue );
	if (Pmax - Pmin < 0.001)
	{
		rc = SCH_INVALID_PARAMETER;
	}


	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);
	}
 
  //## end TruncatedParetoGen::TruncatedParetoGen%3753D8AB02D0.body
}



//## Other Operations (implementation)
//## Operation: nextValue%3753D8AB02F8
//	********************************************************************************
//
//	Name: RetCode nextValue(Float *pValue)
//
//	Description: Returns the next value of the Truncated Pareto distribution.
//
//	Output parameters: Float *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode TruncatedParetoGen::nextValue (Float *pValue)
{
  //## begin TruncatedParetoGen::nextValue%3753D8AB02F8.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Float number;
	
	do
	rc = ParetoGen::nextValue(&number);
	while( (rc == SCH_SUCCESS) && ((number < xMinimum) || (number > xMaximum)) );
	
	
	if (rc == SCH_SUCCESS)
	{
		
		// The method below results in probability masses at the endpoints which not the usual manner of truncation.//
		//		if (number < xMinimum)
		//		{
		//			number = xMinimum;
		//		}
		//		else if (number > xMaximum)
		//		{
		//			number = xMaximum;
		//		}
		
		*pValue = number;
	}
	
	return rc;

  //## end TruncatedParetoGen::nextValue%3753D8AB02F8.body
}

// Additional Declarations
  //## begin TruncatedParetoGen%3753D7910126.declarations preserve=yes
  //## end TruncatedParetoGen%3753D7910126.declarations

//## begin module%3753D7910126.epilog preserve=yes
} // namespace sch
	
//## end module%3753D7910126.epilog
