//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC531E017C.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC531E017C.cm

//## begin module%36AC531E017C.cp preserve=yes
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
//	File Name: BetaGen.h
//
//	Description: Header file for the BetaGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 6 $
//
//	Last check in $Date: 17/05/99 16:35 $
//
//	********************************************************************************
//## end module%36AC531E017C.cp

//## Module: BetaGen%36AC531E017C; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\BetaGen.h

#ifndef BetaGen_h
#define BetaGen_h 1

//## begin module%36AC531E017C.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36AC531E017C.additionalIncludes

//## begin module%36AC531E017C.includes preserve=yes
//## end module%36AC531E017C.includes

// GammaGen
#include "GammaGen.h"
// ContinuousDistribution
#include "ContinuousDistribution.h"
//## begin module%36AC531E017C.additionalDeclarations preserve=yes
namespace sch {
//## end module%36AC531E017C.additionalDeclarations


//## begin BetaGen%36AC531E017C.preface preserve=yes
//## end BetaGen%36AC531E017C.preface

//## Class: BetaGen%36AC531E017C
//	********************************************************************************
//
//	Generates the Beta distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class BetaGen : public ContinuousDistribution  //## Inherits: <unnamed>%373AB9BE003C
{
  //## begin BetaGen%36AC531E017C.initialDeclarations preserve=yes
  //## end BetaGen%36AC531E017C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: BetaGen%36B6E2F60260
      BetaGen (Ulong seed, Float alfa1, Float alfa2, Float aVal = 0, Float bVal = 1);

    //## Destructor (specified)
      //## Operation: ~BetaGen%37133B290145
      ~BetaGen ();


    //## Other Operations (specified)
      //## Operation: nextValue%36B6E2AE01E4
      RetCode nextValue (Float *pValue);

    // Additional Public Declarations
      //## begin BetaGen%36AC531E017C.public preserve=yes
      //## end BetaGen%36AC531E017C.public

  protected:
    // Additional Protected Declarations
      //## begin BetaGen%36AC531E017C.protected preserve=yes
      //## end BetaGen%36AC531E017C.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: a%373059520193
      //	The lower boundary of the generic range.
      //	Generally the value is zero.
      //## begin BetaGen::a%373059520193.attr preserve=no  private: Float {UAC} aVal
      const Float a;
      //## end BetaGen::a%373059520193.attr

      //## Attribute: b%373059660354
      //	The upper boundary of the generic range.
      //	Generally the value is one.
      //## begin BetaGen::b%373059660354.attr preserve=no  private: Float {UAC} bVal
      const Float b;
      //## end BetaGen::b%373059660354.attr

    // Data Members for Associations

      //## Association: RandomVariableGeneration::<unnamed>%373AB5EC0266
      //## Role: BetaGen::pGamma1%373AB5ED004B
      //	A GammaGen object used to generate a random numbers.
      //## begin BetaGen::pGamma1%373AB5ED004B.role preserve=no  private: GammaGen {1 -> 1RHgAN}
      GammaGen *pGamma1;
      //## end BetaGen::pGamma1%373AB5ED004B.role

      //## Association: RandomVariableGeneration::<unnamed>%373AB5F103B8
      //## Role: BetaGen::pGamma2%373AB5F20279
      //	A GammaGen object used to generate a random numbers.
      //## begin BetaGen::pGamma2%373AB5F20279.role preserve=no  private: GammaGen {1 -> 1RHgAN}
      GammaGen *pGamma2;
      //## end BetaGen::pGamma2%373AB5F20279.role

    // Additional Private Declarations
      //## begin BetaGen%36AC531E017C.private preserve=yes
      //## end BetaGen%36AC531E017C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin BetaGen%36AC531E017C.implementation preserve=yes
      //## end BetaGen%36AC531E017C.implementation

};

//## begin BetaGen%36AC531E017C.postscript preserve=yes
//## end BetaGen%36AC531E017C.postscript

// Class BetaGen 

//## begin module%36AC531E017C.epilog preserve=yes
} //namespace sch
//## end module%36AC531E017C.epilog


#endif
