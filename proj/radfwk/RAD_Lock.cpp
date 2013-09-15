#include "RAD_Lock.h"
#include <time.h>
#include <errno.h>

#if defined(RAD_OS_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#include <sys/time.h>
#endif

#if defined(RAD_OS_WIN32)
class RAD_Lock_Win32 {
public:
    RAD_Lock_Win32(bool can_broadcast = false)
        {
            m_bHolding = 0;
            m_bWaiting = 0;
            m_bManualReset = ( can_broadcast ? 1 : 0 );
            m_mutex = CreateMutex(NULL, FALSE, NULL);
            m_event = CreateEvent(NULL, m_bManualReset ? TRUE : FALSE,
                                  FALSE, NULL);
        }
    ~RAD_Lock_Win32()
        {
            if(m_bHolding)
                radlog(L_WARN|L_CONS, "[RAD_Lock_Win32] "
                       "Still holding(=%u) on destruction", m_bHolding);
            if(m_bWaiting)
                radlog(L_WARN|L_CONS, "[RAD_Lock_Win32] "
                       "Still waiting(=%d) on destruction", m_bWaiting);
            CloseHandle(m_event);
            CloseHandle(m_mutex);
        }

    bool TryAcquire(unsigned int delay, const char *file, unsigned int line)
        {
            char buf[256];
            TRACE("[RAD_Lock_Win32] Try acquire %08X %u msec, %s(%u)",
                  m_mutex, delay, file, line);
            DWORD ret = WaitForSingleObject(m_mutex, delay);
            switch(ret) {
            case WAIT_ABANDONED:
                radlog(L_WARN|L_CONS, "[RAD_Lock_Win32] Locked an "
                       "abandoned mutex at %s(%u)", file, line);
            case WAIT_OBJECT_0:
                m_bHolding ++;
                TRACE("[RAD_Lock_Win32] Try acquire ok");
                return true;
            case WAIT_FAILED:
                strerror_r(GetLastError(), buf, sizeof(buf));
                radlog(L_WARN|L_CONS, "[RAD_Lock_Win32] Unable to lock "
                       "at %s(%u): %s", file, line, buf);
            case WAIT_TIMEOUT:
            default:
                TRACE("[RAD_Lock_Win32] Try acquire opps");
            }
            return false;
        }

    bool Acquire(unsigned int delay, const char *file, unsigned int line)
        {
            char buf[256]; buf[0] = 0;
            TRACE("[RAD_Lock_Win32] Acquire %08X %u msec, %s(%u)",
                  m_mutex, delay, file, line);
            DWORD ret = WaitForSingleObject(m_mutex, delay);
            switch(ret) {
            case WAIT_ABANDONED:
                radlog(L_WARN|L_CONS, "[RAD_Lock_Win32] Locked an "
                       "abandoned mutex at %s(%u)", file, line);
            case WAIT_OBJECT_0:
                m_bHolding ++;
                TRACE("[RAD_Lock_Win32] Acquire ok");
                return true;
            case WAIT_FAILED:
                strerror_r(GetLastError(), buf, sizeof(buf));
            case WAIT_TIMEOUT:
            default:
                radlog(L_WARN|L_CONS, "[RAD_Lock_Win32] Unable to lock "
                       "after %u msec at %s(%u): %s", delay, file, line,
                       buf[0] ? buf : "WAIT_TIMEOUT");
            }
            return false;
        }

    bool Release(const char *file, unsigned int line)
        {
            char buf[256];
            m_bHolding --;
            if(!ReleaseMutex(m_mutex)) {
                m_bHolding ++;
                strerror_r(GetLastError(), buf, sizeof(buf));
                radlog(L_WARN|L_CONS, "[RAD_Lock_Win32] Unable to unlock "
                       "because it has NOT been locked, %s(%u): %s",
                       file, line, buf);
                return false;
            }
            TRACE("[RAD_Lock_Win32] %08X Released, %s(%u)",
                  m_mutex, file, line);
            return true;
        }

