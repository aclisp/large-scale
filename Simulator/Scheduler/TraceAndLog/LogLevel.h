//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36C155970071.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36C155970071.cm

//## begin module%36C155970071.cp preserve=yes
//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: Trace and Log
//
//	File Name: LogLevel.h
//
//	Description:	Enumerate type that defines the level of log information.
//
//	Author:	  Eugenio Pacheli Alves
//
//	Creation Date:	jun/24/99
//
//	Visual Source Safe $Revision: 2 $
//
//	Last check in $Date: 29/06/99 15:55 $
//
//	********************************************************************************
//## end module%36C155970071.cp

//## Module: LogLevel%36C155970071; Pseudo Package specification
//## Source file: T:\TraceAndLog\LogLevel.h

#ifndef LogLevel_h
#define LogLevel_h 1

//## begin module%36C155970071.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36C155970071.additionalIncludes

//## begin module%36C155970071.includes preserve=yes
//## end module%36C155970071.includes

//## begin module%36C155970071.additionalDeclarations preserve=yes
  	 
using namespace std;

namespace sch {



//## end module%36C155970071.additionalDeclarations


//## begin LogLevel%36C155970071.preface preserve=yes
//## end LogLevel%36C155970071.preface

//## Class: LogLevel%36C155970071
//	********************************************************************************
//
//	enum {Informative, Warning, Error}
//
//	********************************************************************************
//## Category: TraceAndLog%369E33F80313
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef enum {Informative, Warning, Error} LogLevel;

//## begin LogLevel%36C155970071.postscript preserve=yes
//## end LogLevel%36C155970071.postscript

//## begin module%36C155970071.epilog preserve=yes
    
} // namespace sch


//## end module%36C155970071.epilog


#endif
