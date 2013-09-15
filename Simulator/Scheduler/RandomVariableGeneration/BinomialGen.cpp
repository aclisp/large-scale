//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36D1660E01B1.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36D1660E01B1.cm

//## begin module%36D1660E01B1.cp preserve=yes
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
//	File Name: BinomialGen.cpp
//
//	Description: Implementation file for the BinomialGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: Apr/16/1999
//
//	Visual Source Safe $Revision: 7 $
//
//	Last check in $Date: 7/06/99 14:04 $
//
//	********************************************************************************
//## end module%36D1660E01B1.cp

//## Module: BinomialGen%36D1660E01B1; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\BinomialGen.cpp

//## begin module%36D1660E01B1.additionalIncludes preserve=no
//## end module%36D1660E01B1.additionalIncludes

//## begin module%36D1660E01B1.includes preserve=yes
#include "MemLeak.h"
#include <math.h>
//## end module%36D1660E01B1.includes

// BinomialGen
#include "BinomialGen.h"
//## begin module%36D1660E01B1.additionalDeclarations preserve=yes
namespace sch {
//## end module%36D1660E01B1.additionalDeclarations


// Class BinomialGen 




//## Operation: BinomialGen%36D166D40292
//	********************************************************************************
//
//	Name: BinomialGen(Ulong seed, Ulong numSamples, Float probability)
//
//	Description: Non-default constructor - requires the initial seed, the number of samples and the probability of occurrence of each
//	sample to generate the random variables.
//
//	Input parameters: Ulong seed;        //seed to begin the random number generation
//	                  Ulong numSamples;  //number of samples
//	                  Float probability; //probability of occurrence of each sample
//
//	Returns: none
//
//	Remarks: must invoke the DiscreteDistribution constructor passing the value of the seed parameter.
//	         The seed value must be: 0 <= seed <= MAX_WORD; the probability value must be: 0 < probability < 1;  and numSamples > 0.
//
//	********************************************************************************
BinomialGen::BinomialGen (Ulong seed, Ulong numSamples, Float probability)
  //## begin BinomialGen::BinomialGen%36D166D40292.hasinit preserve=no
      : p(probability),
        r(numSamples)
  //## end BinomialGen::BinomialGen%36D166D40292.hasinit
  //## begin BinomialGen::BinomialGen%36D166D40292.initialization preserve=yes
    , DiscreteDistribution (seed)
  //## end BinomialGen::BinomialGen%36D166D40292.initialization
{
  //## begin BinomialGen::BinomialGen%36D166D40292.body preserve=yes
	RetCode rc = SCH_SUCCESS;

	if ((p <= 0) || (p >= 1) || (r <= 0))
	{
		rc = SCH_INVALID_PARAMETER; 
	}

	if (rc == SCH_SUCCESS)
	{
		if ((p > 0) && (p <= 0.5))
		{
			w = pow (1-p, r);
		}
		else if ((p > 0.5) && (p < 1))
		{
			w = pow (p, r);
		}
		else
		{
			rc = SCH_INVALID_PARAMETER;
		}
	}

	if (rc == SCH_SUCCESS)
	{
		if ((w <= 0) || (w >= 1))
		{
			rc = SCH_INVALID_PARAMETER;
		}
	}
	
	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);
	}
  //## end BinomialGen::BinomialGen%36D166D40292.body
}



//## Other Operations (implementation)
//## Operation: nextValue%36D1665102A8
//	********************************************************************************
//
//	Name: RetCode nextValue(Int *pValue)
//
//	Description: Returns the next value of the Binomial distribution.
//
//	Output parameters: Int *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode BinomialGen::nextValue (Int *pValue)
{
  //## begin BinomialGen::nextValue%36D1665102A8.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	Float q;
	Float b;
	Float c;
	Float binomial;
	Float number;
	Double a;
	Double d;

	if (p <= 0.5)
	{
		q = p;
	}
	else
	{
		q = 1 - p;
	}
	
	a = w;
	b = q / (1 - q);
	c = (r + 1) * b;
	d = w;
	binomial = 0;
	
	rc = getNextValue (&number);

	if (rc == SCH_SUCCESS)
	{
		while ((number > a) && (a <= 0.9999999999999999) )
		{
			binomial ++;
			d = d * (c / binomial - b);
			a += d;
		}

		if (p <= 0.5)
		{
			*pValue = binomial;
		}
		else
		{
			binomial = r - binomial;
			*pValue = binomial;
		}
	}

	return rc;
  //## end BinomialGen::nextValue%36D1665102A8.body
}

//## Operation: redefine%374E8B82015D
//	********************************************************************************
//
//	Name: RetCode redefine( Ulong numSamples, Float probability)
//
//	Description: Redefines the parameters suplied at construction time. As the  constructor,  requires the number of samples and the
//	probability of occurrence of each sample to generate the random variables.
//
//	Input parameters: Ulong numSamples;  //number of samples
//	                  Float probability; //probability of occurrence of each sample
//
//	Returns:SCH_SUCCESS
//	                error code in case of error
//
//	Remarks: the probability value must be: 0 < probability < 1;  and numSamples > 0.
//
//	********************************************************************************
RetCode BinomialGen::redefine (Ulong numSamples, Float probability)
{
  //## begin BinomialGen::redefine%374E8B82015D.body preserve=yes

	if ((probability <= 0) || (probability >= 1) || (numSamples <= 0))
		return SCH_INVALID_PARAMETER; 

	w = (probability <= 0.5) ? pow(1-probability, numSamples) : pow(probability, numSamples);

	if ((w <= 0) || (w >= 1))
		return SCH_INVALID_PARAMETER;

	p = probability;
	r = numSamples;					
	
	return SCH_SUCCESS;

  //## end BinomialGen::redefine%374E8B82015D.body
}

// Additional Declarations
  //## begin BinomialGen%36D1660E01B1.declarations preserve=yes
  //## end BinomialGen%36D1660E01B1.declarations

//## begin module%36D1660E01B1.epilog preserve=yes
} //namespace sch
//## end module%36D1660E01B1.epilog
