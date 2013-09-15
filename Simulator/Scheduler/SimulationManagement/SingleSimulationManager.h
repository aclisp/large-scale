//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%373186A80310.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%373186A80310.cm

//## begin module%373186A80310.cp preserve=yes
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
//	File Name: SingleSimulationManager.h
//
//	Description:
//
//	Author:	Hélio azevedo
//
//	Creation Date: May/06/99
//
//	Visual Source Safe $Revision: 2 $
//
//	Last check in $Date: 13/05/99 9:25 $
//
//	********************************************************************************
//## end module%373186A80310.cp

//## Module: SingleSimulationManager%373186A80310; Pseudo Package specification
//## Source file: T:\SimulationManagement\SingleSimulationManager.h

#ifndef SingleSimulationManager_h
#define SingleSimulationManager_h 1

//## begin module%373186A80310.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%373186A80310.additionalIncludes

//## begin module%373186A80310.includes preserve=yes
//## end module%373186A80310.includes

// SimulationManager
#include "SimulationManager.h"
//## begin module%373186A80310.additionalDeclarations preserve=yes

namespace sch  {

//## end module%373186A80310.additionalDeclarations


//## begin SingleSimulationManager%373186A80310.preface preserve=yes
//## end SingleSimulationManager%373186A80310.preface

//## Class: SingleSimulationManager%373186A80310
//	********************************************************************************
//
//	This class is a generalization for Simulation Manager.  The idea is supply a singleton interface to attend applications that use
//	only one simulation control.
//
//	This class uses the Singleton pattern, which ensures that the class has only one instance (accessed from anywhere int the code)  by:
//	  - providing a global point of access to the class - the instance() operation,
//	  - protecting its constructor to ensure that only one instance is created (see description of the instance() operation).
//
//	********************************************************************************
//## Category: SimulationManagement%369E33A90107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class SingleSimulationManager : public SimulationManager  //## Inherits: <unnamed>%3731912500EA
{
  //## begin SingleSimulationManager%373186A80310.initialDeclarations preserve=yes
  //## end SingleSimulationManager%373186A80310.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: instance%37318DD902E5
      static SingleSimulationManager * instance ();

      //## Operation: clear%3731A898013C
      static void clear ();

    // Additional Public Declarations
      //## begin SingleSimulationManager%373186A80310.public preserve=yes
      //## end SingleSimulationManager%373186A80310.public

  protected:
    //## Constructors (generated)
      SingleSimulationManager();

    //## Destructor (generated)
      ~SingleSimulationManager();

    // Additional Protected Declarations
      //## begin SingleSimulationManager%373186A80310.protected preserve=yes
      //## end SingleSimulationManager%373186A80310.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: pInstance%37318A5400F5
      //	Static pointer to the unique instance of this class.
      //## begin SingleSimulationManager::pInstance%37318A5400F5.attr preserve=no  private: static SingleSimulationManager * {UA} NULL
      static SingleSimulationManager *pInstance;
      //## end SingleSimulationManager::pInstance%37318A5400F5.attr

    // Additional Private Declarations
      //## begin SingleSimulationManager%373186A80310.private preserve=yes
      //## end SingleSimulationManager%373186A80310.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin SingleSimulationManager%373186A80310.implementation preserve=yes
      //## end SingleSimulationManager%373186A80310.implementation

};

//## begin SingleSimulationManager%373186A80310.postscript preserve=yes
//## end SingleSimulationManager%373186A80310.postscript

// Class SingleSimulationManager 

inline SingleSimulationManager::SingleSimulationManager()
  //## begin SingleSimulationManager::SingleSimulationManager%373186A80310_const.hasinit preserve=no
  //## end SingleSimulationManager::SingleSimulationManager%373186A80310_const.hasinit
  //## begin SingleSimulationManager::SingleSimulationManager%373186A80310_const.initialization preserve=yes
  //## end SingleSimulationManager::SingleSimulationManager%373186A80310_const.initialization
{
  //## begin SingleSimulationManager::SingleSimulationManager%373186A80310_const.body preserve=yes
  //## end SingleSimulationManager::SingleSimulationManager%373186A80310_const.body
}


inline SingleSimulationManager::~SingleSimulationManager()
{
  //## begin SingleSimulationManager::~SingleSimulationManager%373186A80310_dest.body preserve=yes
  //## end SingleSimulationManager::~SingleSimulationManager%373186A80310_dest.body
}



//## Other Operations (inline)
//## Operation: instance%37318DD902E5
//	********************************************************************************
//
//	Name:SingleSimulationManager * instance(void)
//
//	Description: Returns the unique instance of this class. This operator uses lazy initialization: it creates the instance when it is
//	first accessed, stores it in pInstance attribute and returns it. Later accesses to instance() just return the value of pInstance.
//	This implementation, combined to the protected constructor, ensures that this class is created and initialized before (and only at
//	that time) its first use.
//
//	Parameters: none
//
//	Returns: the value of the pInstance attribute
//
//	Remarks: this operator is static since it is the global point of access to this class (clients access SingleSimulationManager
//	exclusively through this operator).
//
//	********************************************************************************
inline SingleSimulationManager * SingleSimulationManager::instance ()
{
  //## begin SingleSimulationManager::instance%37318DD902E5.body preserve=yes

		if (pInstance == NULL)
	{
		pInstance = new SingleSimulationManager;
	}
	return (pInstance);


  //## end SingleSimulationManager::instance%37318DD902E5.body
}

//## Operation: clear%3731A898013C
//	********************************************************************************
//
//	Name:void clea(void)
//
//	Description: Makes a orderly destruction for the SingleSimulationManager object. Note that this class is protected. This means that
//	a common user can not use this method. Once a class is singleton does not make sense destroy it because every pointer spawned by
//	instance method will became invalid. Resume: This method exist only to indicate that will not be used !!!
//
//	Parameters: none
//
//	Returns: None
//
//	Remarks: this operator is static since it is the global point to destry  this class.
//
//	********************************************************************************
inline void SingleSimulationManager::clear ()
{
  //## begin SingleSimulationManager::clear%3731A898013C.body preserve=yes
	if (pInstance != NULL) {
		delete pInstance;
		pInstance = NULL;
	}

  //## end SingleSimulationManager::clear%3731A898013C.body
}

//## begin module%373186A80310.epilog preserve=yes
}	// namespace sch
//## end module%373186A80310.epilog


#endif
