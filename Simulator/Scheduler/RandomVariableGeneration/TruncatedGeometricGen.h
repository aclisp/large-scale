//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%37679F880069.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%37679F880069.cm

//## begin module%37679F880069.cp preserve=yes
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
//	File Name: TruncatedGeometricGen.h
//
//	Description: Header file for the GeometricGen class
//
//	Author:	Hélio Azevedo
//
//	Creation Date:	Jun/16/99
//
//	Visual Source Safe $Revision: 2 $
//
//	Last check in $Date: 17/06/99 8:15 $
//
//	********************************************************************************
//## end module%37679F880069.cp

//## Module: TruncatedGeometricGen%37679F880069; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\TruncatedGeometricGen.h

#ifndef TruncatedGeometricGen_h
#define TruncatedGeometricGen_h 1

//## begin module%37679F880069.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%37679F880069.additionalIncludes

//## begin module%37679F880069.includes preserve=yes
//## end module%37679F880069.includes

// GeometricGen
#include "GeometricGen.h"
//## begin module%37679F880069.additionalDeclarations preserve=yes
namespace sch {
//## end module%37679F880069.additionalDeclarations


//## begin TruncatedGeometricGen%37679F880069.preface preserve=yes
//## end TruncatedGeometricGen%37679F880069.preface

//## Class: TruncatedGeometricGen%37679F880069
//	********************************************************************************
//
//	Generates the Truncated Geometric distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class TruncatedGeometricGen : public GeometricGen  //## Inherits: <unnamed>%37679FB40315
{
  //## begin TruncatedGeometricGen%37679F880069.initialDeclarations preserve=yes
  //## end TruncatedGeometricGen%37679F880069.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: TruncatedGeometricGen%3767A0E7012C
      TruncatedGeometricGen (Ulong seed, Float pVal, Int xMin, Int xMax);


    //## Other Operations (specified)
      //## Operation: nextValue%3767A05C00E6
      RetCode nextValue (Int* pValue);

      //## Operation: redefine%3767B8E00162
      RetCode redefine (Float pVal, Int xMin, Int xMax);

    // Additional Public Declarations
      //## begin TruncatedGeometricGen%37679F880069.public preserve=yes
      //## end TruncatedGeometricGen%37679F880069.public

  protected:
    // Additional Protected Declarations
      //## begin TruncatedGeometricGen%37679F880069.protected preserve=yes
      //## end TruncatedGeometricGen%37679F880069.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: xMaximum%3767A79C0153
      //	The maximum value used to truncate the random value obtained.
      //## begin TruncatedGeometricGen::xMaximum%3767A79C0153.attr preserve=no  private: Int {UA} xMax
      Int xMaximum;
      //## end TruncatedGeometricGen::xMaximum%3767A79C0153.attr

      //## Attribute: xMinimum%3767A79C0171
      //	The minimum value used to truncate the random value obtained.
      //## begin TruncatedGeometricGen::xMinimum%3767A79C0171.attr preserve=no  private: Int {UA} xMin
      Int xMinimum;
      //## end TruncatedGeometricGen::xMinimum%3767A79C0171.attr

    // Additional Private Declarations
      //## begin TruncatedGeometricGen%37679F880069.private preserve=yes
      //## end TruncatedGeometricGen%37679F880069.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin TruncatedGeometricGen%37679F880069.implementation preserve=yes
      //## end TruncatedGeometricGen%37679F880069.implementation

};

//## begin TruncatedGeometricGen%37679F880069.postscript preserve=yes
//## end TruncatedGeometricGen%37679F880069.postscript

// Class TruncatedGeometricGen 

//## begin module%37679F880069.epilog preserve=yes
} //namespace sch
//## end module%37679F880069.epilog


#endif
