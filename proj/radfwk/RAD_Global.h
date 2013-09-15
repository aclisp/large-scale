// -*- mode: c++ -*-

/* Author:   Huanghao@cellcom
 * Date:     2005/11/22 13:51:55
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RADFWK_GLOBAL_H__
#define __RADFWK_GLOBAL_H__

#include <radutil/radutil.h>
#include <radsock/radsock.h>
#include <radsql/radsql.h>

#if defined(RAD_OS_WIN32)
#if defined(_MSC_VER) || defined(__BCPLUSPLUS__)
/* Using MSVC++ or Borland C++ to compile. If compiling library as a   */
/* DLL, define RADFWK_DLL. If compiling as a static library,           */
/* define RADFWK_STATIC. If compiling an application that is to be     */
/* statically linked to radfwk, define RADFWK_STATIC (if the           */
/* application is to be dynamically linked, there is no need to define */
/* any of these macros).                                               */
#if defined(RADFWK_DLL) && defined(RADFWK_STATIC)
#error "Both RADFWK_DLL and RADFWK_STATIC are defined."
#elif defined(RADFWK_DLL)
#define RADFWK_API __declspec(dllexport)
#elif !defined(RADFWK_STATIC)
#define RADFWK_API __declspec(dllimport)
#elif defined(RADFWK_STATIC)
#define RADFWK_API
#endif /* RADFWK_DLL && RADFWK_STATIC */

#else /* Not using MSVC++/BC++ to compile */
#define RADFWK_API
#endif

#else /* Not under win32 */
#define RADFWK_API
#endif

/* If build as DLL, ensure STLport is used because we exposed STL
 * components across DLL boundaries */
#if defined(RADFWK_STATIC)
#include <algorithm>
#include <utility>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <map>
using namespace std;

#else // defined(RADFWK_DLL) or nothing defined
#include <stlport/algorithm>
#include <stlport/utility>
#include <stlport/string>
#include <stlport/vector>
#include <stlport/deque>
#include <stlport/list>
#include <stlport/map>

using namespace _STL;

#if defined(RAD_OS_WIN32)

#ifndef _STLP_USE_DECLSPEC
#define _STLP_USE_DECLSPEC
#endif

#ifndef _STLP_DESIGNATED_DLL
#define _STLP_DESIGNATED_DLL
#endif

#pragma warning( disable : 4251 )
/* 'identifier' : class 'type' needs to have dll-interface to be
   used by clients of class 'type2'
   A base class or structure must be declared with the
   __declspec(dllexport) keyword for a function in a derived class
   to be exported.
*/
#endif // RAD_OS_WIN32
#endif // RADFWK_DLL

/* Export utilities from radutil/radsock */
/* @see radfwk.def */

/* A global place to put configurations, enumerations and definitions */
class RADFWK_API RAD_Global
{
private:
    RAD_Global();
    ~RAD_Global();
    RAD_Global(const RAD_Global& );
    const RAD_Global& operator=(const RAD_Global& );
public:
    static void Initialize();
    static void Uninitialize();
};

template <class T> inline void Construct(T* p) { new ( (void*) p ) T; }
template <class T> inline void  Destruct(T* p) { p->~T(); }

#if defined(RAD_OS_WIN32)
#else
  #define INFINITE 0xFFFFFFFF
#endif

#endif // __RADFWK_GLOBAL_H__
