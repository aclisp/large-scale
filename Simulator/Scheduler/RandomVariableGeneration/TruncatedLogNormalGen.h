//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%3711FAB003E4.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3711FAB003E4.cm

//## begin module%3711FAB003E4.cp preserve=yes
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
//	File Name: TruncatedLogNormalGen.h
//
//	Description: Header file for the TruncatedLogNormalGen class
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
//## end module%3711FAB003E4.cp

//## Module: TruncatedLogNormalGen%3711FAB003E4; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\TruncatedLogNormalGen.h

#ifndef TruncatedLogNormalGen_h
#define TruncatedLogNormalGen_h 1

//## begin module%3711FAB003E4.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%3711FAB003E4.additionalIncludes

//## begin module%3711FAB003E4.includes preserve=yes
//## end module%3711FAB003E4.includes

// LogNormalGen
#include "LogNormalGen.h"
//## begin module%3711FAB003E4.additionalDeclarations preserve=yes
namespace sch {
//## end module%3711FAB003E4.additionalDeclarations


//## begin TruncatedLogNormalGen%3711FAB003E4.preface preserve=yes
//## end TruncatedLogNormalGen%3711FAB003E4.preface

//## Class: TruncatedLogNormalGen%3711FAB003E4
//	********************************************************************************
//
//	Generates the Truncated LogNormal distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class TruncatedLogNormalGen : public LogNormalGen  //## Inherits: <unnamed>%3711FC370314
{
  //## begin TruncatedLogNormalGen%3711FAB003E4.initialDeclarations preserve=yes
  //## end TruncatedLogNormalGen%3711FAB003E4.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: TruncatedLogNormalGen%3711FC4E0259
      TruncatedLogNormalGen (Ulong seed, Float mean, Float stdDev, Float xMin, Float xMax);


    //## Other Operations (specified)
      //## Operation: nextValue%3711FC610026
      RetCode nextValue (Float *pValue);

    // Additional Public Declarations
      //## begin TruncatedLogNormalGen%3711FAB003E4.public preserve=yes
      //## end TruncatedLogNormalGen%3711FAB003E4.public

  protected:
    // Additional Protected Declarations
      //## begin TruncatedLogNormalGen%3711FAB003E4.protected preserve=yes
      //## end TruncatedLogNormalGen%3711FAB003E4.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: xMaximum%3711FDD802F6
      //	The maximum value used to truncate the random value obtained.
      //## begin TruncatedLogNormalGen::xMaximum%3711FDD802F6.attr preserve=no  private: Float {UAC} xMax
      const Float xMaximum;
      //## end TruncatedLogNormalGen::xMaximum%3711FDD802F6.attr

      //## Attribute: xMinimum%371F113A00B0
      //	The minimum value used to truncate the random value obtained.
      //## begin TruncatedLogNormalGen::xMinimum%371F113A00B0.attr preserve=no  private: Float {UAC} xMin
      const Float xMinimum;
      //## end TruncatedLogNormalGen::xMinimum%371F113A00B0.attr

    // Additional Private Declarations
      //## begin TruncatedLogNormalGen%3711FAB003E4.private preserve=yes
      //## end TruncatedLogNormalGen%3711FAB003E4.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin TruncatedLogNormalGen%3711FAB003E4.implementation preserve=yes
      //## end TruncatedLogNormalGen%3711FAB003E4.implementation

};

//## begin TruncatedLogNormalGen%3711FAB003E4.postscript preserve=yes
//## end TruncatedLogNormalGen%3711FAB003E4.postscript

// Class TruncatedLogNormalGen 

//## begin module%3711FAB003E4.epilog preserve=yes
} // namespace sch
//## end module%3711FAB003E4.epilog


#endif
