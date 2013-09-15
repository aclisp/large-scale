//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%37499EF703E2.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%37499EF703E2.cm

//## begin module%37499EF703E2.cp preserve=yes
//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: Function Evaluator
//
//	File Name: FunctionEvaluator.h
//
//	Description:
//
//	Author:	Hélio Azevedo
//
//	Creation Date: May/25/99
//
//	Visual Source Safe $Revision: 2 $
//
//	Last check in $Date: 16/06/99 9:21 $
//
//	********************************************************************************
//## end module%37499EF703E2.cp

//## Module: FunctionEvaluator%37499EF703E2; Pseudo Package specification
//## Source file: T:\FunctionEvaluator\FunctionEvaluator.h

#ifndef FunctionEvaluator_h
#define FunctionEvaluator_h 1

//## begin module%37499EF703E2.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%37499EF703E2.additionalIncludes

//## begin module%37499EF703E2.includes preserve=yes
//## end module%37499EF703E2.includes

//## begin module%37499EF703E2.additionalDeclarations preserve=yes
namespace sch {
//## end module%37499EF703E2.additionalDeclarations


//## begin FunctionEvaluator%37499EF703E2.preface preserve=yes
//## end FunctionEvaluator%37499EF703E2.preface

//## Class: FunctionEvaluator%37499EF703E2
//	********************************************************************************
//
//	This class supplies a set of static operations to evaluate mathematical functions. As this functions must mainly be used internally
//	to equations, the error report  do not follow the pattern used in all Scheduler packages.  Instead of  error code return a
//	exception will be generated. The following functions are implemented:
//	         - Gamma function.
//
//	Remarks: All operations presented in this class can be implemented as a trivial C global function. They are grouped in the Function
//	Evaluator class mainly for adherence to OO paradigm.
//
//
//	********************************************************************************
//## Category: FunctionEvaluator%374999C30379
//## Persistence: Transient
//## Cardinality/Multiplicity: 1..1

class FunctionEvaluator 
{
  //## begin FunctionEvaluator%37499EF703E2.initialDeclarations preserve=yes
  //## end FunctionEvaluator%37499EF703E2.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: gamma%3749AACA01A0
      static Float gamma (Float arg);

    // Additional Public Declarations
      //## begin FunctionEvaluator%37499EF703E2.public preserve=yes
      //## end FunctionEvaluator%37499EF703E2.public

  protected:
    // Additional Protected Declarations
      //## begin FunctionEvaluator%37499EF703E2.protected preserve=yes
      //## end FunctionEvaluator%37499EF703E2.protected

  private:
    // Additional Private Declarations
      //## begin FunctionEvaluator%37499EF703E2.private preserve=yes
      //## end FunctionEvaluator%37499EF703E2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin FunctionEvaluator%37499EF703E2.implementation preserve=yes
      //## end FunctionEvaluator%37499EF703E2.implementation

};

//## begin FunctionEvaluator%37499EF703E2.postscript preserve=yes
//## end FunctionEvaluator%37499EF703E2.postscript

// Class FunctionEvaluator 

//## begin module%37499EF703E2.epilog preserve=yes
} //namespace sch
//## end module%37499EF703E2.epilog


#endif
