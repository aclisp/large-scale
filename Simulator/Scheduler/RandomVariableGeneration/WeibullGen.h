//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%370E00320092.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%370E00320092.cm

//## begin module%370E00320092.cp preserve=yes
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
//	File Name: WeibullGen.h
//
//	Description: Header file for the WeibullGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 5 $
//
//	Last check in $Date: 7/06/99 14:20 $
//
//	********************************************************************************
//## end module%370E00320092.cp

//## Module: WeibullGen%370E00320092; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\WeibullGen.h

#ifndef WeibullGen_h
#define WeibullGen_h 1

//## begin module%370E00320092.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%370E00320092.additionalIncludes

//## begin module%370E00320092.includes preserve=yes
//## end module%370E00320092.includes

// NegExpGen
#include "NegExpGen.h"
//## begin module%370E00320092.additionalDeclarations preserve=yes
namespace sch {
//## end module%370E00320092.additionalDeclarations


//## begin WeibullGen%370E00320092.preface preserve=yes
//## end WeibullGen%370E00320092.preface

//## Class: WeibullGen%370E00320092
//	********************************************************************************
//
//	Generates the Weibull distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class WeibullGen : public NegExpGen  //## Inherits: <unnamed>%373C3CDE0267
{
  //## begin WeibullGen%370E00320092.initialDeclarations preserve=yes
  //## end WeibullGen%370E00320092.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: WeibullGen%370E00B601E6
      WeibullGen (Ulong seed, Float lambdaValue, Float bValue);


    //## Other Operations (specified)
      //## Operation: nextValue%370E01B001E5
      RetCode nextValue (Float *pValue);

    // Additional Public Declarations
      //## begin WeibullGen%370E00320092.public preserve=yes
      //## end WeibullGen%370E00320092.public

  protected:
    // Additional Protected Declarations
      //## begin WeibullGen%370E00320092.protected preserve=yes
      //## end WeibullGen%370E00320092.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: b%370E01E801C8
      //	The value used to obtain the exponent value to calculate the Weibull distribution.
      //## begin WeibullGen::b%370E01E801C8.attr preserve=no  private: Float {UA} 
      Float b;
      //## end WeibullGen::b%370E01E801C8.attr

    // Additional Private Declarations
      //## begin WeibullGen%370E00320092.private preserve=yes
      //## end WeibullGen%370E00320092.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin WeibullGen%370E00320092.implementation preserve=yes
      //## end WeibullGen%370E00320092.implementation

};

//## begin WeibullGen%370E00320092.postscript preserve=yes
//## end WeibullGen%370E00320092.postscript

// Class WeibullGen 

//## begin module%370E00320092.epilog preserve=yes
} // namespace sch
//## end module%370E00320092.epilog


#endif
