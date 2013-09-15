//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC5032034D.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC5032034D.cm

//## begin module%36AC5032034D.cp preserve=yes
//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: Random Variable Generation
//
//	File Name: DiscreteGen.h
//
//	Description: Header file for the DiscreteGen class 
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 9 $
//
//	Last check in $Date: 10/04/99 2:41p $
//
//	********************************************************************************
//## end module%36AC5032034D.cp

//## Module: DiscreteGen%36AC5032034D; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\DiscreteGen.h

#ifndef DiscreteGen_h
#define DiscreteGen_h 1

//## begin module%36AC5032034D.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36AC5032034D.additionalIncludes

//## begin module%36AC5032034D.includes preserve=yes
#include <memory>
//## end module%36AC5032034D.includes

// DiscreteDistribution
#include "DiscreteDistribution.h"
//## begin module%36AC5032034D.additionalDeclarations preserve=yes
using namespace std;
namespace sch {
//## end module%36AC5032034D.additionalDeclarations


//## begin DiscreteGen%36AC5032034D.preface preserve=yes
//## end DiscreteGen%36AC5032034D.preface

//## Class: DiscreteGen%36AC5032034D
//	********************************************************************************
//
//	Generates the Discrete distribution using the uniform generation.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class DiscreteGen : public DiscreteDistribution  //## Inherits: <unnamed>%37332444025A
{
  //## begin DiscreteGen%36AC5032034D.initialDeclarations preserve=yes
  //## end DiscreteGen%36AC5032034D.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: DiscreteGen%36B6E43A0310
      DiscreteGen (Ulong seed, Float *pProbArray, Ulong sizeOfArray, Int aValue);

    //## Destructor (specified)
      //## Operation: ~DiscreteGen%374EE5050320
      ~DiscreteGen ();


    //## Other Operations (specified)
      //## Operation: nextValue%36B6E4130238
      RetCode nextValue (Int *pValue);

      //## Operation: redefine%374E91B102CB
      RetCode redefine (Float *pProbArray, Int aValue);

    // Additional Public Declarations
      //## begin DiscreteGen%36AC5032034D.public preserve=yes
      //## end DiscreteGen%36AC5032034D.public

  protected:
    // Additional Protected Declarations
      //## begin DiscreteGen%36AC5032034D.protected preserve=yes
      //## end DiscreteGen%36AC5032034D.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: a%37276BDA00A2
      //	Value of the first sample.
      //	For example: p(a) = 0.3
      //## begin DiscreteGen::a%37276BDA00A2.attr preserve=no  private: Int {UA} aValue
      Int a;
      //## end DiscreteGen::a%37276BDA00A2.attr

      //## Attribute: arraySize%370BAF86022E
      //	Number of elements in the probability array.
      //## begin DiscreteGen::arraySize%370BAF86022E.attr preserve=no  private: Ulong {UAC} sizeOfArray
      const Ulong arraySize;
      //## end DiscreteGen::arraySize%370BAF86022E.attr

      //## Attribute: pProbability%370BAEA50339
      //	Array of probability. The sum of the all probabilities of the array must be 1. Note that the array members are const.
      //## begin DiscreteGen::pProbability%370BAEA50339.attr preserve=no  private: Float * {UA} NULL
      Float *pProbability;
      //## end DiscreteGen::pProbability%370BAEA50339.attr

      //## Attribute: epsilon%374ED5970095
      //	Stores the tolerable error int the sum of the probabilities array.
      //	Remember that this sum must converge to 1.
      //## begin DiscreteGen::epsilon%374ED5970095.attr preserve=no  private: Float {UAC} 0.0001f
      const Float epsilon;
      //## end DiscreteGen::epsilon%374ED5970095.attr

    // Additional Private Declarations
      //## begin DiscreteGen%36AC5032034D.private preserve=yes
      //## end DiscreteGen%36AC5032034D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin DiscreteGen%36AC5032034D.implementation preserve=yes
      //## end DiscreteGen%36AC5032034D.implementation

};

//## begin DiscreteGen%36AC5032034D.postscript preserve=yes
//## end DiscreteGen%36AC5032034D.postscript

// Class DiscreteGen 

//## Operation: ~DiscreteGen%374EE5050320
//	********************************************************************************
//
//	Name: ~DiscreteGen()
//
//	Description: default destructor
//
//	Input parameters: none
//
//	Returns: none
//
//	********************************************************************************
inline DiscreteGen::~DiscreteGen ()
{
  //## begin DiscreteGen::~DiscreteGen%374EE5050320.body preserve=yes
	if (pProbability != NULL )
		delete []  pProbability;
  //## end DiscreteGen::~DiscreteGen%374EE5050320.body
}


//## begin module%36AC5032034D.epilog preserve=yes
} //namespace sch
//## end module%36AC5032034D.epilog


#endif
