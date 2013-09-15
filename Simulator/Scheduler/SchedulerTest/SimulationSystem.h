//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: Scheduler Test
//
//	File Name: SimulationSystem.h
//
//	Description:
//
//	Author:	  Hélio Azevedo
//
//	Creation Date:	May/07/99
//
//	Visual Source Safe $Revision: 1 $
//
//	Last check in $Date: 13/05/99 9:33 $
//
//	********************************************************************************

 
#ifndef SimulationSystem_h
#define SimulationSystem_h 1


#include "Simulation.h"


using namespace sch;




//	********************************************************************************
//
//	A object of this class starts all simulation processes//
//	********************************************************************************

class  SimulationSystem	  : public SimProcess
{

  public:

  protected:

  private:
	void bodyProcess(void);

};

// mark end of module
#endif
