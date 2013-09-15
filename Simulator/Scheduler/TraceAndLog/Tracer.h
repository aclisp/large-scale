//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%36AC46670254.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%36AC46670254.cm

//## begin module%36AC46670254.cp preserve=yes
//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: Trace and Log
//
//	File Name: Tracer.h
//
//	Description:   Header file for the Tracer class.
//
//	Author:	 Eugenio Pacheli Alves / Hélio Azevedo
//
//	Creation Date: Jun/24/99
//
//	Visual Source Safe $Revision: 4 $
//
//	Last check in $Date: 29/06/99 15:56 $
//
//	********************************************************************************
//## end module%36AC46670254.cp

//## Module: Tracer%36AC46670254; Pseudo Package specification
//## Source file: T:\TraceAndLog\Tracer.h

#ifndef Tracer_h
#define Tracer_h 1

//## begin module%36AC46670254.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%36AC46670254.additionalIncludes

//## begin module%36AC46670254.includes preserve=yes
#include <fstream>    // iostreams template classes that manipulate external files
#include <string>	  // template class that implements a string container
#include <time.h>	  // convertion between various time and date formats

//## end module%36AC46670254.includes

// FileSupport
#include "FileSupport.h"
//## begin module%36AC46670254.additionalDeclarations preserve=yes
//## end module%36AC46670254.additionalDeclarations


//## begin Tracer%36AC46670254.preface preserve=yes

using namespace std;

