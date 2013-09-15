//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC50460216.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC50460216.cm

//## begin module%36AC50460216.cp preserve=yes
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
//	File Name: NormalGen.h
//
//	Description: Header file for the NormalGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 5 $
//
//	Last check in $Date: 17/05/99 10:03 $
//
//	********************************************************************************
//## end module%36AC50460216.cp

//## Module: NormalGen%36AC50460216; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\NormalGen.h

#ifndef NormalGen_h
#define NormalGen_h 1

//## begin module%36AC50460216.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36AC50460216.additionalIncludes

//## begin module%36AC50460216.includes preserve=yes
//## end module%36AC50460216.includes

// ContinuousDistribution
#include "ContinuousDistribution.h"
//## begin module%36AC50460216.additionalDeclarations preserve=yes
namespace sch {
//## end module%36AC50460216.additionalDeclarations


//## begin NormalGen%36AC50460216.preface preserve=yes
//## end NormalGen%36AC50460216.preface

//## Class: NormalGen%36AC50460216
//	********************************************************************************
//
//	Generates the Normal distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class NormalGen : public ContinuousDistribution  //## Inherits: <unnamed>%36B196400051
{
  //## begin NormalGen%36AC50460216.initialDeclarations preserve=yes
  //## end NormalGen%36AC50460216.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: NormalGen%36B6E5F502A9
      NormalGen (Ulong seed, Float mean, Float stdDev);


    //## Other Operations (specified)
      //## Operation: nextValue%36B6E5D20104
      RetCode nextValue (Float *pValue);

    // Additional Public Declarations
      //## begin NormalGen%36AC50460216.public preserve=yes
      //## end NormalGen%36AC50460216.public

  protected:
    // Additional Protected Declarations
      //## begin NormalGen%36AC50460216.protected preserve=yes
      //## end NormalGen%36AC50460216.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: a%37276F6A00C2
      //	Add value used in the formula to calculate de Normal distribution.
      //## begin NormalGen::a%37276F6A00C2.attr preserve=no  private: Float {UAC} mean
      const Float a;
      //## end NormalGen::a%37276F6A00C2.attr

      //## Attribute: b%37276F810061
      //	Multiplier value used in the formula to calculate de Normal distribution.
      //## begin NormalGen::b%37276F810061.attr preserve=no  private: Float {UAC} stdDev
      const Float b;
      //## end NormalGen::b%37276F810061.attr

    // Additional Private Declarations
      //## begin NormalGen%36AC50460216.private preserve=yes
      //## end NormalGen%36AC50460216.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin NormalGen%36AC50460216.implementation preserve=yes
      //## end NormalGen%36AC50460216.implementation

};

//## begin NormalGen%36AC50460216.postscript preserve=yes
//## end NormalGen%36AC50460216.postscript

// Class NormalGen 

//## begin module%36AC50460216.epilog preserve=yes
} //namespace sch
//## end module%36AC50460216.epilog


#endif
