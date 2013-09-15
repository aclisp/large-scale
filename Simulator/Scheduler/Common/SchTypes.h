//	********************************************************************************
//
//	Copyright: Nortel Networks 1999
//
//	Classification: Confidential
//
//	Project Name: RRM Simulator / Scheduler
//
//	Package: Common
//
//	File Name: SchTypes.h
//
//	Description: Definition of types used in C++ Scheduler for platform-independent code.
//
//	Author: Eugenio Pacheli Alves
//
//	Creation Date: Mar/12/99
//
//	Visual Source Safe $Revision: 9 $
//
//	Last check in $Date: 7/20/99 10:18a $
//
//	********************************************************************************

#ifndef SchTypes_h
#define SchTypes_h 1

#if defined (WIN32)

// The next pragma command disables the C4056 compiler error.  This action follows
// the recomendation present at http://support.microsoft.com/support/kb/articles/q168/9/12.asp.
// The Microsft question Q120218 recomends this action for Visual version 5.0.
// This action may be removed when using Visual 6.0(the bug was fixed).
#pragma warning (disable:4056)

#include <float.h>
//
// Redefinition of fundamental types
//
	typedef int           Int;         //storage: 4 bytes
	typedef long          Long;		   //storage: 4 bytes
	typedef unsigned int  Uint;		   //storage: 4 bytes
	typedef unsigned long Ulong;	   //storage: 4 bytes
	typedef float         Float;	   //storage: 4 bytes
	typedef double        Double;	   //storage: 8 bytes

//
// Definition of fixed-length integers
//
	typedef __int8         Int8;
	typedef __int16        Int16;
	typedef __int32        Int32;
	typedef __int64        Int64;
	typedef unsigned char  Uint8;
	typedef unsigned short Uint16;
	typedef unsigned long  Uint32;

//Definition of max and minimum value for integer and float

#define MIN_VALUE_INT		INT_MIN
#define MAX_VALUE_INT		INT_MAX
#define MAX_VALUE_UINT		UINT_MAX
#define MAX_VALUE_ULONG		ULONG_MAX
#define MIN_VALUE_FLOAT		FLT_MIN
#define MAX_VALUE_FLOAT		FLT_MAX
#define MIN_VALUE_DOUBLE	DBL_MIN
#define MAX_VALUE_DOUBLE	DBL_MAX
#define SIM_HUGE_VAL		HUGE_VAL

#elif defined(ALPHAEV67_OSF50_CXX62)

#include <float.h>

//Definition of max and minimum value for integer and float

#define MIN_VALUE_INT		INT_MIN
#define MAX_VALUE_INT		INT_MAX
#define MAX_VALUE_UINT		UINT_MAX
#define MAX_VALUE_ULONG		ULONG_MAX
#define MIN_VALUE_FLOAT		FLT_MIN
#define MAX_VALUE_FLOAT		FLT_MAX
#define MIN_VALUE_DOUBLE	DBL_MIN
#define MAX_VALUE_DOUBLE	DBL_MAX


//
// Redefinition of fundamental types
//
	typedef int            Int;         //storage: 4 bytes
	typedef long          Long;		   //storage: 4 bytes
	typedef unsigned int  Uint;		   //storage: 4 bytes
	typedef unsigned long Ulong;	   //storage: 4 bytes
#       if 1
	   typedef float         Float;	   //storage: 4 bytes
#          define SIM_HUGE_VAL (1e30)
#       else
	   typedef double        Float;	   //storage: 4 bytes
#          define SIM_HUGE_VAL HUGE_VAL
#       endif
	typedef double        Double;	   //storage: 8 bytes

//
// Definition of fixed-length integers
//
	typedef char           Int8;
	typedef short          Int16;
	typedef long           Int32;
	typedef long long      Int64;
	typedef unsigned char  Uint8;
	typedef unsigned short Uint16;
	typedef unsigned long  Uint32;

#else
	#error "Invalid Operating System"
#endif		// OS

//
// This constant defines the maximum word of the computer
//
	const Ulong MAX_WORD = 0X10000000;

//
// return code from functions
//
typedef Int RetCode;

#endif      // #define SchTypes_h
