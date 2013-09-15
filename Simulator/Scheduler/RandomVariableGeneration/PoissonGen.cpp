//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC503E011A.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC503E011A.cm

//## begin module%36AC503E011A.cp preserve=yes
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
//	File Name: PoissonGen.cpp
//
//	Description: Implementation file for the PoissonGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 9 $
//
//	Last check in $Date: 1/06/99 14:53 $
//
//	********************************************************************************
//## end module%36AC503E011A.cp

//## Module: PoissonGen%36AC503E011A; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\PoissonGen.cpp

//## begin module%36AC503E011A.additionalIncludes preserve=no
//## end module%36AC503E011A.additionalIncludes

//## begin module%36AC503E011A.includes preserve=yes
#include "MemLeak.h"
#include <math.h>
#include <float.h>
//## end module%36AC503E011A.includes

// PoissonGen
#include "PoissonGen.h"
//## begin module%36AC503E011A.additionalDeclarations preserve=yes
namespace sch {
//## end module%36AC503E011A.additionalDeclarations


// Class PoissonGen 


//## Operation: PoissonGen%36B6E5900308
//	********************************************************************************
//
//	Name: PoissonGen(Ulong seed, Float mean)
//
//	Description: Non-default constructor - requires the initial seed and the mean to generate the random variables.
//
//	Input parameters: Ulong seed;  //seed to begin the random number generation
//	                  Float mean;  //mean
//
//	Returns: none
//
//	Remarks: must invoke the DiscreteDistribution constructor passing the value of the seed parameter.
//	         The seed value must be: 0 <= seed <= MAX_WORD; and mean > 0
//
//	********************************************************************************
PoissonGen::PoissonGen (Ulong seed, Float mean)
  //## begin PoissonGen::PoissonGen%36B6E5900308.hasinit preserve=no
      : exponent(mean)
  //## end PoissonGen::PoissonGen%36B6E5900308.hasinit
  //## begin PoissonGen::PoissonGen%36B6E5900308.initialization preserve=yes
  , DiscreteDistribution (seed)
  //## end PoissonGen::PoissonGen%36B6E5900308.initialization
{
  //## begin PoissonGen::PoissonGen%36B6E5900308.body preserve=yes
	RetCode rc = SCH_SUCCESS;

	if (exponent <= 0)
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (rc != SCH_SUCCESS)
	{
	 	throw SchException (rc);
	}
  //## end PoissonGen::PoissonGen%36B6E5900308.body
}



//## Other Operations (implementation)
//## Operation: nextValue%36B6E57901CF
//	********************************************************************************
//
//	Name: RetCode nextValue(Int *pValue)
//
//	Description: Returns the next value of the Poisson distribution.
//
//	Output parameters: Int *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode PoissonGen::nextValue (Int *pValue)
{
  //## begin PoissonGen::nextValue%36B6E57901CF.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Int N = -1;
	Float number;
	Double p;
	Double q = 1;

#if defined(WIN32)
	p = exp (_chgsign(exponent));
#else
	p = exp (-exponent);
#endif

	do
	{
		rc = getNextValue (&number);
		if (rc != SCH_SUCCESS)
		{
			break;
		}
		q = q * number;
		N++;
	}while (q >= p);

	*pValue = N;

	return rc;

  //## end PoissonGen::nextValue%36B6E57901CF.body
}

//## Operation: redefine%374E8D2C019B
//	********************************************************************************
//
//	Name: RetCode redefine( Float mean)
//
//	Description:  Redefines the parameters suplied at construction time. As the  constructor, requires  the mean to generate the random
//	variables.
//
//	Input parameters: Float mean;  //mean
//
//	Returns: SCH_SUCCESS
//	                error code in case of error
//
//	Remarks:   The mean  value must be greater than  0
//
//	********************************************************************************
RetCode PoissonGen::redefine (Float mean)
{
  //## begin PoissonGen::redefine%374E8D2C019B.body preserve=yes

	if (mean <= 0)
		return SCH_INVALID_PARAMETER;

	exponent = mean;

	return SCH_SUCCESS;

  //## end PoissonGen::redefine%374E8D2C019B.body
}

// Additional Declarations
  //## begin PoissonGen%36AC503E011A.declarations preserve=yes
  //## end PoissonGen%36AC503E011A.declarations

//## begin module%36AC503E011A.epilog preserve=yes
} // namespace sch
//## end module%36AC503E011A.epilog
