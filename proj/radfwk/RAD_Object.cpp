#include "RAD_Object.h"
#include "RAD_Parameters.h"
#include <time.h>
#include <errno.h>
#include <stdio.h>

#if defined(RAD_OS_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#include <sys/time.h>
#endif

lrad_ring_entry RAD_Object::m_TrackRingHead = {NULL, NULL};
RAD_Mutex       RAD_Object::m_TrackRingLock;
map < string, RAD_Object::CounterDescriptor * > RAD_Object::m_ObjectMap;
RAD_Mutex       RAD_Object::m_ObjectMapLock;
unsigned int RAD_Object::m_nStatMemoryCollections = 0;

RAD_Object::RAD_Object(const string& name)
    : m_Name(name)
{
    // see if we are already counting that
    CounterDescriptor * descr = GetDescriptor ( name, true ) ;
    if(descr) {

        // will serialize creation to avoid duplicate ids
        if(rad_lock(descr->m_AllocObjectsLock)) {

            m_nNumber = descr->m_nObjCtr++ ;
            descr->m_nAllocObjCtr ++ ;
            descr->m_AllocObjects [ this ] = time ( 0 ) ;

            rad_unlock(descr->m_AllocObjectsLock) ;
        }
    }
    else
        radlog(L_WARN|L_CONS, "[RAD_Object] Unable to get counter descriptor "
               "to construct '%s'", name.c_str());
}

RAD_Object::~RAD_Object()
{
    // see if we are already counting that
    CounterDescriptor * descr = GetDescriptor ( m_Name, false ) ;
    if(descr) {

        // find this object in the list and remove it
        bool missing = false ;

        if(rad_lock(descr->m_AllocObjectsLock)) {

            descr->m_nAllocObjCtr -- ;
            const map < RAD_Object * , time_t >::iterator i =
                descr->m_AllocObjects.find ( this ) ;
            if ( i == descr->m_AllocObjects.end() )
                missing = true ;
            else
                descr->m_AllocObjects.erase ( i ) ;

            rad_unlock(descr->m_AllocObjectsLock);
        }

        if(missing) {
            radlog(L_WARN|L_CONS, "[RAD_Object] Unable to find "
                   "'%s' #%u in the list", m_Name.c_str(), m_nNumber);
            rad_assert(!"Object missing");
        }
    }
    else
        radlog(L_WARN|L_CONS, "[RAD_Object] Unable to get counter descriptor "
               "to destruct '%s'", m_Name.c_str());
}

void RAD_Object::Swap(RAD_Object& other)
{
    rad_assert(m_Name == other.m_Name);
    swap(m_nNumber, other.m_nNumber);
}

RAD_Object::RAD_Object(const RAD_Object& other)
    : m_Name(other.m_Name)
{
    // see if we are already counting that
    CounterDescriptor * descr = GetDescriptor ( m_Name, false ) ;
    if(descr) {

        // will serialize creation to avoid duplicate ids
        if(rad_lock(descr->m_AllocObjectsLock)) {

            m_nNumber = descr->m_nObjCtr++ ;
            descr->m_nAllocObjCtr ++ ;
            descr->m_AllocObjects [ this ] = time ( 0 ) ;

            rad_unlock(descr->m_AllocObjectsLock) ;
        }
    }
    else
        radlog(L_WARN|L_CONS, "[RAD_Object] Unable to get counter descriptor "
               "to copy-construct '%s'", other.m_Name.c_str());
}

string RAD_Object::ToString() const
{
    char buf[256];
    sNprintf(buf, sizeof(buf), "%s#%u@%08X", m_Name.c_str(), m_nNumber, this);
    return buf;
}

void* RAD_Object::Allocate(size_t size, const char *file, unsigned int line)
{
    Heap_Track_Node* mem;

    if(m_TrackRingHead.next == NULL) {
        radlog(L_FATAL|L_CONS, "[RAD_Object::Allocate] "
               "RAD Framework is NOT initialized properly! "
               "`RAD_Global::Initialize()' should be called firstly");
        exit(1);
    }

    mem = (Heap_Track_Node*)malloc(sizeof(Heap_Track_Node) + size);

    if(mem) {
        mem->size = size;
        mem->file = file;
        mem->line = line;
        m_TrackRingLock.Lock(INFINITE, __FILE__, __LINE__) ;
        lrad_ring_insert_tail(&m_TrackRingHead, &mem->link);
        m_TrackRingLock.Unlock(__FILE__, __LINE__) ;
        return mem+1;
    }
    else {
        radlog(L_FATAL|L_CONS, "[RAD_Object::Allocate] no memory");
        exit(1);
    }

    return NULL;
}

