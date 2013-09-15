//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC503A0255.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC503A0255.cm

//## begin module%36AC503A0255.cp preserve=yes
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
//	File Name: NegExpGen.h
//
//	Description: Header file for the NegExpGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 5 $
//
//	Last check in $Date: 7/06/99 14:19 $
//
//	********************************************************************************
//## end module%36AC503A0255.cp

//## Module: NegExpGen%36AC503A0255; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\NegExpGen.h

#ifndef NegExpGen_h
#define NegExpGen_h 1

//## begin module%36AC503A0255.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36AC503A0255.additionalIncludes

//## begin module%36AC503A0255.includes preserve=yes
//## end module%36AC503A0255.includes

// ContinuousDistribution
#include "ContinuousDistribution.h"
//## begin module%36AC503A0255.additionalDeclarations preserve=yes
namespace sch {
//## end module%36AC503A0255.additionalDeclarations


//## begin NegExpGen%36AC503A0255.preface preserve=yes
//## end NegExpGen%36AC503A0255.preface

//## Class: NegExpGen%36AC503A0255
//	********************************************************************************
//
//	Generates the Negative Exponential distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class NegExpGen : public ContinuousDistribution  //## Inherits: <unnamed>%36B1964203D9
{
  //## begin NegExpGen%36AC503A0255.initialDeclarations preserve=yes
  //## end NegExpGen%36AC503A0255.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: NegExpGen%36B6E5440359
      NegExpGen (Ulong seed, Float lambdaValue);


    //## Other Operations (specified)
      //## Operation: nextValue%36B6E528007E
      RetCode nextValue (Float *pValue);

    // Additional Public Declarations
      //## begin NegExpGen%36AC503A0255.public preserve=yes
      //## end NegExpGen%36AC503A0255.public

  protected:
    // Additional Protected Declarations
      //## begin NegExpGen%36AC503A0255.protected preserve=yes
      //## end NegExpGen%36AC503A0255.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: lambda%370A45330295
      //	The lambda value used to generate the  Negative Exponential distribution.
      //## begin NegExpGen::lambda%370A45330295.attr preserve=no  private: Float {UAC} lambdaValue
      const Float lambda;
      //## end NegExpGen::lambda%370A45330295.attr

    // Additional Private Declarations
      //## begin NegExpGen%36AC503A0255.private preserve=yes
      //## end NegExpGen%36AC503A0255.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin NegExpGen%36AC503A0255.implementation preserve=yes
      //## end NegExpGen%36AC503A0255.implementation

};

//## begin NegExpGen%36AC503A0255.postscript preserve=yes
//## end NegExpGen%36AC503A0255.postscript

// Class NegExpGen 

//## begin module%36AC503A0255.epilog preserve=yes
} // namespace sch
//## end module%36AC503A0255.epilog


#endif