    bool Wait(unsigned int delay, const char *file, unsigned int line)
        {
            char buf[256]; buf[0] = 0;

            // This method should only be called by a thread
            // that is the owner of this object's monitor.

            // Test if we has the ownership, NOTE: The thread that owns
            // a mutex can specify the same mutex in repeated wait function
            // calls without blocking its execution. So `TryAcquire' doesn't
            // work here.

            if(m_bHolding != 1) {
                if(m_bHolding == 0)
                    radlog(L_WARN|L_CONS, "[RAD_Lock_Win32::Wait] "
                           "Current thread is not holding this lock, "
                           "%s(%u)", file, line);
                else
                    radlog(L_WARN|L_CONS, "[RAD_Lock_Win32::Wait] "
                           "Current thread is holding this lock more than "
                           "once(=%u), %s(%u)", m_bHolding, file, line);
                return false;
            }

            TRACE("[RAD_Lock_Win32] Wait %08X %u msec, %s(%u)",
                  m_event, delay, file, line);

            m_bWaiting ++;

            // Release the ownership and enter waiting state

            if(!Release(__FILE__, __LINE__)) return false;

            DWORD ret = WaitForSingleObject(m_event, delay);
            switch ( ret ) {
            case WAIT_ABANDONED:
            case WAIT_OBJECT_0:

                // Got event signal, so re-obtain ownership and return true

                if(m_bManualReset) ResetEvent(m_event);

                if(!Acquire(5001, __FILE__, __LINE__)) return false;

                m_bWaiting --;

                TRACE("[RAD_Lock_Win32] Wait %08X %u msec ok, %s(%u)",
                      m_event, delay, file, line);
                return true;

            case WAIT_FAILED:
                strerror_r(GetLastError(), buf, sizeof(buf));
            case WAIT_TIMEOUT:
            default:

                // No signal, so re-obtain ownership and return false

                if(!Acquire(5002, __FILE__, __LINE__)) return false;

                m_bWaiting --;

                radlog(L_WARN|L_CONS, "[RAD_Lock_Win32] Unable to wait "
                       "after %u msec at %s(%u): %s", delay, file, line,
                       buf[0] ? buf : "WAIT_TIMEOUT");
            }
            return false;
        }

    bool Signal(const char *file, unsigned int line)
        {
            // This method should only be called by a thread
            // that is the owner of this object's monitor.

            // For Win32, a manual-reset event can do broadcast while
            // an auto-reset event just does signal

            if(m_bManualReset) {
                rad_assert(!"[RAD_Lock_Win32] Can not do signal");
                return false;
            }

            // Test if we has the ownership

            if(m_bHolding != 1) {
                if(m_bHolding == 0)
                    radlog(L_WARN|L_CONS, "[RAD_Lock_Win32::Signal] "
                           "Current thread is not holding this lock, "
                           "%s(%u)", file, line);
                else
                    radlog(L_WARN|L_CONS, "[RAD_Lock_Win32::Signal] "
                           "Current thread is holding this lock more than "
                           "once(=%u), %s(%u)", m_bHolding, file, line);
                return false;
            }

            // Set signal, and return true;

            if(m_bWaiting) {
                SetEvent(m_event);
                TRACE("[RAD_Lock_Win32] Signaled %08X, %s(%u)",
                      m_event, file, line);
                return true;
            }

            return false;
        }

