//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%38DB88D1003D.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%38DB88D1003D.cm

//## begin module%38DB88D1003D.cp preserve=no
//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: RandomVariableGeneration
//
//	File Name: TruncatedWeibullGen.h
//
//	Description:
//
//	Author:
//
//	Creation Date:
//
//	Visual Source Safe $Revision: $
//
//	Last check in $Date: $
//
//	********************************************************************************
//## end module%38DB88D1003D.cp

//## Module: TruncatedWeibullGen%38DB88D1003D; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\TruncatedWeibullGen.h

#ifndef TruncatedWeibullGen_h
#define TruncatedWeibullGen_h 1

//## begin module%38DB88D1003D.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%38DB88D1003D.additionalIncludes

//## begin module%38DB88D1003D.includes preserve=yes
//## end module%38DB88D1003D.includes

// WeibullGen
#include "WeibullGen.h"
//## begin module%38DB88D1003D.additionalDeclarations preserve=yes
namespace sch {
//## end module%38DB88D1003D.additionalDeclarations


//## begin TruncatedWeibullGen%38DB88D1003D.preface preserve=yes
//## end TruncatedWeibullGen%38DB88D1003D.preface

//## Class: TruncatedWeibullGen%38DB88D1003D
//	********************************************************************************
//
//	Generates the Truncated Weibull random variates using the Weibull generator (rejection method).
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class TruncatedWeibullGen : public WeibullGen  //## Inherits: <unnamed>%38DB88D1003E
{
  //## begin TruncatedWeibullGen%38DB88D1003D.initialDeclarations preserve=yes
  //## end TruncatedWeibullGen%38DB88D1003D.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: TruncatedWeibullGen%38DB88D1003F
      TruncatedWeibullGen (Ulong seed, Float lambdaValue, Float bValue, Float xMin, Float xMax);


    //## Other Operations (specified)
      //## Operation: nextValue%38DB88D10045
      RetCode nextValue (Float *pValue);

    // Additional Public Declarations
      //## begin TruncatedWeibullGen%38DB88D1003D.public preserve=yes
      //## end TruncatedWeibullGen%38DB88D1003D.public

  protected:
    // Additional Protected Declarations
      //## begin TruncatedWeibullGen%38DB88D1003D.protected preserve=yes
      //## end TruncatedWeibullGen%38DB88D1003D.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: xMaximum%38DB88D10047
      //	The maximum value used to accept/reject Weibull random variates.
      //## begin TruncatedWeibullGen::xMaximum%38DB88D10047.attr preserve=no  private: Float {UA} xMax
      Float xMaximum;
      //## end TruncatedWeibullGen::xMaximum%38DB88D10047.attr

      //## Attribute: xMinimum%38DB88D10048
      //## begin TruncatedWeibullGen::xMinimum%38DB88D10048.attr preserve=no  private: Float {UA} xMin
      Float xMinimum;
      //## end TruncatedWeibullGen::xMinimum%38DB88D10048.attr

    // Additional Private Declarations
      //## begin TruncatedWeibullGen%38DB88D1003D.private preserve=yes
      //## end TruncatedWeibullGen%38DB88D1003D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin TruncatedWeibullGen%38DB88D1003D.implementation preserve=yes
      //## end TruncatedWeibullGen%38DB88D1003D.implementation

};

//## begin TruncatedWeibullGen%38DB88D1003D.postscript preserve=yes
//## end TruncatedWeibullGen%38DB88D1003D.postscript

// Class TruncatedWeibullGen 

//## begin module%38DB88D1003D.epilog preserve=yes
} // namespace sch
//## end module%38DB88D1003D.epilog


#endif
