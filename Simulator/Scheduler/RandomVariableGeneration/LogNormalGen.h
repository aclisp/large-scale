//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC504B0169.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC504B0169.cm

//## begin module%36AC504B0169.cp preserve=yes
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
//	File Name: LogNormalGen.h
//
//	Description: Header file for the LogNormalGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 9 $
//
//	Last check in $Date: 7/06/99 14:19 $
//
//	********************************************************************************
//## end module%36AC504B0169.cp

//## Module: LogNormalGen%36AC504B0169; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\LogNormalGen.h

#ifndef LogNormalGen_h
#define LogNormalGen_h 1

//## begin module%36AC504B0169.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36AC504B0169.additionalIncludes

//## begin module%36AC504B0169.includes preserve=yes
//## end module%36AC504B0169.includes

// ContinuousDistribution
#include "ContinuousDistribution.h"
// NormalGen
#include "NormalGen.h"
//## begin module%36AC504B0169.additionalDeclarations preserve=yes
namespace sch {
//## end module%36AC504B0169.additionalDeclarations


//## begin LogNormalGen%36AC504B0169.preface preserve=yes
//## end LogNormalGen%36AC504B0169.preface

//## Class: LogNormalGen%36AC504B0169
//	********************************************************************************
//
//	Generates the LogNormal distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class LogNormalGen : public ContinuousDistribution  //## Inherits: <unnamed>%3737140200B7
{
  //## begin LogNormalGen%36AC504B0169.initialDeclarations preserve=yes
  //## end LogNormalGen%36AC504B0169.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: LogNormalGen%36B6E65A0038
      LogNormalGen (Ulong seed, Float mean, Float stdDev);

    //## Destructor (specified)
      //## Operation: ~LogNormalGen%3728963C015E
      ~LogNormalGen ();


    //## Other Operations (specified)
      //## Operation: nextValue%36B6E6420214
      RetCode nextValue (Float *pValue);

    // Additional Public Declarations
      //## begin LogNormalGen%36AC504B0169.public preserve=yes
      //## end LogNormalGen%36AC504B0169.public

  protected:
    // Additional Protected Declarations
      //## begin LogNormalGen%36AC504B0169.protected preserve=yes
      //## end LogNormalGen%36AC504B0169.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: euler%375833D202EC
      //## begin LogNormalGen::euler%375833D202EC.attr preserve=no  private: Double {UAC} exp(1)
      const Double euler;
      //## end LogNormalGen::euler%375833D202EC.attr

    // Data Members for Associations

      //## Association: RandomVariableGeneration::<unnamed>%37288EF4019A
      //## Role: LogNormalGen::pNormal%37288EF40367
      //	A NormalGen object used to generate a random numbers.
      //## begin LogNormalGen::pNormal%37288EF40367.role preserve=no  private: NormalGen {1 -> 1RHgAN}
      NormalGen *pNormal;
      //## end LogNormalGen::pNormal%37288EF40367.role

    // Additional Private Declarations
      //## begin LogNormalGen%36AC504B0169.private preserve=yes
      //## end LogNormalGen%36AC504B0169.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin LogNormalGen%36AC504B0169.implementation preserve=yes
      //## end LogNormalGen%36AC504B0169.implementation

};

//## begin LogNormalGen%36AC504B0169.postscript preserve=yes
//## end LogNormalGen%36AC504B0169.postscript

// Class LogNormalGen 

//## begin module%36AC504B0169.epilog preserve=yes
} //namespace sch
//## end module%36AC504B0169.epilog


#endif
