//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%3711F4A60283.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3711F4A60283.cm

//## begin module%3711F4A60283.cp preserve=yes
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
//	File Name: CauchyGen.h
//
//	Description: Header file for the BetaGen class
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
//## end module%3711F4A60283.cp

//## Module: CauchyGen%3711F4A60283; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\CauchyGen.h

#ifndef CauchyGen_h
#define CauchyGen_h 1

//## begin module%3711F4A60283.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%3711F4A60283.additionalIncludes

//## begin module%3711F4A60283.includes preserve=yes
//## end module%3711F4A60283.includes

// ContinuousDistribution
#include "ContinuousDistribution.h"
//## begin module%3711F4A60283.additionalDeclarations preserve=yes
namespace sch {
//## end module%3711F4A60283.additionalDeclarations


//## begin CauchyGen%3711F4A60283.preface preserve=yes
//## end CauchyGen%3711F4A60283.preface

//## Class: CauchyGen%3711F4A60283
//	********************************************************************************
//
//	Generates the Cauchy distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CauchyGen : public ContinuousDistribution  //## Inherits: <unnamed>%3711F4CB0023
{
  //## begin CauchyGen%3711F4A60283.initialDeclarations preserve=yes
  //## end CauchyGen%3711F4A60283.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: CauchyGen%3711F4FF033F
      CauchyGen (Ulong seed, Float alfa, Float beta);


    //## Other Operations (specified)
      //## Operation: nextValue%3711F50B0093
      RetCode nextValue (Float *pValue);

    // Additional Public Declarations
      //## begin CauchyGen%3711F4A60283.public preserve=yes
      //## end CauchyGen%3711F4A60283.public

  protected:
    // Additional Protected Declarations
      //## begin CauchyGen%3711F4A60283.protected preserve=yes
      //## end CauchyGen%3711F4A60283.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: a%3711F695039D
      //	The alfa value used to calculate the next value of the cauchy distribution.
      //## begin CauchyGen::a%3711F695039D.attr preserve=no  private: Float {UAC} alfa
      const Float a;
      //## end CauchyGen::a%3711F695039D.attr

      //## Attribute: b%3711F6A3037F
      //	The beta value used to calculate the next value of the cauchy distribution.
      //## begin CauchyGen::b%3711F6A3037F.attr preserve=no  private: Float {UAC} beta
      const Float b;
      //## end CauchyGen::b%3711F6A3037F.attr

    // Additional Private Declarations
      //## begin CauchyGen%3711F4A60283.private preserve=yes
      //## end CauchyGen%3711F4A60283.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CauchyGen%3711F4A60283.implementation preserve=yes
      //## end CauchyGen%3711F4A60283.implementation

};

//## begin CauchyGen%3711F4A60283.postscript preserve=yes
//## end CauchyGen%3711F4A60283.postscript

// Class CauchyGen 

//## begin module%3711F4A60283.epilog preserve=yes
} //namespace sch
//## end module%3711F4A60283.epilog


#endif
