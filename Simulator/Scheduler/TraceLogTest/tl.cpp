//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: Trace & Log
//
//	File Name: tl.cpp
//
//	Description: This module implements the Trace & Log test.
//
//	Author:	 Eugenio Pacheli Alves / Helio Azevedo
//
//	Creation Date:	jun / 26 /99
//
//	Visual Source Safe $Revision: 3 $
//
//	Last check in $Date: 10/04/99 2:54p $
//
//	********************************************************************************

// include files

#include "trclog.h"

#include <iostream>
#include <exception>
#include <assert.h>
#include <windows.h>
#include <stdio.h>


using namespace std;
using namespace sch;


//
//
//  Function: defineFile
//
//

void defineFile ( string & fileName, Uint & numberFile, Uint & sizeFile , Uint &numberOperations)
{
	Uint output;
	// make the default initializations
	numberFile = 1;
	sizeFile = MAX_VALUE_UINT;


	// Ask the file	name
	cout << endl << "Enter with file name: ";
	cin >> fileName;

	cout << endl << "Do you want define the number of files ? (1=Yes;2=No) "; 
	cin >> output;
	if (output == 1)
	{
		cout << endl << "Enter the number of files: ";
		cin >> numberFile;
	}


	cout << endl << "Do you want define the file size ? (1=Yes;2=No) "; 
	cin >> output;
	if (output == 1)
	{
		cout << endl << "Enter the file size: ";
		cin >> sizeFile;
	}

	cout << endl << "Enter with the number of write operations: ";
	cin >> numberOperations;
	cout << endl;

}

//
//
//  Function: testFileSupport
//
//

RetCode testFileSupport ( void )
{
	Uint			output;								// stores the user response
	string			fileName;							// stores the file name supplied by user
	Uint			fileNumber;							// stores the file number supplied by user
	Uint			fileSize; 							// stores the file size supplied by user
	Uint			numberOperations;					// number of operations
	Uint			counter = 1;						// operations counter
	string	fullMsg;									// aux string to hold whole message
	char	tmpString[60]= "";							// stores the ascii representation of counter
	RetCode rc = SCH_SUCCESS;
	bool	banner;


	// Ask the file	name and if user needs banner
	defineFile (fileName, fileNumber, fileSize, numberOperations);

	cout << endl << "Do you want banners ? (1=Yes;2=No) "; 
	cin >> output;
	banner =  (output == 1)	? true : false;

	// Finally allocates the resource

	FileSupport support(fileName, banner, fileNumber, fileSize);

	// go and test
	
	while (numberOperations-- ) {		
		//
		// defines full message
		//
		sprintf (tmpString, "File Support Test number %d \n", counter);
		fullMsg = tmpString;

		counter++;
		if ((rc = support.write(fullMsg)) != SCH_SUCCESS) 
			return rc;	
	}

	return SCH_SUCCESS;

}


//
//
//  Function: testTrace
//
//

RetCode testTrace ( void )
{
	Uint			output;								// stores the user response
	string			fileName;							// stores the file name supplied by user
	Uint			fileNumber;							// stores the file number supplied by user
	Uint			fileSize; 							// stores the file size supplied by user
	Uint			numberOperations;					// number of operations
	Uint			counter = 1;						// operations counter
	string	fullMsg;									// aux string to hold whole message
	char	tmpString[60]= "";							// stores the ascii representation of counter
	RetCode rc = SCH_SUCCESS;
	bool			setFlag = true;
	//
	// initialize parameters to trace object
	//
	Uint16 setKind = 0x0001;
	Uint16 resKind = 0x0001;
	Uint16 writeKind = 0x0001;
	Uint16 actualKind = 0x0001;

	bool	bTimeStamp;


	// Ask the file	name and if user needs banner
	defineFile (fileName, fileNumber, fileSize, numberOperations);

	cout << endl << "Do you want time stamp ? (1=Yes;2=No) "; 
	cin >> output;
	bTimeStamp =  (output == 1)	? true : false;



	// Finally allocates the resource
	Tracer tracer(fileName, actualKind, bTimeStamp, fileNumber, fileSize);

	// go and test ....
	
	while (numberOperations-- ) {		
		//
		// defines full message
		//
		sprintf (tmpString, "Tracer Test number %d ", counter);
		fullMsg = tmpString;

		//
		// trace - set new kind for each 16 iterations
		//         beginning a new cicle for each 16 * 16 iterations
		//
		if ((counter % 16) == 0)
		{
			actualKind |= setKind;								// updates actualKind
			tracer.setKind (setKind);
			setKind <<= 1;										// shift setKind
			if (actualKind == 0xFFFF) {
				resKind = 0;
				actualKind &= resKind;									// updates actualKind																		
				tracer.resetKind (~resKind);
				setKind = 0x0001;
			}
		}
			

		writeKind =  (writeKind==0) ? 0x0001 : (writeKind << 1);  	// shift writeKind

		//
		// build and write the string
		//
		sprintf (tmpString,  " with kind set in Tracer = %04X \n", actualKind);
		fullMsg += tmpString;

		counter++;
		if ((rc = tracer.trace(writeKind, fullMsg)) != SCH_SUCCESS) 
			return rc;	
	}


	return SCH_SUCCESS;

}

