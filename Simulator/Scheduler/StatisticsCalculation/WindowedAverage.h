//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%370E56FC0001.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%370E56FC0001.cm

//## begin module%370E56FC0001.cp preserve=yes
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
//	File Name: WindowedAverage.h
//
//	Description: Header file for the WindowedAverage class.
//
//	Author: Eugenio Pacheli Alves
//
//	Creation Date: Apr/12/99
//
//	Visual Source Safe $Revision: 4 $
//
//	Last check in $Date: 16/06/99 9:30 $
//
//	********************************************************************************
//## end module%370E56FC0001.cp

//## Module: WindowedAverage%370E56FC0001; Pseudo Package specification
//## Source file: T:\StatisticsCalculation\WindowedAverage.h

#ifndef WindowedAverage_h
#define WindowedAverage_h 1

//## begin module%370E56FC0001.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%370E56FC0001.additionalIncludes

//## begin module%370E56FC0001.includes preserve=yes
//## end module%370E56FC0001.includes

// Mean
#include "Mean.h"
//## begin module%370E56FC0001.additionalDeclarations preserve=yes

using namespace std;

namespace sch {

//## end module%370E56FC0001.additionalDeclarations


//## begin WindowedAverage%370E56FC0001.preface preserve=yes
//## end WindowedAverage%370E56FC0001.preface

//## Class: WindowedAverage%370E56FC0001
//	********************************************************************************
//
//	This class supplies a set of operations to calculate the windowed average of a subset of a sequence of values (last n samples). In
//	addition it maintains a Mean class for the complete set of values in the sequence. Anyone that needs both mean and windowed average
//	may instantiate only an WindowedAverage object and then use its associated mean.
//
//	The windowed average is calculated from the last n samples of the sequence. Such samples are stored in a fixed-size
//	dynamically-allocated array. The nextSample() operation
//
//	********************************************************************************
//## Category: StatisticsCalculation%369E340F008B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class WindowedAverage 
{
  //## begin WindowedAverage%370E56FC0001.initialDeclarations preserve=yes
  //## end WindowedAverage%370E56FC0001.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: WindowedAverage%370E60FC01EC
      WindowedAverage (Int winSize);

    //## Destructor (specified)
      //## Operation: ~WindowedAverage%370E6108015E
      virtual ~WindowedAverage ();


    //## Other Operations (specified)
      //## Operation: getWindowedAverage%370E5768009C
      Float getWindowedAverage ();

      //## Operation: nextSample%370E58D40190
      virtual RetCode nextSample (Float sample);

      //## Operation: reset%374C034D035D
      RetCode reset ();

    // Data Members for Associations

      //## Association: StatisticsCalculation::<unnamed>%370E5B8B0334
      //## Role: WindowedAverage::pMean%370E5B8C01EB
      //	WindowedAverage maintains the non-robust mean for calculating the iterative mean of all values in the sequence.
      //## begin WindowedAverage::pMean%370E5B8C01EB.role preserve=no  public: Mean {1 -> 1RHgAN}
      Mean *pMean;
      //## end WindowedAverage::pMean%370E5B8C01EB.role

    // Additional Public Declarations
      //## begin WindowedAverage%370E56FC0001.public preserve=yes
      //## end WindowedAverage%370E56FC0001.public

  protected:
    // Additional Protected Declarations
      //## begin WindowedAverage%370E56FC0001.protected preserve=yes
      //## end WindowedAverage%370E56FC0001.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: headWA%370E587E038B
      //	Index of next position to insert a sample in the pLastSamples array. Since such array is a circular buffer, headWA ranges from 0 to
      //	(windowSize - 1).
      //## begin WindowedAverage::headWA%370E587E038B.attr preserve=no  private: Int {UA} 0
      Int headWA;
      //## end WindowedAverage::headWA%370E587E038B.attr

      //## Attribute: numWA%370E587E03B3
      //	Current number of elements inserted in pLastSamples array. Since such array is a circular buffer, numWA grows until the first window
      //	Size elements are inserted in the array. From this point on, its value does not change and is equal to windowSize.
      //## begin WindowedAverage::numWA%370E587E03B3.attr preserve=no  private: Int {UA} 0
      Int numWA;
      //## end WindowedAverage::numWA%370E587E03B3.attr

      //## Attribute: pLastSamples%370E587E03DB
      //	Last n samples of this sequence (n = windowSize attribute). This is a FIFO queue; a new sample supplied thru nextSample() is
      //	inserted at the end of the queue. Actually, this is a circular buffer, so after the first n elements are inserted, all subsequent
      //	ones replace the oldest.
      //## begin WindowedAverage::pLastSamples%370E587E03DB.attr preserve=no  private: Float * {UA} NULL
      Float *pLastSamples;
      //## end WindowedAverage::pLastSamples%370E587E03DB.attr

      //## Attribute: windowSize%370E587F001B
      //	Number of samples (window) for calculating the windowed average of the sequence. It must be non-zero.
      //## begin WindowedAverage::windowSize%370E587F001B.attr preserve=no  private: Int {UAC} winSize
      const Int windowSize;
      //## end WindowedAverage::windowSize%370E587F001B.attr

    // Additional Private Declarations
      //## begin WindowedAverage%370E56FC0001.private preserve=yes
      //## end WindowedAverage%370E56FC0001.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin WindowedAverage%370E56FC0001.implementation preserve=yes
      //## end WindowedAverage%370E56FC0001.implementation

};

//## begin WindowedAverage%370E56FC0001.postscript preserve=yes
//## end WindowedAverage%370E56FC0001.postscript

// Class WindowedAverage 

//## Operation: ~WindowedAverage%370E6108015E
//	********************************************************************************
//
//	Name: ~WindowedAverage(void)
//
//	Description: Destructor - deallocates pLastSamples array and pMean object.
//
//	Parameters: none
//
//	Returns: none
//
//	Remarks: this destructor is virtual since the WindowedAverage class may be base for other classes.
//
//	********************************************************************************
inline WindowedAverage::~WindowedAverage ()
{
  //## begin WindowedAverage::~WindowedAverage%370E6108015E.body preserve=yes

	delete pMean;			  // delete object
	delete []pLastSamples;   // delete array

  //## end WindowedAverage::~WindowedAverage%370E6108015E.body
}


//## begin module%370E56FC0001.epilog preserve=yes

} // namespace sch

//## end module%370E56FC0001.epilog


#endif