void RAD_Object::Deallocate(void *mem)
{
    if(mem) {
        Heap_Track_Node* p =
            (Heap_Track_Node*)
            ((char*)mem-sizeof(Heap_Track_Node));
        m_TrackRingLock.Lock(INFINITE, __FILE__, __LINE__) ;
        lrad_ring_remove_entry(&p->link);
        m_TrackRingLock.Unlock(__FILE__, __LINE__) ;
        free(p);
    }
}

void RAD_Object::StartObjectTrack()
{
    lrad_ring_init_head(&m_TrackRingHead);
}

void RAD_Object::StopObjectTrack()
{
    PrintAllocations("full");
    PrintHeapUsage();

    // [TODO] cleanup on RAD_Object::StopObjectTrack
    // 1. clear m_TrackRingHead
    // 2. clear m_ObjectMap
}

RAD_Object::CounterDescriptor * RAD_Object::GetDescriptor(
    const string& name, bool add_new)
{
    rad_assert(name.size() >= 5);

    CounterDescriptor * descr = NULL ;

    if(rad_lock(m_ObjectMapLock)) {

        const map <string, CounterDescriptor * >::const_iterator i =
            m_ObjectMap.find ( name ) ;
        if ( i == m_ObjectMap.end () ) {
            if ( add_new ) {
                DEBUG("[RAD_Object] Allocating new counter descriptor '%s'",
                      name.c_str());

                descr = new CounterDescriptor(name);
                m_ObjectMap[name] = descr;
            }
        }
        else
            descr = i->second;

        rad_unlock(m_ObjectMapLock) ;
    }

    return descr;
}

RAD_Object::CounterDescriptor::CounterDescriptor(const string& name)
    : m_Name(name), m_nAllocObjCtr(0), m_nObjCtr(0)
{
    m_pIterationCallback = NULL;
    m_pIterationParameters = NULL;
}

void RAD_Object::PrintAllocations( const string & name )
{
    bool full = (name != "");
    radlog(L_WARN, "[RAD_Object] Allocations map");
    if(!rad_lock(m_ObjectMapLock))
        return;

    for ( map < string, CounterDescriptor * >::const_iterator mi =
              m_ObjectMap.begin() ; mi != m_ObjectMap.end() ; ++ mi ) {
        CounterDescriptor * descr = mi->second ;
        radlog(L_WARN, " |- %-30.30s - %10u - %6u", descr->m_Name.c_str(),
               descr->m_nObjCtr, descr->m_nAllocObjCtr);
        if ( full ) {
            time_t now = time ( 0 ) ;
            if(rad_lock(descr->m_AllocObjectsLock)) {

                for ( map < RAD_Object * , time_t >::const_iterator oi =
                          descr->m_AllocObjects.begin() ;
                      oi != descr->m_AllocObjects.end() ; ++ oi ) {
                    if ( oi->first->m_Name == name || name == "full" ) {
                        radlog(L_WARN, "   |- %10u age %8d - '%s'",
                               oi->first->m_nNumber,
                               now - oi->second,
                               oi->first->ToString().c_str());
                    }
                }

                rad_unlock(descr->m_AllocObjectsLock) ;
            }
        }
    }

    rad_unlock(m_ObjectMapLock) ;

    radlog(L_WARN, "[RAD_Object] Recovered %u allocations",
           m_nStatMemoryCollections);
}

