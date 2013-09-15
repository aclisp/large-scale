//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%37307E8203C3.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%37307E8203C3.cm

//## begin module%37307E8203C3.cp preserve=yes
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
//	File Name: GeometricGen.h
//
//	Description: Header file for the GeometricGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: May/05/1999
//
//	Visual Source Safe $Revision: 6 $
//
//	Last check in $Date: 29/06/99 15:38 $
//
//	********************************************************************************
//## end module%37307E8203C3.cp

//## Module: GeometricGen%37307E8203C3; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\GeometricGen.h

#ifndef GeometricGen_h
#define GeometricGen_h 1

//## begin module%37307E8203C3.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%37307E8203C3.additionalIncludes

//## begin module%37307E8203C3.includes preserve=yes
//## end module%37307E8203C3.includes

// DiscreteDistribution
#include "DiscreteDistribution.h"
//## begin module%37307E8203C3.additionalDeclarations preserve=yes
namespace sch {
//## end module%37307E8203C3.additionalDeclarations


//## begin GeometricGen%37307E8203C3.preface preserve=yes
//## end GeometricGen%37307E8203C3.preface

//## Class: GeometricGen%37307E8203C3
//	********************************************************************************
//
//	Generates the Geometric distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class GeometricGen : public DiscreteDistribution  //## Inherits: <unnamed>%3733248B013A
{
  //## begin GeometricGen%37307E8203C3.initialDeclarations preserve=yes
  //## end GeometricGen%37307E8203C3.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: GeometricGen%37307F810185
      GeometricGen (Ulong seed, Float pVal);


    //## Other Operations (specified)
      //## Operation: nextValue%37307F8A0340
      RetCode nextValue (Int *pValue);

      //## Operation: redefine%374E97EC018F
      RetCode redefine (Float pVal);

    // Additional Public Declarations
      //## begin GeometricGen%37307E8203C3.public preserve=yes
      //## end GeometricGen%37307E8203C3.public

  protected:
    // Additional Protected Declarations
      //## begin GeometricGen%37307E8203C3.protected preserve=yes
      //## end GeometricGen%37307E8203C3.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: p%373081A10259
      //	Value used to generated the next random number.
      //## begin GeometricGen::p%373081A10259.attr preserve=no  private: Double {UA} pVal
      Double p;
      //## end GeometricGen::p%373081A10259.attr

      //## Attribute: lnP%3777BDD6006D
      //	Used as constant to calculate the next value in the distribuition.
      //## begin GeometricGen::lnP%3777BDD6006D.attr preserve=no  private: Double {UA} 
      Double lnP;
      //## end GeometricGen::lnP%3777BDD6006D.attr

    // Additional Private Declarations
      //## begin GeometricGen%37307E8203C3.private preserve=yes
      //## end GeometricGen%37307E8203C3.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin GeometricGen%37307E8203C3.implementation preserve=yes
      //## end GeometricGen%37307E8203C3.implementation

};

//## begin GeometricGen%37307E8203C3.postscript preserve=yes
//## end GeometricGen%37307E8203C3.postscript

// Class GeometricGen 

//## begin module%37307E8203C3.epilog preserve=yes
} //namespace sch
//## end module%37307E8203C3.epilog


#endif
