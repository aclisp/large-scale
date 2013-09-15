//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%3739B65002DC.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3739B65002DC.cm

//## begin module%3739B65002DC.cp preserve=yes
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
//	File Name: GammaGen.cpp
//
//	Description: Implementation file for the GammaGen class
//
//	Author: Rosangela Pieroni
//
//	Creation Date: May/12/1999
//
//	Visual Source Safe $Revision: 6 $
//
//	Last check in $Date: 8/24/99 9:49a $
//
//	********************************************************************************
//## end module%3739B65002DC.cp

//## Module: GammaGen%3739B65002DC; Pseudo Package body
//## Source file: T:\RandomVariableGeneration\GammaGen.cpp

//## begin module%3739B65002DC.additionalIncludes preserve=no
//## end module%3739B65002DC.additionalIncludes

//## begin module%3739B65002DC.includes preserve=yes
#include "MemLeak.h"
#include <math.h>
//## end module%3739B65002DC.includes

// GammaGen
#include "GammaGen.h"
//## begin module%3739B65002DC.additionalDeclarations preserve=yes
namespace sch {
//## end module%3739B65002DC.additionalDeclarations


// Class GammaGen 





//## Operation: GammaGen%3739B7B502B7
//	********************************************************************************
//
//	Name: GammaGen(Ulong seed, Float alfaValue, Float betaValue)
//
//	Description: Non-default constructor - requires the initial seed, the alfa and  beta value to generate the random variables.
//
//	Input parameters: Ulong seed;   //seed to begin the random number generation
//	                  Float alfaValue;  //alfa value
//	                  Float betaValue;  //beta value
//
//	Returns: none
//
//	Remarks: must invoke the ContinuousDistribution constructor passing the value of the seed parameter.
//	         The seed value must be: 0 <= seed <= MAX_WORD; alfaValue > 0; and betaValue > 0
//
//	********************************************************************************
GammaGen::GammaGen (Ulong seed, Float alfaValue, Float betaValue)
  //## begin GammaGen::GammaGen%3739B7B502B7.hasinit preserve=no
      : alfa(alfaValue),
        beta(betaValue)
  //## end GammaGen::GammaGen%3739B7B502B7.hasinit
  //## begin GammaGen::GammaGen%3739B7B502B7.initialization preserve=yes
  , ContinuousDistribution (seed)
  //## end GammaGen::GammaGen%3739B7B502B7.initialization
{
  //## begin GammaGen::GammaGen%3739B7B502B7.body preserve=yes
	RetCode rc = SCH_SUCCESS;

	if ((alfa <= 0) || (beta <= 0))
	{
		rc = SCH_INVALID_PARAMETER;
	}

	if (rc != SCH_SUCCESS)
	{
		throw SchException (rc);
	}
  //## end GammaGen::GammaGen%3739B7B502B7.body
}



//## Other Operations (implementation)
//## Operation: nextValue%3739B8720377
//	********************************************************************************
//
//	Name: RetCode nextValue(Float *pValue)
//
//	Description: Returns the next value of the Gamma distribution.
//
//	Output parameters: Float *pValue;  //next number generated
//
//	Returns: SCH_SUCCESS
//	         SCH_ALLOCATION_ERROR
//
//	********************************************************************************
RetCode GammaGen::nextValue (Float *pValue)
{
  //## begin GammaGen::nextValue%3739B8720377.body preserve=yes
	RetCode rc = SCH_SUCCESS;
	const Double e = exp(1);
	bool  bGamma = false;
	Float a;
	Float b;
	Float d;
	Float exponent;
	Float factor;
	Float gamma;
	Float number1;
	Float number2;
	Float p;
	Float q;
	Float teta = 4.5;
	Float v;
	Float w;
	Float y;
	Float z;
	Double ey;
	Double ye;

	if ((alfa > 0) && (alfa <= 1))
	{
		b = (Float) ((e + alfa) / e);

		do
		{
			// limits the value of number1 to:  0 <= number1 < 1
			// otherwise  it can occur	a exception	 at log ((b-p)/alfa)
			do	
				rc = getNextValue (&number1);
			while (number1 == 1.0f);

			if (rc == SCH_SUCCESS)
			{
				p = b * number1;

				if (p <= 1)
				{
					rc = getNextValue (&number1);
					if (rc == SCH_SUCCESS)
					{
						exponent = 1 / alfa;
						y = pow (p, exponent);
						ey = pow (e, -y);
						if (number1 <= ey)
						{
							gamma = y;
							bGamma = true;
						}
					}
					else
					{
						break;
					}
				}
				else
				{
					rc = getNextValue (&number1);
					if (rc == SCH_SUCCESS)
					{
						exponent = alfa-1;
						y = - log ((b-p)/alfa);
						ye = pow (y, exponent);
						if (number1 <= ye)
						{
							gamma = y;
							bGamma = true;
						}
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				break;
			}
		} while (!bGamma);
	}
	else if (alfa > 1)
	{
		a = 1 / sqrt (2 * alfa - 1);
		b = alfa - log (4);
		q = alfa + 1 / a;
		d = 1 + log (teta);

		do 
		{
			// limits the value of number1 to:  0 < number1 < 1
			// otherwise  it can occur	a exception	 at log (number1 / (1 - number1))
			do	
				rc = getNextValue (&number1);
			while (number1 == 0.0f || number1 == 1.0f);

			if (rc == SCH_SUCCESS)
			{

				// limits the value of number2 to:  0 < number2 <= 1
				// otherwise  it can occur	a exception	 at log (z)
				do	
					rc = getNextValue (&number2);
				while (number2 == 0.0f);
				
				if (rc == SCH_SUCCESS)
				{
					v = a * log (number1 / (1 - number1));
					y = alfa * pow(e, v);
					z = number1 * number1 * number2;
					w = b + q * v - y;

					factor = w + d - teta * z;

					if (factor >= 0)
					{
						gamma = y;
						bGamma = true;
					}
					else if (w >= log(z))
					{
						gamma = y;
						bGamma = true;
					}
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		} while (!bGamma);
	}

	*pValue = beta * gamma;

	return rc;
  //## end GammaGen::nextValue%3739B8720377.body
}

// Additional Declarations
  //## begin GammaGen%3739B65002DC.declarations preserve=yes
  //## end GammaGen%3739B65002DC.declarations

//## begin module%3739B65002DC.epilog preserve=yes
} // namespace sch
//## end module%3739B65002DC.epilog
