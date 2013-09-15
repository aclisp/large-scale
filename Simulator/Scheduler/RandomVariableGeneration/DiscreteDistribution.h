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
//	File Name: DiscreteDistribution.h
//
//	Description: Header file for the DiscreteDistribution class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: May/11/1999
//
//	Visual Source Safe $Revision: 3 $
//
//	Last check in $Date: 17/05/99 11:18 $
//
//	********************************************************************************
//## end module%37331C4400D9.cp

//## Module: DiscreteDistribution%37331C4400D9; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\DiscreteDistribution.h

#ifndef DiscreteDistribution_h
#define DiscreteDistribution_h 1

//## begin module%37331C4400D9.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%37331C4400D9.additionalIncludes

//## begin module%37331C4400D9.includes preserve=yes
//## end module%37331C4400D9.includes

// BaseDistribution
#include "BaseDistribution.h"
//## begin module%37331C4400D9.additionalDeclarations preserve=yes
namespace sch {
//## end module%37331C4400D9.additionalDeclarations


//## begin DiscreteDistribution%37331C4400D9.preface preserve=yes
//## end DiscreteDistribution%37331C4400D9.preface

//## Class: DiscreteDistribution%37331C4400D9; Abstract
//	********************************************************************************
//
//	All random numbers generation belonging to a discrete distribution returns an int result.
//	 A discrete distribuition allows the user to redefine the original parameters set  at construction time.
//
//	This abstract class models that common behavior.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class DiscreteDistribution : public BaseDistribution  //## Inherits: <unnamed>%3733235F00E8
{
  //## begin DiscreteDistribution%37331C4400D9.initialDeclarations preserve=yes
  //## end DiscreteDistribution%37331C4400D9.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: nextValue%373322D7024B
      virtual RetCode nextValue (Int *pValue);

    // Additional Public Declarations
      //## begin DiscreteDistribution%37331C4400D9.public preserve=yes
      //## end DiscreteDistribution%37331C4400D9.public

  protected:
    //## Constructors (specified)
      //## Operation: DiscreteDistribution%3736D0A2003D
      DiscreteDistribution (Ulong seed, Float minVal = 0, Float maxVal = 1);

    // Additional Protected Declarations
      //## begin DiscreteDistribution%37331C4400D9.protected preserve=yes
      //## end DiscreteDistribution%37331C4400D9.protected

  private:
    // Additional Private Declarations
      //## begin DiscreteDistribution%37331C4400D9.private preserve=yes
      //## end DiscreteDistribution%37331C4400D9.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin DiscreteDistribution%37331C4400D9.implementation preserve=yes
      //## end DiscreteDistribution%37331C4400D9.implementation

};

//## begin DiscreteDistribution%37331C4400D9.postscript preserve=yes
//## end DiscreteDistribution%37331C4400D9.postscript

// Class DiscreteDistribution 

//## begin module%37331C4400D9.epilog preserve=yes
} // namespace sch
//## end module%37331C4400D9.epilog


#endif
