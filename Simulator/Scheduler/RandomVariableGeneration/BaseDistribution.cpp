//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36B1909403BA.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36B1909403BA.cm

//## begin module%36B1909403BA.cp preserve=yes
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
//	File Name: BaseDistribution.cpp
//
//	Description: Implementation file for the BaseDistribution class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 3 $
//
//	Last check in $Date: 17/05/99 16:35 $
//
//	********************************************************************************
//## end module%36B1909403BA.cp

//## Module: BaseDistribution%36B1909403BA; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\BaseDistribution.cpp

//## begin module%36B1909403BA.additionalIncludes preserve=no
//## end module%36B1909403BA.additionalIncludes

//## begin module%36B1909403BA.includes preserve=yes
//## end module%36B1909403BA.includes

// BaseDistribution
#include "MemLeak.h"
#include "BaseDistribution.h"
//## begin module%36B1909403BA.additionalDeclarations preserve=yes
namespace sch {
//## end module%36B1909403BA.additionalDeclarations


// Class BaseDistribution 


//## Operation: BaseDistribution%3736EAB002D3
//	********************************************************************************
//
//	Name: BaseDistribution()
//
//	Description: The default constructor
//
//	Returns: none
//
//	Remarks: a BaseGen object is not instantiated by this constructor, which is protected to enforce this class to be abstract (only
//	descendent classes can instantiate BaseDistribution).
//
//	********************************************************************************
BaseDistribution::BaseDistribution ()
  //## begin BaseDistribution::BaseDistribution%3736EAB002D3.hasinit preserve=no
  //## end BaseDistribution::BaseDistribution%3736EAB002D3.hasinit
  //## begin BaseDistribution::BaseDistribution%3736EAB002D3.initialization preserve=yes
  //## end BaseDistribution::BaseDistribution%3736EAB002D3.initialization
{
  //## begin BaseDistribution::BaseDistribution%3736EAB002D3.body preserve=yes
	pBase = NULL;
  //## end BaseDistribution::BaseDistribution%3736EAB002D3.body
}

//## Operation: BaseDistribution%36E948F8017D
//	********************************************************************************
//
//	Name: BaseDistribution(Ulong seed, Float minVal = 0, Float maxVal = 1)
//
//	Description: Non-default constructor - requires the initial seed and range to generate the random variables.
//
//	Input parameters: Ulong seed;       //seed to begin the random number generation
//	                  Float minVal = 0; //lower boundary of the range
//	                  Float maxVal = 1; //upper boundary of the range
//
//	Returns: none
//
//	Remarks: a BaseGen object is instantiated by this constructor, which is protected to enforce this class to be abstract (only
//	descendent classes can instantiate BaseDistribution).
//
//	********************************************************************************
BaseDistribution::BaseDistribution (Ulong seed, Float minVal, Float maxVal)
  //## begin BaseDistribution::BaseDistribution%36E948F8017D.hasinit preserve=no
  //## end BaseDistribution::BaseDistribution%36E948F8017D.hasinit
  //## begin BaseDistribution::BaseDistribution%36E948F8017D.initialization preserve=yes
  //## end BaseDistribution::BaseDistribution%36E948F8017D.initialization
{
  //## begin BaseDistribution::BaseDistribution%36E948F8017D.body preserve=yes
	RetCode rc = SCH_SUCCESS;

	pBase = new BaseGen(seed, minVal, maxVal);
	
	if (pBase == NULL)
	{
		rc = SCH_ALLOCATION_ERROR;
	}

	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);
	}

  //## end BaseDistribution::BaseDistribution%36E948F8017D.body
}


//## Operation: ~BaseDistribution%36E949740334
//	********************************************************************************
//
//	Name: ~BaseDistribution(void)
//
//	Description: Destructor - deallocates BaseGen object.
//
//	Parameters: none
//
//	Returns: none
//
//	Remarks: this destructor is virtual since the BaseDistribution class is base for other classes.
//
//	********************************************************************************
BaseDistribution::~BaseDistribution ()
{
  //## begin BaseDistribution::~BaseDistribution%36E949740334.body preserve=yes
	if (pBase != NULL)
	{
		delete pBase;
	}
  //## end BaseDistribution::~BaseDistribution%36E949740334.body
}



//## Other Operations (implementation)
//## Operation: getNextValue%373822B602DC
//	********************************************************************************
//
//	Name: RetCode getNextValue (Float *pValue)
//
//	Description: Returns the next value of the uniform distribution calculated in the BaseGen class.
//
//	Output parameters: Float *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	               SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode BaseDistribution::getNextValue (Float *pValue)
{
  //## begin BaseDistribution::getNextValue%373822B602DC.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Float number;

	if (pBase != NULL)
	{
		pBase->nextValue (&number);
		*pValue = number;
	}
	else
	{
		rc = SCH_ALLOCATION_ERROR;
	}
	
	return rc;

  //## end BaseDistribution::getNextValue%373822B602DC.body
}

// Additional Declarations
  //## begin BaseDistribution%36B1909403BA.declarations preserve=yes
  //## end BaseDistribution%36B1909403BA.declarations

//## begin module%36B1909403BA.epilog preserve=yes
} // namespace sch 
//## end module%36B1909403BA.epilog