void RAD_Object::CollectGarbage ( int age, const vector <string> & types )
{
    time_t now = time ( 0 ) ;
    unsigned int obj_del = 0 ;
    vector < RAD_Object * > to_be_deleted ;

    if(!rad_lock(m_ObjectMapLock))
        return;

    for ( map < string, CounterDescriptor * >::const_iterator mi =
              m_ObjectMap.begin() ; mi != m_ObjectMap.end() ; ++ mi ) {
        CounterDescriptor * descr = mi->second ;
        radlog(L_WARN, " |- %-30.30s - %10u - %6u", descr->m_Name.c_str(),
               descr->m_nObjCtr, descr->m_nAllocObjCtr);
        if(rad_lock(descr->m_AllocObjectsLock)) {

            for ( map < RAD_Object *, time_t >::iterator oi =
                      descr->m_AllocObjects.begin() ;
                  oi != descr->m_AllocObjects.end() ; ++ oi ) {
                RAD_Object * obj_ptr = oi->first ;
                int this_age = now - oi->second ;

                for ( vector <string>::const_iterator ti = types.begin () ;
                      ti != types.end () ; ++ ti ) {
                    if ( this_age > age && obj_ptr->m_Name == *ti ) {
                        // cannot delete here - maps are locked. will do later
                        // CAUTION - only heap allocated objects can be deleted
                        radlog(L_WARN, "Freeing '%s' #%u age %d sec",
                               obj_ptr->m_Name.c_str(),
                               obj_ptr->m_nNumber, this_age ) ;
                        to_be_deleted.push_back ( obj_ptr ) ;
                        break ;
                    }
                }
            }

            rad_unlock(descr->m_AllocObjectsLock) ;
        }
    }

    rad_unlock(m_ObjectMapLock) ;

    for ( vector <RAD_Object *>::iterator ci = to_be_deleted.begin () ;
          ci != to_be_deleted.end() ; ++ ci ) {
        RAD_Object * obj_ptr = *ci;
        delete obj_ptr ;
        obj_del ++ ;
    }

    radlog(L_WARN, "Garbage collection: %u objects of age > %d sec",
           obj_del, age);

    m_nStatMemoryCollections += obj_del ;
}

void RAD_Object::SetIterationCallback ( const string& name,
                                        IterationCallback cb,
                                        RAD_Parameters * pptr )
{
    CounterDescriptor * descr = GetDescriptor ( name, false ) ;

    if ( descr )
    {
        rad_assert(descr->m_pIterationCallback == NULL);
        rad_assert(descr->m_pIterationParameters == NULL);
        descr->m_pIterationCallback = cb ;
        descr->m_pIterationParameters = pptr ;
    }
    else
        radlog(L_WARN, "Unable to find descriptor for '%s'", name.c_str());
}

void RAD_Object::Iterate ( const string& name )
{
    CounterDescriptor * descr = GetDescriptor ( name, false ) ;
    if( descr ) {
        if(rad_lock(descr->m_AllocObjectsLock)) {

            for ( map < RAD_Object *, time_t >::iterator oi =
                      descr->m_AllocObjects.begin() ;
                  oi != descr->m_AllocObjects.end() ; ++ oi ) {
                RAD_Object * obj_ptr = oi->first ;

                if ( descr->m_pIterationCallback )
                    descr->m_pIterationCallback (
                        obj_ptr,
                        descr->m_pIterationParameters,
                        oi->second ) ;
            }

            descr->m_pIterationCallback = NULL ;
            descr->m_pIterationParameters = NULL;

            rad_unlock(descr->m_AllocObjectsLock) ;
        }
    }
}

void RAD_Object::FindObject ( RAD_Object * obj, RAD_Parameters * pptr,
                              time_t birth )
{
    unsigned int need_num = atoi ( pptr->m_Data[0].c_str() ) ;
    if ( need_num == obj->m_nNumber )
        printf ( "Object '%s' #%u - '%s'\n", pptr->m_Data[1].c_str(),
                 need_num, obj->ToString().c_str() ) ;
}

void RAD_Object::PrintObject( const string & name, const string & number )
{
    RAD_Parameters params ;
    params.m_Data.push_back ( number ) ;
    params.m_Data.push_back ( name ) ;

    SetIterationCallback ( name, FindObject, &params ) ;
    Iterate ( name ) ;
}

unsigned int RAD_Object::GetCount(const string& name)
{
    unsigned int ctr = 0 ;

    CounterDescriptor * descr = GetDescriptor ( name, false ) ;

    if ( descr )
        ctr = descr->m_nObjCtr ;
    else
        radlog(L_WARN, "Unable to get counter descriptor of '%s'",
               name.c_str()) ;

    return ctr ;
}

