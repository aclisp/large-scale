//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC50460216.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC50460216.cm

//## begin module%36AC50460216.cp preserve=yes
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
//	File Name: NormalGen.cpp
//
//	Description: Implementation file for the NormalGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 7 $
//
//	Last check in $Date: 8/24/99 9:50a $
//
//	********************************************************************************
//## end module%36AC50460216.cp

//## Module: NormalGen%36AC50460216; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\NormalGen.cpp

//## begin module%36AC50460216.additionalIncludes preserve=no
//## end module%36AC50460216.additionalIncludes

//## begin module%36AC50460216.includes preserve=yes
#include "MemLeak.h"
#include <math.h>
#include <float.h>
//## end module%36AC50460216.includes

// NormalGen
#include "NormalGen.h"
//## begin module%36AC50460216.additionalDeclarations preserve=yes
namespace sch {
//## end module%36AC50460216.additionalDeclarations


// Class NormalGen 




//## Operation: NormalGen%36B6E5F502A9
//	********************************************************************************
//
//	Name: NormalGen(Ulong seed, Float mean, Float stdDev)
//
//	Description: Non-default constructor - requires the initial seed, the mean and the stdDev to generate the random variables.
//
//	Input parameters: Ulong seed;   //the seed to begin the random number generation
//	                             Float mean; //mean
//	                             Float stdDev;  //standard deviation
//
//	Returns: none
//
//	Remarks: must invoke the ContinuousDistribution constructor passing the value of the seed parameter.
//	         The seed value must be: 0 <= seed <= MAX_WORD; the mean value does not have restriction; and stdDev > 0
//
//	********************************************************************************
NormalGen::NormalGen (Ulong seed, Float mean, Float stdDev)
  //## begin NormalGen::NormalGen%36B6E5F502A9.hasinit preserve=no
      : a(mean),
        b(stdDev)
  //## end NormalGen::NormalGen%36B6E5F502A9.hasinit
  //## begin NormalGen::NormalGen%36B6E5F502A9.initialization preserve=yes
  , ContinuousDistribution (seed)
  //## end NormalGen::NormalGen%36B6E5F502A9.initialization
{
  //## begin NormalGen::NormalGen%36B6E5F502A9.body preserve=yes
	RetCode rc = SCH_SUCCESS;

	if (b <= 0)
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (rc != SCH_SUCCESS)
	{
	 	throw SchException (rc);
	}
  //## end NormalGen::NormalGen%36B6E5F502A9.body
}



//## Other Operations (implementation)
//## Operation: nextValue%36B6E5D20104
//	********************************************************************************
//
//	Name: RetCode nextValue(Float *pValue)
//
//	Description: Returns the next value of the Normal distribution.
//
//	Output parameters: Float *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode NormalGen::nextValue (Float *pValue)
{
  //## begin NormalGen::nextValue%36B6E5D20104.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Double firstTerm;
	Double secondTerm;
	Float number1;
	Float number2;
	Float normal;
	Float x1;
	Float x11;
	Float x2;
	static Float x22;
	static bool odd = false;
	const Double twoPi = 2 * 3.14159265359;
	const Float minusTwo = -2;

	if (odd)
	{
		odd = false;
		normal = x22;
	}
	else
	{
		// limits the value of number1 to:  0 < number1 <= 1
		// otherwise  it can occur	a exception	 at log (number1)
		do	
			rc = getNextValue (&number1);
		while (number1 == 0.0f );

		if (rc == SCH_SUCCESS)
		{
			rc = getNextValue (&number2);
			if (rc == SCH_SUCCESS)
			{
				firstTerm  = sqrt (minusTwo * log (number1));
				secondTerm = twoPi * number2; 

				x1 = (Float) (firstTerm * cos (secondTerm));
				
				x11 = a + b * x1;

				x2 = (Float) (firstTerm * sin (secondTerm));
				
				x22 = a + b * x2;

				odd = true;
				normal = x11;
			}
		}
	}
	*pValue = normal;

	return rc;
  //## end NormalGen::nextValue%36B6E5D20104.body
}

// Additional Declarations
  //## begin NormalGen%36AC50460216.declarations preserve=yes
  //## end NormalGen%36AC50460216.declarations

//## begin module%36AC50460216.epilog preserve=yes
} //namespace sch
//## end module%36AC50460216.epilog
