//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%37679F880069.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%37679F880069.cm

//## begin module%37679F880069.cp preserve=yes
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
//	File Name: TruncatedGeometricGen.cpp
//
//	Description:  Implementation file for the GeometricGen class
//
//	Author:	Hélio Azevedo
//
//	Creation Date:	Jun/16/1999
//
//	Visual Source Safe $Revision: 2 $
//
//	Last check in $Date: 17/06/99 8:15 $
//
//	********************************************************************************
//## end module%37679F880069.cp

//## Module: TruncatedGeometricGen%37679F880069; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\TruncatedGeometricGen.cpp

//## begin module%37679F880069.additionalIncludes preserve=no
//## end module%37679F880069.additionalIncludes

//## begin module%37679F880069.includes preserve=yes
//## end module%37679F880069.includes

// TruncatedGeometricGen
#include "MemLeak.h"
#include "TruncatedGeometricGen.h"
//## begin module%37679F880069.additionalDeclarations preserve=yes
namespace sch {
//## end module%37679F880069.additionalDeclarations


// Class TruncatedGeometricGen 



//## Operation: TruncatedGeometricGen%3767A0E7012C
//	********************************************************************************
//
//	Name: TruncatedGeometricGen(Ulong seed, Float pVal)
//
//	Description: Non-default constructor - requires the initial seed, the pVal,  the xMin and the xMax values to generate the random
//	variables .
//
//	Input parameters: Ulong seed;   //seed to begin the random number generation
//	                  Float pVal;
//	                  Int xMin;                //minimum value
//	                  Int xMax;              //maximum value
//
//	Returns: none
//
//	Remarks: must invoke the Geometric  constructor passing the values of the seed and the pVal parameters.
//	         The seed value must be: 0 <= seed <= MAX_WORD; 0 < pVal <1  ; xMax >0; xMin >0; and xMin < xMax
//
//	********************************************************************************
TruncatedGeometricGen::TruncatedGeometricGen (Ulong seed, Float pVal, Int xMin, Int xMax)
  //## begin TruncatedGeometricGen::TruncatedGeometricGen%3767A0E7012C.hasinit preserve=no
      : xMaximum(xMax),
        xMinimum(xMin)
  //## end TruncatedGeometricGen::TruncatedGeometricGen%3767A0E7012C.hasinit
  //## begin TruncatedGeometricGen::TruncatedGeometricGen%3767A0E7012C.initialization preserve=yes
	, GeometricGen (seed, pVal)
  //## end TruncatedGeometricGen::TruncatedGeometricGen%3767A0E7012C.initialization
{
  //## begin TruncatedGeometricGen::TruncatedGeometricGen%3767A0E7012C.body preserve=yes
	RetCode rc = SCH_SUCCESS;

	if ((xMinimum <= 0))
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if ((xMaximum <= 0))
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (xMinimum >= xMaximum)
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);
	}



  //## end TruncatedGeometricGen::TruncatedGeometricGen%3767A0E7012C.body
}



//## Other Operations (implementation)
//## Operation: nextValue%3767A05C00E6
//	********************************************************************************
//
//	Name: RetCode nextValue(Float *pValue)
//
//	Description: Returns the next value of the truncatedGeometric distribution.
//
//	Output parameters: Int *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode TruncatedGeometricGen::nextValue (Int* pValue)
{
  //## begin TruncatedGeometricGen::nextValue%3767A05C00E6.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Int number;

	rc = GeometricGen::nextValue(&number);

	if (rc == SCH_SUCCESS)
	{
		if (number < xMinimum)
		{
			number = xMinimum;
		}
		else if (number > xMaximum)
		{
			number = xMaximum;
		}
		*pValue = number;
	}

	return rc;

  //## end TruncatedGeometricGen::nextValue%3767A05C00E6.body
}

//## Operation: redefine%3767B8E00162
//	********************************************************************************
//
//	Name:  RetCode redefine(Float pVal)
//
//	Description:  Redefines the parameters suplied at construction time. As the constructor,  requires the  pVal , the xMin and the x
//	Max values to generate the random variables.
//
//	Input parameters:  Float pVal;
//	                   Int xMin;                //minimum value
//	                   Int xMax;                //maximum value
//
//	Returns: SCH_SUCCESS
//	            SCH_INVALID_PARAMETER
//
//	Remarks:
//	         The pVal  value must be:  0 < pVal < 1 ; xMax >0; xMin >0; and xMin < xMax
//
//	********************************************************************************
RetCode TruncatedGeometricGen::redefine (Float pVal, Int xMin, Int xMax)
{
  //## begin TruncatedGeometricGen::redefine%3767B8E00162.body preserve=yes
	RetCode rc;


	if ((xMin <= 0))
		return SCH_INVALID_PARAMETER;

	if ((xMax <= 0))
		return SCH_INVALID_PARAMETER;

	if (xMin >= xMax)
		return SCH_INVALID_PARAMETER;

	if ((rc = GeometricGen::redefine(pVal)) != SCH_SUCCESS)
		return rc;

	xMaximum = xMax;
	xMinimum = xMin;

	return SCH_SUCCESS;

  //## end TruncatedGeometricGen::redefine%3767B8E00162.body
}

// Additional Declarations
  //## begin TruncatedGeometricGen%37679F880069.declarations preserve=yes
  //## end TruncatedGeometricGen%37679F880069.declarations

//## begin module%37679F880069.epilog preserve=yes
} //namespace sch
//## end module%37679F880069.epilog
