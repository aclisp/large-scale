//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%3744298B0396.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3744298B0396.cm

//## begin module%3744298B0396.cp preserve=yes
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
//	File Name: ContinuousHistogram.h
//
//	Description: Header file for the ContinuousHistogram class
//
//	Author:	  Hélio Azevedo
//
//	Creation Date:	May/20/99
//
//	Visual Source Safe $Revision: 8 $
//
//	Last check in $Date: 16/06/99 9:29 $
//
//	********************************************************************************
//## end module%3744298B0396.cp

//## Module: ContinuousHistogram%3744298B0396; Pseudo Package specification
//## Source file: T:\StatisticsCalculation\ContinuousHistogram.h

#ifndef ContinuousHistogram_h
#define ContinuousHistogram_h 1

//## begin module%3744298B0396.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%3744298B0396.additionalIncludes

//## begin module%3744298B0396.includes preserve=yes
#include <math.h>
//## end module%3744298B0396.includes

//## begin module%3744298B0396.additionalDeclarations preserve=yes

using namespace std;

namespace sch {

//## end module%3744298B0396.additionalDeclarations


//## begin ContinuousHistogram%3744298B0396.preface preserve=yes
//## end ContinuousHistogram%3744298B0396.preface

//## Class: ContinuousHistogram%3744298B0396
//	********************************************************************************
//
//	This class supplies a set of operations to calculate the histogram of a sequence of continuous samples. The histogram shows the
//	frequency of samples within a range (the histogram range) which is divided into several adjacent intervals (all having the same
//	width). Each interval accounts the occurence of samples that fall into its range (the interval range). Should a sample fall outside
//	the histogram range, it is accounted in the nearest interval (that is, samples below the lower boundary are accounted in the lower
//	interval, and samples above the higher boundary are accounted in the higher interval).
//
//	********************************************************************************
//## Category: StatisticsCalculation%369E340F008B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ContinuousHistogram 
{
  //## begin ContinuousHistogram%3744298B0396.initialDeclarations preserve=yes
  //## end ContinuousHistogram%3744298B0396.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ContinuousHistogram%37442A26021C
      //## Preconditions:
      //	MOD ( (max - min) /NumInterval) != 0    => error
      ContinuousHistogram (Float minVal, Float maxVal, Int intervals);

    //## Destructor (specified)
      //## Operation: ~ContinuousHistogram%37442A2B0287
      ~ContinuousHistogram ();


    //## Other Operations (specified)
      //## Operation: getIntervalWidth%37442A3402E5
      Float getIntervalWidth ();

      //## Operation: getNumIntervals%37442A340317
      Int getNumIntervals ();

      //## Operation: getResult%37442A34033F
      RetCode getResult (Int intervalNumber, Int *pResult);

      //## Operation: nextSample%37442A34035D
      RetCode nextSample (Float sample);

      //## Operation: getMinValue%374598BE03A9
      Float getMinValue ();

      //## Operation: getMaxValue%37459AFE00AF
      Float getMaxValue ();

      //## Operation: getMinSample%374598C201B0
      Float getMinSample ();

      //## Operation: getMaxSample%37459C730390
      Float getMaxSample ();

      //## Operation: getNumSamples%3745A00C03DA
      Int getNumSamples ();

      //## Operation: reset%374C031B03A1
      RetCode reset ();

      //## Operation: getMean%3911CC980114
      Float getMean ();

      //## Operation: getStdDev%3911CC9A0009
      Float getStdDev ();

      //## Operation: getCumSum%3911D7A103BF
      Float getCumSum ();

      //## Operation: getCumSumOfSquares%3911D7AC0285
      Float getCumSumOfSquares ();

    // Additional Public Declarations
      //## begin ContinuousHistogram%3744298B0396.public preserve=yes
      //## end ContinuousHistogram%3744298B0396.public

  protected:
    // Additional Protected Declarations
      //## begin ContinuousHistogram%3744298B0396.protected preserve=yes
      //## end ContinuousHistogram%3744298B0396.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: intervalWidth%37442A5D0185
      //	Width of each interval. This value is calculated using the minValue, maxValue and numIntervals.
      //## begin ContinuousHistogram::intervalWidth%37442A5D0185.attr preserve=no  private: Float {UA} 
      Float intervalWidth;
      //## end ContinuousHistogram::intervalWidth%37442A5D0185.attr

      //## Attribute: maxValue%37442A5D01AD
      //	Upper boundary of histogram. All values above maxValue are accounted in the higher interval.
      //## begin ContinuousHistogram::maxValue%37442A5D01AD.attr preserve=no  private: Float {UAC} maxVal
      const Float maxValue;
      //## end ContinuousHistogram::maxValue%37442A5D01AD.attr

      //## Attribute: minValue%37442A5D01CB
      //	Lower boundary of histogram. All values bellow minValue are accounted in the lower interval.
      //## begin ContinuousHistogram::minValue%37442A5D01CB.attr preserve=no  private: Float {UAC} minVal
      const Float minValue;
      //## end ContinuousHistogram::minValue%37442A5D01CB.attr

      //## Attribute: numIntervals%37442A5D01F3
      //	Number of intervals in the histogram. This value is set in the constructor.
      //## begin ContinuousHistogram::numIntervals%37442A5D01F3.attr preserve=no  private: Int {UAC} intervals
      const Int numIntervals;
      //## end ContinuousHistogram::numIntervals%37442A5D01F3.attr

      //## Attribute: pValues%37442A5D021B
      //	Int array that stores the frequency of each interval in the histogram. Its size is dynamically set to the number of intervals and
      //	is allocated in the constructor. Each element is initially set to zero.
      //## begin ContinuousHistogram::pValues%37442A5D021B.attr preserve=no  private: Int* {UA} NULL
      Int* pValues;
      //## end ContinuousHistogram::pValues%37442A5D021B.attr

      //## Attribute: maxSample%37459D9C030B
      //	Maximum value of the sequence. This value is calculated and updated each time nextSample() is called.
      //## begin ContinuousHistogram::maxSample%37459D9C030B.attr preserve=no  private: Float {UA} SIM_HUGE_VAL
      Float maxSample;
      //## end ContinuousHistogram::maxSample%37459D9C030B.attr

      //## Attribute: minSample%37459DA20255
      //	Minimum value of the sequence. This value is calculated and updated each time nextSample() is called.
      //## begin ContinuousHistogram::minSample%37459DA20255.attr preserve=no  private: Float {UA} SIM_HUGE_VAL
      Float minSample;
      //## end ContinuousHistogram::minSample%37459DA20255.attr

      //## Attribute: numSamples%37459DAF039E
      //	Total number of samples in the sequence. This value is incremented each time nextSample() is called.
      //## begin ContinuousHistogram::numSamples%37459DAF039E.attr preserve=no  private: Int {UA} 0
      Int numSamples;
      //## end ContinuousHistogram::numSamples%37459DAF039E.attr

      //## Attribute: cumSum%3911CC4A01E4
      //## begin ContinuousHistogram::cumSum%3911CC4A01E4.attr preserve=no  private: Double {UA} 0.0
      Double cumSum;
      //## end ContinuousHistogram::cumSum%3911CC4A01E4.attr

      //## Attribute: cumSumOfSquares%3911CC610147
      //## begin ContinuousHistogram::cumSumOfSquares%3911CC610147.attr preserve=no  private: Double {UA} 0.0
      Double cumSumOfSquares;
      //## end ContinuousHistogram::cumSumOfSquares%3911CC610147.attr

    // Additional Private Declarations
      //## begin ContinuousHistogram%3744298B0396.private preserve=yes
      //## end ContinuousHistogram%3744298B0396.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ContinuousHistogram%3744298B0396.implementation preserve=yes
      //## end ContinuousHistogram%3744298B0396.implementation

};

//## begin ContinuousHistogram%3744298B0396.postscript preserve=yes
//## end ContinuousHistogram%3744298B0396.postscript

// Class ContinuousHistogram 

//## Operation: ~ContinuousHistogram%37442A2B0287
//	********************************************************************************
//
//	Name: ~ContinuousHistogram(void)
//
//	Description: Destructor - deallocates pValues array.
//
//	Parameters: none
//
//	Returns: none
//
//	********************************************************************************
inline ContinuousHistogram::~ContinuousHistogram ()
{
  //## begin ContinuousHistogram::~ContinuousHistogram%37442A2B0287.body preserve=yes

	delete [] pValues;        // delete array

  //## end ContinuousHistogram::~ContinuousHistogram%37442A2B0287.body
}



//## Other Operations (inline)
//## Operation: getIntervalWidth%37442A3402E5
//	********************************************************************************
//
//	Name: Float getIntervalWidth (void)
//
//	Description: Returns the width of each interval.
//
//	Parameters: none
//
//	Returns: the value of the intervalWidth attribute.
//
//	********************************************************************************
inline Float ContinuousHistogram::getIntervalWidth ()
{
  //## begin ContinuousHistogram::getIntervalWidth%37442A3402E5.body preserve=yes

	return (intervalWidth);

  //## end ContinuousHistogram::getIntervalWidth%37442A3402E5.body
}

//## Operation: getNumIntervals%37442A340317
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
inline Int ContinuousHistogram::getNumIntervals ()
{
  //## begin ContinuousHistogram::getNumIntervals%37442A340317.body preserve=yes

	return (numIntervals);

  //## end ContinuousHistogram::getNumIntervals%37442A340317.body
}

//## Operation: getMinValue%374598BE03A9
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
inline Float ContinuousHistogram::getMinValue ()
{
  //## begin ContinuousHistogram::getMinValue%374598BE03A9.body preserve=yes
	return minValue;
  //## end ContinuousHistogram::getMinValue%374598BE03A9.body
}

//## Operation: getMaxValue%37459AFE00AF
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
inline Float ContinuousHistogram::getMaxValue ()
{
  //## begin ContinuousHistogram::getMaxValue%37459AFE00AF.body preserve=yes
	return maxValue;
  //## end ContinuousHistogram::getMaxValue%37459AFE00AF.body
}

//## Operation: getMinSample%374598C201B0
//	********************************************************************************
//
//	Name: Int getMinSample (void)
//
//	Description: Returns the current minimum value of the sequence .
//
//	Parameters: none
//
//	Returns: the minimum value of the sequence.
//	         SIM_HUGE_VAL - Does not exist samples yet.
//	********************************************************************************
inline Float ContinuousHistogram::getMinSample ()
{
  //## begin ContinuousHistogram::getMinSample%374598C201B0.body preserve=yes
	return minSample;
  //## end ContinuousHistogram::getMinSample%374598C201B0.body
}

//## Operation: getMaxSample%37459C730390
//	********************************************************************************
//
//	Name: Int getMinSample (void)
//
//	Description: Returns the current maximum value of the sequence .
//
//	Parameters: none
//
//	Returns: the maximum value of the sequence.
//	         SIM_HUGE_VAL - Does not exist samples yet.
//	********************************************************************************
inline Float ContinuousHistogram::getMaxSample ()
{
  //## begin ContinuousHistogram::getMaxSample%37459C730390.body preserve=yes
	return maxSample;
  //## end ContinuousHistogram::getMaxSample%37459C730390.body
}

//## Operation: getNumSamples%3745A00C03DA
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
inline Int ContinuousHistogram::getNumSamples ()
{
  //## begin ContinuousHistogram::getNumSamples%3745A00C03DA.body preserve=yes
	return numSamples;
  //## end ContinuousHistogram::getNumSamples%3745A00C03DA.body
}

//## Operation: getMean%3911CC980114
//	********************************************************************************
//
//	Name:
//
//	Description:
//
//	Parameters: none
//
//	Returns:
//
//	********************************************************************************
inline Float ContinuousHistogram::getMean ()
{
  //## begin ContinuousHistogram::getMean%3911CC980114.body preserve=yes
	if(numSamples>0)
		return cumSum/numSamples;
	else
		return 0.0;
  //## end ContinuousHistogram::getMean%3911CC980114.body
}

//## Operation: getStdDev%3911CC9A0009
//	********************************************************************************
//
//	Name:
//
//	Description:
//
//	Parameters:
//
//	Returns:
//
//	********************************************************************************
inline Float ContinuousHistogram::getStdDev ()
{
  //## begin ContinuousHistogram::getStdDev%3911CC9A0009.body preserve=yes
	if(numSamples>1)
	{
		double a = getMean();
		return sqrt((cumSumOfSquares - numSamples*a*a)/(numSamples-1.0));
	}
	else
	{
		return 0.0;
	}

  //## end ContinuousHistogram::getStdDev%3911CC9A0009.body
}

//## Operation: getCumSum%3911D7A103BF
//	********************************************************************************
//
//	Name:
//
//	Description:
//
//	Parameters:
//
//	Returns:
//
//	********************************************************************************
inline Float ContinuousHistogram::getCumSum ()
{
  //## begin ContinuousHistogram::getCumSum%3911D7A103BF.body preserve=yes
	return cumSum;
  //## end ContinuousHistogram::getCumSum%3911D7A103BF.body
}

//## Operation: getCumSumOfSquares%3911D7AC0285
//	********************************************************************************
//
//	Name:
//
//	Description:
//
//	Parameters:
//
//	Returns:
//
//	********************************************************************************
inline Float ContinuousHistogram::getCumSumOfSquares ()
{
  //## begin ContinuousHistogram::getCumSumOfSquares%3911D7AC0285.body preserve=yes
	return cumSumOfSquares;
  //## end ContinuousHistogram::getCumSumOfSquares%3911D7AC0285.body
}

//## begin module%3744298B0396.epilog preserve=yes

} // namespace sch

//## end module%3744298B0396.epilog


#endif
