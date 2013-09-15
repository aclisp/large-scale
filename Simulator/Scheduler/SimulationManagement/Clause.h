//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36B1EFD90022.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36B1EFD90022.cm

//## begin module%36B1EFD90022.cp preserve=yes
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
//	File Name: Clause.h
//
//	Description:
//
//	Author:	Hélio Azevedo
//
//	Creation Date:
//
//	Visual Source Safe $Revision: 7 $
//
//	Last check in $Date: 10/04/99 2:46p $
//
//	********************************************************************************
//## end module%36B1EFD90022.cp

//## Module: Clause%36B1EFD90022; Pseudo Package specification
//## Source file: T:\SimulationManagement\Clause.h

#ifndef Clause_h
#define Clause_h 1

//## begin module%36B1EFD90022.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36B1EFD90022.additionalIncludes

//## begin module%36B1EFD90022.includes preserve=yes
//## end module%36B1EFD90022.includes

// PositionType
#include "PositionType.h"
// ClauseType
#include "ClauseType.h"
//## begin module%36B1EFD90022.additionalDeclarations preserve=yes
namespace sch  {
	class SimProcess;			// forward reference to SimProcess
//## end module%36B1EFD90022.additionalDeclarations


//## begin Clause%36B1EFD90022.preface preserve=yes
//## end Clause%36B1EFD90022.preface

//## Class: Clause%36B1EFD90022
//	********************************************************************************
//
//	This class is a struct that holds parameters used for positioning a process in the Event List.
//
//	********************************************************************************
//## Category: SimulationManagement%369E33A90107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%36FF929901E6;SimProcess { -> F}
//## Uses: <unnamed>%370119D4001F;ClauseType { -> }
//## Uses: <unnamed>%370119D80101;PositionType { -> }

class Clause 
{
  //## begin Clause%36B1EFD90022.initialDeclarations preserve=yes
  //## end Clause%36B1EFD90022.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: setClause%37038661039B
      void setClause (ClauseType type, SimProcess *pprocess, Double time, PositionType positionDef = After);

    // Data Members for Class Attributes

      //## Attribute: clauseType%37010FB20097
      //	Determines the clause type. The clause can use a reference process (RefProcess), a  activation time (ActivationTime) or a delay
      //	time (DelayTime) to determine the insertion point.
      //## begin Clause::clauseType%37010FB20097.attr preserve=no  public: ClauseType {UA} 
      ClauseType clauseType;
      //## end Clause::clauseType%37010FB20097.attr

      //## Attribute: pReferenceProcess%36B1F0E8022A
      //	Pointer to the process in the Event List used as reference for the insertion of the target process.
      //
      //	This atribute is only valid if the clauseType is equal a RefProcess.
      //## begin Clause::pReferenceProcess%36B1F0E8022A.attr preserve=no  public: SimProcess * {UA} 
      SimProcess *pReferenceProcess;
      //## end Clause::pReferenceProcess%36B1F0E8022A.attr

      //## Attribute: simulationTime%36B1F16B0305
      //	Time used as a reference for the insertion of the target process in the Event List. It may be either the activation time itself or
      //	the delay from the system simulation time.
      //
      //	This atribute is only valid if the clauseType is either ActivationTime or DelayTime.
      //## begin Clause::simulationTime%36B1F16B0305.attr preserve=no  public: Double {UA} 
      Double simulationTime;
      //## end Clause::simulationTime%36B1F16B0305.attr

      //## Attribute: position%36B1F23C008F
      //	Indicates whether the target process is inserted before (Before) or after (After) the reference insertion point, which is
      //	determined by:
      //	- the reference process or
      //	- set of processes with the same simulation time. In this case position indicates whether the target process is inserted at the
      //	beginning or end of the set.
      //## begin Clause::position%36B1F23C008F.attr preserve=no  public: PositionType {UA} 
      PositionType position;
      //## end Clause::position%36B1F23C008F.attr

    // Additional Public Declarations
      //## begin Clause%36B1EFD90022.public preserve=yes
      //## end Clause%36B1EFD90022.public

  protected:
    // Additional Protected Declarations
      //## begin Clause%36B1EFD90022.protected preserve=yes
      //## end Clause%36B1EFD90022.protected

  private:
    // Additional Private Declarations
      //## begin Clause%36B1EFD90022.private preserve=yes
      //## end Clause%36B1EFD90022.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin Clause%36B1EFD90022.implementation preserve=yes
      //## end Clause%36B1EFD90022.implementation

};

//## begin Clause%36B1EFD90022.postscript preserve=yes
//## end Clause%36B1EFD90022.postscript

// Class Clause 


//## Other Operations (inline)
//## Operation: setClause%37038661039B
//	********************************************************************************
//
//	Name: RetCode setClause(ClauseType type, SimProcess * pprocess, Double time, PositionType positionDef)
//	Description: Defines a insertion clause.
//
//	Input parameters:
//	ClauseType type                        // Determines the clause type SimProcess * pprocess           // Pointer to process
//	Double time                                  // Time used as reference to insertion
//	PositionType position               // Insertion position
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	********************************************************************************
inline void Clause::setClause (ClauseType type, SimProcess *pprocess, Double time, PositionType positionDef)
{
  //## begin Clause::setClause%37038661039B.body preserve=yes
	clauseType = type;
	pReferenceProcess = pprocess;
	simulationTime = time;
	position = positionDef;
  //## end Clause::setClause%37038661039B.body
}

// Class Clause 





// Additional Declarations
  //## begin Clause%36B1EFD90022.declarations preserve=yes
  //## end Clause%36B1EFD90022.declarations

//## begin module%36B1EFD90022.epilog preserve=yes
}	// namespace sch

//## end module%36B1EFD90022.epilog


#endif
