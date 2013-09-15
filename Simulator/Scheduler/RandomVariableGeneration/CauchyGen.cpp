//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%3711F4A60283.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3711F4A60283.cm

//## begin module%3711F4A60283.cp preserve=yes
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
//	File Name: CauchyGen.cpp
//
//	Description: Implementation file for the CauchyGen class
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
//## end module%3711F4A60283.cp

//## Module: CauchyGen%3711F4A60283; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\CauchyGen.cpp

//## begin module%3711F4A60283.additionalIncludes preserve=no
//## end module%3711F4A60283.additionalIncludes

//## begin module%3711F4A60283.includes preserve=yes
#include "MemLeak.h"
#include <math.h>
//## end module%3711F4A60283.includes

// CauchyGen
#include "CauchyGen.h"
//## begin module%3711F4A60283.additionalDeclarations preserve=yes
namespace sch {
//## end module%3711F4A60283.additionalDeclarations


// Class CauchyGen 



//## Operation: CauchyGen%3711F4FF033F
//	********************************************************************************
//
//	Name: CauchyGen(Ulong seed, Float alfa, Float beta)
//
//	Description: Non-default constructor - requires the initial seed, the alfa and the beta values to  generate the random variables.
//
//	Input parameters: Ulong seed;        //seed to begin the random number generation
//	                  Float alfa;  //alfa value
//	                  Float beta; //beta value
//
//	Returns: none
//
//	Remarks: must invoke the ContinuousDistribution constructor passing the value of the seed parameter.
//	         The seed value must be: 0 <= seed <= MAX_WORD; the alfa value does not have restriction; and beta > 0.
//
//	********************************************************************************
CauchyGen::CauchyGen (Ulong seed, Float alfa, Float beta)
  //## begin CauchyGen::CauchyGen%3711F4FF033F.hasinit preserve=no
      : a(alfa),
        b(beta)
  //## end CauchyGen::CauchyGen%3711F4FF033F.hasinit
  //## begin CauchyGen::CauchyGen%3711F4FF033F.initialization preserve=yes
    , ContinuousDistribution (seed)
  //## end CauchyGen::CauchyGen%3711F4FF033F.initialization
{
  //## begin CauchyGen::CauchyGen%3711F4FF033F.body preserve=yes
	RetCode rc = SCH_SUCCESS;

	if (b <= 0)
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);
	}
  //## end CauchyGen::CauchyGen%3711F4FF033F.body
}



//## Other Operations (implementation)
//## Operation: nextValue%3711F50B0093
//	********************************************************************************
//
//	Name: RetCode nextValue(Float *pValue)
//
//	Description: Returns the next value of the Cauchy distribution.
//
//	Output parameters: Float *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//
//	********************************************************************************
RetCode CauchyGen::nextValue (Float *pValue)
{
  //## begin CauchyGen::nextValue%3711F50B0093.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Float number;
	Float cauchy;
	const Double pi = 3.14159265359;

	rc = getNextValue (&number);

	if (rc == SCH_SUCCESS)
	{
		cauchy = (Float) (a - b / tan (pi * number));
		*pValue = cauchy;
	}
	
	return rc;
  //## end CauchyGen::nextValue%3711F50B0093.body
}

// Additional Declarations
  //## begin CauchyGen%3711F4A60283.declarations preserve=yes
  //## end CauchyGen%3711F4A60283.declarations

//## begin module%3711F4A60283.epilog preserve=yes
} //namespace sch
//## end module%3711F4A60283.epilog
