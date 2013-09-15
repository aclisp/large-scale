//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%37307E8203C3.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%37307E8203C3.cm

//## begin module%37307E8203C3.cp preserve=yes
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
//	File Name: GeometricGen.cpp
//
//	Description: Implementation file for the GeometricGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: May/05/1999
//
//	Visual Source Safe $Revision: 8 $
//
//	Last check in $Date: 8/24/99 9:50a $
//
//	********************************************************************************
//## end module%37307E8203C3.cp

//## Module: GeometricGen%37307E8203C3; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\GeometricGen.cpp

//## begin module%37307E8203C3.additionalIncludes preserve=no
//## end module%37307E8203C3.additionalIncludes

//## begin module%37307E8203C3.includes preserve=yes
#include "MemLeak.h"
#include <math.h>
//## end module%37307E8203C3.includes

// GeometricGen
#include "GeometricGen.h"
//## begin module%37307E8203C3.additionalDeclarations preserve=yes
namespace sch {
//## end module%37307E8203C3.additionalDeclarations


// Class GeometricGen 



//## Operation: GeometricGen%37307F810185
//	********************************************************************************
//
//	Name: GeometricGen(Ulong seed, Float pVal)
//
//	Description: Non-default constructor - requires the initial seed and the pVal to generate the random variables.
//
//	Input parameters: Ulong seed;   //seed to begin the random number generation
//	                  Float pVal;
//
//	Returns: none
//
//	Remarks: must invoke the DiscreteDistribution constructor passing the value of the seed parameter.
//	         The seed value must be: 0 <= seed <= MAX_WORD; and 0 < pVal < 1
//
//	********************************************************************************
GeometricGen::GeometricGen (Ulong seed, Float pVal)
  //## begin GeometricGen::GeometricGen%37307F810185.hasinit preserve=no
      : p(pVal)
  //## end GeometricGen::GeometricGen%37307F810185.hasinit
  //## begin GeometricGen::GeometricGen%37307F810185.initialization preserve=yes
  , DiscreteDistribution (seed)
  //## end GeometricGen::GeometricGen%37307F810185.initialization
{
  //## begin GeometricGen::GeometricGen%37307F810185.body preserve=yes
	RetCode rc = SCH_SUCCESS;


	if ((p <= 0) || (p >= 1))
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);
	}

	lnP = log (1.0 - p);
  //## end GeometricGen::GeometricGen%37307F810185.body
}



//## Other Operations (implementation)
//## Operation: nextValue%37307F8A0340
//	********************************************************************************
//
//	Name: RetCode nextValue(Int *pValue)
//
//	Description: Returns the next value of the Geometric distribution.
//
//	Output parameters: Int *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode GeometricGen::nextValue (Int *pValue)
{
  //## begin GeometricGen::nextValue%37307F8A0340.body preserve=yes


	RetCode rc = SCH_SUCCESS;
	Int    geometric;
	Float  number;
	const Double epsilon = 0.999999;				// 1 - 0.000001

	if (p > epsilon)
	{
		geometric = -1;
		do
		{
			rc = getNextValue (&number);
			if (rc != SCH_SUCCESS)
			{
				break;
			}
			geometric ++;
		} while (number > p);
	}
	else
	{
		// limits the value of number to:  0 < number <= 1
		// otherwise  it can occur	a exception	 at log (number)
		do	
			rc = getNextValue (&number);
		while (number == 0.0f);

		if (rc == SCH_SUCCESS)
			geometric = (Int) ( log (number) / lnP);

	}

	*pValue = geometric + 1 ;

	return rc;

  //## end GeometricGen::nextValue%37307F8A0340.body
}

//## Operation: redefine%374E97EC018F
//	********************************************************************************
//
//	Name:  RetCode redefine(Float pVal)
//
//	Description:  Redefines the parameters suplied at construction time. As the  constructor,   requires the  pVal to generate the
//	random variables.
//
//	Input parameters:  Float pVal;
//
//	Returns:   SCH_SUCCESS
//	            SCH_INVALID_PARAMETER
//
//	Remarks:
//	         The pVal  value must be:  0 < pVal < 1
//
//	********************************************************************************
RetCode GeometricGen::redefine (Float pVal)
{
  //## begin GeometricGen::redefine%374E97EC018F.body preserve=yes

	if ((pVal <= 0) || (pVal >= 1))
		return SCH_INVALID_PARAMETER;

	p = pVal;

	lnP = 	log (1.0 - p);

	return SCH_SUCCESS;

  //## end GeometricGen::redefine%374E97EC018F.body
}

// Additional Declarations
  //## begin GeometricGen%37307E8203C3.declarations preserve=yes
  //## end GeometricGen%37307E8203C3.declarations

//## begin module%37307E8203C3.epilog preserve=yes
} //namespace sch
//## end module%37307E8203C3.epilog
