//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC502E0316.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC502E0316.cm

//## begin module%36AC502E0316.cp preserve=yes
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
//	File Name: RandIntGen.h
//
//	Description: Header file for the RandIntGen class
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
//## end module%36AC502E0316.cp

//## Module: RandIntGen%36AC502E0316; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\RandIntGen.h

#ifndef RandIntGen_h
#define RandIntGen_h 1

//## begin module%36AC502E0316.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36AC502E0316.additionalIncludes

//## begin module%36AC502E0316.includes preserve=yes
//## end module%36AC502E0316.includes

// DiscreteDistribution
#include "DiscreteDistribution.h"
//## begin module%36AC502E0316.additionalDeclarations preserve=yes
namespace sch {
//## end module%36AC502E0316.additionalDeclarations


//## begin RandIntGen%36AC502E0316.preface preserve=yes
//## end RandIntGen%36AC502E0316.preface

//## Class: RandIntGen%36AC502E0316
//	********************************************************************************
//
//	Generates the RandInt distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class RandIntGen : public DiscreteDistribution  //## Inherits: <unnamed>%37332457004F
{
  //## begin RandIntGen%36AC502E0316.initialDeclarations preserve=yes
  //## end RandIntGen%36AC502E0316.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: RandIntGen%36B6E391017D
      RandIntGen (Ulong seed, Ulong minVal, Ulong maxVal);


    //## Other Operations (specified)
      //## Operation: nextValue%36B6E370017F
      RetCode nextValue (Int *pValue);

      //## Operation: redefine%374EEE7B0336
      RetCode redefine (Ulong minVal, Ulong maxVal);

    // Additional Public Declarations
      //## begin RandIntGen%36AC502E0316.public preserve=yes
      //## end RandIntGen%36AC502E0316.public

  protected:
    // Additional Protected Declarations
      //## begin RandIntGen%36AC502E0316.protected preserve=yes
      //## end RandIntGen%36AC502E0316.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: maxValue%370269720054
      //	Upper boundary of the range to generate the random variables.
      //## begin RandIntGen::maxValue%370269720054.attr preserve=no  private: Ulong {UA} maxVal
      Ulong maxValue;
      //## end RandIntGen::maxValue%370269720054.attr

      //## Attribute: minValue%37026948000E
      //	Lower boundary of the range to generate the random variables.
      //## begin RandIntGen::minValue%37026948000E.attr preserve=no  private: Ulong {UA} minVal
      Ulong minValue;
      //## end RandIntGen::minValue%37026948000E.attr

    // Additional Private Declarations
      //## begin RandIntGen%36AC502E0316.private preserve=yes
      //## end RandIntGen%36AC502E0316.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin RandIntGen%36AC502E0316.implementation preserve=yes
      //## end RandIntGen%36AC502E0316.implementation

};

//## begin RandIntGen%36AC502E0316.postscript preserve=yes
//## end RandIntGen%36AC502E0316.postscript

// Class RandIntGen 

//## begin module%36AC502E0316.epilog preserve=yes
} //namespace sch
//## end module%36AC502E0316.epilog


#endif