void RAD_Object::PrintHeapUsage()
{
    if ( ! lrad_ring_is_empty(&m_TrackRingHead) ) {
        if ( rad_lock(m_TrackRingLock) ) {

            for ( lrad_ring_entry * p = m_TrackRingHead.next ;
                  p != &m_TrackRingHead ; p = p->next ) {

                Heap_Track_Node * q = baseptrof(p, Heap_Track_Node, link);
                radlog(L_WARN, "[RAD_Object] Memory leak: "
                       "%u bytes allocated at %s(%u)",
                       q->size, q->file, q->line);
            }

            rad_unlock(m_TrackRingLock) ;
        }
    }
}

int RAD_Bootstrap_Logger::Debug()
{
    return radlog_debug_flag;
}

void RAD_Bootstrap_Logger::Debug(int i)
{
    radlog_debug_flag = i;
}

radlog_dest_t RAD_Bootstrap_Logger::Destination()
{
    return radlog_dest;
}

void RAD_Bootstrap_Logger::Destination(radlog_dest_t d)
{
    radlog_dest = d;
}

string RAD_Bootstrap_Logger::Directory()
{
    return radlog_dir;
}

string RAD_Bootstrap_Logger::FullPath()
{
    return radlog_file;
}

void RAD_Bootstrap_Logger::SetLogFileName(const string& f)
{
    radlog_set_logfile(f.c_str());
}

int RAD_Bootstrap_Logger::Level()
{
    return radlog_level_hold;
}

void RAD_Bootstrap_Logger::Level(int i)
{
    radlog_level_hold = i;
}

bool RAD_Bootstrap_Logger::ShowMilliseconds()
{
    return (radlog_print_millisec != 0);
}

void RAD_Bootstrap_Logger::ShowMilliseconds(bool b)
{
    radlog_print_millisec = (b ? 1 : 0);
}

radlog_listener_t RAD_Bootstrap_Logger::Listener()
{
    return radlog_listener;
}

void RAD_Bootstrap_Logger::Listener(radlog_listener_t l)
{
    radlog_listener = l;
}

