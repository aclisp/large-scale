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
//	File Name: Client.h
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
 
#ifndef Client_h
#define Client_h 1


#include "Simulation.h"


using namespace sch;


//	********************************************************************************
//
//	This  class contain the client element //
//	********************************************************************************

class  	Client	 : public SimProcess
{

  public:

  protected:

  private:
	void bodyProcess(void);

};

// mark end of module
#endif
