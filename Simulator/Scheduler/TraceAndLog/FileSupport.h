//## begin module%1.3%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.3%.codegen_version

//## begin module%376945E303CA.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%376945E303CA.cm

//## begin module%376945E303CA.cp preserve=yes
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
//	File Name: FileSupport.h
//
//	Description: This class offers support for files treatment. 
//
//	Author:	Hélio Azevedo
//
//	Creation Date: Jun/18/1999
//
//	Visual Source Safe $Revision: 2 $
//
//	Last check in $Date: 29/06/99 15:55 $
//
//	********************************************************************************
//## end module%376945E303CA.cp

//## Module: FileSupport%376945E303CA; Pseudo Package specification
//## Source file: T:\TraceAndLog\FileSupport.h

#ifndef FileSupport_h
#define FileSupport_h 1

//## begin module%376945E303CA.additionalIncludes preserve=no
#include "SchTypes.h"
#include "SchException.h"
//## end module%376945E303CA.additionalIncludes

//## begin module%376945E303CA.includes preserve=yes
#include <fstream>    // iostreams template classes that manipulate external files
#include <string>	  // template class that implements a string container
#include <time.h>	  // convertion between various time and date formats

//## end module%376945E303CA.includes

//## begin module%376945E303CA.additionalDeclarations preserve=yes
	 
using namespace std;

