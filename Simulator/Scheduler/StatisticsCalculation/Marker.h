//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%3715FEFF0033.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3715FEFF0033.cm

//## begin module%3715FEFF0033.cp preserve=no
//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: StatisticsCalculation
//
//	File Name: Marker.h
//
//	Description:
//
//	Author:
//
//	Creation Date:
//
//	Visual Source Safe $Revision: $
//
//	Last check in $Date: $
//
//	********************************************************************************
//## end module%3715FEFF0033.cp

//## Module: Marker%3715FEFF0033; Pseudo Package specification
//## Source file: T:\StatisticsCalculation\Marker.h

#ifndef Marker_h
#define Marker_h 1

//## begin module%3715FEFF0033.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%3715FEFF0033.additionalIncludes

//## begin module%3715FEFF0033.includes preserve=yes
//## end module%3715FEFF0033.includes

//## begin module%3715FEFF0033.additionalDeclarations preserve=yes
//## end module%3715FEFF0033.additionalDeclarations


//## begin MarkerStruc%3715FEFF0033.preface preserve=yes
//## end MarkerStruc%3715FEFF0033.preface

//## Class: Marker%3715FEFF0033
//	********************************************************************************
//
//	This structure holds all parameters required for each marker in the iterative computation of the p-quantile.
//
//	Each marker has a location value, which is updated as new samples come in. Actual computed values are different from the ideal
//	values though since the locations are integer values whereas the ideal locations are real values. The ideal locations are computed
//	and stored in the desiredPos attribute.
//
//	********************************************************************************
//## Category: StatisticsCalculation%369E340F008B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

struct MarkerStruc 
{
  //## begin MarkerStruc%3715FEFF0033.initialDeclarations preserve=yes
  //## end MarkerStruc%3715FEFF0033.initialDeclarations

    // Data Members for Class Attributes

      //## Attribute: position%3715FF1201FD
      //	Horizontal marker position, recalculated after each new sample.
      //## begin MarkerStruc::position%3715FF1201FD.attr preserve=no  public: Int {UA} 
      Int position;
      //## end MarkerStruc::position%3715FF1201FD.attr

      //## Attribute: desiredPos%3715FF250255
      //	Ideal horizontal marker location, recalculated after each new sample.
      //## begin MarkerStruc::desiredPos%3715FF250255.attr preserve=no  public: Double {UA} 
      Double desiredPos;
      //## end MarkerStruc::desiredPos%3715FF250255.attr

      //## Attribute: height%3715FF2E01CC
      //	Vertical marker value, recalculated after each new sample.
      //## begin MarkerStruc::height%3715FF2E01CC.attr preserve=no  public: Double {UA} 
      Double height;
      //## end MarkerStruc::height%3715FF2E01CC.attr

      //## Attribute: incDesiredPos%3715FF3A01AB
      //	Increment in the desired marker position, a constant value based on p. Each desired position is incremented after each new sample
      //	by adding its previous contents to this increment.
      //## begin MarkerStruc::incDesiredPos%3715FF3A01AB.attr preserve=no  public: Double {UA} 
      Double incDesiredPos;
      //## end MarkerStruc::incDesiredPos%3715FF3A01AB.attr

  public:
    // Additional Public Declarations
      //## begin MarkerStruc%3715FEFF0033.public preserve=yes
      //## end MarkerStruc%3715FEFF0033.public

  protected:
    // Additional Protected Declarations
      //## begin MarkerStruc%3715FEFF0033.protected preserve=yes
      //## end MarkerStruc%3715FEFF0033.protected

  private:
    // Additional Private Declarations
      //## begin MarkerStruc%3715FEFF0033.private preserve=yes
      //## end MarkerStruc%3715FEFF0033.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin MarkerStruc%3715FEFF0033.implementation preserve=yes
      //## end MarkerStruc%3715FEFF0033.implementation

};

//## begin MarkerStruc%3715FEFF0033.postscript preserve=yes

typedef struct MarkerStruc Marker;

//## end MarkerStruc%3715FEFF0033.postscript

// Class MarkerStruc 

// Class MarkerStruc 







// Additional Declarations
  //## begin MarkerStruc%3715FEFF0033.declarations preserve=yes
  //## end MarkerStruc%3715FEFF0033.declarations

//## begin module%3715FEFF0033.epilog preserve=yes
//## end module%3715FEFF0033.epilog


#endif
