//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%371F1315000F.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%371F1315000F.cm

//## begin module%371F1315000F.cp preserve=yes
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
//	File Name: Statistics.h
//
//	Description:
//
//	Author:	 Hélio Azevedo
//
//	Creation Date:	03/99
//
//	Visual Source Safe $Revision: 3 $
//
//	Last check in $Date: 13/05/99 9:25 $
//
//	********************************************************************************
//## end module%371F1315000F.cp

//## Module: Statistics%371F1315000F; Pseudo Package specification
//## Source file: T:\SimulationManagement\Statistics.h

#ifndef Statistics_h
#define Statistics_h 1

//## begin module%371F1315000F.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%371F1315000F.additionalIncludes

//## begin module%371F1315000F.includes preserve=yes
//## end module%371F1315000F.includes

//## begin module%371F1315000F.additionalDeclarations preserve=yes
namespace sch  {


//## end module%371F1315000F.additionalDeclarations


//## Class: Statistics%371F1315000F
//	********************************************************************************
//
//	This class is a struct that holds statistics associated with a Simulation application.
//
//	********************************************************************************
//## Category: SimulationManagement%369E33A90107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class Statistics 
{
  public:
    // Data Members for Class Attributes

      //## Attribute: timeElapsed%371F1B9D00B9
      //	Stores the total processing time spended by the simulation application.
      //## begin Statistics::timeElapsed%371F1B9D00B9.attr preserve=no  public: Int64 {UA} 
      Int64 timeElapsed;
      //## end Statistics::timeElapsed%371F1B9D00B9.attr

      //## Attribute: totalProcessCreated%371F1BC60072
      //	Maintain the number of process created until the present moment.
      //## begin Statistics::totalProcessCreated%371F1BC60072.attr preserve=no  public: Ulong {UA} 
      Ulong totalProcessCreated;
      //## end Statistics::totalProcessCreated%371F1BC60072.attr

      //## Attribute: totalProcessTerm%371F1C0C024A
      //	Maintain the number of process created until the present moment.
      //## begin Statistics::totalProcessTerm%371F1C0C024A.attr preserve=no  public: Ulong {UA} 
      Ulong totalProcessTerm;
      //## end Statistics::totalProcessTerm%371F1C0C024A.attr

      //## Attribute: fatalError%372739C1004C
      //	This attribute maintains a scheduler  internal error that can not be reported through trivial return function. The presence of a
      //	error signals that the scheduler execution was aborted.
      //## begin Statistics::fatalError%372739C1004C.attr preserve=no  public: RetCode {UA} 
      RetCode fatalError;
      //## end Statistics::fatalError%372739C1004C.attr

      //## Attribute: simulationTime%37285EF20143
      //	Indicates the current system simulation time.
      //## begin Statistics::simulationTime%37285EF20143.attr preserve=no  public: Double {UA} 
      Double simulationTime;
      //## end Statistics::simulationTime%37285EF20143.attr

      //## Attribute: running%37285F48013C
      //	Indicates if the simulation is still running.
      //## begin Statistics::running%37285F48013C.attr preserve=no  public: bool {UA} 
      bool running;
      //## end Statistics::running%37285F48013C.attr

  protected:
  private:
  private: //## implementation
};

// Class Statistics 

// Class Statistics 







//## begin module%371F1315000F.epilog preserve=yes
}		// end of namespace


//## end module%371F1315000F.epilog


#endif
