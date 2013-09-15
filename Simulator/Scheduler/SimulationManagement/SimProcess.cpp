//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%369E357E00ED.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%369E357E00ED.cm

//## begin module%369E357E00ED.cp preserve=yes
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
//	File Name: SimProcess.cpp
//
//	Description:
//
//	Author:	 Hélio Azevedo
//
//	Creation Date:	03/99
//
//	Visual Source Safe $Revision: 7 $
//
//	Last check in $Date: 14/06/99 16:23 $
//
//	********************************************************************************
//## end module%369E357E00ED.cp

//## Module: SimProcess%369E357E00ED; Pseudo Package body
//## Source file: T:\SimulationManagement\SimProcess.cpp

//## begin module%369E357E00ED.additionalIncludes preserve=no
//## end module%369E357E00ED.additionalIncludes
#include "MemLeak.h"
//## begin module%369E357E00ED.includes preserve=yes
#if defined (ALPHAEV67_OSF50_CXX62)
#include <pthread.h>
#endif
//## end module%369E357E00ED.includes

// ControlProcess
#include "ControlProcess.h"
// SimProcess
#include "SimProcess.h"
// ProcessList
#include "ProcessList.h"
//## begin module%369E357E00ED.additionalDeclarations preserve=yes
namespace sch  {

//------------------------------------------------------------------------------------
// Define some variables required for thread synchr. on UNIX.
// We use 20 such variables for now. The simulator uses only 8 of them at this time.
//------------------------------------------------------------------------------------
#if defined (ALPHAEV67_OSF50_CXX62)
static int fiberPred[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static pthread_cond_t  fiberCond[20] = { PTHREAD_COND_INITIALIZER,
											PTHREAD_COND_INITIALIZER,
											PTHREAD_COND_INITIALIZER,
											PTHREAD_COND_INITIALIZER,
											PTHREAD_COND_INITIALIZER,
											PTHREAD_COND_INITIALIZER,
											PTHREAD_COND_INITIALIZER,
											PTHREAD_COND_INITIALIZER,
											PTHREAD_COND_INITIALIZER,
											PTHREAD_COND_INITIALIZER,
											PTHREAD_COND_INITIALIZER,
											PTHREAD_COND_INITIALIZER,
											PTHREAD_COND_INITIALIZER,
											PTHREAD_COND_INITIALIZER,
											PTHREAD_COND_INITIALIZER,
											PTHREAD_COND_INITIALIZER,
											PTHREAD_COND_INITIALIZER,
											PTHREAD_COND_INITIALIZER,
											PTHREAD_COND_INITIALIZER,
											PTHREAD_COND_INITIALIZER};
static pthread_mutex_t fiberMutex[20] = { PTHREAD_MUTEX_INITIALIZER,
											PTHREAD_MUTEX_INITIALIZER,
											PTHREAD_MUTEX_INITIALIZER,
											PTHREAD_MUTEX_INITIALIZER,
											PTHREAD_MUTEX_INITIALIZER,
											PTHREAD_MUTEX_INITIALIZER,
											PTHREAD_MUTEX_INITIALIZER,
											PTHREAD_MUTEX_INITIALIZER,
											PTHREAD_MUTEX_INITIALIZER,
											PTHREAD_MUTEX_INITIALIZER,
											PTHREAD_MUTEX_INITIALIZER,
											PTHREAD_MUTEX_INITIALIZER,
											PTHREAD_MUTEX_INITIALIZER,
											PTHREAD_MUTEX_INITIALIZER,
											PTHREAD_MUTEX_INITIALIZER,
											PTHREAD_MUTEX_INITIALIZER,
											PTHREAD_MUTEX_INITIALIZER,
											PTHREAD_MUTEX_INITIALIZER,
											PTHREAD_MUTEX_INITIALIZER,
											PTHREAD_MUTEX_INITIALIZER};
#endif



//## end module%369E357E00ED.additionalDeclarations


// Class SimProcess 
















//## Operation: SimProcess%36FF7ED70247
//	********************************************************************************
//
//	Name: SimProcess(void)
//
//	Description: constructor - initializes local variables.
//
//	Parameters: SimulationManager& SimMan       //Determines which is the SimulationManager that SimProcess is associated
//
//	Returns: none
//
//
//	********************************************************************************
SimProcess::SimProcess ()
  //## begin SimProcess::SimProcess%36FF7ED70247.hasinit preserve=no
      : state(NonInitialized),
        processId(0),
        pNext(NULL),
        pPrior(NULL),
        destroyFlag(false)
  //## end SimProcess::SimProcess%36FF7ED70247.hasinit
  //## begin SimProcess::SimProcess%36FF7ED70247.initialization preserve=yes
  //## end SimProcess::SimProcess%36FF7ED70247.initialization
{
  //## begin SimProcess::SimProcess%36FF7ED70247.body preserve=yes
  //## end SimProcess::SimProcess%36FF7ED70247.body
}


//## Operation: ~SimProcess%370D0205011B
//	********************************************************************************
//
//	Name: ~SimProcess(void)
//
//	Description: This destructor terminate the simulation process associated with this object.
//
//	Parameters: none
//
//	Returns: none
//
//	Remarks: This is a virtual operator.
//	********************************************************************************
SimProcess::~SimProcess ()
{
  //## begin SimProcess::~SimProcess%370D0205011B.body preserve=yes

	// if the simulation process associate with this object is not at Terminated
	// or NonInitialized state we can not kill it!!!
	
	if (state == Terminated || state == NonInitialized) return;
	
	// If the simulation has finished we do not have problems
	if (! pControlProcess->pSimulationMan->isRunning()) return;

	// what to do if the involved simulation process is alive ? I do not know !!!
	// I can not call DeleteFiber because I can be in another thread so
	// I decide finish the simulation indicating one error.
	
	pControlProcess->timeElapsed.calculateCount(pControlProcess->schedulerHandle);  // determines the thread execution time
	pControlProcess->setFatalError(SCH_INVALID_DESTRUCTOR);
	pControlProcess->pSimulationMan->endSimulationManager();
	
  //## end SimProcess::~SimProcess%370D0205011B.body
}



//## Other Operations (implementation)
//## Operation: initialize%370E608C0062
//	********************************************************************************
//
//	Name: void initialize (Double activeTime ,  ProcessState sta,LPVOID fId, ControlProcess * pControl, Ulong procId, string name, bool
//	destroy)
//
//	Description:
//	This method initializes all attributes of a SimProcess object.
//
//	Input parameters: activeTime: Activation time of the process
//	                      sta: Current state of the process.
//	                       fId: fiber id associated with the simulator process
//	                       pControl:Maintain a pointer to control process
//	                       procId:Identifies the process inside the scheduler
//	                       name: Identifies the process for external world.
//	                       destroy: Determines if the object associate with process must be destroy at the end of process lifetime.
//
//	Returns: Void
//
//
//	********************************************************************************
void SimProcess::initialize (Double activeTime, ProcessState sta, LPVOID fId, sch::ControlProcess *pControl, Ulong procId, string name, bool destroy)
{
  //## begin SimProcess::initialize%370E608C0062.body preserve=yes
	activationTime = activeTime;
	state =	sta;
	processId =	procId;
	fiberId	 = fId;
	pControlProcess = pControl;
	processName	= name;
	pNext = pPrior = NULL;
	pProcessList = NULL;
	destroyFlag = destroy;

  //## end SimProcess::initialize%370E608C0062.body
}

//## Operation: setActivationTime%36ADEF0300ED
//	********************************************************************************
//
//	Name: RetCode setActivationTime(Double newTime)
//
//	Description: Sets the activation time of the process.
//
//	Input parameters: Double newTime; //new activation time of the process
//
//	Returns: None
//
//	Remarks: Only the Simulation Manager is allowed to change the activation time.
//
//	********************************************************************************
void SimProcess::setActivationTime (Double newTime)
{
  //## begin SimProcess::setActivationTime%36ADEF0300ED.body preserve=yes
		activationTime = newTime;
  //## end SimProcess::setActivationTime%36ADEF0300ED.body
}

//## Operation: setState%36ADEEB3012E
//	********************************************************************************
//
//	Name: RetCode setState(ProcessState newState)
//
//	Description: Changes the current state of the process.
//
//	Input parameters: ProcessState newState;  //new state of the process
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	Remarks: Only the Simulation Manager is allowed to change a process state.
//
//	********************************************************************************
RetCode SimProcess::setState (ProcessState newState)
{
  //## begin SimProcess::setState%36ADEEB3012E.body preserve=yes
	state = newState;
	return SCH_SUCCESS;
  //## end SimProcess::setState%36ADEEB3012E.body
}

//## Operation: fiberProcess%370BA862010E
//	********************************************************************************
//
//	Name:VOID WINAPI fiberProcess (pSimProcess LPVOID)
//
//	Description:
//	This method surrounds the implementation of every simulation process present at the system. In fact, it implements the pre and
//	pos processing necessary for the fiber control.
//
//	Input parameters:pSimProcess  :contain a pointer to the nre simulation process.
//
//	Returns: Void
//
//	Remarks: This function is platform-dependent - the context depends on the platform. Only the Simulation Manager is allowed to run
//	this function. The static declarator associated with this method was applied to permit its use as parameter to CreateFiber OS
//	function.
//
//	********************************************************************************
VOID WINAPI SimProcess::fiberProcess (PVOID pSimProcess)
{
  //## begin SimProcess::fiberProcess%370BA862010E.body preserve=yes


	ControlProcess * const pControl = ((SimProcess *) pSimProcess)->pControlProcess;

#if defined (ALPHAEV67_OSF50_CXX62)
	//-------------------------------------------------
	// Get the index corresponding to the current fiber
	//-------------------------------------------------
	LPVOID temp1;
	temp1 = pthread_self();
	int i=0;
	int done=0;
	while(done==0)
	   {
	   if(temp1==fiberIdent[i])
	      {
	      done=1;
	      }
	   ++i;
	   }
	--i;

	//-------------------------------------------------
	// Loop until the predicate has changed value
	//-------------------------------------------------
	pthread_mutex_lock(&fiberMutex[i]);
	while(fiberPred[i] == 0)
	{
		pthread_cond_wait(&fiberCond[i], &fiberMutex[i]);
	} 
	fiberPred[i] = 0;
	pthread_mutex_unlock(&fiberMutex[i]);
#endif
	
	try {					// begin of try bock for exception handling

		((SimProcess *) pSimProcess)->bodyProcess();			// this function implements the simulation process
	
	}						// end of try block for exception handling

	// handlers for SchException, note that only SchExceptions occured in simulation 
	// context will catch here, others execeptions will abort the thread execution. 
		
	catch (SchException& exc) {
			// save the error code and finished the simulation
			((SimProcess *) pSimProcess)->pControlProcess->fatalError = exc.getError();
		    ((SimProcess *) pSimProcess)->pControlProcess->pSimulationMan->endSimulationManager(); 			// terminates simulation
		    // The execution never reach this point 
		    assert (true);
	}

	catch (...) {
			// If the program reachs this point, then other exception than SchException
			// has occurred inside some bodyProcess. Note that does not exist local treatment
		    // for this case, the only action is make use of an assert to inform the user
		    // (in the debug mode).
		    assert (true);
			throw;
	}

	

	// At the end of simulation process update pControlProcess->fiberId with
	// the identification of this fiber and returns to the pControlProcess.
	// Note that we do not have sincronization problems because all fibers are
	// executing under control of the same thread.
	pControl->setClientFiberId (((SimProcess *) pSimProcess)->fiberId);

	SwitchToFiber( pControl->getFiberId());					// returns to ControlProcess
		
	// The execution never reach this point 
	assert (true);

  //## end SimProcess::fiberProcess%370BA862010E.body
}

//## Operation: switchContext%36CD6B3701A2
//	********************************************************************************
//
//	Name: RetCode switchContext (SimProcess& targetProcess)
//
//	Description: Switches the context to targetProcess.
//
//	Input parameters: SimProcess& targetProcess;  //process to switch context to
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	Remarks: This function is platform-dependent - the context depends on the platform. Only the Simulation Manager is allowed to run
//	this function.
//
//	********************************************************************************
RetCode SimProcess::switchContext (SimProcess& targetProcess)
{
  //## begin SimProcess::switchContext%36CD6B3701A2.body preserve=yes
	// 	 Avoid switching to the current fiber 
#if defined(ALPHAEV67_OSF50_CXX62)
#else
	if (GetCurrentFiber() == targetProcess.fiberId)	
		return SCH_SUCCESS;								
#endif

	// go to targetProcess execution
	SwitchToFiber( targetProcess.fiberId);			

	return SCH_SUCCESS;
  //## end SimProcess::switchContext%36CD6B3701A2.body
}

//## Operation: freeContext%3714B2960265
//	********************************************************************************
//
//	Name: RetCode freeContext ()
//
//	Description: Free current process context.
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
RetCode SimProcess::freeContext ()
{
  //## begin SimProcess::freeContext%3714B2960265.body preserve=yes
#if defined(ALPHAEV67_OSF50_CXX62)
#else
		DeleteFiber(fiberId);
#endif
		return SCH_SUCCESS;
  //## end SimProcess::freeContext%3714B2960265.body
}

//## Operation: getpSimulationManager%3731CE8F00FB
//	********************************************************************************
//
//	Name: SimulationManager * getActivationTime(void)
//
//	Description: Returnsa pointer to the SimulationManager object that controls the simulation.
//
//	Parameters: none
//
//	Returns:pointer to SimulationManager object associated with this process..
//
//	********************************************************************************
SimulationManager * SimProcess::getpSimulationManager ()
{
  //## begin SimProcess::getpSimulationManager%3731CE8F00FB.body preserve=yes

	if (GetCurrentFiber() != fiberId)				// is the caller inside the correct fiber ?
		return NULL;								// invalid fiber => returns NULL

	return (pControlProcess != NULL) ? pControlProcess->pSimulationMan : NULL;

  //## end SimProcess::getpSimulationManager%3731CE8F00FB.body
}

// Additional Declarations
  //## begin SimProcess%369E357E00ED.declarations preserve=yes


//-----------------------------------------------------------
// The following functions implements minimum support for 
// the Microsoft fibers the UNIX plateforms. ttt
//-----------------------------------------------------------
#if defined (ALPHAEV67_OSF50_CXX62)

//*********************************************************************************************
//
//
//
//
//
//*********************************************************************************************
LPVOID ConvertThreadToFiber( LPVOID lpParameter ) 
{

	fiberIdent[fiberNumber]=  pthread_self();
	++fiberNumber;
	return(&fiberIdent[fiberNumber-1]);
}



//*********************************************************************************************
//
//
//
//
//
//*********************************************************************************************
LPVOID CreateFiber( DWORD dwStackSize,              // initial thread stack size, in bytes
                    void (*lpStartAddress)(void*),  // pointer to fiber function
                    LPVOID lpParameter)				// argument for new fiber
{

	pthread_create(&fiberIdent[fiberNumber], NULL, (void * (*) (void*))lpStartAddress, lpParameter);
	++fiberNumber;
	return(&fiberIdent[fiberNumber-1]);
}



//*********************************************************************************************
//
//
//
//
//
//*********************************************************************************************
VOID SwitchToFiber( LPVOID lpFiber )
{
	pthread_t *a;
	a=&fiberIdent[0];
	int done;
	LPVOID currentFiber;
	int i;
	int i2;

	//---------------------------------------
	// Get identity of current running fiber
	//---------------------------------------
	currentFiber = GetCurrentFiber();

	//-------------------------------
	// Check if fiber to run is ours 
	//-------------------------------
	if(lpFiber == currentFiber)
	{
		//--------------------------------------------------------------------------------
		// it is ours -> since we are here, we were already running, continue to do it...
		//--------------------------------------------------------------------------------
	}
   else
   {
		//--------------------------------
		// Find index of thread to start
		//--------------------------------
		i=0;
		done=0;
		while(done==0)
		{
			if(lpFiber==&fiberIdent[i])
			{
			done=1;
			}
			++i;
		}
		--i;

		//----------------------------
		// Find our own thread index
		//----------------------------
		int done2=0;
		i2=0;
		while(done2==0)
		{
			if(currentFiber==&fiberIdent[i2])
			{
			done2=1;
			}
		++i2;
		}
		--i2;

		//-------------------------------------------------------------------
		// Lock our own mutex such that when the other process is started, 
		// it cannot change the predicate before we are ready
		//-------------------------------------------------------------------
		pthread_mutex_lock(&fiberMutex[i2]);

		//-------------------------
		// Start the other thread
		//-------------------------
		pthread_mutex_lock(&fiberMutex[i]);
		fiberPred[i] = 1;
		pthread_cond_signal(&fiberCond[i]);
		pthread_mutex_unlock(&fiberMutex[i]);

		//-------------------------------------------------
		// Loop until our predicate has changed value
		//-------------------------------------------------
		while(fiberPred[i2] == 0)
		{
			pthread_cond_wait(&fiberCond[i2], &fiberMutex[i2]);
		} 
		fiberPred[i2] = 0;
		pthread_mutex_unlock(&fiberMutex[i2]);
	}
}



//*********************************************************************************************
//
//
//
//
//
//*********************************************************************************************
LPVOID GetCurrentFiber(VOID)
{

	pthread_t currentThread;

	currentThread = pthread_self();

	int i=0;
	while(i<fiberNumber)
	{
		if(currentThread == fiberIdent[i])
		{
			return &fiberIdent[i];
		}
	++i;
	}

	//We should never reach this point
	return(NULL);
}



//*********************************************************************************************
//
//
//
//
//
//*********************************************************************************************
VOID DeleteFiber(LPVOID lpfiber)
{

	pthread_t *a;
	a = (pthread_t *) lpfiber;
	pthread_cancel(*a);
}

#endif

  //## end SimProcess%369E357E00ED.declarations
//## begin module%369E357E00ED.epilog preserve=yes
}	// namespace sch

//## end module%369E357E00ED.epilog