//
//
//  Function: testLog
//
//

RetCode testLog ( void )
{
	Uint			output;								// stores the user response
	string			fileName;							// stores the file name supplied by user
	Uint			fileNumber;							// stores the file number supplied by user
	Uint			fileSize; 							// stores the file size supplied by user
	Uint			numberOperations;					// number of operations
	Uint			counter = 1;						// operations counter
	string			fullMsg;							// aux string to hold whole message
	char			tmpString[90]= "";					// stores the ascii representation of counter
	RetCode			rc = SCH_SUCCESS;
	bool			bStdOutput;							// must the results send to standard output ?
	bool			bActive;							// indicates if the logger is active
	bool			bTime = true;						// indicates if time stamp must be written
	LogLevel		level;								// log level of the message


	// Ask the file	name and if the results goes to the standard output
	defineFile (fileName, fileNumber, fileSize, numberOperations);

	cout << endl << "Do you want that the results also go to screen ? (1=Yes;2=No) "; 
	cin >> output;
	bStdOutput =  (output == 1)	? true : false;

	// Finally allocates the resource
	Logger      *pLogger = NULL;
	pLogger = Logger::instance ();
	pLogger->init (fileName, bStdOutput, fileNumber, fileSize);


	// go and test
	
	while (numberOperations-- ) {		
		//
		// defines full message
		//
		sprintf (tmpString, "Logger Test number %d with log level = ", counter);
		fullMsg = tmpString;


		if ((counter %5) == 0)
		{
			//
			// set/reset active  state for each 5 iterations
			//
			bActive = pLogger->isActive ();    // toggle active state;
			pLogger->setActive (!bActive);
		}

		Int levelState = counter % 3; // set log level according to counter
		switch (levelState) 
		{
			case 0:
				level = Informative;
				break;
			case 1:
				level = Warning;
				break;
			case 2:
				level = Error;
				break;
		}

		bTime = !bTime;						       // toggle bTime

		counter++;

		//
		// build and write the string
		//



		switch (level)
		{
			case Informative:
				fullMsg += "I\n";
				break;		   

			case Warning:
				fullMsg += "W\n";
				break;

			case Error:
				fullMsg += "E\n";
				break;

		}

		if ((rc = pLogger->log (level, bTime, fullMsg)  ) != SCH_SUCCESS) 
			return rc;	
	}							// end while

	pLogger->clear();			// close file and free memory

	return SCH_SUCCESS;

}




void main(void)
{

	// Ask which distribution the user wants generate
	 Uint userResp;
	 RetCode			rc = SCH_SUCCESS;

	try
	{
		cout << endl << "\t\t TRACE AND LOG TEST " << endl << endl;

		do
		{
			cout << endl;
			cout << " 1  - File Support "		<< endl;
			cout << " 2  - Trace "				<< endl;
			cout << " 3  - Log "				<< endl;
			cout << " 4 -  QUIT "				<< endl;

			cout << "Which report strategy do you want test? ";
			cin  >> userResp;	
			cout  << endl;
		}while (( userResp < 1) && ( userResp > 4) );

		switch (userResp)
		{
			case 1:	rc = testFileSupport();
					break;

			case 2: rc = testTrace();
					break;

			case 3: rc = testLog();
					break;
		}

	}
	catch (SchException& ex)
	{
		cerr <<  endl << ex.what() << endl;
		rc = ex.getError();
	}

	catch (exception & ex) {
			// If the program reachs this point, then other exception than SchException
			// has occurred inside some function. Note that does not exist local treatment
		    // for this case, the only action is make use of an assert to inform the user
		    // (in the debug mode) and presents the reason.
		    assert (true);
			cerr << endl << "System Exception: " << ex.what() << endl ; 
	}


	ExitProcess (rc);

}