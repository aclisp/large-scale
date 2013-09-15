//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC5032034D.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC5032034D.cm

//## begin module%36AC5032034D.cp preserve=yes
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
//	File Name: DiscreteGen.cpp
//
//	Description: Implementation file for the DiscreteGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 13 $
//
//	Last check in $Date: 10/04/99 2:41p $
//
//	********************************************************************************
//## end module%36AC5032034D.cp

//## Module: DiscreteGen%36AC5032034D; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\DiscreteGen.cpp

//## begin module%36AC5032034D.additionalIncludes preserve=no
//## end module%36AC5032034D.additionalIncludes

//## begin module%36AC5032034D.includes preserve=yes
#include "MemLeak.h"
#include <math.h>
//## end module%36AC5032034D.includes

// DiscreteGen
#include "DiscreteGen.h"
//## begin module%36AC5032034D.additionalDeclarations preserve=yes
namespace sch {
//## end module%36AC5032034D.additionalDeclarations


// Class DiscreteGen 





//## Operation: DiscreteGen%36B6E43A0310
//	********************************************************************************
//
//	Name: DiscreteGen(Ulong seed, Float* pProbArray, Ulong sizeOfArray, Int aValue)
//
//	Description: Non-default constructor - requires the initial seed, the probability array, the number of elements in the array and
//	the aValue to generate the random variables.
//
//	Input parameters: Ulong seed;         //seed to begin the random number generation
//	                  Float* pProbArray; //a probabilities array
//	                  Ulong sizeOfArray;  //number of elements in the array
//	                  Int aValue;  //minimum possible value of the sample
//
//	Returns: none
//
//	Remarks: must invoke the DiscreteDistribution constructor passing the value of the seed parameter.
//	         The seed value must be: 0 <= seed <= MAX_WORD;  sizeOfArray > 0; and pProbArray != NULL.
//
//	********************************************************************************
DiscreteGen::DiscreteGen (Ulong seed, Float *pProbArray, Ulong sizeOfArray, Int aValue)
  //## begin DiscreteGen::DiscreteGen%36B6E43A0310.hasinit preserve=no
      : a(aValue),
        arraySize(sizeOfArray),
        pProbability(NULL),
        epsilon(0.0001f)
  //## end DiscreteGen::DiscreteGen%36B6E43A0310.hasinit
  //## begin DiscreteGen::DiscreteGen%36B6E43A0310.initialization preserve=yes
    , DiscreteDistribution (seed)
  //## end DiscreteGen::DiscreteGen%36B6E43A0310.initialization
{
  //## begin DiscreteGen::DiscreteGen%36B6E43A0310.body preserve=yes

	Ulong i;
	Float probSum, probability;
	RetCode rc = SCH_SUCCESS;

	if (arraySize <= 0)
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (pProbArray == NULL)
	{
		rc = SCH_INVALID_PARAMETER;
	}

	pProbability = new Float[arraySize];

	if (pProbability == NULL)
	{
		rc = SCH_ALLOCATION_ERROR;
	}

	if (rc == SCH_SUCCESS)
	{
		probSum = 0;
		for (i=0; i<arraySize; i++)
		{
			probability = pProbArray[i]; 
			if (( probability < 0.0f) || ( probability > 1.0f))
			{
				rc = SCH_INVALID_PARAMETER;
				break;
			}
			probSum += probability;
			pProbability[i] = probability;
		}
	}

	if (rc == SCH_SUCCESS)	
	{
		if (fabs(1.0 - probSum) > epsilon )
		{
			rc = SCH_INVALID_PARAMETER;
		}
	}

	if (rc != SCH_SUCCESS)
	{
		delete [] pProbability;
		throw SchException (rc);
	}

  //## end DiscreteGen::DiscreteGen%36B6E43A0310.body
}



//## Other Operations (implementation)
//## Operation: nextValue%36B6E4130238
//	********************************************************************************
//
//	Name: RetCode nextValue(Int *pValue)
//
//	Description: Returns the next value of the Discrete distribution.
//
//	Output parameters: Int *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//
//	********************************************************************************
RetCode DiscreteGen::nextValue (Int *pValue)
{
  //## begin DiscreteGen::nextValue%36B6E4130238.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Int   discrete = 0;
	Long i;
	Float number;
	Float sum1;
	Float sum2;
	bool  bFound = false; 
			
	rc = getNextValue (&number);

	if (rc == SCH_SUCCESS)
	{
		sum1 = 0;
		sum2 = 0;

		for (i=0; i<arraySize; i++)
		{
			sum1 = sum2;
			sum2 += pProbability[i];

			if ((sum1 <= number) && (number < sum2))
			{
				bFound = true;
				break;
			}
		}

		if (bFound)
		{
			discrete = i + a;		
		}
		else
		{
			// Return the last value having non-zero probability, the array size cannot be 0 by constructor
			for(i = arraySize - 1; i>=0 ; --i)
			{
				if(pProbability[i] > 0.0)
				{
				discrete = i + a;
				break;
				}

			}
		}

		*pValue = discrete;
	}

	return rc;
  //## end DiscreteGen::nextValue%36B6E4130238.body
}

//## Operation: redefine%374E91B102CB
//	********************************************************************************
//
//	Name: RetCode redefine ( Float* pProbArray, Int aValue)
//
//	Description: Redefines the parameters suplied at construction time. Requires the probability array and  the aValue to generate the
//	random variables.
//
//	Input parameters: Float* pProbArray; //a probabilities array
//	                  Int aValue;  //minimum possible value of the sample
//
//	Returns:SCH_SUCCESS
//	                SCH_INVALID_PARAMETER
//	                SCH_ALLOCATION_ERROR
//
//	Remarks: Note that the array dimention is not supplied. In fact, it will be used  the same suplied at construction time.
//
//	********************************************************************************
RetCode DiscreteGen::redefine (Float *pProbArray, Int aValue)
{
  //## begin DiscreteGen::redefine%374E91B102CB.body preserve=yes

	Ulong i;
	Float probSum, probability;
	RetCode rc = SCH_SUCCESS;

	if (pProbArray == NULL)
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (pProbability == NULL)
	{
		rc = SCH_ALLOCATION_ERROR;
	}

	if (rc == SCH_SUCCESS)
	{
		a = aValue;
		probSum = 0;
		for (i=0; i<arraySize; i++)
		{
			probability = pProbArray[i]; 
			if (( probability < 0.0f) || ( probability > 1.0f))
			{
				rc = SCH_INVALID_PARAMETER;
				break;
			}
			probSum += probability;
			pProbability[i] = probability;
		}
	}

	if (rc == SCH_SUCCESS)
	{
		if (fabs(1.0 - probSum) > epsilon )
		{
			rc = SCH_INVALID_PARAMETER;
		}
	}

	return rc;

  //## end DiscreteGen::redefine%374E91B102CB.body
}

// Additional Declarations
  //## begin DiscreteGen%36AC5032034D.declarations preserve=yes
  //## end DiscreteGen%36AC5032034D.declarations

//## begin module%36AC5032034D.epilog preserve=yes
} //namespace sch
//## end module%36AC5032034D.epilog
