//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC503603CB.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC503603CB.cm

//## begin module%36AC503603CB.cp preserve=yes
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
//	File Name: DrawGen.cpp
//
//	Description: Implementation file for the DrawGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 8 $
//
//	Last check in $Date: 1/06/99 14:52 $
//
//	********************************************************************************
//## end module%36AC503603CB.cp

//## Module: DrawGen%36AC503603CB; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\DrawGen.cpp

//## begin module%36AC503603CB.additionalIncludes preserve=no
//## end module%36AC503603CB.additionalIncludes

//## begin module%36AC503603CB.includes preserve=yes
//## end module%36AC503603CB.includes

// DrawGen
#include "MemLeak.h"
#include "DrawGen.h"
//## begin module%36AC503603CB.additionalDeclarations preserve=yes
namespace sch {
//## end module%36AC503603CB.additionalDeclarations


// Class DrawGen 


//## Operation: DrawGen%36B6E4DF021D
//	********************************************************************************
//
//	Name: DrawGen(Ulong seed, Float threshold)
//
//	Description: Non-default constructor - requires the initial seed and the threshold to generate the random variables.
//
//	Input parameters: Ulong seed;      //seed to begin the random number generation
//	                  Float threshold; //threshold
//
//	Returns: none
//
//	Remarks: must invoke the BooleanDistribution constructor passing the value of the seed parameter.
//	         The seed value must be: 0 <= seed <= MAX_WORD; and the threshold value must be: 0 <= threshold <= 1
//
//	********************************************************************************
DrawGen::DrawGen (Ulong seed, Float threshold)
  //## begin DrawGen::DrawGen%36B6E4DF021D.hasinit preserve=no
      : thresholdNumber(threshold)
  //## end DrawGen::DrawGen%36B6E4DF021D.hasinit
  //## begin DrawGen::DrawGen%36B6E4DF021D.initialization preserve=yes
  , BooleanDistribution (seed)
  //## end DrawGen::DrawGen%36B6E4DF021D.initialization
{
  //## begin DrawGen::DrawGen%36B6E4DF021D.body preserve=yes
	RetCode rc = SCH_SUCCESS;

	if ((thresholdNumber < 0) || (thresholdNumber > 1))
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);
	}
  //## end DrawGen::DrawGen%36B6E4DF021D.body
}



//## Other Operations (implementation)
//## Operation: nextValue%36B6E4C701BF
//	********************************************************************************
//
//	Name: RetCode nextValue(bool *pValue)
//
//	Description: Returns the next value of the Draw distribution.
//
//	Output parameters: bool *pValue;  //next result generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode DrawGen::nextValue (bool *pValue)
{
  //## begin DrawGen::nextValue%36B6E4C701BF.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Float number;
	bool draw;
	
	if (thresholdNumber == 1)
	{
		draw = true;
	}
	else if (thresholdNumber == 0)
	{
		draw = false;
	}
	else
	{
		do
		{
			rc = getNextValue (&number);
			if (rc != SCH_SUCCESS)
			{
				break;
			}
		}while (number == thresholdNumber); 

		if (rc == SCH_SUCCESS)
		{
			if (number < thresholdNumber)
			{
				 draw = true;
			}
			else
			{
				draw = false;
			}
		}
	}
	*pValue = draw;	

	return rc;
  //## end DrawGen::nextValue%36B6E4C701BF.body
}

//## Operation: redefine%374E98FF02D4
//	********************************************************************************
//
//	Name: RetCode redefine(  Float threshold)
//
//	Description: Redefines the parameters suplied at construction time. As the  constructor,   requires the threshold to generate the
//	random variables.
//
//	Input parameters:  Float threshold; //threshold
//
//	Returns: none
//
//	Remarks:
//	         The  threshold value must be: 0 <= threshold <= 1
//
//	********************************************************************************
RetCode DrawGen::redefine (Float threshold)
{
  //## begin DrawGen::redefine%374E98FF02D4.body preserve=yes

	if ((threshold < 0) || (threshold > 1))
		return SCH_INVALID_PARAMETER;

	thresholdNumber = threshold;

	return SCH_SUCCESS;

  //## end DrawGen::redefine%374E98FF02D4.body
}

// Additional Declarations
  //## begin DrawGen%36AC503603CB.declarations preserve=yes
  //## end DrawGen%36AC503603CB.declarations

//## begin module%36AC503603CB.epilog preserve=yes
} //namespace sch
//## end module%36AC503603CB.epilog
