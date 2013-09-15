//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%379CA42802FE.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%379CA42802FE.cm

//## begin module%379CA42802FE.cp preserve=yes
//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: Statistics Calculation
//
//	File Name: HighPrecisionWinAvg.h
//
//	Description: Header file for the HighPrecisionWinAvg class.
//
//	Author: Eugenio Pacheli Alves
//
//	Creation Date: Jul/26/99
//
//	Visual Source Safe $Revision: 1 $
//
//	Last check in $Date: 26/07/99 16:31 $
//
//	********************************************************************************
//## end module%379CA42802FE.cp

//## Module: HighPrecisionWinAvg%379CA42802FE; Pseudo Package specification
//## Source file: T:\StatisticsCalculation\HighPrecisionWinAvg.h

#ifndef HighPrecisionWinAvg_h
#define HighPrecisionWinAvg_h 1

//## begin module%379CA42802FE.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%379CA42802FE.additionalIncludes

//## begin module%379CA42802FE.includes preserve=yes
//## end module%379CA42802FE.includes

//## begin module%379CA42802FE.additionalDeclarations preserve=yes

using namespace std;

namespace sch {

//## end module%379CA42802FE.additionalDeclarations


//## begin HighPrecisionWinAvg%379CA42802FE.preface preserve=yes
//## end HighPrecisionWinAvg%379CA42802FE.preface

//## Class: HighPrecisionWinAvg%379CA42802FE
//	********************************************************************************
//
//	This class supplies a set of operations to calculate the windowed average of a subset of a sequence of values (last n samples). It
//	has the same functionality as the WindowedAverage class, except maintaining a Mean class. The WindowedAverage class stores "float"
//	values, whereas the HighPrecisionWinAvg class stores "double" values.
//
//	The windowed average is calculated from the last n samples of the sequence. Such samples are stored in a fixed-size
//	dynamically-allocated array.
//
//	********************************************************************************
//## Category: StatisticsCalculation%369E340F008B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class HighPrecisionWinAvg 
{
  //## begin HighPrecisionWinAvg%379CA42802FE.initialDeclarations preserve=yes
  //## end HighPrecisionWinAvg%379CA42802FE.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: HighPrecisionWinAvg%379CA63D020A
      HighPrecisionWinAvg (Int winSize);

    //## Destructor (specified)
      //## Operation: ~HighPrecisionWinAvg%379CA63D0232
      virtual ~HighPrecisionWinAvg ();


    //## Other Operations (specified)
      //## Operation: getWindowedAverage%379CA63D025A
      Double getWindowedAverage ();

      //## Operation: nextSample%379CA63D0282
      virtual RetCode nextSample (Double sample);

      //## Operation: reset%379CA63D02AA
      RetCode reset ();

    // Additional Public Declarations
      //## begin HighPrecisionWinAvg%379CA42802FE.public preserve=yes
      //## end HighPrecisionWinAvg%379CA42802FE.public

  protected:
    // Additional Protected Declarations
      //## begin HighPrecisionWinAvg%379CA42802FE.protected preserve=yes
      //## end HighPrecisionWinAvg%379CA42802FE.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: headWA%379CA64502DD
      //	Index of next position to insert a sample in the pLastSamples array. Since such array is a circular buffer, headWA ranges from 0 to
      //	(windowSize - 1).
      //## begin HighPrecisionWinAvg::headWA%379CA64502DD.attr preserve=no  private: Int {UA} 0
      Int headWA;
      //## end HighPrecisionWinAvg::headWA%379CA64502DD.attr

      //## Attribute: numWA%379CA6450305
      //	Current number of elements inserted in pLastSamples array. Since such array is a circular buffer, numWA grows until the first window
      //	Size elements are inserted in the array. From this point on, its value does not change and is equal to windowSize.
      //## begin HighPrecisionWinAvg::numWA%379CA6450305.attr preserve=no  private: Int {UA} 0
      Int numWA;
      //## end HighPrecisionWinAvg::numWA%379CA6450305.attr

      //## Attribute: pLastSamples%379CA6450338
      //	Last n samples of this sequence (n = windowSize attribute). This is a FIFO queue; a new sample supplied thru nextSample() is
      //	inserted at the end of the queue. Actually, this is a circular buffer, so after the first n elements are inserted, all subsequent
      //	ones replace the oldest.
      //## begin HighPrecisionWinAvg::pLastSamples%379CA6450338.attr preserve=no  private: Double * {UA} NULL
      Double *pLastSamples;
      //## end HighPrecisionWinAvg::pLastSamples%379CA6450338.attr

      //## Attribute: windowSize%379CA6450360
      //	Number of samples (window) for calculating the windowed average of the sequence. It must be non-zero.
      //## begin HighPrecisionWinAvg::windowSize%379CA6450360.attr preserve=no  private: Int {UAC} winSize
      const Int windowSize;
      //## end HighPrecisionWinAvg::windowSize%379CA6450360.attr

    // Additional Private Declarations
      //## begin HighPrecisionWinAvg%379CA42802FE.private preserve=yes
      //## end HighPrecisionWinAvg%379CA42802FE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin HighPrecisionWinAvg%379CA42802FE.implementation preserve=yes
      //## end HighPrecisionWinAvg%379CA42802FE.implementation

};

//## begin HighPrecisionWinAvg%379CA42802FE.postscript preserve=yes
//## end HighPrecisionWinAvg%379CA42802FE.postscript

// Class HighPrecisionWinAvg 

//## begin module%379CA42802FE.epilog preserve=yes

} // namespace sch

//## end module%379CA42802FE.epilog


#endif