namespace sch {

//## end Tracer%36AC46670254.preface

//## Class: Tracer%36AC46670254
//	********************************************************************************
//
//	This class supports debugging trace facility. It aids developers to debug the code by placing specific messages in a debug trace
//	file. It is possible to define different kinds of tracing which may be independently turned on or off, making it possible to the
//	developer to filter out groups of messages of same nature (kind). The user of this class first chooses the trace file name, along
//	with the kinds to be set. Then, all that needs to be done in order to record a trace in the file is to pass on the message and its
//	kind. This class may have several instances, thus allowing several categories of trace, each of them placed in a separate file.
//
//	Each debug trace file has a maximum size set when a Tracer object is instantiated. When the trace file is full, another file is
//	created to resume recording traces. Again, when the second trace file is full, a third file is created.And so on, until the maximum
//	number of files (supplied at construction time) is reached. Finally when the last file is full, recording wraps to the first file,
//	erasing its previous contents. This allows for a large amount of trace information to be recorded. Those files are named by adding
//	a numbered suffix to the supplied file name; e.g., if the supplied file name is "trace.trc" then the actual names are "trace1.trc",
//	"trace2.trc" and "trace3.trc".
//
//	Trace messages may carry timestamp information (number of seconds elapsed since tracing has started). This setting is static: it is
//	set at the constructor and cannot be toggled during the trace object lifetime.
//
//	********************************************************************************
//## Category: TraceAndLog%369E33F80313
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class Tracer : public FileSupport  //## Inherits: <unnamed>%3769488F02C9
{
  //## begin Tracer%36AC46670254.initialDeclarations preserve=yes
  //## end Tracer%36AC46670254.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: Tracer%36B5F71E034D
      Tracer (const string& trcFileName, Uint16 kindSet, bool bTimestamp, Uint trcMaxFile = 1, Uint trcFileSize = MAX_VALUE_UINT);

    //## Destructor (specified)
      //## Operation: ~Tracer%36DFC86100AC
      ~Tracer ();


    //## Other Operations (specified)
      //## Operation: isActive%36B5FD600134
      bool isActive (Uint16 kind);

      //## Operation: resetKind%36B5FCB10287
      void resetKind (Uint16 newKind);

      //## Operation: setKind%36B5FBDA015B
      void setKind (Uint16 newKind);

      //## Operation: trace%36B5FA8D006C
      RetCode trace (Uint16 kind, const string& traceMsg);

    // Additional Public Declarations
      //## begin Tracer%36AC46670254.public preserve=yes
      //## end Tracer%36AC46670254.public

  protected:
    // Additional Protected Declarations
      //## begin Tracer%36AC46670254.protected preserve=yes
      //## end Tracer%36AC46670254.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: initTime%370130DB0013
      //	Time at which tracing started. It is used as reference - all timestamp recorded in trace messages indicate time elapsed since init
      //	time. If timestamp is not enabled then initTime = 0.
      //## begin Tracer::initTime%370130DB0013.attr preserve=no  private: time_t {UA} 
      time_t initTime;
      //## end Tracer::initTime%370130DB0013.attr

      //## Attribute: kindMask%36B5F58703AC
      //	Bit mask indicating state (on/off) of each kind. The LSB corresponds to KIND 0; the MSB corresponds to KIND 15. Bit set indicates
      //	kind turned on; bit reset indicates kind turned off.
      //## begin Tracer::kindMask%36B5F58703AC.attr preserve=no  private: Uint16 {UA} kindSet
      Uint16 kindMask;
      //## end Tracer::kindMask%36B5F58703AC.attr

    // Additional Private Declarations
      //## begin Tracer%36AC46670254.private preserve=yes
      //## end Tracer%36AC46670254.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin Tracer%36AC46670254.implementation preserve=yes
      //## end Tracer%36AC46670254.implementation

};

//## begin Tracer%36AC46670254.postscript preserve=yes
//## end Tracer%36AC46670254.postscript

// Class Tracer 


//## Other Operations (inline)
//## Operation: isActive%36B5FD600134
//	********************************************************************************
//
//	Name: bool isActive(Uint16 kind)
//
//	Description: This function indicates whether tracing is enabled for a particular kind.
//
//	Input parameters: Uint16 kind;  //kind to be checked
//
//	Returns: TRUE if kind is set; FALSE otherwise.
//
//	Remarks: Only one bit (the one related to the kind to be checked) should be set in the kind parameter. If more than one is set in
//	kind parameter, this function returns TRUE only if all requested kinds are set. If more than one kind needs to be checked
//	individually, the best approach is to check each one at a time.
//
//	********************************************************************************
inline bool Tracer::isActive (Uint16 kind)
{
  //## begin Tracer::isActive%36B5FD600134.body preserve=yes
	//
	// return true only if all kinds set in kind are also set in kindMask
	//
	return (kind & kindMask ? true : false);

  //## end Tracer::isActive%36B5FD600134.body
}

//## Operation: resetKind%36B5FCB10287
//	********************************************************************************
//
//	Name: void resetKind(Uint16 newKind)
//
//	Description: Turns passed kinds off. The passed kinds are bit-masked with the existing kindMask. The bits in newKind parameter are
//	logically NEGATIVE-ANDed with the existing kindMask, that is, this operation does not set any kind already reset.
//
//	Input parameters: Uint16 newKind;  //kinds to be turned OFF
//
//	Returns: none
//
//	Remarks: The bits corresponding to the kinds to be reset must be ON in the newKind parameter. The others must be OFF.
//
//	********************************************************************************
inline void Tracer::resetKind (Uint16 newKind)
{
  //## begin Tracer::resetKind%36B5FCB10287.body preserve=yes
	//
	// reset bits by performing an AND operation between kindMask and negative-newKind
	//
	kindMask &= ~newKind;

  //## end Tracer::resetKind%36B5FCB10287.body
}

//## Operation: setKind%36B5FBDA015B
//	********************************************************************************
//
//	Name: void setKind(Uint16 newKind)
//
//	Description: Turns passed kinds on. The passed kinds are bit-masked with the existing kindMask. The bits in newKind parameter are
//	logically ORed with the existing kindMask, that is, this operation does not reset any kind already set.
//
//	Input parameters: Uint16 newKind;  //kinds to be turned ON
//
//	Returns: none
//
//	********************************************************************************
inline void Tracer::setKind (Uint16 newKind)
{
  //## begin Tracer::setKind%36B5FBDA015B.body preserve=yes
	//
	// set bits by performing an OR operation between kindMask and newKind
	//
	kindMask |= newKind;

  //## end Tracer::setKind%36B5FBDA015B.body
}

//## begin module%36AC46670254.epilog preserve=yes

} // namespace sch

//## end module%36AC46670254.epilog


#endif
