//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36B6DD7000B6.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36B6DD7000B6.cm

//## begin module%36B6DD7000B6.cp preserve=yes
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
//	File Name: DescriptiveStatistic.h
//
//	Description: Header file for the DescriptiveStatistic class.
//
//	Author: Eugenio Pacheli Alves
//
//	Creation Date: Apr/12/99
//
//	Visual Source Safe $Revision: 7 $
//
//	Last check in $Date: 8/03/99 1:36p $
//
//	********************************************************************************
//## end module%36B6DD7000B6.cp

//## Module: DescriptiveStatistic%36B6DD7000B6; Pseudo Package specification
//## Source file: T:\StatisticsCalculation\DescriptiveStatistic.h

#ifndef DescriptiveStatistic_h
#define DescriptiveStatistic_h 1

//## begin module%36B6DD7000B6.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36B6DD7000B6.additionalIncludes

//## begin module%36B6DD7000B6.includes preserve=yes
#include <math.h>      // common mathematical functions
//## end module%36B6DD7000B6.includes

// Mean
#include "Mean.h"
//## begin module%36B6DD7000B6.additionalDeclarations preserve=yes

using namespace std;

namespace sch {

//## end module%36B6DD7000B6.additionalDeclarations


//## begin DescriptiveStatistic%36B6DD7000B6.preface preserve=yes
//## end DescriptiveStatistic%36B6DD7000B6.preface

//## Class: DescriptiveStatistic%36B6DD7000B6
//	********************************************************************************
//
//	This class supplies a set of operations to calculate variance and standard deviation. The variance is calculated iteratively,
//	without the need of storing the value in the sequence. The standard deviation is derived from the variance, since it is equal to
//	the square root of the variance. The reckoning requires the mean of the corresponding sequence. Anyone that needs both mean and
//	variance may instantiate only a DescriptiveStatistic object and then use its associated mean.
//
//	********************************************************************************
//## Category: StatisticsCalculation%369E340F008B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class DescriptiveStatistic 
{
  //## begin DescriptiveStatistic%36B6DD7000B6.initialDeclarations preserve=yes
  //## end DescriptiveStatistic%36B6DD7000B6.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: DescriptiveStatistic%3713428B038F
      DescriptiveStatistic ();

    //## Destructor (specified)
      //## Operation: ~DescriptiveStatistic%3713429002F6
      virtual ~DescriptiveStatistic ();


    //## Other Operations (specified)
      //## Operation: getStdDev%36B7443C0201
      Float getStdDev ();

      //## Operation: getVariance%36B74432021A
      Float getVariance ();

      //## Operation: nextSample%36B743DD0312
      virtual RetCode nextSample (Float sample);

      //## Operation: reset%374C03550318
      RetCode reset ();

    // Data Members for Associations

      //## Association: StatisticsCalculation::<unnamed>%370E5B8E0143
      //## Role: DescriptiveStatistic::pMean%370E5B8F01B3
      //	DescriptiveStatistic maintains the non-robust mean for calculating the iterative mean of all values in the sequence.
      //## begin DescriptiveStatistic::pMean%370E5B8F01B3.role preserve=no  public: Mean {1 -> 1RHgAN}
      Mean *pMean;
      //## end DescriptiveStatistic::pMean%370E5B8F01B3.role

    // Additional Public Declarations
      //## begin DescriptiveStatistic%36B6DD7000B6.public preserve=yes
      //## end DescriptiveStatistic%36B6DD7000B6.public

  protected:
    // Additional Protected Declarations
      //## begin DescriptiveStatistic%36B6DD7000B6.protected preserve=yes
      //## end DescriptiveStatistic%36B6DD7000B6.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: varianceValue%36B7436C0298
      //	Variance. This value is calculated and updated each time nextSample() is called. It is iteratively reckoned, that is, everytime a
      //	new sample is inserted in the sequence the new variance is reckoned from the previous variance, without the need of storing all
      //	values in the sequence. The reckoning requires the mean of the sequence.
      //## begin DescriptiveStatistic::varianceValue%36B7436C0298.attr preserve=no  private: Double {UA} 0.0
      Double varianceValue;
      //## end DescriptiveStatistic::varianceValue%36B7436C0298.attr

    // Additional Private Declarations
      //## begin DescriptiveStatistic%36B6DD7000B6.private preserve=yes
      //## end DescriptiveStatistic%36B6DD7000B6.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin DescriptiveStatistic%36B6DD7000B6.implementation preserve=yes
      //## end DescriptiveStatistic%36B6DD7000B6.implementation

};

//## begin DescriptiveStatistic%36B6DD7000B6.postscript preserve=yes
//## end DescriptiveStatistic%36B6DD7000B6.postscript

// Class DescriptiveStatistic 

//## Operation: ~DescriptiveStatistic%3713429002F6
//	********************************************************************************
//
//	Name: ~DescriptiveStatistic(void)
//
//	Description: Destructor - deallocates pMean object.
//
//	Parameters: none
//
//	Returns: none
//
//	Remarks: this destructor is virtual since the DescriptiveStatistic class may be base for other classes.
//
//	********************************************************************************
inline DescriptiveStatistic::~DescriptiveStatistic ()
{
  //## begin DescriptiveStatistic::~DescriptiveStatistic%3713429002F6.body preserve=yes

	delete pMean;			  // delete object

  //## end DescriptiveStatistic::~DescriptiveStatistic%3713429002F6.body
}



//## Other Operations (inline)
//## Operation: getStdDev%36B7443C0201
//	********************************************************************************
//
//	Name: Float getStdDev (void)
//
//	Description: Returns the current standard deviation of the sequence. This value is calculated as the square root of the variance.
//
//	Parameters: none
//
//	Returns: the calculated standard deviation.
//	         SIM_HUGE_VAL- Does not exist samples yet.
//
//	********************************************************************************
inline Float DescriptiveStatistic::getStdDev ()
{
  //## begin DescriptiveStatistic::getStdDev%36B7443C0201.body preserve=yes

	//
	// Note that, at least two samples are needed for a trusted varianceValue
	//
	return (pMean->getNumSamples() > 0) ? sqrt (varianceValue) : SIM_HUGE_VAL;

  //## end DescriptiveStatistic::getStdDev%36B7443C0201.body
}

//## Operation: getVariance%36B74432021A
//	********************************************************************************
//
//	Name: Float getVariance (void)
//
//	Description: Returns the current variance of the sequence.
//
//	Parameters: none
//
//	Returns: the value of the varianceValue attribute.
//	                  SIM_HUGE_VAL- Does not exist samples yet.
//	********************************************************************************
inline Float DescriptiveStatistic::getVariance ()
{
  //## begin DescriptiveStatistic::getVariance%36B74432021A.body preserve=yes

	//
	// Note that, at least two samples are needed for a trusted varianceValue
	//
	return (pMean->getNumSamples() > 0) ? varianceValue : SIM_HUGE_VAL;

  //## end DescriptiveStatistic::getVariance%36B74432021A.body
}

//## begin module%36B6DD7000B6.epilog preserve=yes

} // namespace sch

//## end module%36B6DD7000B6.epilog


#endif
