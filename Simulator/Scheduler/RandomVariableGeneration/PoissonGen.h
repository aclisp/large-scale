//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC503E011A.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC503E011A.cm

//## begin module%36AC503E011A.cp preserve=yes
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
//	File Name: PoissonGen.h
//
//	Description: Header file for the PoissonGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 7 $
//
//	Last check in $Date: 1/06/99 14:53 $
//
//	********************************************************************************
//## end module%36AC503E011A.cp

//## Module: PoissonGen%36AC503E011A; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\PoissonGen.h

#ifndef PoissonGen_h
#define PoissonGen_h 1

//## begin module%36AC503E011A.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36AC503E011A.additionalIncludes

//## begin module%36AC503E011A.includes preserve=yes
//## end module%36AC503E011A.includes

// DiscreteDistribution
#include "DiscreteDistribution.h"
//## begin module%36AC503E011A.additionalDeclarations preserve=yes
namespace sch {
//## end module%36AC503E011A.additionalDeclarations


//## begin PoissonGen%36AC503E011A.preface preserve=yes
//## end PoissonGen%36AC503E011A.preface

//## Class: PoissonGen%36AC503E011A
//	********************************************************************************
//
//	Generates the Poisson distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class PoissonGen : public DiscreteDistribution  //## Inherits: <unnamed>%3733246D024F
{
  //## begin PoissonGen%36AC503E011A.initialDeclarations preserve=yes
  //## end PoissonGen%36AC503E011A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: PoissonGen%36B6E5900308
      PoissonGen (Ulong seed, Float mean);


    //## Other Operations (specified)
      //## Operation: nextValue%36B6E57901CF
      RetCode nextValue (Int *pValue);

      //## Operation: redefine%374E8D2C019B
      RetCode redefine (Float mean);

    // Additional Public Declarations
      //## begin PoissonGen%36AC503E011A.public preserve=yes
      //## end PoissonGen%36AC503E011A.public

  protected:
    // Additional Protected Declarations
      //## begin PoissonGen%36AC503E011A.protected preserve=yes
      //## end PoissonGen%36AC503E011A.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: exponent%370267C20017
      //	Exponent value used to calculate the next value of the poisson generation.
      //## begin PoissonGen::exponent%370267C20017.attr preserve=no  private: Float {UA} mean
      Float exponent;
      //## end PoissonGen::exponent%370267C20017.attr

    // Additional Private Declarations
      //## begin PoissonGen%36AC503E011A.private preserve=yes
      //## end PoissonGen%36AC503E011A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin PoissonGen%36AC503E011A.implementation preserve=yes
      //## end PoissonGen%36AC503E011A.implementation

};

//## begin PoissonGen%36AC503E011A.postscript preserve=yes
//## end PoissonGen%36AC503E011A.postscript

// Class PoissonGen 

//## begin module%36AC503E011A.epilog preserve=yes
} // namespace sch
//## end module%36AC503E011A.epilog


#endif
