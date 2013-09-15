//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%37499EF703E2.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%37499EF703E2.cm

//## begin module%37499EF703E2.cp preserve=no
//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: FunctionEvaluator
//
//	File Name: FunctionEvaluator.cpp
//
//	Description:
//
//	Author:
//
//	Creation Date:
//
//	Visual Source Safe $Revision: $
//
//	Last check in $Date: $
//
//	********************************************************************************
//## end module%37499EF703E2.cp

//## Module: FunctionEvaluator%37499EF703E2; Pseudo Package body
//## Source file: T:\FunctionEvaluator\FunctionEvaluator.cpp

//## begin module%37499EF703E2.additionalIncludes preserve=no
//## end module%37499EF703E2.additionalIncludes

//## begin module%37499EF703E2.includes preserve=yes
#include "MemLeak.h"
#include <math.h>

//## end module%37499EF703E2.includes

// FunctionEvaluator
#include "FunctionEvaluator.h"
//## begin module%37499EF703E2.additionalDeclarations preserve=yes

namespace sch {

//## end module%37499EF703E2.additionalDeclarations


// Class FunctionEvaluator 


//## Other Operations (implementation)
//## Operation: gamma%3749AACA01A0
//	********************************************************************************
//
//	Name:Float gamma( Float arg)
//
//	Description:Returns gamma  function using number as argument. The calculus of the gamma function is obtained using the
//	approximation derived by Lanczos.
//
//	Parameters: arg: argument for the function , must be > 0 and <= 142.9 (for double with 8 bytes)
//
//	Returns: The result of the Gamma function applied to argument parameter
//	           SIM_HUGE_VAL - in the case of invalid argument.
//
//	Remarks: This function is static to allow easy access from any point in the code. All internal calculation are made in double
//	precision to avoid overflow.
//	********************************************************************************
Float FunctionEvaluator::gamma (Float arg)
{
  //## begin FunctionEvaluator::gamma%3749AACA01A0.body preserve=yes

	// local variables

	const Double PI_SQUARED_ROOT =  2.5066282746310;	// equal to squared root of 2*PI
	const Double c[] ={  1.000000000178,				// c0 const
						76.180091729406,				// c1 const
					   -86.505320327112,				// c2 const
					    24.014098222230,				// c3 const
					   - 1.231739516140,				// c4 const
					     0.001208580030,				// c5 const
					   - 0.000005363820					// c6 const
						};

	Double z = arg - 1.0;
	Double base = z + 5.50;			// base = z + gamma + 1/2 => base = z + 5.0 +0.5
	Double expoent = z + 0.5;		// expoent = z + 1/2
	Double sum = c[0];				// result of the sum: c0 + c1/(z+1) +  ... c6/(z+6)

	// The gamma function is only defined for >0
	if ((arg <= 0) || (arg > 142.9))
		return SIM_HUGE_VAL;
  
	for (Int i=1; i <= 6 ; i++) sum += c[i]/(z+(Float)i);

	return (pow(base, expoent) * exp(-base) * (PI_SQUARED_ROOT * sum));

  //## end FunctionEvaluator::gamma%3749AACA01A0.body
}

// Additional Declarations
  //## begin FunctionEvaluator%37499EF703E2.declarations preserve=yes
  //## end FunctionEvaluator%37499EF703E2.declarations

//## begin module%37499EF703E2.epilog preserve=yes

} // namespace sch

//## end module%37499EF703E2.epilog