    bool Broadcast(const char *file, unsigned int line)
        {
            // This method should only be called by a thread
            // that is the owner of this object's monitor.

            // For Win32, a manual-reset event can do broadcast while
            // an auto-reset event just does signal

            if(!m_bManualReset) {
                rad_assert(!"[RAD_Lock_Win32] Can not do broadcast");
                return false;
            }

            // Test if we has the ownership

            if(m_bHolding != 1) {
                if(m_bHolding == 0)
                    radlog(L_WARN|L_CONS, "[RAD_Lock_Win32::Broadcast] "
                           "Current thread is not holding this lock, "
                           "%s(%u)", file, line);
                else
                    radlog(L_WARN|L_CONS, "[RAD_Lock_Win32::Broadcast] "
                           "Current thread is holding this lock more than "
                           "once(=%u), %s(%u)", m_bHolding, file, line);
                return false;
            }

            // Set signal, and return true;

            if(m_bWaiting) {
                SetEvent(m_event);
                TRACE("[RAD_Lock_Win32] Broadcasted %08X, %s(%u)",
                      m_event, file, line);
                return true;
            }

            return false;
        }

    bool WaitEvent(unsigned int delay, const char *file, unsigned int line)
        {
            if(!Acquire(5003, __FILE__, __LINE__)) return false;
            bool rc = Wait(delay, file, line);
            Release(__FILE__, __LINE__);
            return rc;
        }

    void PulseEvent(const char *file, unsigned int line)
        {
            if(!Acquire(5004, __FILE__, __LINE__)) return;
            if(m_bManualReset)
                Broadcast(__FILE__, __LINE__);
            else
                Signal(__FILE__, __LINE__);
            Release(__FILE__, __LINE__);
        }

    void MakeRecursive(bool)
        {
            // For Win32, it is recursive essentially
        }

    int IsHolding() const
        {
            return m_bHolding;
        }

    int IsWaiting() const
        {
            return m_bWaiting;
        }

    void Reset()
        {
            // It's never need reset
            m_bHolding = 0;
            m_bWaiting = 0;
            if(m_bManualReset) ResetEvent(m_event);
        }

    HANDLE  m_mutex;
    HANDLE  m_event;
    char    m_bManualReset;
    unsigned char  m_bHolding;
    unsigned short m_bWaiting;
};
#define LOCK_IMPLEMENT RAD_Lock_Win32
#else
class RAD_Lock_POSIX {
public:
    RAD_Lock_POSIX(bool can_broadcast = false)
        {
            m_bHolding = 0;
            m_bWaiting = 0;
            pthread_mutex_init(&m_mutex, NULL);
            pthread_cond_init(&m_event, NULL);
        }
    ~RAD_Lock_POSIX()
        {
            if(m_bHolding)
                radlog(L_WARN|L_CONS, "[RAD_Lock_POSIX] "
                       "Still holding(=%u) on destruction", m_bHolding);
            if(m_bWaiting)
                radlog(L_WARN|L_CONS, "[RAD_Lock_POSIX] "
                       "Still waiting(=%d) on destruction", m_bWaiting);
            pthread_cond_destroy(&m_event);
            pthread_mutex_destroy(&m_mutex);
        }

    bool TryAcquire(unsigned int delay, const char *file, unsigned int line)
        {
            int rc = pthread_mutex_trylock(&m_mutex);
            if(rc == 0) {
                m_bHolding ++;
                TRACE("[RAD_Lock_POSIX] Try acquire ok");
                return true;
            }
            if(rc == EBUSY) {
                TRACE("[RAD_Lock_POSIX] Try acquire opps");
                return false;
            }
            radlog(L_WARN|L_CONS, "[RAD_Lock_POSIX] Unable to lock "
                   "at %s(%u): pthread_mutex_trylock returns %d",
                   file, line, rc);
            TRACE("[RAD_Lock_POSIX] Try acquire opps");
            return false;
        }

    bool Acquire(unsigned int delay, const char *file, unsigned int line)
        {
            int rc;

#ifdef LACK_PTHREAD_MUTEX_TIMEDLOCK
            rc = pthread_mutex_lock(&m_mutex);
#else
            if(delay == INFINITE) {
                rc = pthread_mutex_lock(&m_mutex);
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

                rc = pthread_mutex_timedlock(&m_mutex, &timeout);
            }
#endif

            if(rc == 0) {
                m_bHolding ++;
                TRACE("[RAD_Lock_POSIX] Acquire ok");
                return true;
            }
            radlog(L_WARN|L_CONS, "[RAD_Mutex_POSIX] Unable to lock "
                   "at %s(%u): pthread_mutex_lock returns %d, "
                   "maybe already locked or timed out?", file, line, rc);
            return false;
        }

