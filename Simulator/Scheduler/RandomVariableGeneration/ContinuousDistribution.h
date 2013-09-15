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
//	File Name: ContinuousDistribution.h
//
//	Description: Header file for the ContinuousDistribution class
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

//## Module: ContinuousDistribution%37331C1800B7; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\ContinuousDistribution.h

#ifndef ContinuousDistribution_h
#define ContinuousDistribution_h 1

//## begin module%37331C1800B7.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%37331C1800B7.additionalIncludes

//## begin module%37331C1800B7.includes preserve=yes
//## end module%37331C1800B7.includes

// BaseDistribution
#include "BaseDistribution.h"
//## begin module%37331C1800B7.additionalDeclarations preserve=yes
namespace sch {
//## end module%37331C1800B7.additionalDeclarations


//## begin ContinuousDistribution%37331C1800B7.preface preserve=yes
//## end ContinuousDistribution%37331C1800B7.preface

//## Class: ContinuousDistribution%37331C1800B7; Abstract
//	********************************************************************************
//
//	All random numbers generation belonging to a continuous distribution returns a float result.
//
//	This abstract class models that common behavior.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ContinuousDistribution : public BaseDistribution  //## Inherits: <unnamed>%3733235A0330
{
  //## begin ContinuousDistribution%37331C1800B7.initialDeclarations preserve=yes
  //## end ContinuousDistribution%37331C1800B7.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: nextValue%37331C67035A
      virtual RetCode nextValue (Float* pValue);

    // Additional Public Declarations
      //## begin ContinuousDistribution%37331C1800B7.public preserve=yes
      //## end ContinuousDistribution%37331C1800B7.public

  protected:
    //## Constructors (specified)
      //## Operation: ContinuousDistribution%3736E9590230
      ContinuousDistribution ();

      //## Operation: ContinuousDistribution%3736CECF0000
      ContinuousDistribution (Ulong seed, Float minVal = 0, Float maxVal = 1);

    // Additional Protected Declarations
      //## begin ContinuousDistribution%37331C1800B7.protected preserve=yes
      //## end ContinuousDistribution%37331C1800B7.protected

  private:
    // Additional Private Declarations
      //## begin ContinuousDistribution%37331C1800B7.private preserve=yes
      //## end ContinuousDistribution%37331C1800B7.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ContinuousDistribution%37331C1800B7.implementation preserve=yes
      //## end ContinuousDistribution%37331C1800B7.implementation

};

//## begin ContinuousDistribution%37331C1800B7.postscript preserve=yes
//## end ContinuousDistribution%37331C1800B7.postscript

// Class ContinuousDistribution 

//## begin module%37331C1800B7.epilog preserve=yes
} // namespace sch
//## end module%37331C1800B7.epilog


#endif
