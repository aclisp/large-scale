//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%37331C300383.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%37331C300383.cm

//## begin module%37331C300383.cp preserve=yes
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
//	File Name: BooleanDistribution.cpp
//
//	Description: Implementation file for the BooleanDistribution class
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
//## end module%37331C300383.cp

//## Module: BooleanDistribution%37331C300383; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\BooleanDistribution.cpp

//## begin module%37331C300383.additionalIncludes preserve=no
//## end module%37331C300383.additionalIncludes

//## begin module%37331C300383.includes preserve=yes
//## end module%37331C300383.includes

// BooleanDistribution
#include "MemLeak.h"
#include "BooleanDistribution.h"
//## begin module%37331C300383.additionalDeclarations preserve=yes
namespace sch {
//## end module%37331C300383.additionalDeclarations


// Class BooleanDistribution 

//## Operation: BooleanDistribution%3736CCA203DE
//	********************************************************************************
//
//	Name: BooleanDistribution(Ulong seed, Float minVal = 0, Float maxVal = 1)
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
BooleanDistribution::BooleanDistribution (Ulong seed, Float minVal, Float maxVal)
  //## begin BooleanDistribution::BooleanDistribution%3736CCA203DE.hasinit preserve=no
  //## end BooleanDistribution::BooleanDistribution%3736CCA203DE.hasinit
  //## begin BooleanDistribution::BooleanDistribution%3736CCA203DE.initialization preserve=yes
	: BaseDistribution (seed)
  //## end BooleanDistribution::BooleanDistribution%3736CCA203DE.initialization
{
  //## begin BooleanDistribution::BooleanDistribution%3736CCA203DE.body preserve=yes
  //## end BooleanDistribution::BooleanDistribution%3736CCA203DE.body
}



//## Other Operations (implementation)
//## Operation: nextValue%373323100121
//	********************************************************************************
//
//	Name: RetCode nextValue (bool *pValue)
//
//	Description: A virtual method that set the boolean distribution behavior (returns a boolean result).
//
//	Output parameters: bool *pValue;  //next result  generated
//
//	Returns: -1
//
//	********************************************************************************
RetCode BooleanDistribution::nextValue (bool *pValue)
{
  //## begin BooleanDistribution::nextValue%373323100121.body preserve=yes
	return -1;
  //## end BooleanDistribution::nextValue%373323100121.body
}

// Additional Declarations
  //## begin BooleanDistribution%37331C300383.declarations preserve=yes
  //## end BooleanDistribution%37331C300383.declarations

//## begin module%37331C300383.epilog preserve=yes
} // namespace sch
//## end module%37331C300383.epilog