    bool Release(const char *file, unsigned int line)
        {
            m_bHolding --;
            int rc = pthread_mutex_unlock(&m_mutex);
            if(rc == 0)
                return true;
            m_bHolding ++;
            radlog(L_WARN|L_CONS, "[RAD_Mutex_POSIX] Unable to unlock "
                   "at %s(%u): pthread_mutex_unlock returns %d, "
                   "maybe has NOT locked?", file, line, rc);
            return false;
        }

    bool Wait(unsigned int delay, const char *file, unsigned int line)
        {
            if(m_bHolding != 1) {
                if(m_bHolding == 0)
                    radlog(L_WARN|L_CONS, "[RAD_Lock_POSIX::Wait] "
                           "Current thread is not holding this lock, "
                           "%s(%u)", file, line);
                else
                    radlog(L_WARN|L_CONS, "[RAD_Lock_POSIX::Wait] "
                           "Current thread is holding this lock more than "
                           "once(=%u), %s(%u)", m_bHolding, file, line);
                return false;
            }

            TRACE("[RAD_Lock_POSIX] Wait %08X %u msec, %s(%u)",
                  m_event, delay, file, line);

            m_bWaiting ++;
            m_bHolding --;

            int rc;
            if(delay == INFINITE) {
                rc = pthread_cond_wait(&m_event, &m_mutex);
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

                rc = pthread_cond_timedwait(&m_event, &m_mutex, &timeout);
            }

            if(rc == 0) {
                m_bWaiting --;
                m_bHolding ++;

                TRACE("[RAD_Lock_POSIX] Wait %08X %u msec ok, %s(%u)",
                      m_event, delay, file, line);
                return true;
            }

            m_bWaiting --;
            m_bHolding ++;

            radlog(L_WARN|L_CONS, "[RAD_Lock_POSIX] Unable to wait "
                   "after %u msec at %s(%u): %s", delay, file, line,
                   rc != ETIMEDOUT ? "INVAL_PARAM" : "WAIT_TIMEOUT");
            return false;
        }

    bool Signal(const char *file, unsigned int line)
        {
            if(m_bHolding != 1) {
                if(m_bHolding == 0)
                    radlog(L_WARN|L_CONS, "[RAD_Lock_POSIX::Signal] "
                           "Current thread is not holding this lock, "
                           "%s(%u)", file, line);
                else
                    radlog(L_WARN|L_CONS, "[RAD_Lock_POSIX::Signal] "
                           "Current thread is holding this lock more than "
                           "once(=%u), %s(%u)", m_bHolding, file, line);
                return false;
            }

            if(m_bWaiting) {
                pthread_cond_signal(&m_event);
                TRACE("[RAD_Lock_POSIX] Signaled %08X, %s(%u)",
                      m_event, file, line);
                return true;
            }

            return false;
        }

    bool Broadcast(const char *file, unsigned int line)
        {
            if(m_bHolding != 1) {
                if(m_bHolding == 0)
                    radlog(L_WARN|L_CONS, "[RAD_Lock_POSIX::Broadcast] "
                           "Current thread is not holding this lock, "
                           "%s(%u)", file, line);
                else
                    radlog(L_WARN|L_CONS, "[RAD_Lock_POSIX::Broadcast] "
                           "Current thread is holding this lock more than "
                           "once(=%u), %s(%u)", m_bHolding, file, line);
                return false;
            }

            if(m_bWaiting) {
                pthread_cond_broadcast(&m_event);
                TRACE("[RAD_Lock_POSIX] Broadcasted %08X, %s(%u)",
                      m_event, file, line);
                return true;
            }

            return false;
        }

