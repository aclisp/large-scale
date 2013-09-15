//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%398ED01B005C.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%398ED01B005C.cm

//## begin module%398ED01B005C.cp preserve=no
//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: RandomVariableGeneration
//
//	File Name: BinomialGenBtpec.h
//
//	Description:
//
//	Author:
//
//	Creation Date:
//
//	Visual Source Safe $Revision: $
//
//	Last check in $Date: $
//
//	********************************************************************************
//## end module%398ED01B005C.cp

//## Module: BinomialGenBtpec%398ED01B005C; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\BinomialGenBtpec.h

#ifndef BinomialGenBtpec_h
#define BinomialGenBtpec_h 1

//## begin module%398ED01B005C.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%398ED01B005C.additionalIncludes

//## begin module%398ED01B005C.includes preserve=yes
//## end module%398ED01B005C.includes

// DiscreteDistribution
#include "DiscreteDistribution.h"
//## begin module%398ED01B005C.additionalDeclarations preserve=yes
//## end module%398ED01B005C.additionalDeclarations


//## begin BinomialGenBtpec%398ED01B005C.preface preserve=yes
namespace sch {
//## end BinomialGenBtpec%398ED01B005C.preface

//## Class: BinomialGenBtpec%398ED01B005C
//	********************************************************************************
//
//	Generates the Binomial distribution using the uniform generation and the BTPEC algorithm.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class BinomialGenBtpec : public DiscreteDistribution  //## Inherits: <unnamed>%398ED01B006E
{
  //## begin BinomialGenBtpec%398ED01B005C.initialDeclarations preserve=yes
  //## end BinomialGenBtpec%398ED01B005C.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: BinomialGenBtpec%398ED01B0060
      BinomialGenBtpec (Ulong seed, Ulong numSamples, Float probability);


    //## Other Operations (specified)
      //## Operation: nextValue%398ED01B0069
      RetCode nextValue (Int *pValue);

      //## Operation: redefine%398ED01B006B
      RetCode redefine (Ulong numSamples, Float probability);

      //## Operation: redefineAndNextValue%398ED12E004D
      RetCode redefineAndNextValue (Int *pValue, Ulong n, Float pp);

    // Additional Public Declarations
      //## begin BinomialGenBtpec%398ED01B005C.public preserve=yes
      //## end BinomialGenBtpec%398ED01B005C.public

  protected:
    // Additional Protected Declarations
      //## begin BinomialGenBtpec%398ED01B005C.protected preserve=yes
      //## end BinomialGenBtpec%398ED01B005C.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: psave%398ED01B005D
      //	The p value used to calculate the binomial distribution.
      //## begin BinomialGenBtpec::psave%398ED01B005D.attr preserve=no  private: Float {UA} probability
      Float psave;
      //## end BinomialGenBtpec::psave%398ED01B005D.attr

      //## Attribute: nsave%398ED01B005E
      //	The r value used to calculate the binomial distribution.
      //## begin BinomialGenBtpec::nsave%398ED01B005E.attr preserve=no  private: Ulong {UA} numSamples
      Ulong nsave;
      //## end BinomialGenBtpec::nsave%398ED01B005E.attr

    // Additional Private Declarations
      //## begin BinomialGenBtpec%398ED01B005C.private preserve=yes
      //## end BinomialGenBtpec%398ED01B005C.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin BinomialGenBtpec%398ED01B005C.implementation preserve=yes
      //## end BinomialGenBtpec%398ED01B005C.implementation

};

//## begin BinomialGenBtpec%398ED01B005C.postscript preserve=yes
//## end BinomialGenBtpec%398ED01B005C.postscript

// Class BinomialGenBtpec 

//## begin module%398ED01B005C.epilog preserve=yes
} //namespace sch
//## end module%398ED01B005C.epilog


#endif
