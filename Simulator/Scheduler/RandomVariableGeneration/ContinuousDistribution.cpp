//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%37331C1800B7.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%37331C1800B7.cm

//## begin module%37331C1800B7.cp preserve=yes
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
//	File Name: ContinuousDistribution.cpp
//
//	Description: Implementation file for the ContinuousDistribution class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: May/11/1999
//
//	Visual Source Safe $Revision: 2 $
//
//	Last check in $Date: 17/05/99 8:59 $
//
//	********************************************************************************
//## end module%37331C1800B7.cp

//## Module: ContinuousDistribution%37331C1800B7; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\ContinuousDistribution.cpp

//## begin module%37331C1800B7.additionalIncludes preserve=no
//## end module%37331C1800B7.additionalIncludes

//## begin module%37331C1800B7.includes preserve=yes
//## end module%37331C1800B7.includes

// ContinuousDistribution
#include "MemLeak.h"
#include "ContinuousDistribution.h"
//## begin module%37331C1800B7.additionalDeclarations preserve=yes
namespace sch {
//## end module%37331C1800B7.additionalDeclarations


// Class ContinuousDistribution 

//## Operation: ContinuousDistribution%3736E9590230
//	********************************************************************************
//
//	Name: ContinuousDistribution()
//
//	Description: The default constructor
//
//	Returns: none
//
//	Remarks: a BaseGen object is not instantiated by this constructor, which is protected to enforce this class to be abstract (only
//	descendent classes can instantiate ContinuousDistribution).
//
//	********************************************************************************
ContinuousDistribution::ContinuousDistribution ()
  //## begin ContinuousDistribution::ContinuousDistribution%3736E9590230.hasinit preserve=no
  //## end ContinuousDistribution::ContinuousDistribution%3736E9590230.hasinit
  //## begin ContinuousDistribution::ContinuousDistribution%3736E9590230.initialization preserve=yes
  //## end ContinuousDistribution::ContinuousDistribution%3736E9590230.initialization
{
  //## begin ContinuousDistribution::ContinuousDistribution%3736E9590230.body preserve=yes
  //## end ContinuousDistribution::ContinuousDistribution%3736E9590230.body
}

//## Operation: ContinuousDistribution%3736CECF0000
//	********************************************************************************
//
//	Name: ContinuousDistribution(Ulong seed, Float minVal = 0, Float maxVal = 1)
//
//	Description: Non-default constructor - requires the initial seed and range to generate the random variables.
//
//	Input parameters: Ulong seed;       //seed to begin the random number generation
//	                  Float minVal = 0; //lower boundary of the range
//	                  Float maxVal = 1; //upper boundary of the range
//
//	Returns: none
//
//	Remarks: must invoke the BaseDistribution constructor passing the value of the seed parameter.
//
//	********************************************************************************
ContinuousDistribution::ContinuousDistribution (Ulong seed, Float minVal, Float maxVal)
  //## begin ContinuousDistribution::ContinuousDistribution%3736CECF0000.hasinit preserve=no
  //## end ContinuousDistribution::ContinuousDistribution%3736CECF0000.hasinit
  //## begin ContinuousDistribution::ContinuousDistribution%3736CECF0000.initialization preserve=yes
	: BaseDistribution (seed, minVal, maxVal)
  //## end ContinuousDistribution::ContinuousDistribution%3736CECF0000.initialization
{
  //## begin ContinuousDistribution::ContinuousDistribution%3736CECF0000.body preserve=yes
  //## end ContinuousDistribution::ContinuousDistribution%3736CECF0000.body
}



//## Other Operations (implementation)
//## Operation: nextValue%37331C67035A
//	********************************************************************************
//
//	Name: RetCode nextValue (Float *pValue)
//
//	Description: A virtual method that set the continuous distribution behavior (returns a float result).
//
//	Output parameters: Float *pValue;  //next result  generated
//
//	Returns: -1
//
//	********************************************************************************
RetCode ContinuousDistribution::nextValue (Float* pValue)
{
  //## begin ContinuousDistribution::nextValue%37331C67035A.body preserve=yes
	return -1;
  //## end ContinuousDistribution::nextValue%37331C67035A.body
}

// Additional Declarations
  //## begin ContinuousDistribution%37331C1800B7.declarations preserve=yes
  //## end ContinuousDistribution%37331C1800B7.declarations

//## begin module%37331C1800B7.epilog preserve=yes
} // namespace sch
//## end module%37331C1800B7.epilog
