//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC504B0169.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC504B0169.cm

//## begin module%36AC504B0169.cp preserve=yes
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
//	File Name: LogNormalGen.cpp
//
//	Description: Implementation file for the LogNormalGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 12 $
//
//	Last check in $Date: 7/06/99 14:19 $
//
//	********************************************************************************
//## end module%36AC504B0169.cp

//## Module: LogNormalGen%36AC504B0169; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\LogNormalGen.cpp

//## begin module%36AC504B0169.additionalIncludes preserve=no
//## end module%36AC504B0169.additionalIncludes

//## begin module%36AC504B0169.includes preserve=yes
#include "MemLeak.h"
#include <math.h>
//## end module%36AC504B0169.includes

// LogNormalGen
#include "LogNormalGen.h"
//## begin module%36AC504B0169.additionalDeclarations preserve=yes
namespace sch {
//## end module%36AC504B0169.additionalDeclarations


// Class LogNormalGen 



//## Operation: LogNormalGen%36B6E65A0038
//	********************************************************************************
//
//	Name: LogNormalGen(Ulong seed, Float mean, Float stdDev)
//
//	Description: Non-default constructor - requires the initial seed, the mean and the stdDev  of the Normal distribution used as base
//	to  to generate the random variables.
//
//	Input parameters: Ulong seed;   //seed to begin the random number generation.
//	                  Float mean;   //mean from Normal distribution
//	                  Float stdDev; //standard deviation from Normal distribution
//
//	Returns: none
//
//	Remarks: must invoke the default ContinuousDistribution constructor.
//	         The seed value must be: 0 <= seed <= MAX_WORD; mean  value does not have restriction; and stdDev > 0
//
//	********************************************************************************
LogNormalGen::LogNormalGen (Ulong seed, Float mean, Float stdDev)
  //## begin LogNormalGen::LogNormalGen%36B6E65A0038.hasinit preserve=no
      : euler(exp(1))
  //## end LogNormalGen::LogNormalGen%36B6E65A0038.hasinit
  //## begin LogNormalGen::LogNormalGen%36B6E65A0038.initialization preserve=yes
  ,ContinuousDistribution ()
  //## end LogNormalGen::LogNormalGen%36B6E65A0038.initialization
{
  //## begin LogNormalGen::LogNormalGen%36B6E65A0038.body preserve=yes
	RetCode rc = SCH_SUCCESS;

	if ((seed < 0) || (seed > MAX_WORD) || (stdDev <= 0))
	{
		rc = SCH_INVALID_PARAMETER;
	}


	if (rc == SCH_SUCCESS)
	{
		pNormal = new NormalGen (seed, mean, stdDev);
		if (pNormal == NULL)
		{
			rc = SCH_ALLOCATION_ERROR;
		}
	}

	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);
	}

  //## end LogNormalGen::LogNormalGen%36B6E65A0038.body
}


//## Operation: ~LogNormalGen%3728963C015E
//	********************************************************************************
//
//	Name: ~LogNormalGen(void)
//
//	Description: Destructor - deallocates NormalGen object.
//
//	Parameters: none
//
//	Returns: none
//
//	********************************************************************************
LogNormalGen::~LogNormalGen ()
{
  //## begin LogNormalGen::~LogNormalGen%3728963C015E.body preserve=yes
	delete pNormal;
  //## end LogNormalGen::~LogNormalGen%3728963C015E.body
}



//## Other Operations (implementation)
//## Operation: nextValue%36B6E6420214
//	********************************************************************************
//
//	Name: RetCode nextValue(Float *pValue)
//
//	Description: Returns the next value of the LogNormal distribution.
//
//	Output parameters: Float *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode LogNormalGen::nextValue (Float *pValue)
{
  //## begin LogNormalGen::nextValue%36B6E6420214.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Float number;
	Float logNormal;

	rc = pNormal->nextValue (&number);
	if (rc == SCH_SUCCESS)
	{
		logNormal = (Float) pow (euler, number);
		*pValue = logNormal;
	}

	return rc;
  //## end LogNormalGen::nextValue%36B6E6420214.body
}

// Additional Declarations
  //## begin LogNormalGen%36AC504B0169.declarations preserve=yes
  //## end LogNormalGen%36AC504B0169.declarations

//## begin module%36AC504B0169.epilog preserve=yes
} //namespace sch
//## end module%36AC504B0169.epilog
