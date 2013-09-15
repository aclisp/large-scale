//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%3753D7910126.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3753D7910126.cm

//## begin module%3753D7910126.cp preserve=yes
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
//	File Name: TruncatedParetoGen.h
//
//	Description:  Header file for the TruncatedPareto class
//
//	Author:	 Hélio Azevedo
//
//	Creation Date: june / 01 /  1999
//
//	Visual Source Safe $Revision: 2 $
//
//	Last check in $Date: 7/06/99 14:20 $
//
//	********************************************************************************
//## end module%3753D7910126.cp

//## Module: TruncatedParetoGen%3753D7910126; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\TruncatedParetoGen.h

#ifndef TruncatedParetoGen_h
#define TruncatedParetoGen_h 1

//## begin module%3753D7910126.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%3753D7910126.additionalIncludes

//## begin module%3753D7910126.includes preserve=yes
//## end module%3753D7910126.includes

// ParetoGen
#include "ParetoGen.h"
//## begin module%3753D7910126.additionalDeclarations preserve=yes
namespace sch {
//## end module%3753D7910126.additionalDeclarations


//## begin TruncatedParetoGen%3753D7910126.preface preserve=yes
//## end TruncatedParetoGen%3753D7910126.preface

//## Class: TruncatedParetoGen%3753D7910126
//	********************************************************************************
//
//	Generates the Truncated Pareto distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class TruncatedParetoGen : public ParetoGen  //## Inherits: <unnamed>%3753D7B50376
{
  //## begin TruncatedParetoGen%3753D7910126.initialDeclarations preserve=yes
  //## end TruncatedParetoGen%3753D7910126.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: TruncatedParetoGen%3753D8AB02D0
      TruncatedParetoGen (Ulong seed, Float alfaValue, Float kValue, Float xMin, Float xMax);


    //## Other Operations (specified)
      //## Operation: nextValue%3753D8AB02F8
      RetCode nextValue (Float *pValue);

    // Additional Public Declarations
      //## begin TruncatedParetoGen%3753D7910126.public preserve=yes
      //## end TruncatedParetoGen%3753D7910126.public

  protected:
    // Additional Protected Declarations
      //## begin TruncatedParetoGen%3753D7910126.protected preserve=yes
      //## end TruncatedParetoGen%3753D7910126.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: xMaximum%3753DF6B022A
      //	The maximum value used to truncate the random value obtained.
      //## begin TruncatedParetoGen::xMaximum%3753DF6B022A.attr preserve=no  private: Float {UAC} xMax
      const Float xMaximum;
      //## end TruncatedParetoGen::xMaximum%3753DF6B022A.attr

      //## Attribute: xMinimum%3753DF6B0252
      //	The minimum value used to truncate the random value obtained.
      //## begin TruncatedParetoGen::xMinimum%3753DF6B0252.attr preserve=no  private: Float {UAC} xMin
      const Float xMinimum;
      //## end TruncatedParetoGen::xMinimum%3753DF6B0252.attr

    // Additional Private Declarations
      //## begin TruncatedParetoGen%3753D7910126.private preserve=yes
      //## end TruncatedParetoGen%3753D7910126.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin TruncatedParetoGen%3753D7910126.implementation preserve=yes
      //## end TruncatedParetoGen%3753D7910126.implementation

};

//## begin TruncatedParetoGen%3753D7910126.postscript preserve=yes
//## end TruncatedParetoGen%3753D7910126.postscript

// Class TruncatedParetoGen 

//## begin module%3753D7910126.epilog preserve=yes
} // namespace sch

//## end module%3753D7910126.epilog


#endif
