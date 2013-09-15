//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%3712004402C5.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3712004402C5.cm

//## begin module%3712004402C5.cp preserve=yes
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
//	File Name: TruncatedCauchyGen.h
//
//	Description: Header file for the TruncatedCauchyGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 4 $
//
//	Last check in $Date: 17/05/99 10:03 $
//
//	********************************************************************************
//## end module%3712004402C5.cp

//## Module: TruncatedCauchyGen%3712004402C5; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\TruncatedCauchyGen.h

#ifndef TruncatedCauchyGen_h
#define TruncatedCauchyGen_h 1

//## begin module%3712004402C5.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%3712004402C5.additionalIncludes

//## begin module%3712004402C5.includes preserve=yes
//## end module%3712004402C5.includes

// CauchyGen
#include "CauchyGen.h"
//## begin module%3712004402C5.additionalDeclarations preserve=yes
namespace sch {
//## end module%3712004402C5.additionalDeclarations


//## begin TruncatedCauchyGen%3712004402C5.preface preserve=yes
//## end TruncatedCauchyGen%3712004402C5.preface

//## Class: TruncatedCauchyGen%3712004402C5
//	********************************************************************************
//
//	Generates the Truncated Cauchy distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class TruncatedCauchyGen : public CauchyGen  //## Inherits: <unnamed>%371200580184
{
  //## begin TruncatedCauchyGen%3712004402C5.initialDeclarations preserve=yes
  //## end TruncatedCauchyGen%3712004402C5.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: TruncatedCauchyGen%371200C60286
      TruncatedCauchyGen (Ulong seed, Float alfa, Float beta, Float xMin, Float xMax);


    //## Other Operations (specified)
      //## Operation: nextValue%371200D300F4
      RetCode nextValue (Float *pValue);

    // Additional Public Declarations
      //## begin TruncatedCauchyGen%3712004402C5.public preserve=yes
      //## end TruncatedCauchyGen%3712004402C5.public

  protected:
    // Additional Protected Declarations
      //## begin TruncatedCauchyGen%3712004402C5.protected preserve=yes
      //## end TruncatedCauchyGen%3712004402C5.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: xMaximum%3712017C0260
      //	The maximum value used to truncate the random value obtained.
      //## begin TruncatedCauchyGen::xMaximum%3712017C0260.attr preserve=no  private: Float {UAC} xMax
      const Float xMaximum;
      //## end TruncatedCauchyGen::xMaximum%3712017C0260.attr

      //## Attribute: xMinimum%371F10A100CE
      //	The minimum value used to truncate the random value obtained.
      //## begin TruncatedCauchyGen::xMinimum%371F10A100CE.attr preserve=no  private: Float {UAC} xMin
      const Float xMinimum;
      //## end TruncatedCauchyGen::xMinimum%371F10A100CE.attr

    // Additional Private Declarations
      //## begin TruncatedCauchyGen%3712004402C5.private preserve=yes
      //## end TruncatedCauchyGen%3712004402C5.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin TruncatedCauchyGen%3712004402C5.implementation preserve=yes
      //## end TruncatedCauchyGen%3712004402C5.implementation

};

//## begin TruncatedCauchyGen%3712004402C5.postscript preserve=yes
//## end TruncatedCauchyGen%3712004402C5.postscript

// Class TruncatedCauchyGen 

//## begin module%3712004402C5.epilog preserve=yes
} // namespace sch
//## end module%3712004402C5.epilog


#endif
