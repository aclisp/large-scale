//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%373711FF0283.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%373711FF0283.cm

//## begin module%373711FF0283.cp preserve=yes
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
//	File Name: UniformGen.h
//
//	Description: Header file for the UniformGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 5 $
//
//	Last check in $Date: 17/05/99 9:44 $
//
//	********************************************************************************
//## end module%373711FF0283.cp

//## Module: UniformGen%373711FF0283; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\UniformGen.h

#ifndef UniformGen_h
#define UniformGen_h 1

//## begin module%373711FF0283.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%373711FF0283.additionalIncludes

//## begin module%373711FF0283.includes preserve=yes
//## end module%373711FF0283.includes

// ContinuousDistribution
#include "ContinuousDistribution.h"
//## begin module%373711FF0283.additionalDeclarations preserve=yes
namespace sch {
//## end module%373711FF0283.additionalDeclarations


//## begin UniformGen%373711FF0283.preface preserve=yes
//## end UniformGen%373711FF0283.preface

//## Class: UniformGen%373711FF0283
//	********************************************************************************
//
//	Generates the Uniform distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class UniformGen : public ContinuousDistribution  //## Inherits: <unnamed>%373713550068
{
  //## begin UniformGen%373711FF0283.initialDeclarations preserve=yes
  //## end UniformGen%373711FF0283.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: UniformGen%3737125C02CD
      UniformGen (Ulong seed = 78562, Float minVal = 0, Float maxVal = 1);


    //## Other Operations (specified)
      //## Operation: nextValue%373712F001C1
      RetCode nextValue (Float *pValue);

    // Additional Public Declarations
      //## begin UniformGen%373711FF0283.public preserve=yes
      //## end UniformGen%373711FF0283.public

  protected:
    // Additional Protected Declarations
      //## begin UniformGen%373711FF0283.protected preserve=yes
      //## end UniformGen%373711FF0283.protected

  private:
    // Additional Private Declarations
      //## begin UniformGen%373711FF0283.private preserve=yes
      //## end UniformGen%373711FF0283.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin UniformGen%373711FF0283.implementation preserve=yes
      //## end UniformGen%373711FF0283.implementation

};

//## begin UniformGen%373711FF0283.postscript preserve=yes
//## end UniformGen%373711FF0283.postscript

// Class UniformGen 

//## begin module%373711FF0283.epilog preserve=yes
} // namespace sch
//## end module%373711FF0283.epilog


#endif
