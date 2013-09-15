//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC5028032B.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC5028032B.cm

//## begin module%36AC5028032B.cp preserve=yes
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
//	File Name: BaseGen.h
//
//	Description: Header file for the BaseGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 3 $
//
//	Last check in $Date: 10/04/99 2:34p $
//
//	********************************************************************************
//## end module%36AC5028032B.cp

//## Module: BaseGen%36AC5028032B; Pseudo Package specification
//## Source file: T:\RandomVariableGeneration\BaseGen.h

#ifndef BaseGen_h
#define BaseGen_h 1

//## begin module%36AC5028032B.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36AC5028032B.additionalIncludes

//## begin module%36AC5028032B.includes preserve=yes
//## end module%36AC5028032B.includes

//## begin module%36AC5028032B.additionalDeclarations preserve=yes
namespace sch {
//## end module%36AC5028032B.additionalDeclarations


//## begin BaseGen%36AC5028032B.preface preserve=yes
//## end BaseGen%36AC5028032B.preface

//## Class: BaseGen%36AC5028032B
//	********************************************************************************
//
//	Generates an uniform distribution between two real values according to the Algorithm M (A quite random sequence). Two sequences X
//	and Y are generated using the linear congruential method. Each number in a sequence is calculated by:
//
//	   number = (multiplier * seed + increment) % modulus
//
//	The value of seed changes each time a new number is generated, and is based on the previous calculation. The values of multiplier,
//	increment and modulus are constant and optimized for best sequence period and randomness.
//
//	An auxiliary table V is filled with the first k values of the X-sequence.
//
//	Each random number is calculated by first generating the next member of the X and Y sequences. Then an index j is generated from
//	the Y-sequence:
//
//	   j = (k * Y) / modulusY
//
//	where 0 <= j < k, Y is the next Y-value and modulusY is the modulus used in the Y-sequence.
//
//	The resulting random number is the contents of V[j]. The contents of V[j] is replaced by the next X-value after getting the random
//	number.
//
//	This distribution is used by all non-uniform distributions as a basis for generating their own numbers.
//
//	********************************************************************************
//## Category: RandomVariableGeneration%369E33C50099
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class BaseGen 
{
  //## begin BaseGen%36AC5028032B.initialDeclarations preserve=yes
  //## end BaseGen%36AC5028032B.initialDeclarations

  public:
    // Additional Public Declarations
      //## begin BaseGen%36AC5028032B.public preserve=yes
      //## end BaseGen%36AC5028032B.public

  protected:
    // Additional Protected Declarations
      //## begin BaseGen%36AC5028032B.protected preserve=yes
      //## end BaseGen%36AC5028032B.protected

    //## Constructors (specified)
      //## Operation: BaseGen%36B6DB4B02FE
      BaseGen (Ulong seed = 78562, Float minVal = 0, Float maxVal = 1);

    //## Destructor (specified)
      //## Operation: ~BaseGen%371345CF00C2
      ~BaseGen ();


    //## Other Operations (specified)
      //## Operation: generateX%37022DF1021D
      RetCode generateX ();

      //## Operation: generateY%37022E0D010F
      RetCode generateY ();

      //## Operation: nextValue%36B6D4CD036C
      RetCode nextValue (Float *pValue);

    // Data Members for Class Attributes

      //## Attribute: firstSeedX%36B6CF970007
      //	Value of a seed used to generate the first random number of the X-sequence is supplied by the user in the constructor.
      //## begin BaseGen::firstSeedX%36B6CF970007.attr preserve=no  private: Ulong {UAC} seed
      const Ulong firstSeedX;
      //## end BaseGen::firstSeedX%36B6CF970007.attr

      //## Attribute: firstSeedY%37022A0902F2
      //	Value of a constant seed used to generate the first random number of the Y-sequence. The seed used to generate the first random
      //	number of the X-sequence is supplied by the user in the constructor.
      //## begin BaseGen::firstSeedY%37022A0902F2.attr preserve=no  private: Ulong {UAC} 78562
      const Ulong firstSeedY;
      //## end BaseGen::firstSeedY%37022A0902F2.attr

      //## Attribute: incrementX%36B6D0890253
      //	Increment used in the calculation of the X-sequence. This is an optimized value.
      //## begin BaseGen::incrementX%36B6D0890253.attr preserve=no  private: Ulong {UAC} 1
      const Ulong incrementX;
      //## end BaseGen::incrementX%36B6D0890253.attr

      //## Attribute: incrementY%36E576A60034
      //	Increment used in the calculation of the Y-sequence. This is an optimized value.
      //## begin BaseGen::incrementY%36E576A60034.attr preserve=no  private: Ulong {UAC} 1
      const Ulong incrementY;
      //## end BaseGen::incrementY%36E576A60034.attr

      //## Attribute: indVector%37022CE302C9
      //	Index of the vector.
      //## begin BaseGen::indVector%37022CE302C9.attr preserve=no  private: Int {UA} 
      Int indVector;
      //## end BaseGen::indVector%37022CE302C9.attr

      //## Attribute: lenVector%37022CF7037C
      //	Length of the vector.
      //## begin BaseGen::lenVector%37022CF7037C.attr preserve=no  private: Int {UA} 100
      Int lenVector;
      //## end BaseGen::lenVector%37022CF7037C.attr

      //## Attribute: maxValue%37022CC30006
      //	Upper boundary of the range to generate the random variables.
      //## begin BaseGen::maxValue%37022CC30006.attr preserve=no  private: Float {UAC} maxVal
      const Float maxValue;
      //## end BaseGen::maxValue%37022CC30006.attr

      //## Attribute: minValue%37022C9C0169
      //	Lower boundary of the range to generate the random variables.
      //## begin BaseGen::minValue%37022C9C0169.attr preserve=no  private: Float {UAC} minVal
      const Float minValue;
      //## end BaseGen::minValue%37022C9C0169.attr

      //## Attribute: moduleX%36B6D09F0010
      //	Modulus used in the calculation of the X-sequence. This is an optimized value.
      //## begin BaseGen::moduleX%36B6D09F0010.attr preserve=no  private: Ulong {UAC} 0x10000000
      const Ulong moduleX;
      //## end BaseGen::moduleX%36B6D09F0010.attr

      //## Attribute: moduleY%36E576B1033D
      //	Modulus used in the calculation of the Y-sequence. This is an optimized value.
      //## begin BaseGen::moduleY%36E576B1033D.attr preserve=no  private: Ulong {UAC} 0x04000000
      const Ulong moduleY;
      //## end BaseGen::moduleY%36E576B1033D.attr

      //## Attribute: multiplierX%36B6CFC40264
      //	Multiplier used in the calculation of the X-sequence. This is an optimized value.
      //## begin BaseGen::multiplierX%36B6CFC40264.attr preserve=no  private: Ulong {UAC} 5
      const Ulong multiplierX;
      //## end BaseGen::multiplierX%36B6CFC40264.attr

      //## Attribute: multiplierY%36E5769D0226
      //	Multiplier used in the calculation of the Y-sequence. This is an optimized value.
      //## begin BaseGen::multiplierY%36E5769D0226.attr preserve=no  private: Ulong {UAC} 9
      const Ulong multiplierY;
      //## end BaseGen::multiplierY%36E5769D0226.attr

      //## Attribute: nextSeedX%36B6CFB00388
      //	Seed to be used in the current random number generation of the X-sequence. This value is recalculated each time nextValue() is
      //	called.
      //## begin BaseGen::nextSeedX%36B6CFB00388.attr preserve=no  private: Ulong {UA} 
      Ulong nextSeedX;
      //## end BaseGen::nextSeedX%36B6CFB00388.attr

      //## Attribute: nextSeedY%36EFBAB40145
      //	Seed to be used in the current random number generation of the Y-sequence. This value is recalculated each time nextValue() is
      //	called.
      //## begin BaseGen::nextSeedY%36EFBAB40145.attr preserve=no  private: Ulong {UA} 
      Ulong nextSeedY;
      //## end BaseGen::nextSeedY%36EFBAB40145.attr

      //## Attribute: pV%37022DD5019B
      //	Random number sequence generated based on X-sequence and Y-sequence.
      //## begin BaseGen::pV%37022DD5019B.attr preserve=no  private: Ulong * {UA} 
      Ulong *pV;
      //## end BaseGen::pV%37022DD5019B.attr

      //## Attribute: pX%37022D9B0043
      //	Random number sequence called X-sequence.
      //## begin BaseGen::pX%37022D9B0043.attr preserve=no  private: Ulong * {UA} 
      Ulong *pX;
      //## end BaseGen::pX%37022D9B0043.attr

      //## Attribute: pY%37022DC80319
      //	Random number sequence called Y-sequence.
      //## begin BaseGen::pY%37022DC80319.attr preserve=no  private: Ulong * {UA} 
      Ulong *pY;
      //## end BaseGen::pY%37022DC80319.attr

  private:
    // Additional Private Declarations
      //## begin BaseGen%36AC5028032B.private preserve=yes
      //## end BaseGen%36AC5028032B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin BaseGen%36AC5028032B.implementation preserve=yes
      //## end BaseGen%36AC5028032B.implementation

    friend class BaseDistribution;
};

//## begin BaseGen%36AC5028032B.postscript preserve=yes
//## end BaseGen%36AC5028032B.postscript

// Class BaseGen 

//## begin module%36AC5028032B.epilog preserve=yes
} //namespace sch
//## end module%36AC5028032B.epilog


#endif
