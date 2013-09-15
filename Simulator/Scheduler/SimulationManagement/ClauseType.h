//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%37010B600051.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%37010B600051.cm

//## begin module%37010B600051.cp preserve=yes
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
//	File Name: ClauseType.h
//
//	Description:
//
//	Author:	Hélio Azevedo
//
//	Creation Date:
//
//	Visual Source Safe $Revision: 5 $
//
//	Last check in $Date: 13/05/99 9:23 $
//
//	********************************************************************************
//## end module%37010B600051.cp

//## Module: ClauseType%37010B600051; Pseudo Package specification
//## Source file: T:\SimulationManagement\ClauseType.h

#ifndef ClauseType_h
#define ClauseType_h 1

//## begin module%37010B600051.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%37010B600051.additionalIncludes

//## begin module%37010B600051.includes preserve=yes
//## end module%37010B600051.includes

//## begin module%37010B600051.additionalDeclarations preserve=yes
//## end module%37010B600051.additionalDeclarations


//## begin ClauseType%37010B600051.preface preserve=yes
namespace sch  {

//## end ClauseType%37010B600051.preface

//## Class: ClauseType%37010B600051
//	********************************************************************************
//
//	enum {RefProcess, ActivationTime, DelayTime}
//
//	********************************************************************************
//## Category: SimulationManagement%369E33A90107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum {RefProcess, ActivationTime, DelayTime} ClauseType;

//## begin ClauseType%37010B600051.postscript preserve=yes
//## end ClauseType%37010B600051.postscript

//## begin module%37010B600051.epilog preserve=yes
}			// end of namespace
//## end module%37010B600051.epilog


#endif
