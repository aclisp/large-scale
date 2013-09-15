//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%370DE9B8005D.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%370DE9B8005D.cm

//## begin module%370DE9B8005D.cp preserve=yes
//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: Simulation Management
//
//	File Name: ControlProcess.cpp
//
//	Description:
//
//	Author:	  Hélio Azevedo
//
//	Creation Date:	03/99
//
//	Visual Source Safe $Revision: 6 $
//
//	Last check in $Date: 14/07/99 8:59 $
//
//	********************************************************************************
//## end module%370DE9B8005D.cp

//## Module: ControlProcess%370DE9B8005D; Pseudo Package body
//## Source file: T:\SimulationManagement\ControlProcess.cpp

//## begin module%370DE9B8005D.additionalIncludes preserve=no
//## end module%370DE9B8005D.additionalIncludes

#include "MemLeak.h"

//## begin module%370DE9B8005D.includes preserve=yes
#if defined(ALPHAEV67_OSF50_CXX62)
#include <pthread.h>
#endif
//## end module%370DE9B8005D.includes

// ControlProcess
#include "ControlProcess.h"
// SimProcess
#include "SimProcess.h"
// SimulationManager
#include "SimulationManager.h"
//## begin module%370DE9B8005D.additionalDeclarations preserve=yes

namespace sch  {
#if defined(ALPHAEV67_OSF50_CXX62)
void *ttt;
#endif
//## end module%370DE9B8005D.additionalDeclarations


// Class ControlProcess 











//## Operation: ControlProcess%370DF7780148
//	********************************************************************************
//
//	Name: ControlProcess(void)
//
//	Description: constructor - initializes local variables.
//
//	Parameters:none
//
//	Returns: none
//
//
//	********************************************************************************
ControlProcess::ControlProcess ()
  //## begin ControlProcess::ControlProcess%370DF7780148.hasinit preserve=no
      : schedulerHandle(NULL),
        schThreadId(0)
  //## end ControlProcess::ControlProcess%370DF7780148.hasinit
  //## begin ControlProcess::ControlProcess%370DF7780148.initialization preserve=yes
//  : pSimulationMan( &SimMan)
  //## end ControlProcess::ControlProcess%370DF7780148.initialization
{
  //## begin ControlProcess::ControlProcess%370DF7780148.body preserve=yes
  //## end ControlProcess::ControlProcess%370DF7780148.body
}


//## Operation: ~ControlProcess%370DF5FA0239
//	********************************************************************************
//
//	Name: ~ControlProcess(void)
//
//	Description: This destructor terminates  the schedulerThread  associated with this simulation.
//
//	Parameters: none
//
//	Returns: none
//
//	Remarks: This is a virtual operator.
//	********************************************************************************
ControlProcess::~ControlProcess ()
{
  //## begin ControlProcess::~ControlProcess%370DF5FA0239.body preserve=yes
	RetCode returnCode; 
	
	if ( (returnCode = terminateControlProcess ()) != SCH_SUCCESS)	 
	// problems wirh terminating the scheduler thread => generates a exception.
	// Note that this exception will be catch by the thread that activates the scheduler thread.
	// When the exception occurs inside the scheduler thread the report is made through attribute
	// fatalError since it can not be raised for another thread.
	throw SchException (returnCode);

  //## end ControlProcess::~ControlProcess%370DF5FA0239.body
}



//## Other Operations (implementation)
//## Operation: startControlProcess%370E0EF401CF
//	********************************************************************************
//
//	Name: RetCode startControlProcess (SimulationManager*  const pSimulationManager , SimProcess& mainProcess )
//
//	Description: Creates a thread for the private method schedulerThread(). Upon the creation, this thread is converted into a fiber.
//	Then, the mainProcess that was received as  parameter  is started.
//
//	Input parameters: SimulationManager*  pSimulationManager //Determines which is the SimulationManager that SimProcess is associated
//	          SimProcess& mainProcess;  // process to be started
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	Remarks: This function is platform-dependent . Only the Simulation Manager is allowed to run this function. Note that we are using
//	the _beginthreadex function instead of the Win32 CreateThread function. This happens bacause the standard C run-time library was
//	not designed for multithread applications then  we have to use new developed libraries.  These new libraries need that the thread
//	have been started with appropriated function.
//
//	********************************************************************************
RetCode ControlProcess::startControlProcess (SimulationManager *pSimulationManager, SimProcess& mainProcess)
{
  //## begin ControlProcess::startControlProcess%370E0EF401CF.body preserve=yes

	// saves mainProcess, this value is necessary at schedulerThread function.
	pMainProcess = &mainProcess;

	// saves pointer to Simulation Manager, this value is necessary at schedulerThread function.
	pSimulationMan = pSimulationManager;

	// start local variables
	clientFiberId = fiberId = NULL;
	fatalError = SCH_SUCCESS;			// There is no fatal error !!!

	// start controlProcess thread
	//	  unsigned long _beginthreadex( void *security, unsigned stack_size, 
	//	  unsigned ( __stdcall *start_address )( void * ), void *arglist, 
	//	  unsigned initflag, unsigned *thrdaddr );

#if defined(ALPHAEV67_OSF50_CXX62)
	int flag;
	pthread_t temp;
	flag = pthread_create(&temp, NULL, (void * (*)(void *)) ControlProcess::schedulerThread, (LPVOID) this);
	fiberIdent[fiberNumber] = temp;
	schThreadId = &fiberIdent[fiberNumber];
	++fiberNumber;
	schedulerHandle = schThreadId;
	if (flag != 0) return SCH_THREAD_NOT_CREATED;
#elif defined(WIN32)
	schedulerHandle  = (HANDLE) _beginthreadex (
			NULL,								// pointer to thread security attributes 
			0,									// initial thread stack size 
			ControlProcess::schedulerThread,	// pointer to thread function 
			( LPVOID) this,						// argument for new thread 
			0,									// creation flags = start execution
			&schThreadId );						// pointer to returned thread identifier
	if (schedulerHandle == NULL) return SCH_THREAD_NOT_CREATED;
#endif

	return SCH_SUCCESS;


  //## end ControlProcess::startControlProcess%370E0EF401CF.body
}

//## Operation: terminateControlProcess%37122DEB0112
//	********************************************************************************
//
//	Name: RetCode terminateControlProcess ()
//
//	Description: If simulation thread (associated with ControlProcess) exist terminates your processing.
//
//	Input parameters: none
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	Remarks: This function is platform-dependent. Only the Simulation Manager is allowed to run this function. Note that we are using
//	the _endthreadex function instead of the Win32 ExitThread function. This happens bacause the standard C run-time library was not
//	designed for multithread applications then  we have to use new developed libraries.  These new libraries need that the thread have
//	been finished  with appropriated function.
//
//	********************************************************************************
RetCode ControlProcess::terminateControlProcess ()
{
  //## begin ControlProcess::terminateControlProcess%37122DEB0112.body preserve=yes

	if (schedulerHandle == NULL) 				  // Already defunct
		return SCH_SUCCESS;

	// The user can call this function from two points:
	//		1. inside the simulation thread
	//		2. outside the simulation thread

	// CASE 1. Inside the simulation thread
#if defined(ALPHAEV67_OSF50_CXX62)
	if (GetCurrentFiber() == schThreadId) {		// is the caller inside the correct thread ? 
		timeElapsed.calculateCount();				// determines the thread execution time
		// if not inside the fiber associated with the controlProcess destroy controlProcess fiber
		if (GetCurrentFiber() != fiberId)
			DeleteFiber(fiberId);
			pthread_exit(ttt);

		// The execution will never reach this point 
		assert (true);
#else
//HOHOHO
	if (GetCurrentThreadId() == schThreadId) {		// is the caller inside the correct thread ? 
		timeElapsed.calculateCount();				// determines the thread execution time
		// if inside thread we do not need more the Thread Handle
		if (CloseHandle (schedulerHandle)== false) 
			return SCH_IMMORTAL_THREAD;
		schedulerHandle = NULL;
		// if not inside the fiber associated with the controlProcess destroy controlProcess fiber
		if (GetCurrentFiber() != fiberId)
			DeleteFiber(fiberId);
		_endthreadex( (UINT) SCH_SUCCESS);			// terminates thread

		// The execution will never reach this point 
		assert (true);
#endif
	}



	// CASE 2. Outside the simulation thread
	timeElapsed.calculateCount(schedulerHandle);	// determines the thread execution time

#if defined(ALPHAEV67_OSF50_CXX62)
#else
	if (TerminateThread (schedulerHandle, (DWORD)SCH_SUCCESS) == false)
		return SCH_IMMORTAL_THREAD;

	// if the thread is defunct we do not need more the Thread Handle
	if (CloseHandle (schedulerHandle)== false) 
		return SCH_IMMORTAL_THREAD;

	// Remarks: see the value of schedulerHandle after the CloseHandle
#endif
	schedulerHandle = NULL;
	return SCH_SUCCESS;

  //## end ControlProcess::terminateControlProcess%37122DEB0112.body
}

//## Operation: schedulerThread%370DF5C9030B
//	********************************************************************************
//
//	Name:unsigned  WINAPI schedulerThread  (mainProcess LPVOID)
//
//	Description:  This method implements a thread that spawns all the fibers presents at this simulation.
//
//
//	Input parameters:pcontrolProcess : points to ControlProcess object associate with this simulation.
//
//	Returns of the thread: Zero - successful
//	                                          Otherwise - error
//
//	Remarks: This function is platform-dependent - the context depends on the platform. The static declarator associated with this
//	method was applied to permit its use as parameter to CreateThread OS  function.
//
//	********************************************************************************
unsigned WINAPI ControlProcess::schedulerThread (LPVOID pControlProcess)
{
  //## begin ControlProcess::schedulerThread%370DF5C9030B.body preserve=yes
	DWORD dwResult = 0;							// stores the return thread value
	LPVOID auxFiberId;
	RetCode retCode;
	SimProcess  * const pSim =  ((ControlProcess * )pControlProcess)->pMainProcess;
	SimProcess * pActualProcess;
	SimProcess *pNewActiveProcess;				// pointer for a new active process

	((ControlProcess * )pControlProcess)->timeElapsed.startCount(); 
	// Convert simulationThread to Fiber
	if ((((ControlProcess * )pControlProcess)->fiberId = ConvertThreadToFiber( NULL )) == NULL) {
		// determines the thread execution time
		((ControlProcess * )pControlProcess)->timeElapsed.calculateCount();
		// save the error code and finished the simulation
		((ControlProcess * )pControlProcess)->fatalError = SCH_ERROR_IN_FIBER_CREATION;
		((ControlProcess * )pControlProcess)->pSimulationMan->endSimulationManager(); 			// terminates simulation
		// The execution never reach this point 
		assert (true);
	}

	// Start the simulation mainProcess	using fiberProcess as a surrounding.
	auxFiberId = CreateFiberEx( 
		0,										// The initial commit size of the stack, in bytes.
		64000,										// The initial reserve size of the stack, in bytes. 
		FIBER_FLAG_FLOAT_SWITCH,					// the floating-point state is switched for the fiber.
		SimProcess::fiberProcess,					// pointer to fiber function 
		 (LPVOID) pSim);	// argument for new fiber

	if (auxFiberId == NULL)	{
		// determines the thread execution time
		((ControlProcess * )pControlProcess)->timeElapsed.calculateCount();

		// save the error code and finished the simulation
		((ControlProcess * )pControlProcess)->fatalError = SCH_ERROR_IN_FIBER_CREATION;
		retCode = ((ControlProcess * )pControlProcess)->pSimulationMan->endSimulationManager(); 			// terminates simulation
		// The execution never reach this point 
		assert (true);
	}

	//initialize mainProcess attributes
	pSim-> initialize ( 0,									// activationTime = 0;
						Active,								// state = Active
						auxFiberId,							// fiberId = auxFiberId
						(ControlProcess *) pControlProcess,	// pControlProcess = pControlProcess
						1,									// processId = 1
						"MainProcess",						// process name
						false								// defines the destroy flag, the MainProcess
															// destruction is user responsability 
						);
	// go to simulation mainProcess execution
	SwitchToFiber( auxFiberId);			

	// All active fiber before defunct must pass by this point
	for (;;) {									// loop for ever waiting child dead
		// before suicide the fiber set its id at controlProcess.clientFiberId then
		// this method operates under the correct id.
		auxFiberId = ((ControlProcess *)pControlProcess)->clientFiberId;
 		DeleteFiber( auxFiberId); 
		((ControlProcess *) pControlProcess)->clientFiberId = NULL;	// signal that client fiber was attendanced
		((ControlProcess *) pControlProcess)->pSimulationMan->countTerminateProc();

		// sets the state of the ActiveProcess as Terminated
		pActualProcess = ((ControlProcess *) pControlProcess)->pSimulationMan->pActiveProcess;
		if ((retCode = pActualProcess->setState(Terminated)) != SCH_SUCCESS) {
		    // The execution never reach this point because actually setState always returns SCH_SUCCESS
		    assert (true);
		}

		// call the SimulationManager to obtain  another simulation process	
		retCode = ((ControlProcess *) pControlProcess)->pSimulationMan->updateActiveProcess(pNewActiveProcess);
		if (retCode != SCH_SUCCESS)	{
		    // determines the thread execution time
		    ((ControlProcess * )pControlProcess)->timeElapsed.calculateCount();

			// save the error code and finished the simulation
			((ControlProcess * )pControlProcess)->fatalError = retCode;
		    retCode = ((ControlProcess * )pControlProcess)->pSimulationMan->endSimulationManager(); 			// terminates simulation
		    // The execution never reach this point 
		    assert (true);
		}

		SwitchToFiber( pNewActiveProcess->getFiberId());			// go execute a new process
			
	}											// end loop for ever
		 
	return (dwResult);							// end of thread

  //## end ControlProcess::schedulerThread%370DF5C9030B.body
}

//## Operation: setClientFiberId%3711FDA50339
//	********************************************************************************
//
//	Name: void setClientFiberId ()
//
//	Description: Update the clientFiberId with a new fiber Id requesting a ControlProcess service. The  clientFiberId must be NULL for
//	the correct operation. After  the end of ControlProcess service the method has the worring of set this attribute to NULL.
//
//	Input parameters: fiberIdent = Id of a client fiber
//
//	Returns:none
//
//	Remarks: This function is platform-dependent - the context depends on the platform. Only the Simulation Manager is allowed to run
//	this function.
//
//	********************************************************************************
void ControlProcess::setClientFiberId (LPVOID fiberIdent)
{
  //## begin ControlProcess::setClientFiberId%3711FDA50339.body preserve=yes
	// clientFiberId must be NULL, else we have trouble
	assert (clientFiberId == NULL);

	clientFiberId = fiberIdent;

  //## end ControlProcess::setClientFiberId%3711FDA50339.body
}

//## Operation: isEnvironmentValid%37132FA502E0
//	********************************************************************************
//
//	Name: LPVOID getFiberId  ()
//
//	Description:Returns if the environment is valid. Basically, this routine must be used before call any method of Simulation
//	Management class. The objective is verify if the caller is running in the appropriate environment. In the case of Windows NT is
//	verified if the current thread is the simulation thread and if the current fiber is active.
//
//	Input parameters: current : stores the current simulation process
//
//	Returns: true: if the environment is ok
//	                false: otherwise
//
//	Remarks: This function is platform-dependent - the context depends on the platform. Only the Simulation Manager is allowed to run
//	this function.
//
//	********************************************************************************
bool ControlProcess::isEnvironmentValid (SimProcess *current)
{
  //## begin ControlProcess::isEnvironmentValid%37132FA502E0.body preserve=yes

	if (schedulerHandle == NULL)				// thread not created yet
		return false;

#if defined(ALPHAEV67_OSF50_CXX62)
#else
	if (GetCurrentThreadId() != schThreadId)		// is the caller inside the correct thread ? 
		return false;								// invalid thread => returns false
#endif

	if (GetCurrentFiber() != current->getFiberId())	// is the caller inside the correct fiber ?
		return false;								// invalid fiber => returns false

	return true;

  //## end ControlProcess::isEnvironmentValid%37132FA502E0.body
}

//## Operation: createContext%3716279A0017
//	********************************************************************************
//
//	Name: RetCode createContext ( SimProcess& simProcess, Ulong process,  bool destroy )
//
//	Description: Creates a new context and associates it to this process.
//
//	Input parameters: simProcess : SimProcess associated with this simulation
//	                          process : identifier for this process.
//	                          name: process name
//	                          destroy: Determines if the object associate with process must be destroy at the end of process lifetime.
//
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	Remarks: This function is platform-dependent - the context depends on the platform. Only the Simulation Manager is allowed to run
//	this function.
//
//	********************************************************************************
RetCode ControlProcess::createContext (SimProcess& simProcess, Ulong process, string name, bool destroy)
{
  //## begin ControlProcess::createContext%3716279A0017.body preserve=yes
	DWORD dwResult = 0;
	LPVOID fiberIdent;



	// Create the simulation process using fiberProcess as a surrounding.
	fiberIdent = CreateFiberEx( 
		0,									// The initial commit size of the stack, in bytes.
		64000,									// The initial reserve size of the stack, in bytes. 
		FIBER_FLAG_FLOAT_SWITCH,				// the floating-point state is switched for the fiber.
		SimProcess::fiberProcess,				// pointer to fiber function 
		(PVOID) &simProcess );					// argument for new fiber
	
	if (fiberIdent == NULL)
		return SCH_ERROR_IN_FIBER_CREATION;

	// initialize others SimProcess attributes. Note that this initialization can be
	// done after the fiber creation because all fiber runs under the same thread
	// then the risk of lose the control before the end of initialization does not exist.
	simProcess.initialize ( 0,				// activationTime = 0;
						Passive,				// state = Passive
						fiberIdent,				// fiberId = FiberIdent
						this,					// serverFiberId = controlProcess
						process,				// processId = process
						name,					// processName = name
						destroy);				// destroyFlag = destroy	

	
	return SCH_SUCCESS;
  //## end ControlProcess::createContext%3716279A0017.body
}

//## Operation: goToControlProcess%37177A14032B
//	********************************************************************************
//
//	Name: RetCode goToControlProcess(void)
//
//	Description: Switchesto ControlProcess contex.
//
//	Input parameters:none
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	Remarks: This function is platform-dependent - the context depends on the platform. Only the Simulation Manager is allowed to run
//	this function.
//
//	********************************************************************************
RetCode ControlProcess::goToControlProcess ()
{
  //## begin ControlProcess::goToControlProcess%37177A14032B.body preserve=yes

	// 	 Avoid switching to the current fiber 
	if (GetCurrentFiber() == fiberId)	
		return SCH_SUCCESS;								

	// go to ControlProcess execution
	SwitchToFiber( fiberId);			

	return SCH_SUCCESS;

  //## end ControlProcess::goToControlProcess%37177A14032B.body
}

//## Operation: killProcess%378B6C700234
//	********************************************************************************
//
//	Name:RetCode killProcess ( SimProcess& simProcess )
//
//	Description: If in the appropriate environment, kill the process suplied as parameter.
//
//
//	Input parameters: simProcess : process that must be killed.
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	Remarks: This function is platform-dependent - the context depends on the platform. Only the Simulation Manager is allowed to run
//	this function.
//
//	********************************************************************************
RetCode ControlProcess::killProcess (SimProcess& simProcess)
{
  //## begin ControlProcess::killProcess%378B6C700234.body preserve=yes

	// This function is only valid if called from inside the simulation thread

	if (GetCurrentThreadId() == schThreadId)		// is the caller inside the correct thread ? 
		DeleteFiber(simProcess.fiberId);

	return SCH_SUCCESS;



  //## end ControlProcess::killProcess%378B6C700234.body
}

// Additional Declarations
  //## begin ControlProcess%370DE9B8005D.declarations preserve=yes
  //## end ControlProcess%370DE9B8005D.declarations

//## begin module%370DE9B8005D.epilog preserve=yes
}	// end of sch namespace

//## end module%370DE9B8005D.epilog
