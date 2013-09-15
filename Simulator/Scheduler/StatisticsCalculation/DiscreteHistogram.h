//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36B6DCD603AD.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36B6DCD603AD.cm

//## begin module%36B6DCD603AD.cp preserve=yes
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
//	File Name: DiscreteHistogram.h
//
//	Description: Header file for the DiscreteHistogram class.
//
//	Author: Eugenio Pacheli Alves
//
//	Creation Date: Apr/12/99
//
//	Visual Source Safe $Revision: 6 $
//
//	Last check in $Date: 16/06/99 9:30 $
//
//	********************************************************************************
//## end module%36B6DCD603AD.cp

//## Module: DiscreteHistogram%36B6DCD603AD; Pseudo Package specification
//## Source file: T:\StatisticsCalculation\DiscreteHistogram.h

#ifndef DiscreteHistogram_h
#define DiscreteHistogram_h 1

//## begin module%36B6DCD603AD.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36B6DCD603AD.additionalIncludes

//## begin module%36B6DCD603AD.includes preserve=yes
#include <math.h>
//## end module%36B6DCD603AD.includes

//## begin module%36B6DCD603AD.additionalDeclarations preserve=yes

using namespace std;

namespace sch {

//## end module%36B6DCD603AD.additionalDeclarations


//## begin DiscreteHistogram%36B6DCD603AD.preface preserve=yes
//## end DiscreteHistogram%36B6DCD603AD.preface

//## Class: DiscreteHistogram%36B6DCD603AD
//	********************************************************************************
//
//	This class supplies a set of operations to calculate the histogram of a sequence of discrete samples. The histogram shows the
//	frequency of samples within a range (the histogram range) which is divided into several adjacent intervals (all having the same
//	width). Each interval accounts the occurence of samples that fall into its range (the interval range). Should a sample fall outside
//	the histogram range, it is accounted in the nearest interval (that is, samples below the lower boundary are accounted in the lower
//	interval, and samples above the higher boundary are accounted in the higher interval).
//
//	********************************************************************************
//## Category: StatisticsCalculation%369E340F008B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class DiscreteHistogram 
{
  //## begin DiscreteHistogram%36B6DCD603AD.initialDeclarations preserve=yes
  //## end DiscreteHistogram%36B6DCD603AD.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: DiscreteHistogram%36B74612038D
      //## Preconditions:
      //	MOD ( (max - min) /NumInterval) != 0    => error
      DiscreteHistogram (Int minVal, Int maxVal, Int intervals);

    //## Destructor (specified)
      //## Operation: ~DiscreteHistogram%36DFECDF01A9
      virtual ~DiscreteHistogram ();


    //## Other Operations (specified)
      //## Operation: getIntervalWidth%36B746620111
      Int getIntervalWidth ();

      //## Operation: getNumIntervals%36B746750064
      Int getNumIntervals ();

      //## Operation: getResult%36B7468000CE
      RetCode getResult (Int intervalNumber, Int *pResult);

      //## Operation: nextSample%36B74653034A
      RetCode nextSample (Int sample);

      //## Operation: getMinValue%37459F5001A9
      Int getMinValue ();

      //## Operation: getMaxValue%37459F5001DB
      Int getMaxValue ();

      //## Operation: getMinSample%37459F500203
      Int getMinSample ();

      //## Operation: getMaxSample%37459F50022C
      Int getMaxSample ();

      //## Operation: getNumSamples%37459FF701A9
      Int getNumSamples ();

      //## Operation: reset%374C032A0262
      RetCode reset ();

    // Additional Public Declarations
      //## begin DiscreteHistogram%36B6DCD603AD.public preserve=yes
      //## end DiscreteHistogram%36B6DCD603AD.public

  protected:
    // Additional Protected Declarations
      //## begin DiscreteHistogram%36B6DCD603AD.protected preserve=yes
      //## end DiscreteHistogram%36B6DCD603AD.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: intervalWidth%36B74563034F
      //	Width of each interval. This value is calculated using the minValue, maxValue and numIntervals.
      //## begin DiscreteHistogram::intervalWidth%36B74563034F.attr preserve=no  private: Int {UA} 
      Int intervalWidth;
      //## end DiscreteHistogram::intervalWidth%36B74563034F.attr

      //## Attribute: maxValue%36B745610067
      //	Upper boundary of histogram. All values above maxValue are accounted in the higher interval.
      //## begin DiscreteHistogram::maxValue%36B745610067.attr preserve=no  private: Int {UAC} maxVal
      const Int maxValue;
      //## end DiscreteHistogram::maxValue%36B745610067.attr

      //## Attribute: minValue%36B7455D0288
      //	Lower boundary of histogram. All values bellow minValue are accounted in the lower interval.
      //## begin DiscreteHistogram::minValue%36B7455D0288.attr preserve=no  private: Int {UAC} minVal
      const Int minValue;
      //## end DiscreteHistogram::minValue%36B7455D0288.attr

      //## Attribute: numIntervals%36B7454B0305
      //	Number of intervals in the histogram. This value is set in the constructor.
      //## begin DiscreteHistogram::numIntervals%36B7454B0305.attr preserve=no  private: Int {UAC} intervals
      const Int numIntervals;
      //## end DiscreteHistogram::numIntervals%36B7454B0305.attr

      //## Attribute: pValues%36B74513019C
      //	Integer array that stores the frequency of each interval in the histogram. Its size is dynamically set to the number of intervals
      //	and is allocated in the constructor. Each element is initially set to zero.
      //## begin DiscreteHistogram::pValues%36B74513019C.attr preserve=no  private: Int * {UA} NULL
      Int *pValues;
      //## end DiscreteHistogram::pValues%36B74513019C.attr

      //## Attribute: maxSample%37459F020107
      //	Maximum value of the sequence. This value is calculated and updated each time nextSample() is called.
      //## begin DiscreteHistogram::maxSample%37459F020107.attr preserve=no  private: Int {UA} MAX_VALUE_INT
      Int maxSample;
      //## end DiscreteHistogram::maxSample%37459F020107.attr

      //## Attribute: minSample%37459F020139
      //	Minimum value of the sequence. This value is calculated and updated each time nextSample() is called.
      //## begin DiscreteHistogram::minSample%37459F020139.attr preserve=no  private: Int {UA} MIN_VALUE_INT
      Int minSample;
      //## end DiscreteHistogram::minSample%37459F020139.attr

      //## Attribute: numSamples%37459F020161
      //	Total number of samples in the sequence. This value is incremented each time nextSample() is called.
      //## begin DiscreteHistogram::numSamples%37459F020161.attr preserve=no  private: Int {UA} 0
      Int numSamples;
      //## end DiscreteHistogram::numSamples%37459F020161.attr

    // Additional Private Declarations
      //## begin DiscreteHistogram%36B6DCD603AD.private preserve=yes
      //## end DiscreteHistogram%36B6DCD603AD.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin DiscreteHistogram%36B6DCD603AD.implementation preserve=yes
      //## end DiscreteHistogram%36B6DCD603AD.implementation

};

//## begin DiscreteHistogram%36B6DCD603AD.postscript preserve=yes
//## end DiscreteHistogram%36B6DCD603AD.postscript

// Class DiscreteHistogram 

//## Operation: ~DiscreteHistogram%36DFECDF01A9
//	********************************************************************************
//
//	Name: ~DiscreteHistogram(void)
//
//	Description: Destructor - deallocates pValues array.
//
//	Parameters: none
//
//	Returns: none
//
//	Remarks: this destructor is virtual since the DiscreteHistogram class may be base for other classes.
//
//	********************************************************************************
inline DiscreteHistogram::~DiscreteHistogram ()
{
  //## begin DiscreteHistogram::~DiscreteHistogram%36DFECDF01A9.body preserve=yes

	delete [] pValues;        // delete array

  //## end DiscreteHistogram::~DiscreteHistogram%36DFECDF01A9.body
}



//## Other Operations (inline)
//## Operation: getIntervalWidth%36B746620111
//	********************************************************************************
//
//	Name: Int getIntervalWidth (void)
//
//	Description: Returns the width of each interval.
//
//	Parameters: none
//
//	Returns: the value of the intervalWidth attribute.
//
//	********************************************************************************
inline Int DiscreteHistogram::getIntervalWidth ()
{
  //## begin DiscreteHistogram::getIntervalWidth%36B746620111.body preserve=yes

	return (intervalWidth);

  //## end DiscreteHistogram::getIntervalWidth%36B746620111.body
}

//## Operation: getNumIntervals%36B746750064
//	********************************************************************************
//
//	Name: Int getNumIntervals (void)
//
//	Description: Returns the number of intervals of the histogram.
//
//	Parameters: none
//
//	Returns: the value of the numIntervals attribute.
//
//	********************************************************************************
inline Int DiscreteHistogram::getNumIntervals ()
{
  //## begin DiscreteHistogram::getNumIntervals%36B746750064.body preserve=yes

	return (numIntervals);

  //## end DiscreteHistogram::getNumIntervals%36B746750064.body
}

//## Operation: getMinValue%37459F5001A9
//	********************************************************************************
//
//	Name: Float  getMinValue (void)
//
//	Description: Returns the lower boundary of the input stream data suplied by the user at  the construction time.
//	Parameters: none
//
//	Returns: the value of the minValue attribute.
//
//	********************************************************************************
inline Int DiscreteHistogram::getMinValue ()
{
  //## begin DiscreteHistogram::getMinValue%37459F5001A9.body preserve=yes
	return minValue;
  //## end DiscreteHistogram::getMinValue%37459F5001A9.body
}

//## Operation: getMaxValue%37459F5001DB
//	********************************************************************************
//
//	Name: Float  getMaxValue (void)
//
//	Description: Returns the upper boundary of the input stream data suplied by the user at  the construction time.
//	Parameters: none
//
//	Returns: the value of the maxValue attribute.
//
//	********************************************************************************
inline Int DiscreteHistogram::getMaxValue ()
{
  //## begin DiscreteHistogram::getMaxValue%37459F5001DB.body preserve=yes
	return maxValue;
  //## end DiscreteHistogram::getMaxValue%37459F5001DB.body
}

//## Operation: getMinSample%37459F500203
//	********************************************************************************
//
//	Name: Int getMinSample (void)
//
//	Description: Returns the current minimum value of the sequence .
//
//	Parameters: none
//
//	Returns: the minimum value of the sequence.
//	        MIN_VALUE_INT - Does not exist samples yet.
//
//
//
//	********************************************************************************
inline Int DiscreteHistogram::getMinSample ()
{
  //## begin DiscreteHistogram::getMinSample%37459F500203.body preserve=yes
	return minSample;
  //## end DiscreteHistogram::getMinSample%37459F500203.body
}

//## Operation: getMaxSample%37459F50022C
//	********************************************************************************
//
//	Name: Int getMinSample (void)
//
//	Description: Returns the current maximum value of the sequence .
//
//	Parameters: none
//
//	Returns: the maximum value of the sequence.
//	        MAX_VALUE_INT - Does not exist samples yet.
//	********************************************************************************
inline Int DiscreteHistogram::getMaxSample ()
{
  //## begin DiscreteHistogram::getMaxSample%37459F50022C.body preserve=yes
	return maxSample;
  //## end DiscreteHistogram::getMaxSample%37459F50022C.body
}

//## Operation: getNumSamples%37459FF701A9
//	********************************************************************************
//
//	Name: Int getNumSamples (void)
//
//	Description: Returns the number of samples.
//
//	Parameters: none
//
//	Returns: the value of the numSamples attribute.
//
//	********************************************************************************
inline Int DiscreteHistogram::getNumSamples ()
{
  //## begin DiscreteHistogram::getNumSamples%37459FF701A9.body preserve=yes
	return numSamples;
  //## end DiscreteHistogram::getNumSamples%37459FF701A9.body
}

//## begin module%36B6DCD603AD.epilog preserve=yes

} // namespace sch

//## end module%36B6DCD603AD.epilog


#endif
