//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36D1660E01B1.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36D1660E01B1.cm

//## begin module%36D1660E01B1.cp preserve=yes
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
//	File Name: BinomialGen.h
//
//	Description: Header file for the BinomialGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 5 $
//
//	Last check in $Date: 1/06/99 14:49 $
//
//	********************************************************************************
//## end module%36D1660E01B1.cp

//## Module: BinomialGen%36D1660E01B1; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\BinomialGen.h

#ifndef BinomialGen_h
#define BinomialGen_h 1

//## begin module%36D1660E01B1.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36D1660E01B1.additionalIncludes

//## begin module%36D1660E01B1.includes preserve=yes
//## end module%36D1660E01B1.includes

// DiscreteDistribution
#include "DiscreteDistribution.h"
//## begin module%36D1660E01B1.additionalDeclarations preserve=yes
namespace sch {
//## end module%36D1660E01B1.additionalDeclarations


//## begin BinomialGen%36D1660E01B1.preface preserve=yes
//## end BinomialGen%36D1660E01B1.preface

//## Class: BinomialGen%36D1660E01B1
//	********************************************************************************
//
//	Generates the Binomial distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class BinomialGen : public DiscreteDistribution  //## Inherits: <unnamed>%3733247803B3
{
  //## begin BinomialGen%36D1660E01B1.initialDeclarations preserve=yes
  //## end BinomialGen%36D1660E01B1.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: BinomialGen%36D166D40292
      BinomialGen (Ulong seed, Ulong numSamples, Float probability);


    //## Other Operations (specified)
      //## Operation: nextValue%36D1665102A8
      RetCode nextValue (Int *pValue);

      //## Operation: redefine%374E8B82015D
      RetCode redefine (Ulong numSamples, Float probability);

    // Additional Public Declarations
      //## begin BinomialGen%36D1660E01B1.public preserve=yes
      //## end BinomialGen%36D1660E01B1.public

  protected:
    // Additional Protected Declarations
      //## begin BinomialGen%36D1660E01B1.protected preserve=yes
      //## end BinomialGen%36D1660E01B1.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: p%3711EA5C02A3
      //	The p value used to calculate the binomial distribution.
      //## begin BinomialGen::p%3711EA5C02A3.attr preserve=no  private: Float {UA} probability
      Float p;
      //## end BinomialGen::p%3711EA5C02A3.attr

      //## Attribute: r%3711EA2200BF
      //	The r value used to calculate the binomial distribution.
      //## begin BinomialGen::r%3711EA2200BF.attr preserve=no  private: Ulong {UA} numSamples
      Ulong r;
      //## end BinomialGen::r%3711EA2200BF.attr

      //## Attribute: w%3711EDD8026A
      //	This value is set from the r and p values. It is used to calculate the binomial distribution.
      //## begin BinomialGen::w%3711EDD8026A.attr preserve=no  private: Float {UA} 
      Float w;
      //## end BinomialGen::w%3711EDD8026A.attr

    // Additional Private Declarations
      //## begin BinomialGen%36D1660E01B1.private preserve=yes
      //## end BinomialGen%36D1660E01B1.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin BinomialGen%36D1660E01B1.implementation preserve=yes
      //## end BinomialGen%36D1660E01B1.implementation

};

//## begin BinomialGen%36D1660E01B1.postscript preserve=yes
//## end BinomialGen%36D1660E01B1.postscript

// Class BinomialGen 

//## begin module%36D1660E01B1.epilog preserve=yes
} //namespace sch
//## end module%36D1660E01B1.epilog


#endif
