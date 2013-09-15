//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC5042027E.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC5042027E.cm

//## begin module%36AC5042027E.cp preserve=yes
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
//	File Name: ErlangGen.h
//
//	Description: Header file for the ErlangGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 5 $
//
//	Last check in $Date: 7/06/99 14:18 $
//
//	********************************************************************************
//## end module%36AC5042027E.cp

//## Module: ErlangGen%36AC5042027E; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\ErlangGen.h

#ifndef ErlangGen_h
#define ErlangGen_h 1

//## begin module%36AC5042027E.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36AC5042027E.additionalIncludes

//## begin module%36AC5042027E.includes preserve=yes
//## end module%36AC5042027E.includes

// ContinuousDistribution
#include "ContinuousDistribution.h"
//## begin module%36AC5042027E.additionalDeclarations preserve=yes
namespace sch {
//## end module%36AC5042027E.additionalDeclarations


//## begin ErlangGen%36AC5042027E.preface preserve=yes
//## end ErlangGen%36AC5042027E.preface

//## Class: ErlangGen%36AC5042027E
//	********************************************************************************
//
//	Generates the Erlang distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ErlangGen : public ContinuousDistribution  //## Inherits: <unnamed>%36B1963C0023
{
  //## begin ErlangGen%36AC5042027E.initialDeclarations preserve=yes
  //## end ErlangGen%36AC5042027E.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ErlangGen%36B6E1C80144
      ErlangGen (Ulong seed, Float lambdaValue, Ulong kValue);


    //## Other Operations (specified)
      //## Operation: nextValue%36B6E1880047
      RetCode nextValue (Float *pValue);

    // Additional Public Declarations
      //## begin ErlangGen%36AC5042027E.public preserve=yes
      //## end ErlangGen%36AC5042027E.public

  protected:
    // Additional Protected Declarations
      //## begin ErlangGen%36AC5042027E.protected preserve=yes
      //## end ErlangGen%36AC5042027E.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: k%370A5C210047
      //	The number of the random numbers that is added to calculate the Erlang distribution.
      //## begin ErlangGen::k%370A5C210047.attr preserve=no  private: Ulong {UAC} kValue
      const Ulong k;
      //## end ErlangGen::k%370A5C210047.attr

      //## Attribute: lambda%370A5C490261
      //	The value used to obtain the Erlang distribution.
      //## begin ErlangGen::lambda%370A5C490261.attr preserve=no  private: Float {UAC} lambdaValue
      const Float lambda;
      //## end ErlangGen::lambda%370A5C490261.attr

    // Additional Private Declarations
      //## begin ErlangGen%36AC5042027E.private preserve=yes
      //## end ErlangGen%36AC5042027E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ErlangGen%36AC5042027E.implementation preserve=yes
      //## end ErlangGen%36AC5042027E.implementation

};

//## begin ErlangGen%36AC5042027E.postscript preserve=yes
//## end ErlangGen%36AC5042027E.postscript

// Class ErlangGen 

//## begin module%36AC5042027E.epilog preserve=yes
} //namespace sch
//## end module%36AC5042027E.epilog


#endif
