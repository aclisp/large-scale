// -*- mode: c++ -*-

/* Author:   Huanghao@cellcom
 * Date:     2005/11/22 13:36:40
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RADFWK_OBJECT_H__
#define __RADFWK_OBJECT_H__

#include <radfwk/RAD_Global.h>

class RADFWK_API RAD_Mutex
{
private:
    RAD_Mutex(const RAD_Mutex& );
    const RAD_Mutex& operator=(const RAD_Mutex& );
    struct Impl {
#ifdef RAD_OS_WIN32
        unsigned int mutex;
#else
        pthread_mutex_t mutex;
#endif
    } m_impl;
public:
    RAD_Mutex();
    ~RAD_Mutex();
    bool TryLock(unsigned int delay, const char *file, unsigned int line);
    bool Lock(unsigned int delay, const char *file, unsigned int line);
    bool Unlock(const char *file, unsigned int line);
};

class RAD_Parameters;

/* The base class for all RAD framework objects.
 * Provides: 1. Tracked free-store (de)allocation
 *           2. Object counter
 *           3. Garbage collection
 */
class RADFWK_API RAD_Object
{
public:
    explicit RAD_Object(const string& name);
    virtual ~RAD_Object();

    void Swap(RAD_Object& other); /* throw() */
    RAD_Object(const RAD_Object& );
    RAD_Object& operator=(const RAD_Object& other)
        { RAD_Object temp(other); Swap(temp); return *this; }

    virtual string ToString() const;

    static void * Allocate(size_t size, const char *file, unsigned int line);
    static void Deallocate(void *mem);

    static void* operator new(size_t size, const char *file, unsigned int line)
        { return Allocate(size, file, line); }
    static void operator delete(void *p)
        { if(p) Deallocate(p); }
    static void operator delete(void *p, const char *, unsigned int)
        { if(p) Deallocate(p); }

    static void StartObjectTrack();
    static void StopObjectTrack();

    struct Heap_Track_Node
    {
        lrad_ring_entry link;
        size_t          size;
        const char*     file;
        unsigned int    line;
    };
    static lrad_ring_entry m_TrackRingHead;
    static RAD_Mutex       m_TrackRingLock;

    typedef void ( *IterationCallback )( RAD_Object *, RAD_Parameters *,
                                         time_t birth ) ;
    class CounterDescriptor
    {
    public :
        explicit CounterDescriptor ( const string& name ) ;
        // name of the descriptor
        string   m_Name ;
        // total counter of currently allocated objects
        unsigned int m_nAllocObjCtr ;
        // total counter of created objects
        unsigned int m_nObjCtr ;
        // when a particular object was created
        map < RAD_Object * , time_t > m_AllocObjects ;
        RAD_Mutex m_AllocObjectsLock ;
        IterationCallback m_pIterationCallback ;
        RAD_Parameters * m_pIterationParameters ;
    } ;
    static map < string, CounterDescriptor * > m_ObjectMap;
    static RAD_Mutex      m_ObjectMapLock;
    static CounterDescriptor * GetDescriptor (
        const string& name, bool add_new = true );
    static void PrintAllocations( const string & );
    static void PrintHeapUsage();
    static unsigned int GetCount ( const string& name );

    static void CollectGarbage ( int age, const vector <string> & );
    static unsigned int m_nStatMemoryCollections;

    static void SetIterationCallback ( const string& name,
                                       IterationCallback cb,
                                       RAD_Parameters * = NULL ) ;
    static void Iterate ( const string& name ) ;
    static void FindObject ( RAD_Object * obj, RAD_Parameters * pptr,
                             time_t birth ) ;
    static void PrintObject( const string &, const string &);

    const string m_Name;
    unsigned int m_nNumber;
};

class RADFWK_API RAD_Bootstrap_Logger
{
public:
    static int Debug();
    static void Debug(int);

    static radlog_dest_t Destination();
    static void Destination(radlog_dest_t);

    static string Directory();
    static string FullPath();
    static void SetLogFileName(const string&);

    static int Level();
    static void Level(int);

    static bool ShowMilliseconds();
    static void ShowMilliseconds(bool);

    static radlog_listener_t Listener();
    static void Listener(radlog_listener_t);
};

#define rad_new                new(__FILE__, __LINE__)
#define rad_lock(mutex)        (mutex).Lock(5000, __FILE__, __LINE__)
#define rad_unlock(mutex)      (mutex).Unlock(__FILE__, __LINE__)

#endif // __RADFWK_OBJECT_H__
