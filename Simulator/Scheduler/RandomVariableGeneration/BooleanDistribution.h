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
//	File Name: BooleanDistribution.h
//
//	Description: Header file for the BooleanDistribution class
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

//## Module: BooleanDistribution%37331C300383; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\BooleanDistribution.h

#ifndef BooleanDistribution_h
#define BooleanDistribution_h 1

//## begin module%37331C300383.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%37331C300383.additionalIncludes

//## begin module%37331C300383.includes preserve=yes
//## end module%37331C300383.includes

// BaseDistribution
#include "BaseDistribution.h"
//## begin module%37331C300383.additionalDeclarations preserve=yes
namespace sch {
//## end module%37331C300383.additionalDeclarations


//## begin BooleanDistribution%37331C300383.preface preserve=yes
//## end BooleanDistribution%37331C300383.preface

//## Class: BooleanDistribution%37331C300383; Abstract
//	********************************************************************************
//
//	All random numbers generation belonging to a boolean distribution returns a boolean result. A boolean distribuition allows the user
//	to redefine the original parameters set at construction time.
//
//	This abstract class models that common behavior.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class BooleanDistribution : public BaseDistribution  //## Inherits: <unnamed>%373323630008
{
  //## begin BooleanDistribution%37331C300383.initialDeclarations preserve=yes
  //## end BooleanDistribution%37331C300383.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: nextValue%373323100121
      virtual RetCode nextValue (bool *pValue);

    // Additional Public Declarations
      //## begin BooleanDistribution%37331C300383.public preserve=yes
      //## end BooleanDistribution%37331C300383.public

  protected:
    //## Constructors (specified)
      //## Operation: BooleanDistribution%3736CCA203DE
      BooleanDistribution (Ulong seed, Float minVal = 0, Float maxVal = 1);

    // Additional Protected Declarations
      //## begin BooleanDistribution%37331C300383.protected preserve=yes
      //## end BooleanDistribution%37331C300383.protected

  private:
    // Additional Private Declarations
      //## begin BooleanDistribution%37331C300383.private preserve=yes
      //## end BooleanDistribution%37331C300383.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin BooleanDistribution%37331C300383.implementation preserve=yes
      //## end BooleanDistribution%37331C300383.implementation

};

//## begin BooleanDistribution%37331C300383.postscript preserve=yes
//## end BooleanDistribution%37331C300383.postscript

// Class BooleanDistribution 

//## begin module%37331C300383.epilog preserve=yes
} // namespace sch
//## end module%37331C300383.epilog


#endif
