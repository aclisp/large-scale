//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator 
//
//	Package: Common
//
//	File Name: MemLeak.h
//
//	Description: Memory leak debug support for Windows NT only
//
//	Author: Joao Loureiro Jr
//
//	Creation Date: Jun/11/99
//
//	Visual Source Safe $Revision: 1 $
//
//	Last check in $Date: 11/06/99 9:14 $
//
//	********************************************************************************

#ifndef MemLeak_h
#define MemLeak_h 1


#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


#if defined(_DEBUG) && defined(WIN32)
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif


#define DumpMemoryLeaks() _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF )


// Since this file is included at first place in all cpp files
// we disable necessary warnings here
#pragma warning( disable : 4244 ) // conversion from 'Double' to 'Float', possible loss of data
#pragma warning( disable : 4018 ) // signed/unsigned mismatch
#pragma warning( disable : 4102 ) // unreferenced label
#pragma warning( disable : 4305 ) // truncation from 'double' to 'Float'


#endif
