//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%370E09160239.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%370E09160239.cm

//## begin module%370E09160239.cp preserve=yes
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
//	File Name: RayleighGen.h
//
//	Description: Header file for the RayleighGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 5 $
//
//	Last check in $Date: 7/06/99 14:20 $
//
//	********************************************************************************
//## end module%370E09160239.cp

//## Module: RayleighGen%370E09160239; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\RayleighGen.h

#ifndef RayleighGen_h
#define RayleighGen_h 1

//## begin module%370E09160239.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%370E09160239.additionalIncludes

//## begin module%370E09160239.includes preserve=yes
//## end module%370E09160239.includes

// ContinuousDistribution
#include "ContinuousDistribution.h"
//## begin module%370E09160239.additionalDeclarations preserve=yes
namespace sch {
//## end module%370E09160239.additionalDeclarations


//## begin RayleighGen%370E09160239.preface preserve=yes
//## end RayleighGen%370E09160239.preface

//## Class: RayleighGen%370E09160239
//	********************************************************************************
//
//	Generates the Rayleigh distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class RayleighGen : public ContinuousDistribution  //## Inherits: <unnamed>%373C05700082
{
  //## begin RayleighGen%370E09160239.initialDeclarations preserve=yes
  //## end RayleighGen%370E09160239.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: RayleighGen%370E096502C9
      RayleighGen (Ulong seed, Float lambdaValue);


    //## Other Operations (specified)
      //## Operation: nextValue%370E097500D7
      RetCode nextValue (Float *pValue);

    // Additional Public Declarations
      //## begin RayleighGen%370E09160239.public preserve=yes
      //## end RayleighGen%370E09160239.public

  protected:
    // Additional Protected Declarations
      //## begin RayleighGen%370E09160239.protected preserve=yes
      //## end RayleighGen%370E09160239.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: lambda%373C059003C7
      //	The value used to obtain the Rayleigh distribution.
      //## begin RayleighGen::lambda%373C059003C7.attr preserve=no  private: Float {UAC} lambdaValue
      const Float lambda;
      //## end RayleighGen::lambda%373C059003C7.attr

    // Additional Private Declarations
      //## begin RayleighGen%370E09160239.private preserve=yes
      //## end RayleighGen%370E09160239.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin RayleighGen%370E09160239.implementation preserve=yes
      //## end RayleighGen%370E09160239.implementation

};

//## begin RayleighGen%370E09160239.postscript preserve=yes
//## end RayleighGen%370E09160239.postscript

// Class RayleighGen 

//## begin module%370E09160239.epilog preserve=yes
} // namespace sch
//## end module%370E09160239.epilog


#endif
