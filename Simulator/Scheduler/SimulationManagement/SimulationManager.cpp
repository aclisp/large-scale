//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%369E34A700DA.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%369E34A700DA.cm

//## begin module%369E34A700DA.cp preserve=yes
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
//	File Name: SimulationManager.cpp
//
//	Description:
//
//	Author:	 Hélio Azevedo
//
//	Creation Date:	 03/99
//
//	Visual Source Safe $Revision: 11 $
//
//	Last check in $Date: 14/07/99 9:00 $
//
//	********************************************************************************
//## end module%369E34A700DA.cp

//## Module: SimulationManager%369E34A700DA; Pseudo Package body
//## Source file: T:\SimulationManagement\SimulationManager.cpp

//## begin module%369E34A700DA.additionalIncludes preserve=no
//## end module%369E34A700DA.additionalIncludes

//## begin module%369E34A700DA.includes preserve=yes
//## end module%369E34A700DA.includes

// SimulationManager
#include "SimulationManager.h"
//## begin module%369E34A700DA.additionalDeclarations preserve=yes
namespace sch  {

//## end module%369E34A700DA.additionalDeclarations


// Class SimulationManager 











//## Operation: SimulationManager%36FF7B0B0131
//	********************************************************************************
//
//	Name:SimulationManager(void)
//
//	Description: Default constructor -initializes systemSimulationTime.
//
//	Parameters: none
//
//	Returns: none
//
//
//	********************************************************************************
SimulationManager::SimulationManager ()
  //## begin SimulationManager::SimulationManager%36FF7B0B0131.hasinit preserve=no
      : systemSimulationTime(0.0),
        totalProcess(0),
        totalProcessTerminated(0),
        running(false)
  //## end SimulationManager::SimulationManager%36FF7B0B0131.hasinit
  //## begin SimulationManager::SimulationManager%36FF7B0B0131.initialization preserve=yes
  , pActiveProcess (NULL)
  //## end SimulationManager::SimulationManager%36FF7B0B0131.initialization
{
  //## begin SimulationManager::SimulationManager%36FF7B0B0131.body preserve=yes
  //## end SimulationManager::SimulationManager%36FF7B0B0131.body
}


//## Operation: ~SimulationManager%3729B761039B
//	********************************************************************************
//
//	Name:~SimulationManager(void)
//
//	Description: Destructor for SimulationManager objects.
//	Parameters: none
//
//	Returns: none
//
//
//	********************************************************************************
SimulationManager::~SimulationManager ()
{
  //## begin SimulationManager::~SimulationManager%3729B761039B.body preserve=yes
	endSimulationManager();
  //## end SimulationManager::~SimulationManager%3729B761039B.body
}



//## Other Operations (implementation)
//## Operation: startSimulationManager%36ADEBC101B6
//	********************************************************************************
//
//	Name: RetCode startSimulationManager (SimProcess& mainProcess )
//
//	Description: Initializes the Simulation Management and all resources necessary to maintain the main process.
//
//	Input parameters: SimProcess& mainProcess;  //process responsible for controlling the main flow of events in the simulation
//	application.
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	********************************************************************************
RetCode SimulationManager::startSimulationManager (SimProcess& mainProcess)
{
  //## begin SimulationManager::startSimulationManager%36ADEBC101B6.body preserve=yes

	RetCode returnCode;

	// mainProcess will running under another thread, then we have to update
	// total process now because the present thread may loose control.
	if (running )					// check if SimulationManager is running
	   return SCH_SIMULATION_ALREADY_RUNNING;

	pActiveProcess = &mainProcess;			// hopping that all be well

	systemSimulationTime = 0.0;
	totalProcess = 1;					// the mainProcess must be started here !!
	totalProcessTerminated = 0;

	// start process lists
	eventList.startProcessList(this);
	passiveList.startProcessList(this);

    // create a new thread in the user process. This thread must control all fibers
	// associate with a simulation process.
	returnCode = controlProcess.startControlProcess(this, mainProcess);

	if (returnCode != SCH_SUCCESS)
		totalProcess = 0;
	else
		running = true;
		
	
	return returnCode;

  //## end SimulationManager::startSimulationManager%36ADEBC101B6.body
}

//## Operation: endSimulationManager%36ADECA20278
//	********************************************************************************
//
//	Name: RetCode endSimulationManager(void)
//
//	Description: Finishes the Simulation Management by destroying the processes and resources associated to the Simulation Management.
//
//	Parameters: none
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	********************************************************************************
RetCode SimulationManager::endSimulationManager ()
{
  //## begin SimulationManager::endSimulationManager%36ADECA20278.body preserve=yes
	SimProcess *pProcess;

	// if the SimulationManager is not running  we do not need proceed

	if ( ! running) return SCH_SUCCESS;

	// updates running attribute allowing a new simulation cicle
	running = false;

	// check  if caller is the active process. In this case, the caller is deliberating  
	// stopping its execution, thus the totalProcessTerminated must be appropriately updated 
	if (controlProcess.isEnvironmentValid(pActiveProcess)) {	 
		totalProcessTerminated++;
		pActiveProcess->setState(Terminated);			// go to Terminated state
	}
	
		if (pActiveProcess->getDestroyFlag())	// if indicated at creation time, destroys the Sim Proc
			delete pActiveProcess;					// destroy the  Process
		else
		   pActiveProcess->setState(NonInitialized);	// go back to original state

	// clear eventList 
	while ((pProcess = eventList.removeHead()) != NULL)	 {
		controlProcess.killProcess (*pProcess);	// dealocates all environment structures associated with process
		if (pProcess->getDestroyFlag())			// if indicated at creation time, destroys the Sim Proc
			delete pProcess;					// destroy the  Process
		else
		   pProcess->setState(NonInitialized);	// go back to original state
	}
	

	// clear passiveList 
	while ((pProcess = passiveList.removeHead()) != NULL) 	{
		controlProcess.killProcess (*pProcess);	// dealocates all environment structures associated with process
		if (pProcess->getDestroyFlag())			// if indicated at creation time, destroys the Sim Proc
			delete pProcess;					// destroy the  Process
		else
		   pProcess->setState(NonInitialized);	// go back to original state
	}
	
	// finish the simulation execution
	return controlProcess.terminateControlProcess();


  //## end SimulationManager::endSimulationManager%36ADECA20278.body
}

//## Operation: getStatistics%371F53FB007D
//	********************************************************************************
//
//	Name: RetCode getStatistics (Statistics& staResp)
//
//	Description: Returns the statistics associated with the current  simulation.
//
//	Parameters: statResp: struct with the statistics resume.
//
//	 Returns: Zero - successful
//	         Otherwise - error
//
//	********************************************************************************
RetCode SimulationManager::getStatistics (Statistics& statResp)
{
  //## begin SimulationManager::getStatistics%371F53FB007D.body preserve=yes
	statResp.running = 	running;
	statResp.simulationTime = systemSimulationTime;
	statResp.timeElapsed = controlProcess.getElapsedTime();
	statResp.totalProcessCreated = totalProcess;
	statResp.totalProcessTerm = totalProcessTerminated;
	statResp.fatalError = controlProcess.getFatalError();
	return SCH_SUCCESS;

  //## end SimulationManager::getStatistics%371F53FB007D.body
}

//## Operation: startProcess%36ADEC7E0276
//	********************************************************************************
//
//	Name: RetCode startProcess(SimProcess& newProcess, string name, bool destroy )
//
//	Description: Initializes all resources necessary to maintain a process during its lifetime. Basically, sets the process state to
//	Passive and inserts one in the passiveList.
//
//	Input parameters: SimProcess& newProcess;  //new process to be started
//	         name: process name
//	                       name: Identifies the process for external world.
//	                       destroy: Determines if the object associate with process must be destroy at the end of process lifetime.
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	********************************************************************************
RetCode SimulationManager::startProcess (SimProcess& newProcess, string name, bool destroy)
{
  //## begin SimulationManager::startProcess%36ADEC7E0276.body preserve=yes

	RetCode retCode;

  	// check if controlProcess is already initialized and if caller is the active process
	if (!controlProcess.isEnvironmentValid(pActiveProcess))	 
		return SCH_INVALID_ENVIRONMENT;

	// only NonInitialized process can be started
	if (newProcess.getState() != NonInitialized)
		return SCH_INVALID_STATE;

	// start new process at the passive state
	totalProcess++;
	retCode=controlProcess.createContext(newProcess, totalProcess, name, destroy);

	// insert new process in the passiveList
	if (retCode == SCH_SUCCESS)					// creation of the context ok!
			  passiveList.insertHead(newProcess);
	else
		totalProcess--;
	
	return retCode;


  //## end SimulationManager::startProcess%36ADEC7E0276.body
}

//## Operation: terminateProcess%36D16E0400FD
//	********************************************************************************
//
//	Name: RetCode terminateProcess(SimProcess& targetProcess)
//
//	Description: Remove the process from the ProcessList where it is inserted in and deallocates its resources.
//
//	Input parameters: SimProcess& targetProcess;  //process to be terminated
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	********************************************************************************
RetCode SimulationManager::terminateProcess (SimProcess &targetProcess)
{
  //## begin SimulationManager::terminateProcess%36D16E0400FD.body preserve=yes

	RetCode retCode;

	// check if controlProcess is already initialized and if caller is the active process
	if (!controlProcess.isEnvironmentValid(pActiveProcess))	 
		return SCH_INVALID_ENVIRONMENT;

	//checks targetProcess state	
	switch (targetProcess.getState()) {
		case Active:
			if (eventList.isEmpty())				// must exist one process to actived
				return SCH_EMPTY_EVENT_LIST;

			// sets the state as Terminated
			if ((retCode = targetProcess.setState(Terminated)) != SCH_SUCCESS)
				return retCode;

 			// The current context belongs to the activeProcess. Then it is necessary
			// go to another context before destroy the activeProcess.
			// The controlProcess will be used for this purpose. This one will free
			// all structures of the targetProcess and activate the process	present
			// at the head of eventList.  Do not forget do setting the client fiber as
			// input for the controlProcess (like a message to another process)

			controlProcess.setClientFiberId(targetProcess.fiberId);

			retCode = controlProcess.goToControlProcess();
			if (retCode != SCH_SUCCESS)
				return retCode;

			// The execution never reach this point 
			assert (true);

			break;

		case Passive:
		case Scheduled:
			// the total process terminated is explicitly updated only for Passive
			// and Scheduled states. For the Active state this operation must be
			// executed by the Control Process.
			totalProcessTerminated++;		

			// remove the targetProcess from the eventList
			if ((retCode = eventList.remove(targetProcess)) != SCH_SUCCESS)
				return retCode;

			// sets the state as Terminated
			if ((retCode = targetProcess.setState(Terminated)) != SCH_SUCCESS)
				return retCode;

			// free context associate with targetProcess
			if ((retCode = targetProcess.freeContext()) != SCH_SUCCESS)
				return retCode;
			
			// if indicated at creation time, destroys the Simulation Process
			if (targetProcess.getDestroyFlag())
			delete &targetProcess;				// destroy the  Process
			break;

		case Terminated:		// Already in appropriated state
			break;

		case NonInitialized:
			return SCH_INVALID_STATE;
			break;

	}

	return SCH_SUCCESS;

  //## end SimulationManager::terminateProcess%36D16E0400FD.body
}

//## Operation: terminateProcess%3729DD2503E3
//	********************************************************************************
//
//	Name: RetCode terminateProcess()
//
//	Description: Terminates the current active process.
//
//	Input parameters: none
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	********************************************************************************
RetCode SimulationManager::terminateProcess ()
{
  //## begin SimulationManager::terminateProcess%3729DD2503E3.body preserve=yes
	return terminateProcess (*pActiveProcess);
  //## end SimulationManager::terminateProcess%3729DD2503E3.body
}

//## Operation: activate%36ADEC05018C
//	********************************************************************************
//
//	Name: RetCode activate(SimProcess& targetProcess)
//
//	Description: Activates the target process.
//
//	Input parameters: SimProcess& targetProcess;  //process to be activated
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	********************************************************************************
RetCode SimulationManager::activate (SimProcess& targetProcess)
{
  //## begin SimulationManager::activate%36ADEC05018C.body preserve=yes
	ProcessState state;
	RetCode retCode;
	SimProcess *pOldActiveProcess = pActiveProcess;		// saves present active process

	// check if controlProcess is already initialized and if caller is the active process
	if (!controlProcess.isEnvironmentValid(pActiveProcess))	 
		return SCH_INVALID_ENVIRONMENT;

	//checks if targetProcess is in valid state
	state = targetProcess.getState();
	if (state != Scheduled  &&  state != Passive) 
		return SCH_INVALID_STATE;

	// put the old active process in the head of EventList, keeping its activation time
	eventList.insertHead (*pOldActiveProcess);

	// changes the state of the old active process to "scheduled"
	if ((retCode = pOldActiveProcess->setState(Scheduled)) != SCH_SUCCESS)
		return retCode;

	// remove the targetProcess from its present list
	if (state == Scheduled) {			
		 if ((retCode = eventList.remove(targetProcess)) != SCH_SUCCESS)
			 return retCode;
	}
	else											// have to be in passiveList
		 if ((retCode = passiveList.remove(targetProcess)) != SCH_SUCCESS)
			 return retCode;

	// updates activation time of the target process
	targetProcess.setActivationTime(systemSimulationTime);

	 // the state of the target process is set to "active"
	if ((retCode = targetProcess.setState(Active)) != SCH_SUCCESS)
		return retCode;

	// Control is transferred to the target process
	pActiveProcess = &targetProcess;
	return pOldActiveProcess-> switchContext(targetProcess);
	



  //## end SimulationManager::activate%36ADEC05018C.body
}

//## Operation: passivate%36ADED120188
//	********************************************************************************
//
//	Name: RetCode passivate (void)
//
//	Description: Moves the active process to the passive state.
//
//	Input parameters:none
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	********************************************************************************
RetCode SimulationManager::passivate ()
{
  //## begin SimulationManager::passivate%36ADED120188.body preserve=yes

	return passivate(*pActiveProcess);
	
  //## end SimulationManager::passivate%36ADED120188.body
}

//## Operation: passivate%3737135901ED
//	********************************************************************************
//
//	Name: RetCode passivate (SimProcess& targetProcess)
//
//	Description: Moves the target process to the passive state.
//
//	Input parameters: SimProcess& targetProcess;  //process to be moved to the passive state
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	********************************************************************************
RetCode SimulationManager::passivate (SimProcess& targetProcess)
{
  //## begin SimulationManager::passivate%3737135901ED.body preserve=yes

	RetCode retCode;
	SimProcess *pOldActiveProcess = pActiveProcess;
	

  	// check if controlProcess is already initialized and if caller is the active process
	if (!controlProcess.isEnvironmentValid(pActiveProcess))	 
		return SCH_INVALID_ENVIRONMENT;

	//checks targetProcess  state
	switch (targetProcess.getState())	 {
		case Active:
			if (eventList.isEmpty())				// must exist one process to actived
				return SCH_EMPTY_EVENT_LIST;
			// put the target process in the head of passiveList and changes its state
			passiveList.insertHead (targetProcess);

			if ((retCode = targetProcess.setState(Passive)) != SCH_SUCCESS)
				return retCode;

 			// Control is transferred to the head process of the eventList
			pActiveProcess = eventList.removeHead();

			assert ( systemSimulationTime <= pActiveProcess->getActivationTime()); //time always grows up
			systemSimulationTime = pActiveProcess->getActivationTime();
			if ((retCode = pActiveProcess->setState(Active)) != SCH_SUCCESS)
				return retCode;
			if ((retCode = pOldActiveProcess->switchContext(*pActiveProcess)) != SCH_SUCCESS)
				return retCode;
			break;

		case Scheduled:
			eventList.remove(targetProcess);  // remove target process from the eventList

			// put the target process in the head of passiveList and changes its state
			passiveList.insertHead (targetProcess);

			if ((retCode = targetProcess.setState(Passive)) != SCH_SUCCESS)
				return retCode;
			break;

		case Passive:								// OK!! => already in Passive state				
			break;

		case NonInitialized:
		case Terminated: 
			return SCH_INVALID_STATE;
			break;
	}

	return SCH_SUCCESS;

 
  //## end SimulationManager::passivate%3737135901ED.body
}

//## Operation: schedule%36ADED1F00DD
//	********************************************************************************
//
//	Name: RetCode schedule (SimProcess& targetProcess, const Clause& position)
//
//	Description: Schedules a passive process or re-schedules a scheduled process at the user requested position in the Event List. The
//	exact position is defined by the passed clause.
//
//	Input parameters: SimProcess& targetProcess; //process to be scheduled
//	                  const Clause& position;    //position to insert the process in the Event List
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	********************************************************************************
RetCode SimulationManager::schedule (SimProcess& targetProcess, const Clause& position)
{
  //## begin SimulationManager::schedule%36ADED1F00DD.body preserve=yes

		SimProcess *pOldActiveProcess = pActiveProcess;
		RetCode retCode;

  	// check if controlProcess is already initialized and if caller is the active process
	if (!controlProcess.isEnvironmentValid(pActiveProcess))	 
		return SCH_INVALID_ENVIRONMENT;

	//checks targetProcess state
	switch (targetProcess.getState())	 {

		case Passive:						
			// remove target process from the passiveList
			if ((retCode = passiveList.remove(targetProcess)) != SCH_SUCCESS)
				return retCode;
			break;

		case Scheduled:
			// remove target process from the eventList
			if ((retCode = eventList.remove(targetProcess)) != SCH_SUCCESS)
				return retCode;
			break;
		
		case Active:
		case NonInitialized:
		case Terminated: 
			return SCH_INVALID_STATE;
			break;
	}

	// put the target process at the appropriated position in the passiveList and changes its state
	if ((retCode = eventList.insert(targetProcess, position)) != SCH_SUCCESS)
		return retCode;

	return targetProcess.setState(Scheduled);


  //## end SimulationManager::schedule%36ADED1F00DD.body
}

//## Operation: hold%36ADED0702A5
//	********************************************************************************
//
//	Name: RetCode hold (const Clause& position)
//
//	Description: Suspends the execution of an active process and schedules it at the user requested position in the Event List. The
//	exact position is defined by the passed clause.
//
//	Input parameters: const Clause& position;  //position to insert the process in the Event List
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	********************************************************************************
RetCode SimulationManager::hold (const Clause& position)
{
  //## begin SimulationManager::hold%36ADED0702A5.body preserve=yes

	RetCode retCode;
	SimProcess *pOldActiveProcess = pActiveProcess;

	// check if controlProcess is already initialized and if caller is the active process
	if (!controlProcess.isEnvironmentValid(pActiveProcess))	 
		return SCH_INVALID_ENVIRONMENT;

	// insert the active process in the eventList using clause position
	if ((retCode = eventList.insert(*pActiveProcess, position)) != SCH_SUCCESS)
		return retCode;

 	// changes the state of the old active process to "scheduled"
	if ((retCode = pActiveProcess->setState(Scheduled)) != SCH_SUCCESS)
		return retCode;

	// Control is transferred to the head process of the eventList
	pActiveProcess = eventList.removeHead();	// at least the old active process is there

	assert ( systemSimulationTime <= pActiveProcess->getActivationTime()); //time always grows up
	systemSimulationTime = pActiveProcess->getActivationTime();


	if ((retCode = pActiveProcess->setState(Active)) != SCH_SUCCESS)
		return retCode;

	// active new Active Process and returns
	return pOldActiveProcess->switchContext(*pActiveProcess);
		
  //## end SimulationManager::hold%36ADED0702A5.body
}

//## Operation: updateActiveProcess%370CB53903A6
//	********************************************************************************
//
//	Name: RetCode updateActiveProcess ( (SimProcess*)& pNewActive)
//
//	Description: Remove the active process and activate the process present at the EventLIst head.
//
//
//	Input parameters: pNewActive = returns a pointer for the new active process
//
//	Returns: Zero - successful
//	         Otherwise - error
//
//	********************************************************************************
RetCode SimulationManager::updateActiveProcess (SimProcess * &pNewActive)
{
  //## begin SimulationManager::updateActiveProcess%370CB53903A6.body preserve=yes

	RetCode retCode;

	if (eventList.isEmpty())				// must exist at least one process to actived
		return SCH_EMPTY_EVENT_LIST;

	// sets the state of activeProcesss as Terminated
	if ((retCode = pActiveProcess->setState(Terminated)) != SCH_SUCCESS)
		return retCode;

	// if indicated at creation time, destroys the Simulation Process
	if (pActiveProcess->getDestroyFlag())
		delete pActiveProcess;				// destroy the Old Active Process

	// Control is transferred to the head process of the eventList
	pActiveProcess = eventList.removeHead();

	// uppdate new Simulation Time
	assert ( systemSimulationTime <= pActiveProcess->getActivationTime()); //time always grows up
	systemSimulationTime = pActiveProcess->getActivationTime();

	// prepare pNewActive for the return
	pNewActive = pActiveProcess;

	return pActiveProcess->setState(Active);

  //## end SimulationManager::updateActiveProcess%370CB53903A6.body
}

// Additional Declarations
  //## begin SimulationManager%369E34A700DA.declarations preserve=yes
  //## end SimulationManager%369E34A700DA.declarations

//## begin module%369E34A700DA.epilog preserve=yes
}	// namespace sch

//## end module%369E34A700DA.epilog
