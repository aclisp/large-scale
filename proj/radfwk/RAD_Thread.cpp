#include "RAD_Thread.h"
#include "RAD_Util.h"

#if defined(RAD_OS_WIN32)
#include <windows.h>
#include <process.h>
#else
#include <pthread.h>
#include <signal.h>
#endif

unsigned int RAD_Thread::m_nRelaunched = 0;

RAD_Thread::RAD_Thread(const string& objname)
    : RAD_Object(objname)
    , m_handle(0)
    , m_thr_id(0xffffffff)
    , m_ThreadName("Unnamed")
    , m_Alive(ASUnused)
    , m_bShouldWatch(true)
    , m_nCheckCtr(0)
    , m_nLoopCtr(0)
    , m_bStop(false)
{
}

RAD_Thread::~RAD_Thread()
{
    // this is an undetached thread, it would be reset after join
    if(m_handle) {
        radlog(L_WARN|L_CONS, "[RAD_Thread] dtor called, "
               "but thread '%s' is still running, forced stop",
               m_ThreadName.c_str());
        StopAndWait(0);
    }
}

void RAD_Thread::Reset()
{
    m_handle = 0;
    m_Alive = ASUnused;
    m_bShouldWatch = true;
    m_bStop = false;
    // Other states just keep their old values until re-assignment
}

void RAD_Thread::KillThread(int exit_code)
{
    radlog(L_WARN, "[RAD_Thread] Killing '%s' with code %d",
           m_ThreadName.c_str(), exit_code);
    KillThreadHook();
#ifdef RAD_OS_WIN32
    BOOL rc = TerminateThread((HANDLE)m_handle, exit_code);
    if(!rc)
        radlog(L_WARN, "[RAD_Thread] Failed to kill: %s",
               RAD_Util::GetLastErrDesc("killing thread").c_str());
#else
    int rc = pthread_cancel((pthread_t)m_handle);
    if(rc)
        radlog(L_WARN, "[RAD_Thread] Failed to kill: %d", rc);
#endif
}

int RAD_Thread::StopAndWait(unsigned int delay)
{
    if(!m_StartStopLock.Acquire(5000, __FILE__, __LINE__))
        return -1;

    // Check state to see whether it could be stopped
    if(m_handle == 0) {
        radlog(L_WARN, "[RAD_Thread] '%s' already stopped",
               m_ThreadName.c_str());
        m_StartStopLock.Release(__FILE__, __LINE__);
        return -1;
    }

    m_bShouldWatch = false; // prevent from watching-dogged

    if(!IsStopped())
        RequestStop();

    DEBUG("[RAD_Thread] Requested termination of '%s', "
          "waiting %u msec for end", m_ThreadName.c_str(), delay);

    // wait for the thread to stop if it is still running
    rad_assert(m_Alive != ASUnused);
    bool isKilled = false;
    if(m_Alive == ASAlive) {
        bool ret = m_StartStopLock.Wait(delay, __FILE__, __LINE__);
        if(ret == false) {
            radlog(L_WARN, "[RAD_Thread] '%s' did not end within %u msec",
                   m_ThreadName.c_str(), delay);
            KillThread(9); // 9 - SIGKILL
            m_Alive = ASStop;
            isKilled = true;
        }
    }
    rad_assert(m_Alive == ASStop || m_Alive == ASDead);

    // join the thread to deallocate thread resources
#ifdef RAD_OS_WIN32
    if(WaitForSingleObject((HANDLE)m_handle, INFINITE) != WAIT_OBJECT_0) {
        radlog(L_ERROR|L_CONS, "[RAD_Thread] Failed to join: %s",
               RAD_Util::GetLastErrDesc("WaitForSingleObject").c_str());
        m_StartStopLock.Release(__FILE__, __LINE__);
        return -1;
    }
    DWORD dwExitCode;
    if(!GetExitCodeThread((HANDLE)m_handle, &dwExitCode)) {
        radlog(L_ERROR|L_CONS, "[RAD_Thread] %s",
               RAD_Util::GetLastErrDesc("GetExitCodeThread").c_str());
        m_StartStopLock.Release(__FILE__, __LINE__);
        return -1;
    }
    if(!CloseHandle((HANDLE)m_handle)) {
        radlog(L_ERROR|L_CONS, "[RAD_Thread] %s",
               RAD_Util::GetLastErrDesc("CloseHandle").c_str());
        m_StartStopLock.Release(__FILE__, __LINE__);
        return -1;
    }
    Reset();
    m_StartStopLock.Release(__FILE__, __LINE__);
    return dwExitCode;
#else
    void* retcode;
    if(pthread_join((pthread_t)m_handle, &retcode)) {
        radlog(L_ERROR|L_CONS, "[RAD_Thread] %s",
               RAD_Util::GetLastErrDesc("pthread_join").c_str());
        m_StartStopLock.Release(__FILE__, __LINE__);
        return -1;
    }
    Reset();
    m_StartStopLock.Release(__FILE__, __LINE__);
    return (isKilled ? 9 : (int)retcode);
#endif
}

void RAD_Thread::RequestStop()
{
    m_bStop = true;
}

