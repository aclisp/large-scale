//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%37331C4400D9.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%37331C4400D9.cm

//## begin module%37331C4400D9.cp preserve=yes
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
//	File Name: DiscreteDistribution.cpp
//
//	Description: Implementation file for the DiscreteDistribution class
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
//## end module%37331C4400D9.cp

//## Module: DiscreteDistribution%37331C4400D9; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\DiscreteDistribution.cpp

//## begin module%37331C4400D9.additionalIncludes preserve=no
//## end module%37331C4400D9.additionalIncludes

//## begin module%37331C4400D9.includes preserve=yes
//## end module%37331C4400D9.includes

// DiscreteDistribution
#include "MemLeak.h"
#include "DiscreteDistribution.h"
//## begin module%37331C4400D9.additionalDeclarations preserve=yes
namespace sch {
//## end module%37331C4400D9.additionalDeclarations


// Class DiscreteDistribution 

//## Operation: DiscreteDistribution%3736D0A2003D
//	********************************************************************************
//
//	Name: DiscreteDistributions(Ulong seed, Float minVal = 0, Float maxVal = 1)
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
DiscreteDistribution::DiscreteDistribution (Ulong seed, Float minVal, Float maxVal)
  //## begin DiscreteDistribution::DiscreteDistribution%3736D0A2003D.hasinit preserve=no
  //## end DiscreteDistribution::DiscreteDistribution%3736D0A2003D.hasinit
  //## begin DiscreteDistribution::DiscreteDistribution%3736D0A2003D.initialization preserve=yes
	: BaseDistribution (seed)
  //## end DiscreteDistribution::DiscreteDistribution%3736D0A2003D.initialization
{
  //## begin DiscreteDistribution::DiscreteDistribution%3736D0A2003D.body preserve=yes
  //## end DiscreteDistribution::DiscreteDistribution%3736D0A2003D.body
}



//## Other Operations (implementation)
//## Operation: nextValue%373322D7024B
//	********************************************************************************
//
//	Name: RetCode nextValue (Int *pValue)
//
//	Description: A virtual method that set the discrete distribution behavior (returns a Int result).
//
//	Output parameters: Int *pValue;  //next result  generated
//
//	Returns: -1
//
//	********************************************************************************
RetCode DiscreteDistribution::nextValue (Int *pValue)
{
  //## begin DiscreteDistribution::nextValue%373322D7024B.body preserve=yes
	return -1;
  //## end DiscreteDistribution::nextValue%373322D7024B.body
}

// Additional Declarations
  //## begin DiscreteDistribution%37331C4400D9.declarations preserve=yes
  //## end DiscreteDistribution%37331C4400D9.declarations

//## begin module%37331C4400D9.epilog preserve=yes
} // namespace sch
//## end module%37331C4400D9.epilog
