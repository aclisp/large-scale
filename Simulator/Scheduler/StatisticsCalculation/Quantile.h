//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC5A940175.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC5A940175.cm

//## begin module%36AC5A940175.cp preserve=yes
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
//	File Name: Quantile.h
//
//	Description: Header file for the Quantile class.
//
//	Author: Eugenio Pacheli Alves
//
//	Creation Date: Apr/14/99
//
//	Visual Source Safe $Revision: 9 $
//
//	Last check in $Date: 10/04/99 2:51p $
//
//	********************************************************************************
//## end module%36AC5A940175.cp

//## Module: Quantile%36AC5A940175; Pseudo Package specification
//## Source file: T:\StatisticsCalculation\Quantile.h

#ifndef Quantile_h
#define Quantile_h 1

//## begin module%36AC5A940175.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36AC5A940175.additionalIncludes

//## begin module%36AC5A940175.includes preserve=yes
#include <math.h>
//## end module%36AC5A940175.includes

// Marker
#include "Marker.h"
//## begin module%36AC5A940175.additionalDeclarations preserve=yes

using namespace std;

namespace sch {

//## end module%36AC5A940175.additionalDeclarations


//## begin Quantile%36AC5A940175.preface preserve=yes
//## end Quantile%36AC5A940175.preface

//## Class: Quantile%36AC5A940175
//	********************************************************************************
//
//	This class supplies a set of operations to calculate the p-quantile of a sequence of continuous samples. The p-quantile of a
//	distribution is the value below which 100p percent of the distribution lies.
//
//	The p-quantile is calculated iteratively, without the need of storing the values in the sequence. The algorithm consists of
//	maintaining five markers:
//	   - the minimum
//	   - p/2-quantile
//	   - p-quantile
//	   - (1+p)/2-quantile
//	   - maximum.
//
//	The vertical height of each marker is equal to the corresponding quantile value, and its horizontal location is equal to the number
//	of observations that are less than or equal to the marker. True values of the quantiles are not known; at any point in time, the
//	marker heights are the current estimates of the quantiles, and these estimates are updated after every sample is inserted in the
//	sequence.
//
//	Actual marker locations (integer values) are different than ideal locations (real values), so a piecewise-parabolic prediction (PP
//	or P2 formula) is used to adjust the height and location of markers.
//
//	The vertical heights are represented as q(i) and the horizontal locations are represented as n(i), where i = 1 thru 5.
//
//	If many quantiles of the same variable are required (e.g., 0.10, 0.50, 0.90 etc), it may be more efficient as well as more accurate
//	to calculate a histogram. In fact, the algorithm used for quantiles may also be used to calculate cumulative histograms of real
//	values. The number of markers in such case, rather than 5, is equal to the number of desired intervals plus one.
//
//	********************************************************************************
//## Category: StatisticsCalculation%369E340F008B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class Quantile 
{
  //## begin Quantile%36AC5A940175.initialDeclarations preserve=yes
  //## end Quantile%36AC5A940175.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: Quantile%36B700850272
      Quantile (Float quantileThreshold);

    //## Destructor (specified)
      //## Operation: ~Quantile%36EE4B36018D
      ~Quantile ();


    //## Other Operations (specified)
      //## Operation: getMax%371B1894015E
      Float getMax ();

      //## Operation: getMin%371B189B00D1
      Float getMin ();

      //## Operation: getNumSamples%3715F7550078
      Int getNumSamples ();

      //## Operation: getQuantile%36B715D70158
      Float getQuantile ();

      //## Operation: getThreshold%3715F746015D
      Float getThreshold ();

      //## Operation: nextSample%36B700DE02C0
      RetCode nextSample (Float sample);

      //## Operation: reset%374C035E0393
      RetCode reset ();

    // Additional Public Declarations
      //## begin Quantile%36AC5A940175.public preserve=yes
      //## end Quantile%36AC5A940175.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: numMarkers%37162B96033B
      //	Number of markers used in computations. This is the size of the pMarkers array.  The algorithm consists of maintaining five
      //	markers:
      //	   - the minimum
      //	   - p/2-quantile
      //	   - p-quantile
      //	   - (1+p)/2-quantile
      //	   - maximum.
      //## begin Quantile::numMarkers%37162B96033B.attr preserve=no  protected: Int {UAC} 5
      const Int numMarkers;
      //## end Quantile::numMarkers%37162B96033B.attr

      //## Attribute: numSamples%3715F5720150
      //	Total number of samples in the sequence. This value is incremented each time nextSample() is called.
      //## begin Quantile::numSamples%3715F5720150.attr preserve=no  protected: Int {UA} 0
      Int numSamples;
      //## end Quantile::numSamples%3715F5720150.attr

    // Data Members for Associations

      //## Association: StatisticsCalculation::<unnamed>%371633300343
      //## Role: Quantile::pMarkers%37163332001A
      //	Array for storing the markers. The Markers structure holds all parameters required for each marker such as position and height.
      //## begin Quantile::pMarkers%37163332001A.role preserve=no  protected: MarkerStruc {1 -> nVHgAN}
      MarkerStruc *pMarkers;
      //## end Quantile::pMarkers%37163332001A.role

    // Additional Protected Declarations
      //## begin Quantile%36AC5A940175.protected preserve=yes
      //## end Quantile%36AC5A940175.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: threshold%3715F8060055
      //	Stores the p value used to calculate the p-quantile.
      //## begin Quantile::threshold%3715F8060055.attr preserve=no  private: Float {UAC} quantileThreshold
      const Float threshold;
      //## end Quantile::threshold%3715F8060055.attr

    // Additional Private Declarations
      //## begin Quantile%36AC5A940175.private preserve=yes
      //## end Quantile%36AC5A940175.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin Quantile%36AC5A940175.implementation preserve=yes
	  void selectionSort (void);
	  Double parabolic (Int i, Int deltaSign);
	  Double linear (Int i, Int deltaSign);
      //## end Quantile%36AC5A940175.implementation
};

//## begin Quantile%36AC5A940175.postscript preserve=yes
//## end Quantile%36AC5A940175.postscript

// Class Quantile 

//## Operation: ~Quantile%36EE4B36018D
//	********************************************************************************
//
//	Name: ~Quantile(void)
//
//	Description: Destructor - deallocates pMarkers array.
//
//	Parameters: none
//
//	Returns: none
//
//
//	********************************************************************************
inline Quantile::~Quantile ()
{
  //## begin Quantile::~Quantile%36EE4B36018D.body preserve=yes
		delete [] pMarkers;        // delete array
  //## end Quantile::~Quantile%36EE4B36018D.body
}



//## Other Operations (inline)
//## Operation: getMax%371B1894015E
//	********************************************************************************
//
//	Name: Float getMax (void)
//
//	Description: Returns the current maximum value of the sequence.
//
//	Parameters: none
//
//	Returns: the height of the last position of pMarkers.
//	         SIM_HUGE_VAL - so far less than 5 samples were inserted in the sequence.
//
//
//	********************************************************************************
inline Float Quantile::getMax ()
{
  //## begin Quantile::getMax%371B1894015E.body preserve=yes

	//
	// should be called only after 6th sample is inserted
	//
	return (numSamples < 6)? SIM_HUGE_VAL : pMarkers[numMarkers-1].height;

  //## end Quantile::getMax%371B1894015E.body
}

//## Operation: getMin%371B189B00D1
//	********************************************************************************
//
//	Name: Float getMin (void)
//
//	Description: Returns the current minimum value of the sequence.
//
//	Parameters: none
//
//	Returns: the height of the first position of pMarkers.
//	         SIM_HUGE_VAL - so far less than 5 samples were inserted in the sequence.
//
//
//	********************************************************************************
inline Float Quantile::getMin ()
{
  //## begin Quantile::getMin%371B189B00D1.body preserve=yes

	//
	// should be called only after 6th sample is inserted
	//
	return (numSamples < 6)? SIM_HUGE_VAL : pMarkers[0].height;	

  //## end Quantile::getMin%371B189B00D1.body
}

//## Operation: getNumSamples%3715F7550078
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
inline Int Quantile::getNumSamples ()
{
  //## begin Quantile::getNumSamples%3715F7550078.body preserve=yes

	return (numSamples);

  //## end Quantile::getNumSamples%3715F7550078.body
}

//## Operation: getThreshold%3715F746015D
//	********************************************************************************
//
//	Name: Int getThreshold (void)
//
//	Description: Returns the p value used to calculate the p-quantile.
//
//	Parameters: none
//
//	Returns: the value of the threshold attribute.
//
//	********************************************************************************
inline Float Quantile::getThreshold ()
{
  //## begin Quantile::getThreshold%3715F746015D.body preserve=yes

	return (threshold);

  //## end Quantile::getThreshold%3715F746015D.body
}

//## begin module%36AC5A940175.epilog preserve=yes

} // namespace sch

//## end module%36AC5A940175.epilog


#endif
