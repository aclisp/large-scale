//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC503603CB.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC503603CB.cm

//## begin module%36AC503603CB.cp preserve=yes
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
//	File Name: DrawGen.h
//
//	Description: Header file for the DrawGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 6 $
//
//	Last check in $Date: 1/06/99 14:52 $
//
//	********************************************************************************
//## end module%36AC503603CB.cp

//## Module: DrawGen%36AC503603CB; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\DrawGen.h

#ifndef DrawGen_h
#define DrawGen_h 1

//## begin module%36AC503603CB.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36AC503603CB.additionalIncludes

//## begin module%36AC503603CB.includes preserve=yes
//## end module%36AC503603CB.includes

// BooleanDistribution
#include "BooleanDistribution.h"
//## begin module%36AC503603CB.additionalDeclarations preserve=yes
namespace sch {
//## end module%36AC503603CB.additionalDeclarations


//## begin DrawGen%36AC503603CB.preface preserve=yes
//## end DrawGen%36AC503603CB.preface

//## Class: DrawGen%36AC503603CB
//	********************************************************************************
//
//	Generates the Draw distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class DrawGen : public BooleanDistribution  //## Inherits: <unnamed>%373324000266
{
  //## begin DrawGen%36AC503603CB.initialDeclarations preserve=yes
  //## end DrawGen%36AC503603CB.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: DrawGen%36B6E4DF021D
      DrawGen (Ulong seed, Float threshold);


    //## Other Operations (specified)
      //## Operation: nextValue%36B6E4C701BF
      RetCode nextValue (bool *pValue);

      //## Operation: redefine%374E98FF02D4
      RetCode redefine (Float threshold);

    // Additional Public Declarations
      //## begin DrawGen%36AC503603CB.public preserve=yes
      //## end DrawGen%36AC503603CB.public

  protected:
    // Additional Protected Declarations
      //## begin DrawGen%36AC503603CB.protected preserve=yes
      //## end DrawGen%36AC503603CB.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: thresholdNumber%3702645003C6
      //	Threshold used to calculate the next value of the draw generation.
      //## begin DrawGen::thresholdNumber%3702645003C6.attr preserve=no  private: Float {UA} threshold
      Float thresholdNumber;
      //## end DrawGen::thresholdNumber%3702645003C6.attr

    // Additional Private Declarations
      //## begin DrawGen%36AC503603CB.private preserve=yes
      //## end DrawGen%36AC503603CB.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin DrawGen%36AC503603CB.implementation preserve=yes
      //## end DrawGen%36AC503603CB.implementation

};

//## begin DrawGen%36AC503603CB.postscript preserve=yes
//## end DrawGen%36AC503603CB.postscript

// Class DrawGen 

//## begin module%36AC503603CB.epilog preserve=yes
} //namespace sch
//## end module%36AC503603CB.epilog


#endif