/* Bootstrap cross-platform light-weight recursive mutex implementation */
#if defined(RAD_OS_WIN32)
class RAD_Mutex_Win32 {
public:
    RAD_Mutex_Win32() { m_hMutex = CreateMutex(NULL, FALSE, NULL); }
    ~RAD_Mutex_Win32() { CloseHandle(m_hMutex); }
    bool TryLock(unsigned int delay, const char *file, unsigned int line);
    bool Lock(unsigned int delay, const char *file, unsigned int line);
    bool Unlock(const char *file, unsigned int line);
    HANDLE m_hMutex;
};
bool RAD_Mutex_Win32::TryLock(unsigned int delay, const char *file,
                              unsigned int line)
{
    char buf[256];
    DWORD ret = WaitForSingleObject(m_hMutex, delay);
    switch(ret) {
    case WAIT_ABANDONED:
        radlog(L_WARN|L_CONS, "[RAD_Mutex_Win32] Locked an "
               "abandoned mutex at %s(%u)", file, line);
    case WAIT_OBJECT_0:
        return true;
    case WAIT_FAILED:
        strerror_r(GetLastError(), buf, sizeof(buf));
        radlog(L_WARN|L_CONS, "[RAD_Mutex_Win32] Unable to lock "
               "at %s(%u): %s", file, line, buf);
    case WAIT_TIMEOUT:
    default:
        ;
    }
    return false;
}
bool RAD_Mutex_Win32::Lock(unsigned int delay, const char *file,
                           unsigned int line)
{
    char buf[256]; buf[0] = 0;
    DWORD ret = WaitForSingleObject(m_hMutex, delay);
    switch(ret) {
    case WAIT_ABANDONED:
        radlog(L_WARN|L_CONS, "[RAD_Mutex_Win32] Locked an "
               "abandoned mutex at %s(%u)", file, line);
    case WAIT_OBJECT_0:
        return true;
    case WAIT_FAILED:
        strerror_r(GetLastError(), buf, sizeof(buf));
    case WAIT_TIMEOUT:
    default:
        radlog(L_WARN|L_CONS, "[RAD_Mutex_Win32] Unable to lock "
               "after %u msec at %s(%u): %s", delay, file, line,
               buf[0] ? buf : "WAIT_TIMEOUT");
    }
    return false;
}
bool RAD_Mutex_Win32::Unlock(const char *file, unsigned int line)
{
    char buf[256];
    if(!ReleaseMutex(m_hMutex)) {
        strerror_r(GetLastError(), buf, sizeof(buf));
        radlog(L_WARN|L_CONS, "[RAD_Mutex_Win32] Unable to unlock "
               "because it has NOT been locked, %s(%u): %s", file, line, buf);
        return false;
    }
    return true;
}
#define MUTEX_IMPLEMENT RAD_Mutex_Win32
#else
class RAD_Mutex_POSIX {
public:
    RAD_Mutex_POSIX()
        {
            if(pthread_mutex_init(&m_hMutex, NULL))
                radlog(L_WARN|L_CONS, "[RAD_Mutex_POSIX] "
                       "pthread_mutex_init returns non-zero");
        }
    ~RAD_Mutex_POSIX()
        {
            int rc;
            if((rc = pthread_mutex_destroy(&m_hMutex)))
                radlog(L_WARN|L_CONS, "[RAD_Mutex_POSIX] "
                       "pthread_mutex_destroy returns %d, "
                       "the mutex is currently locked?", rc);
        }
    bool TryLock(unsigned int delay, const char *file, unsigned int line);
    bool Lock(unsigned int delay, const char *file, unsigned int line);
    bool Unlock(const char *file, unsigned int line);
    pthread_mutex_t m_hMutex;
};
bool RAD_Mutex_POSIX::TryLock(unsigned int delay, const char *file,
                              unsigned int line)
{
    int rc = pthread_mutex_trylock(&m_hMutex);
    if(rc == 0)
        return true;
    if(rc == EBUSY)
        return false;
    radlog(L_WARN|L_CONS, "[RAD_Mutex_POSIX] Unable to lock "
           "at %s(%u): pthread_mutex_trylock returns %d",
           file, line, rc);
    return false;
}
bool RAD_Mutex_POSIX::Lock(unsigned int delay, const char *file,
                           unsigned int line)
{
    int rc;

#ifdef LACK_PTHREAD_MUTEX_TIMEDLOCK
    rc = pthread_mutex_lock(&m_hMutex);
#else
    if(delay == INFINITE) {
        rc = pthread_mutex_lock(&m_hMutex);
    }
    else {
        struct timeval end_time ;
        gettimeofday ( &end_time, NULL ) ;
        end_time.tv_usec += ( delay % 1000 ) * 1000 ;
        end_time.tv_sec += delay / 1000 ;
        if ( end_time.tv_usec > 1000000 )
        {
            end_time.tv_usec -= 1000000 ;
            end_time.tv_sec += 1 ;
        }
        struct timespec timeout = { end_time.tv_sec,
                                    end_time.tv_usec * 1000 } ;

        rc = pthread_mutex_timedlock(&m_hMutex, &timeout);
    }
#endif

    if(rc == 0)
        return true;
    radlog(L_WARN|L_CONS, "[RAD_Mutex_POSIX] Unable to lock "
           "at %s(%u): pthread_mutex_lock returns %d, "
           "maybe already locked or timed out?",
           file, line, rc);
    return false;
}
bool RAD_Mutex_POSIX::Unlock(const char *file, unsigned int line)
{
    int rc = pthread_mutex_unlock(&m_hMutex);
    if(rc == 0)
        return true;
    radlog(L_WARN|L_CONS, "[RAD_Mutex_POSIX] Unable to unlock "
           "at %s(%u): pthread_mutex_unlock returns %d, maybe has NOT locked?",
           file, line, rc);
    return false;
}
#define MUTEX_IMPLEMENT RAD_Mutex_POSIX
#endif

RAD_Mutex::RAD_Mutex()
{
    rad_static_assert(sizeof(Impl) == sizeof(MUTEX_IMPLEMENT));
    Construct((MUTEX_IMPLEMENT*)(&m_impl));
}

RAD_Mutex::~RAD_Mutex()
{
    Destruct((MUTEX_IMPLEMENT*)(&m_impl));
}

bool RAD_Mutex::Lock(unsigned int delay, const char *file, unsigned int line)
{
    return ((MUTEX_IMPLEMENT*)(&m_impl))->Lock(delay, file, line);
}

bool RAD_Mutex::Unlock(const char *file, unsigned int line)
{
    return ((MUTEX_IMPLEMENT*)(&m_impl))->Unlock(file, line);
}