namespace sch {



//## end module%376945E303CA.additionalDeclarations


//## begin FileSupport%376945E303CA.preface preserve=yes
//## end FileSupport%376945E303CA.preface

//## Class: FileSupport%376945E303CA
//	********************************************************************************
//
//	This class offers support for files treatment. The user of this class first chooses the file name. Then, all that needs to be done
//	in order to record data in this file is supply  the information. This class may have several instances, thus allowing several
//	simultaneous files.
//
//	The user can indicate if desires a banner at the beginnig and the end of the file. The banner has the following format :
//	--- <bannerType> at <week day> <month> <day> <hour>:<min>:<sec> <year>  [Sequential number = <number>]
//	 Ex: --- ENDED at Mon Mar 29 08:30:54 1999
//	The sequential number will only be presented in the beginning banner.
//
//	Each file may have a optional maximum size set when a FileSupport  object is instantiated. When the file is full, another file is
//	created to resume file writting. Again, when the second file is full, a third file is created. And so on, until the maximum number
//	of files (supplied at construction time) is reached. Finally when the last file is full, recording wraps to the first file, erasing
//	its previous contents. This allows for a large amount of information to be recorded.  If the default values are used the system
//	allows the presence of only one file with unlimited size.
//
//	 Those files are named by adding a numbered suffix to the supplied file base name; e.g., if the supplied file name is "result.txt"
//	then the actual names are "result1.txt", "result2.txt" and "result3.txt". Note that the "txt"  file extension is preserved being
//	changed only the file base name ("result").
//
//	********************************************************************************
//## Category: TraceAndLog%369E33F80313
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class FileSupport 
{
  //## begin FileSupport%376945E303CA.initialDeclarations preserve=yes
  //## end FileSupport%376945E303CA.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: FileSupport%376946440293
      FileSupport (const string& fileName, bool bBanner, Uint maxFile = 1, Uint fileSize = MAX_VALUE_UINT);

    //## Destructor (specified)
      //## Operation: ~FileSupport%376947A401F9
      virtual ~FileSupport ();


    //## Other Operations (specified)
      //## Operation: write%376946C202B2
      RetCode write (const string& msg);

      //## Operation: GetSeqFileNum%377773520077
      Uint GetSeqFileNum ();

    // Additional Public Declarations
      //## begin FileSupport%376945E303CA.public preserve=yes
      //## end FileSupport%376945E303CA.public

  protected:
    //## Constructors (specified)
      //## Operation: FileSupport%37723B4C03CF
      FileSupport ();


    //## Other Operations (specified)
      //## Operation: open%3769466C0132
      RetCode open ();

      //## Operation: GetActualName%376A97240376
      const string GetActualName ();

      //## Operation: writeString%376AA056021D
      RetCode writeString (const string& msg);

      //## Operation: init%37723336012A
      RetCode init (const string& fileName, bool bBanner, Uint maxFile = 1, Uint fileSize = MAX_VALUE_UINT);

      //## Operation: close%37724041026B
      RetCode close ();

      //## Operation: active%377283320028
      bool active ();

    // Additional Protected Declarations
      //## begin FileSupport%376945E303CA.protected preserve=yes
      //## end FileSupport%376945E303CA.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: curSize%376946E80126
      //	Current size of working file. Incremented each time a new information is appended in the file.
      //## begin FileSupport::curSize%376946E80126.attr preserve=no  private: Uint {UA} 0
      Uint curSize;
      //## end FileSupport::curSize%376946E80126.attr

      //## Attribute: name%376946ED023C
      //	Original name of file (actual name is <file_base_name><number>.<ext>, where <file_base_name> is the base name supplied by the user,
      //	<number> is the number of the working file, and <ext> is the extension name supplied by the user ).
      //## begin FileSupport::name%376946ED023C.attr preserve=no  private: string {UA} 
      string name;
      //## end FileSupport::name%376946ED023C.attr

      //## Attribute: fileNum%376946F20149
      //	Number of working trace file.
      //## begin FileSupport::fileNum%376946F20149.attr preserve=no  private: Uint {UA} 1
      Uint fileNum;
      //## end FileSupport::fileNum%376946F20149.attr

      //## Attribute: maxSize%376946FE011E
      //	Maximum length allowed for each file. When this limit is reached a new file is used.
      //## begin FileSupport::maxSize%376946FE011E.attr preserve=no  private: Uint {UA} MAX_VALUE_UINT
      Uint maxSize;
      //## end FileSupport::maxSize%376946FE011E.attr

      //## Attribute: maxFileNum%3769470F010E
      //	Maximum number of files in the stream.
      //## begin FileSupport::maxFileNum%3769470F010E.attr preserve=no  private: Uint {UA} 1
      Uint maxFileNum;
      //## end FileSupport::maxFileNum%3769470F010E.attr

      //## Attribute: pFile%376947910129
      //	Pointer to a stream containing information about the file. The stream is instantiated by the constructor.
      //## begin FileSupport::pFile%376947910129.attr preserve=no  private: ofstream * {UA} NULL
      ofstream *pFile;
      //## end FileSupport::pFile%376947910129.attr

      //## Attribute: banner%37713DAF016F
      //	 indicates if banner is need.
      //## begin FileSupport::banner%37713DAF016F.attr preserve=no  private: bool {UA} false
      bool banner;
      //## end FileSupport::banner%37713DAF016F.attr

      //## Attribute: seqFileNum%377773140263
      //	Sequential umber of working trace file.
      //## begin FileSupport::seqFileNum%377773140263.attr preserve=no  private: Uint {UA} 1
      Uint seqFileNum;
      //## end FileSupport::seqFileNum%377773140263.attr

    // Additional Private Declarations
      //## begin FileSupport%376945E303CA.private preserve=yes
      //## end FileSupport%376945E303CA.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin FileSupport%376945E303CA.implementation preserve=yes
      //## end FileSupport%376945E303CA.implementation

};

//## begin FileSupport%376945E303CA.postscript preserve=yes
//## end FileSupport%376945E303CA.postscript

// Class FileSupport 

//## Operation: FileSupport%37723B4C03CF
//	********************************************************************************
//
//	Name: FileSupport ()
//
//	Description: default constructor - This constructor is protected, for its use is directed to singleton derived class.
//
//	Input parameters: none
//
//	Returns: none
//
//	Exceptions: none
//
//	Remarks:
//
//	********************************************************************************
inline FileSupport::FileSupport ()
  //## begin FileSupport::FileSupport%37723B4C03CF.hasinit preserve=no
      : curSize(0),
        fileNum(1),
        maxSize(MAX_VALUE_UINT),
        maxFileNum(1),
        pFile(NULL),
        banner(false),
        seqFileNum(1)
  //## end FileSupport::FileSupport%37723B4C03CF.hasinit
  //## begin FileSupport::FileSupport%37723B4C03CF.initialization preserve=yes
  //## end FileSupport::FileSupport%37723B4C03CF.initialization
{
  //## begin FileSupport::FileSupport%37723B4C03CF.body preserve=yes
  //## end FileSupport::FileSupport%37723B4C03CF.body
}



//## Other Operations (inline)
//## Operation: active%377283320028
//	********************************************************************************
//
//	Name:bool active(void)
//
//	Description: Indicates if exist a file stream associated with object.
//
//	Parameters: none
//
//	Returns: true -> exist a file stream associated with this object
//	        false-> does not exist a file stream associated with this object
//
//	********************************************************************************
inline bool FileSupport::active ()
{
  //## begin FileSupport::active%377283320028.body preserve=yes
	return (pFile == NULL) ? false : true ;
  //## end FileSupport::active%377283320028.body
}

//## Operation: GetSeqFileNum%377773520077
//	********************************************************************************
//
//	Name:  const string GetActualName()
//
//	Description: Return the current value of the sequential file number.
//	Remember that the user can specify if he wants several files working in a round robin model. This number informs how many files
//	were created since the start of the operation.
//
//	Input parameters: none
//
//	Returns: the current value of the sequential file number.
//
//
//	********************************************************************************
inline Uint FileSupport::GetSeqFileNum ()
{
  //## begin FileSupport::GetSeqFileNum%377773520077.body preserve=yes
	return seqFileNum;
  //## end FileSupport::GetSeqFileNum%377773520077.body
}

//## begin module%376945E303CA.epilog preserve=yes
 
} // namespace sch

//## end module%376945E303CA.epilog


#endif
