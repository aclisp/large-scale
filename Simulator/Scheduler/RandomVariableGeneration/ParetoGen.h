//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%370A4B8E03DF.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%370A4B8E03DF.cm

//## begin module%370A4B8E03DF.cp preserve=yes
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
//	File Name: ParetoGen.h
//
//	Description: Header file for the ParetoGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 6 $
//
//	Last check in $Date: 7/06/99 14:19 $
//
//	********************************************************************************
//## end module%370A4B8E03DF.cp

//## Module: ParetoGen%370A4B8E03DF; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\ParetoGen.h

#ifndef ParetoGen_h
#define ParetoGen_h 1

//## begin module%370A4B8E03DF.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%370A4B8E03DF.additionalIncludes

//## begin module%370A4B8E03DF.includes preserve=yes
//## end module%370A4B8E03DF.includes

// ContinuousDistribution
#include "ContinuousDistribution.h"
//## begin module%370A4B8E03DF.additionalDeclarations preserve=yes
namespace sch {
//## end module%370A4B8E03DF.additionalDeclarations


//## begin ParetoGen%370A4B8E03DF.preface preserve=yes
//## end ParetoGen%370A4B8E03DF.preface

//## Class: ParetoGen%370A4B8E03DF
//	********************************************************************************
//
//	Generates the Pareto distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ParetoGen : public ContinuousDistribution  //## Inherits: <unnamed>%373C3AC10024
{
  //## begin ParetoGen%370A4B8E03DF.initialDeclarations preserve=yes
  //## end ParetoGen%370A4B8E03DF.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ParetoGen%370A4CA502A0
      ParetoGen (Ulong seed, Float alfaValue, Float kValue);


    //## Other Operations (specified)
      //## Operation: nextValue%370A4D090004
      virtual RetCode nextValue (Float *pValue);

    // Additional Public Declarations
      //## begin ParetoGen%370A4B8E03DF.public preserve=yes
      //## end ParetoGen%370A4B8E03DF.public

  protected:
    // Additional Protected Declarations
      //## begin ParetoGen%370A4B8E03DF.protected preserve=yes
      //## end ParetoGen%370A4B8E03DF.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: k%37304A560057
      //	Multiplier factor of the Pareto distribution.
      //## begin ParetoGen::k%37304A560057.attr preserve=no  private: Float {UAC} kValue
      const Float k;
      //## end ParetoGen::k%37304A560057.attr

      //## Attribute: alfa%373C3D2301E4
      //	The value used to obtain the exponent value to calculate the Pareto distribution.
      //## begin ParetoGen::alfa%373C3D2301E4.attr preserve=no  private: Float {UAC} alfaValue
      const Float alfa;
      //## end ParetoGen::alfa%373C3D2301E4.attr

    // Additional Private Declarations
      //## begin ParetoGen%370A4B8E03DF.private preserve=yes
      //## end ParetoGen%370A4B8E03DF.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ParetoGen%370A4B8E03DF.implementation preserve=yes
      //## end ParetoGen%370A4B8E03DF.implementation

};

//## begin ParetoGen%370A4B8E03DF.postscript preserve=yes
//## end ParetoGen%370A4B8E03DF.postscript

// Class ParetoGen 

//## begin module%370A4B8E03DF.epilog preserve=yes
} // namespace sch
//## end module%370A4B8E03DF.epilog


#endif
