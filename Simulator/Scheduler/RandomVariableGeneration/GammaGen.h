//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%3739B65002DC.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3739B65002DC.cm

//## begin module%3739B65002DC.cp preserve=yes
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
//	File Name: GammaGen.h
//
//	Description: Header file for the GammaGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: May/12/1999
//
//	Visual Source Safe $Revision: 3 $
//
//	Last check in $Date: 7/06/99 14:19 $
//
//	********************************************************************************
//## end module%3739B65002DC.cp

//## Module: GammaGen%3739B65002DC; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\GammaGen.h

#ifndef GammaGen_h
#define GammaGen_h 1

//## begin module%3739B65002DC.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%3739B65002DC.additionalIncludes

//## begin module%3739B65002DC.includes preserve=yes
//## end module%3739B65002DC.includes

// ContinuousDistribution
#include "ContinuousDistribution.h"
//## begin module%3739B65002DC.additionalDeclarations preserve=yes
namespace sch {
//## end module%3739B65002DC.additionalDeclarations


//## begin GammaGen%3739B65002DC.preface preserve=yes
//## end GammaGen%3739B65002DC.preface

//## Class: GammaGen%3739B65002DC
//	********************************************************************************
//
//	Generates the Gamma distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class GammaGen : public ContinuousDistribution  //## Inherits: <unnamed>%3739B66B0231
{
  //## begin GammaGen%3739B65002DC.initialDeclarations preserve=yes
  //## end GammaGen%3739B65002DC.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: GammaGen%3739B7B502B7
      GammaGen (Ulong seed, Float alfaValue, Float betaValue);


    //## Other Operations (specified)
      //## Operation: nextValue%3739B8720377
      RetCode nextValue (Float *pValue);

    // Additional Public Declarations
      //## begin GammaGen%3739B65002DC.public preserve=yes
      //## end GammaGen%3739B65002DC.public

  protected:
    // Additional Protected Declarations
      //## begin GammaGen%3739B65002DC.protected preserve=yes
      //## end GammaGen%3739B65002DC.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: alfa%3739B7A300F9
      //	The value used to obtain the Gamma distribution.
      //## begin GammaGen::alfa%3739B7A300F9.attr preserve=no  private: Float {UAC} alfaValue
      const Float alfa;
      //## end GammaGen::alfa%3739B7A300F9.attr

      //## Attribute: beta%3739BF740100
      //	Multiplier factor of the Gamma distribution.
      //## begin GammaGen::beta%3739BF740100.attr preserve=no  private: Float {UAC} betaValue
      const Float beta;
      //## end GammaGen::beta%3739BF740100.attr

    // Additional Private Declarations
      //## begin GammaGen%3739B65002DC.private preserve=yes
      //## end GammaGen%3739B65002DC.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin GammaGen%3739B65002DC.implementation preserve=yes
      //## end GammaGen%3739B65002DC.implementation

};

//## begin GammaGen%3739B65002DC.postscript preserve=yes
//## end GammaGen%3739B65002DC.postscript

// Class GammaGen 

//## begin module%3739B65002DC.epilog preserve=yes
} // namespace sch
//## end module%3739B65002DC.epilog


#endif
