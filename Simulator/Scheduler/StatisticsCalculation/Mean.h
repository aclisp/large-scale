//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36C027010384.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36C027010384.cm

//## begin module%36C027010384.cp preserve=yes
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
//	File Name: Mean.h
//
//	Description: Header file for the Mean class.
//
//	Author: Eugenio Pacheli Alves
//
//	Creation Date: Apr/08/99
//
//	Visual Source Safe $Revision: 8 $
//
//	Last check in $Date: 8/03/99 1:36p $
//
//	********************************************************************************
//## end module%36C027010384.cp

//## Module: Mean%36C027010384; Pseudo Package specification
//## Source file: T:\StatisticsCalculation\Mean.h

#ifndef Mean_h
#define Mean_h 1

//## begin module%36C027010384.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36C027010384.additionalIncludes

//## begin module%36C027010384.includes preserve=yes
#include <math.h>
//## end module%36C027010384.includes

//## begin module%36C027010384.additionalDeclarations preserve=yes

using namespace std;

namespace sch {

//## end module%36C027010384.additionalDeclarations


//## begin Mean%36C027010384.preface preserve=yes
//## end Mean%36C027010384.preface

//## Class: Mean%36C027010384
//	********************************************************************************
//
//	This class supplies a set of operations to calculate the minimum, maximum and mean of a sequence of values. The mean is calculated
//	iteratively, without the need of storing the values in the sequence.
//
//	This class provides an operator+ (and corresponding operator+=) for adding two means.
//
//	********************************************************************************
//## Category: StatisticsCalculation%369E340F008B
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class Mean 
{
  //## begin Mean%36C027010384.initialDeclarations preserve=yes
  //## end Mean%36C027010384.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: Mean%36D1949801BC
      Mean ();


    //## Other Operations (specified)
      //## Operation: getMax%36C07B540212
      Float getMax ();

      //## Operation: getMean%36C0272E00C2
      Float getMean ();

      //## Operation: getMin%36C07B650285
      Float getMin ();

      //## Operation: getNumSamples%36C07B350069
      Int getNumSamples ();

      //## Operation: getSum%36C07B820088
      Float getSum ();

      //## Operation: nextSample%36C033E50311
      virtual RetCode nextSample (Float sample);

      //## Operation: operator+=%36C16BF803A4
      Mean& operator += (const Mean& rhs);

      //## Operation: reset%374C00B00210
      RetCode reset ();

    // Additional Public Declarations
      //## begin Mean%36C027010384.public preserve=yes
      //## end Mean%36C027010384.public

  protected:
    // Additional Protected Declarations
      //## begin Mean%36C027010384.protected preserve=yes
      //## end Mean%36C027010384.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: maxValue%36C07B060007
      //	Maximum value of the sequence. This value is calculated and updated each time nextSample() is called.
      //## begin Mean::maxValue%36C07B060007.attr preserve=no  private: Float {UA} SIM_HUGE_VAL 
      Float maxValue;
      //## end Mean::maxValue%36C07B060007.attr

      //## Attribute: meanValue%36C0270D007E
      //	Mean. This value is calculated and updated each time nextSample() is called. It is iteratively reckoned, that is, everytime a new
      //	sample is inserted in the sequence the new mean is reckoned from the previous mean, without the need of storing all values in the
      //	sequence.
      //## begin Mean::meanValue%36C0270D007E.attr preserve=no  private: Double {UA} SIM_HUGE_VAL 
      Double meanValue;
      //## end Mean::meanValue%36C0270D007E.attr

      //## Attribute: minValue%36C07B170305
      //	Minimum value of the sequence. This value is calculated and updated each time nextSample() is called.
      //## begin Mean::minValue%36C07B170305.attr preserve=no  private: Float {UA} SIM_HUGE_VAL 
      Float minValue;
      //## end Mean::minValue%36C07B170305.attr

      //## Attribute: numSamples%36C07AF5028E
      //	Total number of samples in the sequence. This value is incremented each time nextSample() is called.
      //## begin Mean::numSamples%36C07AF5028E.attr preserve=no  private: Int {UA} 0
      Int numSamples;
      //## end Mean::numSamples%36C07AF5028E.attr

    // Additional Private Declarations
      //## begin Mean%36C027010384.private preserve=yes
      //## end Mean%36C027010384.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin Mean%36C027010384.implementation preserve=yes
      //## end Mean%36C027010384.implementation

};

//## begin Mean%36C027010384.postscript preserve=yes
//## end Mean%36C027010384.postscript

// Class Mean 

//## Operation: Mean%36D1949801BC
//	********************************************************************************
//
//	Name: Mean(void)
//
//	Description: Constructor for members initialization.
//
//	Parameters: none
//
//	Returns: none
//
//	Remarks: Inlined since this constructor does not have body implementation.
//
//	********************************************************************************
inline Mean::Mean ()
  //## begin Mean::Mean%36D1949801BC.hasinit preserve=no
      : maxValue(SIM_HUGE_VAL ),
        meanValue(SIM_HUGE_VAL ),
        minValue(SIM_HUGE_VAL ),
        numSamples(0)
  //## end Mean::Mean%36D1949801BC.hasinit
  //## begin Mean::Mean%36D1949801BC.initialization preserve=yes
  //## end Mean::Mean%36D1949801BC.initialization
{
  //## begin Mean::Mean%36D1949801BC.body preserve=yes
  //## end Mean::Mean%36D1949801BC.body
}



//## Other Operations (inline)
//## Operation: getMax%36C07B540212
//	********************************************************************************
//
//	Name: Float getMax (void)
//
//	Description: Returns the current maximum value of the sequence.
//
//	Parameters: none
//
//	Returns: the value of the maxValue attribute.
//	         SIM_HUGE_VAL - Does not exist samples yet.
//	********************************************************************************
inline Float Mean::getMax ()
{
  //## begin Mean::getMax%36C07B540212.body preserve=yes

	return (maxValue);

  //## end Mean::getMax%36C07B540212.body
}

//## Operation: getMean%36C0272E00C2
//	********************************************************************************
//
//	Name: Float getMean (void)
//
//	Description: Returns the current mean of the sequence.
//
//	Parameters: none
//
//	Returns: the value of the meanValue attribute.
//	         SIM_HUGE_VAL - Does not exist samples yet.
//	********************************************************************************
inline Float Mean::getMean ()
{
  //## begin Mean::getMean%36C0272E00C2.body preserve=yes

	return (meanValue);

  //## end Mean::getMean%36C0272E00C2.body
}

//## Operation: getMin%36C07B650285
//	********************************************************************************
//
//	Name: Float getMin (void)
//
//	Description: Returns the current minimum value of the sequence.
//
//	Parameters: none
//
//	Returns: the value of the minValue attribute.
//	         SIM_HUGE_VAL - Does not exist samples yet.
//	********************************************************************************
inline Float Mean::getMin ()
{
  //## begin Mean::getMin%36C07B650285.body preserve=yes

	return (minValue);

  //## end Mean::getMin%36C07B650285.body
}

//## Operation: getNumSamples%36C07B350069
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
inline Int Mean::getNumSamples ()
{
  //## begin Mean::getNumSamples%36C07B350069.body preserve=yes

	return (numSamples);

  //## end Mean::getNumSamples%36C07B350069.body
}

//## Operation: getSum%36C07B820088
//	********************************************************************************
//
//	Name: Float getSum (void)
//
//	Description: Returns the current sum of all values of the sequence. This value is calculated as
//	   meanValue * numSamples
//
//	Parameters: none
//	         SIM_HUGE_VAL - Does not exist samples yet.
//	Returns: the calculated sum.
//
//	********************************************************************************
inline Float Mean::getSum ()
{
  //## begin Mean::getSum%36C07B820088.body preserve=yes

	return (numSamples == 0) ? SIM_HUGE_VAL : (meanValue * numSamples);

  //## end Mean::getSum%36C07B820088.body
}

//## begin module%36C027010384.epilog preserve=yes

} // namespace sch

//## end module%36C027010384.epilog


#endif
