//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%373186A80310.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%373186A80310.cm

//## begin module%373186A80310.cp preserve=yes
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
//	File Name: SingleSimulationManager.cpp
//
//	Description:
//
//	Author:	Hélio Azevedo
//
//	Creation Date: May/06/99
//
//	Visual Source Safe $Revision: 2 $
//
//	Last check in $Date: 13/05/99 9:25 $
//
//	********************************************************************************
//## end module%373186A80310.cp

//## Module: SingleSimulationManager%373186A80310; Pseudo Package body
//## Source file: T:\SimulationManagement\SingleSimulationManager.cpp

//## begin module%373186A80310.additionalIncludes preserve=no
//## end module%373186A80310.additionalIncludes

//## begin module%373186A80310.includes preserve=yes
//## end module%373186A80310.includes
#include "MemLeak.h"
// SingleSimulationManager
#include "SingleSimulationManager.h"
//## begin module%373186A80310.additionalDeclarations preserve=yes
namespace sch  {
//## end module%373186A80310.additionalDeclarations


// Class SingleSimulationManager 

//## begin SingleSimulationManager::pInstance%37318A5400F5.attr preserve=no  private: static SingleSimulationManager * {UA} NULL
SingleSimulationManager *SingleSimulationManager::pInstance = NULL;
//## end SingleSimulationManager::pInstance%37318A5400F5.attr

// Additional Declarations
  //## begin SingleSimulationManager%373186A80310.declarations preserve=yes
  //## end SingleSimulationManager%373186A80310.declarations

//## begin module%373186A80310.epilog preserve=yes
}	// namespace sch
//## end module%373186A80310.epilog
