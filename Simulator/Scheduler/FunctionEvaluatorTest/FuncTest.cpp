//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: Function Evaluator
//
//	File Name: FuncTest.cpp
//
//	Description: This module implements the first Function Evaluator test.
//               The test objectives consist in allow the user to select a function
//				 from the package and test its use.
//				 The following  Function Evaluator methods are exercised:
//					- Gamma
//
//	Author:	 Helio azevedo
//
//	Creation Date:	25/05/99
//
//	Visual Source Safe $Revision: 2 $
//
//	Last check in $Date: 10/04/99 2:30p $
//
//	********************************************************************************

// include files
#include "MemLeak.h"
#include <iostream>
#include <windows.h>
#include "FunctionEvaluator.h"
#include <memory>
#include <math.h>

// constants



using namespace std;
using namespace sch;


// declarates basic objects associated with the current function
RetCode error;

//	Handle several math errors caused by abnormal operation in
//  Function Evaluator package. When this happens, _matherr only 
//  reports the error and follows the normal processing.

Int _matherr( struct _exception *except )
{
	cout << endl << "ERROR IN:" << 	except->name << " with " << except->arg1 << endl;

    return 0;    /*  use the default actions */    
}

//
//*			BEGIN OF MAIN  FUNCTION
//


	
int main (void) {
    Double funcArgument;	// argument for the function
	Double result;			// function result
	Int    userResponse;

	DumpMemoryLeaks();

	try {					// begin of try bock for exception handling

		// determine the upper and lower boundaries

		cout << "\n Lets locate the upper and lower boundary for the gamma function \n";
		for (funcArgument = 0.5;;funcArgument += 0.01) {

			result  = FunctionEvaluator::gamma( funcArgument );
			if (errno) break;

			cout << "Result of gamma(" << funcArgument << ")= ";
			cout << result << endl;
		}
		cout << "\n\nMaximum function argument = " << funcArgument << endl;

		cout << "\n\n Now control is with the user...\n\n";
		for (;;) {
			do {
				cout << "Do you want to calculate Gamma function? (1=Yes;2=No)\n";
				cin >> userResponse;
				} while ((userResponse != 1) && (userResponse != 2));

			if (userResponse == 2) break;

			cout << "\n\nEnter with gamma function argument: ";
			cin >> funcArgument;

			result  = FunctionEvaluator::gamma( funcArgument );

			cout << "\nResult of gamma(" << funcArgument << ")= ";
			cout << result << endl;
		}

	

		return 0;								// finish execution

	}						// end of try block for exception handling

	// handlers for possible exceptions, note that only exceptions occured in application 
	// context will catch here. Exceptions occured in the simulation context will be 
	// reported by the getStatistics function.
		
	catch (SchException& exc) {
		error = exc.getError();
		cout << endl << "Occur a scheduler exception number " << error << endl;
	}

	catch (exception& exc) {
		cout << endl << "Occur a STL exception because " << exc.what()<< endl;
	}

	return error;
		

}
