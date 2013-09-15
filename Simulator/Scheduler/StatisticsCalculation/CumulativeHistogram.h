//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36B6DCF801E0.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36B6DCF801E0.cm

//## begin module%36B6DCF801E0.cp preserve=yes
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
//	File Name: CumulativeHistogram.h
//
//	Description: Header file for the ContinuousHistogram class. 
//
//	Author:	Eugenio Pacheli Alves  / Hélio Azevedo
//
//	Creation Date:	May/20/99
//
//	Visual Source Safe $Revision: 5 $
//
//	Last check in $Date: 8/24/99 9:56a $
//
//	********************************************************************************
//## end module%36B6DCF801E0.cp

//## Module: CumulativeHistogram%36B6DCF801E0; Pseudo Package specification
//## Source file: T:\StatisticsCalculation\CumulativeHistogram.h

#ifndef CumulativeHistogram_h
#define CumulativeHistogram_h 1

//## begin module%36B6DCF801E0.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36B6DCF801E0.additionalIncludes

//## begin module%36B6DCF801E0.includes preserve=yes
#include <math.h>
//## end module%36B6DCF801E0.includes

// Marker
#include "Marker.h"
//## begin module%36B6DCF801E0.additionalDeclarations preserve=yes

using namespace std;

namespace sch {


//## end module%36B6DCF801E0.additionalDeclarations


//## begin CumulativeHistogram%36B6DCF801E0.preface preserve=yes
//## end CumulativeHistogram%36B6DCF801E0.preface

//## Class: CumulativeHistogram%36B6DCF801E0
//	********************************************************************************
//
//	This class supplies a set of operations to calculate the histogram of a sequence of real samples. The histogram shows the frequency
//	of samples within a range (the histogram range) which is divided into several adjacent intervals.
//	Equiprobable intervals are used instead of equal-size intervals because it would be hard to determine the optimum interval size.
//
//	To plot a histogram with k equiprobable intervals, all we need are values of (k+1) quantile markers. The cumulative histogram is
//	obtained by ploting the markers heights on x-axis and the markers locations divided by the number of samples on the y-axis.
//
//	The quantile algorithm is used to compute the markers locations and heights.
//	********************************************************************************
//## Category: StatisticsCalculation%369E340F008B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CumulativeHistogram 
{
  //## begin CumulativeHistogram%36B6DCF801E0.initialDeclarations preserve=yes
  //## end CumulativeHistogram%36B6DCF801E0.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: CumulativeHistogram%36B72F2F0067
      CumulativeHistogram (Int intervals);

    //## Destructor (specified)
      //## Operation: ~CumulativeHistogram%36E007A301D4
      ~CumulativeHistogram ();


    //## Other Operations (specified)
      //## Operation: getCumulativeResult%36B742F400FB
      RetCode getCumulativeResult (Int markerNumber, Float *pX, Float *pY);

      //## Operation: getDerivedResult%372F25DD027B
      RetCode getDerivedResult (Int markerNumber, Float *pX, Float *pY);

      //## Operation: getMax%373B0B29002B
      Float getMax ();

      //## Operation: getMin%373B0B2E0028
      Float getMin ();

      //## Operation: getNumIntervals%373B0B320237
      Int getNumIntervals ();

      //## Operation: getNumSamples%373B0B36035F
      Int getNumSamples ();

      //## Operation: nextSample%373B0B46022C
      RetCode nextSample (Float sample);

      //## Operation: reset%374C03380276
      RetCode reset ();

    // Additional Public Declarations
      //## begin CumulativeHistogram%36B6DCF801E0.public preserve=yes
      //## end CumulativeHistogram%36B6DCF801E0.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: numMarkers%373B0B5001EA
      //	Number of markers used in computations. This is the size of the pMarkers array.
      //## begin CumulativeHistogram::numMarkers%373B0B5001EA.attr preserve=no  protected: Int {UAC} intervals + 1
      const Int numMarkers;
      //## end CumulativeHistogram::numMarkers%373B0B5001EA.attr

      //## Attribute: numSamples%373B0B550300
      //	Total number of samples in the sequence. This value is incremented each time nextSample() is called.
      //## begin CumulativeHistogram::numSamples%373B0B550300.attr preserve=no  protected: Int {UA} 0
      Int numSamples;
      //## end CumulativeHistogram::numSamples%373B0B550300.attr

    // Data Members for Associations

      //## Association: StatisticsCalculation::<unnamed>%373B0B910099
      //## Role: CumulativeHistogram::pMarkers%373B0B91037E
      //	Array for storing the markers. The Markers structure holds all parameters required for each marker such as position and height.
      //## begin CumulativeHistogram::pMarkers%373B0B91037E.role preserve=no  protected: MarkerStruc {1 -> nVHgAN}
      MarkerStruc *pMarkers;
      //## end CumulativeHistogram::pMarkers%373B0B91037E.role

    // Additional Protected Declarations
      //## begin CumulativeHistogram%36B6DCF801E0.protected preserve=yes
      //## end CumulativeHistogram%36B6DCF801E0.protected

  private:
    // Additional Private Declarations
      //## begin CumulativeHistogram%36B6DCF801E0.private preserve=yes
      //## end CumulativeHistogram%36B6DCF801E0.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CumulativeHistogram%36B6DCF801E0.implementation preserve=yes

	  void selectionSort (void);
	  Double parabolic (Int i, Int deltaSign);
	  Double linear (Int i, Int deltaSign);

      //## end CumulativeHistogram%36B6DCF801E0.implementation
};

//## begin CumulativeHistogram%36B6DCF801E0.postscript preserve=yes
//## end CumulativeHistogram%36B6DCF801E0.postscript

// Class CumulativeHistogram 

//## Operation: ~CumulativeHistogram%36E007A301D4
//	********************************************************************************
//
//	Name: ~ContinuousHistogram(void)
//
//	Description: Destructor - empty body.
//
//	Parameters: none
//
//	Returns: none
//
//	Remarks: this destructor is virtual since the ContinuousHistogram class may be base for other classes.
//
//	********************************************************************************
inline CumulativeHistogram::~CumulativeHistogram ()
{
  //## begin CumulativeHistogram::~CumulativeHistogram%36E007A301D4.body preserve=yes
		delete [] pMarkers;        // delete array
  //## end CumulativeHistogram::~CumulativeHistogram%36E007A301D4.body
}



//## Other Operations (inline)
//## Operation: getMax%373B0B29002B
//	********************************************************************************
//
//	Name: Float getMax (void)
//
//	Description: Returns the current maximum value of the sequence.
//
//	Parameters: none
//
//	Returns: the height of the last position of pMarkers.
//	    SIM_HUGE_VAL- so far less than numMarkers samples were inserted in the sequence.
//	********************************************************************************
inline Float CumulativeHistogram::getMax ()
{
  //## begin CumulativeHistogram::getMax%373B0B29002B.body preserve=yes

	//
	// should be called only after numMarkers samples are inserted
	//
	return (numSamples < numMarkers)? SIM_HUGE_VAL : pMarkers[numMarkers].height;

  //## end CumulativeHistogram::getMax%373B0B29002B.body
}

//## Operation: getMin%373B0B2E0028
//	********************************************************************************
//
//	Name: Float getMin (void)
//
//	Description: Returns the current minimum value of the sequence.
//
//	Parameters: none
//
//	Returns: the height of the first position of pMarkers.
//	    SIM_HUGE_VAL- so far less than numMarkers samples were inserted in the sequence.
//	********************************************************************************
inline Float CumulativeHistogram::getMin ()
{
  //## begin CumulativeHistogram::getMin%373B0B2E0028.body preserve=yes

	//
	// should be called only after numMarkers samples are inserted
	//
	return (numSamples < numMarkers)? SIM_HUGE_VAL : pMarkers[1].height;

  //## end CumulativeHistogram::getMin%373B0B2E0028.body
}

//## Operation: getNumIntervals%373B0B320237
//	********************************************************************************
//
//	Name: Int getNumIntervals (void)
//
//	Description: Returns the number of intervals of the histogram, which is equal to the number of markers minus one.
//
//	Parameters: none
//
//	Returns: the value of the numMarkers attribute minus 1.
//
//	********************************************************************************
inline Int CumulativeHistogram::getNumIntervals ()
{
  //## begin CumulativeHistogram::getNumIntervals%373B0B320237.body preserve=yes
	
	return (numMarkers - 1);

  //## end CumulativeHistogram::getNumIntervals%373B0B320237.body
}

//## Operation: getNumSamples%373B0B36035F
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
inline Int CumulativeHistogram::getNumSamples ()
{
  //## begin CumulativeHistogram::getNumSamples%373B0B36035F.body preserve=yes
	
	return (numSamples);

  //## end CumulativeHistogram::getNumSamples%373B0B36035F.body
}

//## begin module%36B6DCF801E0.epilog preserve=yes

} // namespace sch

//## end module%36B6DCF801E0.epilog


#endif
