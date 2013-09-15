//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36BAD82F02FE.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36BAD82F02FE.cm

//## begin module%36BAD82F02FE.cp preserve=yes
//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: Simulation Management
//
//	File Name: ProcessState.h
//
//	Description:
//
//	Author:	 Hélio Azevedo
//
//	Creation Date: 03/99
//
//	Visual Source Safe $Revision: 6 $
//
//	Last check in $Date: 13/05/99 9:24 $
//
//	********************************************************************************
//## end module%36BAD82F02FE.cp

//## Module: ProcessState%36BAD82F02FE; Pseudo Package specification
//## Source file: T:\SimulationManagement\ProcessState.h

#ifndef ProcessState_h
#define ProcessState_h 1

//## begin module%36BAD82F02FE.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36BAD82F02FE.additionalIncludes

//## begin module%36BAD82F02FE.includes preserve=yes
//## end module%36BAD82F02FE.includes

//## begin module%36BAD82F02FE.additionalDeclarations preserve=yes
namespace sch  {

//## end module%36BAD82F02FE.additionalDeclarations


//## begin ProcessState%36BAD82F02FE.preface preserve=yes
//## end ProcessState%36BAD82F02FE.preface

//## Class: ProcessState%36BAD82F02FE
//	********************************************************************************
//
//	enum {NonInitialized, Active, Scheduled, Passive,Terminated}
//
//	********************************************************************************
//## Category: SimulationManagement%369E33A90107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum {NonInitialized, Active, Scheduled, Passive,Terminated} ProcessState;

//## begin ProcessState%36BAD82F02FE.postscript preserve=yes
//## end ProcessState%36BAD82F02FE.postscript

//## begin module%36BAD82F02FE.epilog preserve=yes
}	// namespace sch

//## end module%36BAD82F02FE.epilog


#endif