    bool WaitEvent(unsigned int delay, const char *file, unsigned int line)
        {
            if(!Acquire(5003, __FILE__, __LINE__)) return false;
            bool rc = Wait(delay, file, line);
            Release(__FILE__, __LINE__);
            return rc;
        }

    void PulseEvent(const char *file, unsigned int line)
        {
            if(!Acquire(5004, __FILE__, __LINE__)) return;
            Broadcast(__FILE__, __LINE__);
            Release(__FILE__, __LINE__);
        }

    void MakeRecursive(bool)
        {
            pthread_mutexattr_t mta;

            pthread_mutexattr_init(&mta);
            pthread_mutexattr_settype(&mta, PTHREAD_MUTEX_RECURSIVE );
            pthread_mutex_destroy(&m_mutex);
            pthread_mutex_init(&m_mutex, &mta);
        }

    int IsHolding() const
        {
            return m_bHolding;
        }

    int IsWaiting() const
        {
            return m_bWaiting;
        }

    void Reset()
        {
            // It's never need reset
            m_bHolding = 0;
            m_bWaiting = 0;
        }

    pthread_mutex_t  m_mutex;
    pthread_cond_t   m_event;
    char    m_bManualReset;
    unsigned char  m_bHolding;
    unsigned short m_bWaiting;
};
#define LOCK_IMPLEMENT RAD_Lock_POSIX
#endif

RAD_Lock::RAD_Lock(bool can_broadcast)
    : RAD_Object("RAD_Lock")
{
    rad_static_assert(sizeof(Impl) == sizeof(LOCK_IMPLEMENT));
    new( (void*) (&m_impl) ) LOCK_IMPLEMENT(can_broadcast);
}

RAD_Lock::~RAD_Lock()
{
    Destruct( (LOCK_IMPLEMENT*) (&m_impl) );
}

bool RAD_Lock::TryAcquire(unsigned int delay, const char *file,
                          unsigned int line)
{
    return ((LOCK_IMPLEMENT*)(&m_impl))->TryAcquire(delay, file, line);
}

bool RAD_Lock::Acquire(unsigned int delay, const char *file, unsigned int line)
{
    return ((LOCK_IMPLEMENT*)(&m_impl))->Acquire(delay, file, line);
}

bool RAD_Lock::Release(const char *file, unsigned int line)
{
    return ((LOCK_IMPLEMENT*)(&m_impl))->Release(file, line);
}

bool RAD_Lock::Wait(unsigned int delay, const char *file, unsigned int line)
{
    return ((LOCK_IMPLEMENT*)(&m_impl))->Wait(delay, file, line);
}

bool RAD_Lock::Signal(const char *file, unsigned int line)
{
    return ((LOCK_IMPLEMENT*)(&m_impl))->Signal(file, line);
}

bool RAD_Lock::Broadcast(const char *file, unsigned int line)
{
    return ((LOCK_IMPLEMENT*)(&m_impl))->Broadcast(file, line);
}

bool RAD_Lock::WaitEvent(unsigned int delay, const char *file,
                         unsigned int line)
{
    return ((LOCK_IMPLEMENT*)(&m_impl))->WaitEvent(delay, file, line);
}

void RAD_Lock::PulseEvent(const char *file, unsigned int line)
{
    ((LOCK_IMPLEMENT*)(&m_impl))->PulseEvent(file, line);
}

void RAD_Lock::MakeRecursive(bool bRecursive)
{
    ((LOCK_IMPLEMENT*)(&m_impl))->MakeRecursive(bRecursive);
}

int RAD_Lock::IsHolding() const
{
    return ((LOCK_IMPLEMENT*)(&m_impl))->IsHolding();
}

int RAD_Lock::IsWaiting() const
{
    return ((LOCK_IMPLEMENT*)(&m_impl))->IsWaiting();
}

void RAD_Lock::Reset()
{
    ((LOCK_IMPLEMENT*)(&m_impl))->Reset();
}
