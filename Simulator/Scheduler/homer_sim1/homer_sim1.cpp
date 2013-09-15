// homer_sim1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


using namespace std;
using namespace sch;


class SimulationSystem : public SimProcess
{
private:
	void bodyProcess();
};


class SimEntity : public SimProcess 
{
private:
	void bodyProcess();
};


void printGlobalStatistics (SimulationManager& simul)
{
	Statistics stat;
	Logger *pLogger = Logger::instance();
	char  tmpMsg[200];
	Double timeElap;

	simul.getStatistics(stat);

	pLogger->log (Informative, true, "\nSimulation Statistics         :");

	if (stat.running) 
		pLogger->log (Informative, false, "     Simulation state        : Running");
	else
		pLogger->log (Informative, false, "     Simulation state        : Stopped");

	sprintf (tmpMsg, "     Simulation Time         : %02f", stat.simulationTime);  
	pLogger->log (Informative, false, tmpMsg);

	timeElap = ((Double) stat.timeElapsed) / 10000.0;					// converts in ms
	sprintf (tmpMsg, "     CPU Simulation Time (ms): %02f", timeElap);
	pLogger->log (Informative, false, tmpMsg);
	
	sprintf (tmpMsg, "     Total Process Created   : %d", stat.totalProcessCreated);  
	pLogger->log (Informative, false, tmpMsg);

	sprintf (tmpMsg, "     Total Process Terminated: %d", stat.totalProcessTerm);  
	pLogger->log (Informative, false, tmpMsg);

	sprintf (tmpMsg, "     Return Code             : %d", stat.fatalError);  
	pLogger->log (Informative, false, tmpMsg);
}


void errorTreatment(string name, RetCode error)
{
	SingleSimulationManager *pSimMgr = SingleSimulationManager::instance();
	Logger *pLogger = Logger::instance();
	char  tmpMsg[200];

	sprintf (tmpMsg, "#Simulation time: %02f : %s ERROR: %d \n", 
			 pSimMgr->getSystemSimulationTime(), name.data(), error);
	pLogger->log (Error, true, tmpMsg);
	
	pSimMgr->endSimulationManager(); // abort the simulation!
}


void SimEntity::bodyProcess()
{
	char maxstack[50000];
}


void SimulationSystem::bodyProcess()
{
	SingleSimulationManager *pSimMgr = SingleSimulationManager::instance();
	RetCode result;

	for (int i=0;i<20000;i++)
	{
		SimEntity *pSimEntity = new SimEntity();
		if ((result = pSimMgr->startProcess(*pSimEntity, "SimEntity", true)) != SCH_SUCCESS)
			errorTreatment(getProcessName(), result);
		
		if ((result = pSimMgr->activate(*pSimEntity)) != SCH_SUCCESS)
			errorTreatment(getProcessName(), result);
	}

	pSimMgr->endSimulationManager(); // simulation exit with SCH_EMPTY_EVENT_LIST unless we metioned this!
}


int _tmain(int argc, _TCHAR* argv[])
{
	DumpMemoryLeaks();

	RetCode result;
	SingleSimulationManager *pSimMgr = SingleSimulationManager::instance();
	Logger *pLogger = Logger::instance();
	char  tmpMsg[200];
	SimulationSystem simulationSystem; // Must instantiate the main sim process!

	if ((result = pLogger->init("homer_sim1.log", true)) != SCH_SUCCESS)
	{
		cout << "Error in pLogger->init: " <<  result << endl;
		return result;
	}
	
	if ((result = pSimMgr->startSimulationManager(simulationSystem)) != SCH_SUCCESS)
	{
		sprintf(tmpMsg, "Error in startSimulationManager: %d \n", result); 
		pLogger->log (Error, true, tmpMsg);
		return result;
	}	

	while (pSimMgr->isRunning())
	{
		Sleep(2000);
	}

	Sleep(5000);
	pSimMgr->endSimulationManager();
	printGlobalStatistics(*pSimMgr);
	pLogger->clear();
	pSimMgr->clear();
	return 0;
}