bool RAD_Thread::IsStopped() const
{
    return m_bStop;
}

bool RAD_Thread::StartThread()
{
    if(!m_StartStopLock.Acquire(5000, __FILE__, __LINE__))
        return false;

    if(m_handle) {
        radlog(L_WARN, "[RAD_Thread] '%s' already started",
               m_ThreadName.c_str());
        m_StartStopLock.Release(__FILE__, __LINE__);
        return false;
    }

    m_bShouldWatch = false; // prevent from watching-dogged

    rad_assert(m_bStop == false && m_Alive == ASUnused);

    int ret = StartThreadHook();
    if(ret != 0) {
        radlog(L_WARN, "[RAD_Thread] StartThreadHook returns %d. "
               "The thread is not started", ret);
        m_bShouldWatch = true;
        m_StartStopLock.Release(__FILE__, __LINE__);
        return false;
    }

#ifdef RAD_OS_WIN32
    m_handle = _beginthreadex(NULL,      // security
                              256*1024,  // stack_size 256kb
                              ThreadProc,
                              this,
                              0, /* initflag: (0 for running or
                                  * CREATE_SUSPENDED for suspended) */
                              &m_thr_id);
    if(m_handle == 0) { /* _beginthreadex returns 0 on an error */
        radlog(L_ERROR|L_CONS, "[RAD_Thread] Fail to _beginthreadex: %d %s",
               errno, strerror(errno));
        m_bShouldWatch = true;
        m_StartStopLock.Release(__FILE__, __LINE__);
        return false;
    }
#else
    pthread_attr_t attr;
    ret = pthread_attr_init(&attr);
    if(ret)
        radlog(L_WARN|L_CONS, "[RAD_Thread] Unable to create a thread "
               "attribute for '%s', rc %d", m_ThreadName.c_str(), ret);
    ret = pthread_attr_setstacksize(&attr, 256*1024);
    if(ret)
        radlog(L_WARN|L_CONS, "[RAD_Thread] Unable to set stack size "
               "on thread attribute for '%s', rc %d",
               m_ThreadName.c_str(), ret);
    ret = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    if(ret)
        radlog(L_WARN|L_CONS, "[RAD_Thread] Unable to set "
               "scope(PTHREAD_SCOPE_SYSTEM) on thread attribute "
               "for '%s', rc %d", m_ThreadName.c_str(), ret);
    ret = pthread_create((pthread_t*)&m_handle, &attr, ThreadProc, this);
    m_thr_id = m_handle;
    pthread_attr_destroy(&attr);
    if(ret) {
        radlog(L_ERROR|L_CONS, "[RAD_Thread] Fail to pthread_create: %d", ret);
        m_bShouldWatch = true;
        m_StartStopLock.Release(__FILE__, __LINE__);
        return false;
    }
#endif

    bool rc = m_StartStopLock.Wait(3000, __FILE__, __LINE__);
    if(!rc)
        radlog(L_WARN, "[RAD_Thread] '%s' did not signal start",
               m_ThreadName.c_str());
    else
        rad_assert(m_Alive == ASAlive);

    m_bShouldWatch = true;
    m_StartStopLock.Release(__FILE__, __LINE__);
    return true;
}

THREAD_CALLBACK RAD_Thread::ThreadProc(void* arg)
{
    RAD_Thread * me = (RAD_Thread*) arg;
    me->m_StartStopLock.Acquire(5000, __FILE__, __LINE__);
    DEBUG("[RAD_Thread] Starting '%s', id %d",
          me->m_ThreadName.c_str(), me->m_thr_id);
    me->m_Alive = ASAlive;
    me->m_StartStopLock.Signal(__FILE__, __LINE__);
    me->m_StartStopLock.Release(__FILE__, __LINE__);

#ifdef RAD_OS_WIN32
#else
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
#endif

    for ( ; ; )
    {
        int rc;
        try
        {
            rc = me->MainLoop();
            if(rc >= 0)
                break;
        }
        catch ( ... )
        {
            radlog(L_ERROR|L_CONS, "[RAD_Thread] '%s' crashed by exception, "
                   "restarting", me->m_ThreadName.c_str());
            m_nRelaunched ++;
            continue;
        }
        radlog(L_WARN|L_CONS, "[RAD_Thread] Restarting '%s' by rc %d",
               me->m_ThreadName.c_str(), rc);
        m_nRelaunched ++;
        continue;
    }

    me->m_StartStopLock.Acquire(5000, __FILE__, __LINE__);
    me->m_Alive = ASStop;
    me->m_StartStopLock.Signal(__FILE__, __LINE__);
    me->m_StartStopLock.Release(__FILE__, __LINE__);
    return 0;
}

void RAD_Thread::Tick()
{
    m_nLoopCtr ++;
}

unsigned int RAD_Thread::LoopCount() const
{
    return m_nLoopCtr;
}

void RAD_Thread::Delay(unsigned int delay)
{
    Sleep(delay);
}

unsigned int RAD_Thread::CurrentThreadId()
{
#ifdef RAD_OS_WIN32
    return GetCurrentThreadId();
#else
    return pthread_self();
#endif
}
