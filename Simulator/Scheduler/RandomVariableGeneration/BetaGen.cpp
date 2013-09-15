//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC531E017C.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC531E017C.cm

//## begin module%36AC531E017C.cp preserve=yes
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
//	File Name: BetaGen.cpp
//
//	Description: Implementation file for the BetaGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 6 $
//
//	Last check in $Date: 17/05/99 16:35 $
//
//	********************************************************************************
//## end module%36AC531E017C.cp

//## Module: BetaGen%36AC531E017C; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\BetaGen.cpp

//## begin module%36AC531E017C.additionalIncludes preserve=no
//## end module%36AC531E017C.additionalIncludes

//## begin module%36AC531E017C.includes preserve=yes
#include "MemLeak.h"
#include <math.h>
//## end module%36AC531E017C.includes

// BetaGen
#include "BetaGen.h"
//## begin module%36AC531E017C.additionalDeclarations preserve=yes
namespace sch {
//## end module%36AC531E017C.additionalDeclarations


// Class BetaGen 





//## Operation: BetaGen%36B6E2F60260
//	********************************************************************************
//
//	Name: BetaGen(Ulong seed, Float alfa1, Float alfa2, Float aVal, Float bVal)
//
//	Description: Non-default constructor - requires the initial seed, the alfa1, the alfa2, the aVal and the bVal  to generate the
//	random variables.
//
//	Input parameters: Ulong seed;   //seed to begin the random number generation
//	                  Float alfa1; // alfa value to calculate one of the Gamma distribution
//	                  Float alfa2; // alfa value to calculate the other Gamma distribution
//	                  Float aVal; //lower boundary of the range
//	                  Float bVal; //upper boundary of the range
//
//	Returns: none
//
//	Remarks: must invoke the default ContinuousDistribution constructor.
//	         The seed value must be: 0 <= seed <= MAX_WORD; alfa1 > 0; alfa2 > 0; and aVal < bVal;
//
//	********************************************************************************
BetaGen::BetaGen (Ulong seed, Float alfa1, Float alfa2, Float aVal, Float bVal)
  //## begin BetaGen::BetaGen%36B6E2F60260.hasinit preserve=no
      : a(aVal),
        b(bVal)
  //## end BetaGen::BetaGen%36B6E2F60260.hasinit
  //## begin BetaGen::BetaGen%36B6E2F60260.initialization preserve=yes
	, ContinuousDistribution ()
  //## end BetaGen::BetaGen%36B6E2F60260.initialization
{
  //## begin BetaGen::BetaGen%36B6E2F60260.body preserve=yes
	RetCode rc = SCH_SUCCESS;

	if ((seed < 0) || (seed > MAX_WORD) || (a >= b))
	{
	    rc = SCH_INVALID_PARAMETER;
	}

	if (rc == SCH_SUCCESS)
	{
		pGamma1 = new GammaGen (seed, alfa1, 1);
		if (pGamma1 == NULL)
		{
			rc = SCH_ALLOCATION_ERROR;
		}
		if (rc == SCH_SUCCESS)
		{
			pGamma2 = new GammaGen (seed, alfa2, 1);
			if (pGamma2 == NULL)
			{
				rc = SCH_ALLOCATION_ERROR;
			}
		}
	}

	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);
	}
  //## end BetaGen::BetaGen%36B6E2F60260.body
}


//## Operation: ~BetaGen%37133B290145
//	********************************************************************************
//
//	Name: ~BetaGen(void)
//
//	Description: Destructor - deallocates GammaGen objects.
//
//	Parameters: none
//
//	Returns: none
//
//	********************************************************************************
BetaGen::~BetaGen ()
{
  //## begin BetaGen::~BetaGen%37133B290145.body preserve=yes
	delete pGamma1;
	delete pGamma2;
  //## end BetaGen::~BetaGen%37133B290145.body
}



//## Other Operations (implementation)
//## Operation: nextValue%36B6E2AE01E4
//	********************************************************************************
//
//	Name: RetCode nextValue(Float *pValue)
//
//	Description: Returns the next value of the Beta distribution.
//
//	Output parameters: Float *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode BetaGen::nextValue (Float *pValue)
{
  //## begin BetaGen::nextValue%36B6E2AE01E4.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Float number1;
	Float number2;
	Float beta;

	rc = pGamma1->nextValue (&number1);
	if (rc == SCH_SUCCESS)
	{
		do
		{
			rc = pGamma2->nextValue (&number2);
			if (rc != SCH_SUCCESS)
			{
				break;
			}
		} while (number1 == number2);

		if (rc == SCH_SUCCESS)
		{
			beta = number1 / (number1 + number2);
			beta = a + ((b - a) * beta);
			*pValue = beta;
		}
	}

	return rc;
  //## end BetaGen::nextValue%36B6E2AE01E4.body
}

// Additional Declarations
  //## begin BetaGen%36AC531E017C.declarations preserve=yes
  //## end BetaGen%36AC531E017C.declarations

//## begin module%36AC531E017C.epilog preserve=yes
} //namespace sch
//## end module%36AC531E017C.epilog
