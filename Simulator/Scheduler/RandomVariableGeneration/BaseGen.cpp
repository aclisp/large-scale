//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC5028032B.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC5028032B.cm

//## begin module%36AC5028032B.cp preserve=yes
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
//	File Name: BaseGen.cpp
//
//	Description: Implementation file for the BaseGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 4 $
//
//	Last check in $Date: 10/04/99 2:34p $
//
//	********************************************************************************
//## end module%36AC5028032B.cp

//## Module: BaseGen%36AC5028032B; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\BaseGen.cpp

//## begin module%36AC5028032B.additionalIncludes preserve=no
//## end module%36AC5028032B.additionalIncludes

//## begin module%36AC5028032B.includes preserve=yes
#include "MemLeak.h"
//## end module%36AC5028032B.includes

// BaseGen
#include "BaseGen.h"
//## begin module%36AC5028032B.additionalDeclarations preserve=yes
namespace sch {
//## end module%36AC5028032B.additionalDeclarations


// Class BaseGen 



















//## Operation: BaseGen%36B6DB4B02FE
//	********************************************************************************
//
//	Name: BaseGen(Ulong seed = 78562, Float minVal = 0, Float maxVal = 1)
//
//	Description: Non-default constructor - requires the initial seed and range to generate the random variables.
//
//	Input parameters: Ulong seed;       //seed to begin the random number generation
//	                  Float minVal = 0; //lower boundary of the range
//	                  Float maxVal = 1; //upper boundary of the range
//
//	Returns: none
//
//	Remarks: The seed value must be: 0 <= seed <= MAX_WORD; and minVal < maxVal.
//
//	********************************************************************************
BaseGen::BaseGen (Ulong seed, Float minVal, Float maxVal)
  //## begin BaseGen::BaseGen%36B6DB4B02FE.hasinit preserve=no
      : firstSeedX(seed),
        firstSeedY(78562),
        incrementX(1),
        incrementY(1),
        lenVector(100),
        maxValue(maxVal),
        minValue(minVal),
        moduleX(0x10000000),
        moduleY(0x04000000),
        multiplierX(5),
        multiplierY(9)
  //## end BaseGen::BaseGen%36B6DB4B02FE.hasinit
  //## begin BaseGen::BaseGen%36B6DB4B02FE.initialization preserve=yes
  //## end BaseGen::BaseGen%36B6DB4B02FE.initialization
{
  //## begin BaseGen::BaseGen%36B6DB4B02FE.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Int i;

	if ((firstSeedX < 0) || (firstSeedX > MAX_WORD) || (minValue >= maxValue))
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (rc == SCH_SUCCESS)
	{
		pX = new Ulong [lenVector];
		if (pX == NULL)
		{
			rc = SCH_ALLOCATION_ERROR;
		}
	}

	if (rc == SCH_SUCCESS)
	{
		pY = new Ulong [lenVector];
		if (pY == NULL)
		{
			rc = SCH_ALLOCATION_ERROR;
		}
	}

	if (rc == SCH_SUCCESS)
	{
		pV = new Ulong [lenVector];
		if (pV == NULL)
		{
			rc = SCH_ALLOCATION_ERROR;
		}
	}

	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);
	}

	for (i=0; i<lenVector; i++)
	{
		pX[i] = 0;
		pY[i] = 0;
		pV[i] = 0;
	}

	// Initialize: generating the first numbers
	nextSeedX = firstSeedX;
	nextSeedY = firstSeedY;
	generateX();
	generateY();
	indVector = 0;
	for (i=0; i<lenVector; i++)
	{
		pV[i] = pX[i];
	}
	generateX();

  //## end BaseGen::BaseGen%36B6DB4B02FE.body
}


//## Operation: ~BaseGen%371345CF00C2
//	********************************************************************************
//
//	Name: ~BaseGen(void)
//
//	Description: Destructor - deallocates the X, Y and V vectors used to store the generated random numbers.
//
//	Parameters: none
//
//	Returns: none
//
//	********************************************************************************
BaseGen::~BaseGen ()
{
  //## begin BaseGen::~BaseGen%371345CF00C2.body preserve=yes
	delete [] pX;
	delete [] pY;
	delete [] pV;
  //## end BaseGen::~BaseGen%371345CF00C2.body
}



//## Other Operations (implementation)
//## Operation: generateX%37022DF1021D
//	********************************************************************************
//
//	Name: RetCode generateX(void)
//
//	Description: Generate a random number sequence called X-sequence.
//
//	Parameters: none
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	********************************************************************************
RetCode BaseGen::generateX ()
{
  //## begin BaseGen::generateX%37022DF1021D.body preserve=yes
	Int i;

	for (i=0; i<lenVector; i++)
	{
		nextSeedX = (Ulong) ((multiplierX * nextSeedX + incrementX) % moduleX);
		pX[i] = nextSeedX;
	}
	
	return SCH_SUCCESS;
  //## end BaseGen::generateX%37022DF1021D.body
}

//## Operation: generateY%37022E0D010F
//	********************************************************************************
//
//	Name: RetCode generateY(void)
//
//	Description: Generate a random number sequence called Y-sequence.
//
//	Parameters: none
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	********************************************************************************
RetCode BaseGen::generateY ()
{
  //## begin BaseGen::generateY%37022E0D010F.body preserve=yes
	Int i;

	for (i=0; i<lenVector; i++)
	{
		nextSeedY = (Ulong) ((multiplierY * nextSeedY + incrementY) % moduleY);
		pY[i] = nextSeedY;
	}

	return SCH_SUCCESS;
  //## end BaseGen::generateY%37022E0D010F.body
}

//## Operation: nextValue%36B6D4CD036C
//	********************************************************************************
//
//	Name: RetCode nextValue(Float *pValue)
//
//	Description: Returns the next value of the Uniform distribution.
//
//	Output parameters: Float *pValue;  //next number generated.
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	********************************************************************************
RetCode BaseGen::nextValue (Float *pValue)
{
  //## begin BaseGen::nextValue%36B6D4CD036C.body preserve=yes
	Int j;
	Float nextNumber;
	Float uniform;

	j = (Int) ((__int64)lenVector * pY[indVector] / moduleY);
	nextNumber = ((Float)pV[j]) / moduleX;
	uniform = nextNumber * (maxValue - minValue) + minValue;
	pV[j] = pX[indVector];
	indVector++;
	if (indVector >= lenVector)
	{
		generateX();
		generateY();
		indVector = 0;
	}
	*pValue = uniform;

	return SCH_SUCCESS;
  //## end BaseGen::nextValue%36B6D4CD036C.body
}

// Additional Declarations
  //## begin BaseGen%36AC5028032B.declarations preserve=yes
  //## end BaseGen%36AC5028032B.declarations

//## begin module%36AC5028032B.epilog preserve=yes
} // namespace sch
//## end module%36AC5028032B.epilog
