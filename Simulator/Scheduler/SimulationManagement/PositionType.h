//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%37010B8E0115.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%37010B8E0115.cm

//## begin module%37010B8E0115.cp preserve=yes
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
//	File Name: PositionType.h
//
//	Description:
//
//	Author:	 Hélio Azevedo
//
//	Creation Date:	03/99
//
//	Visual Source Safe $Revision: 5 $
//
//	Last check in $Date: 13/05/99 9:24 $
//
//	********************************************************************************
//## end module%37010B8E0115.cp

//## Module: PositionType%37010B8E0115; Pseudo Package specification
//## Source file: T:\SimulationManagement\PositionType.h

#ifndef PositionType_h
#define PositionType_h 1

//## begin module%37010B8E0115.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%37010B8E0115.additionalIncludes

//## begin module%37010B8E0115.includes preserve=yes
//## end module%37010B8E0115.includes

//## begin module%37010B8E0115.additionalDeclarations preserve=yes
//## end module%37010B8E0115.additionalDeclarations


//## begin PositionType%37010B8E0115.preface preserve=yes
//## end PositionType%37010B8E0115.preface

//## Class: PositionType%37010B8E0115
//	********************************************************************************
//
//	enum  {After, Before}
//
//	********************************************************************************
//## Category: SimulationManagement%369E33A90107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum  {After, Before} PositionType;

//## begin PositionType%37010B8E0115.postscript preserve=yes
//## end PositionType%37010B8E0115.postscript

//## begin module%37010B8E0115.epilog preserve=yes
//## end module%37010B8E0115.epilog


#endif
