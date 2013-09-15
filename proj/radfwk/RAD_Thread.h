// -*- mode: c++ -*-

/* Author:   Huanghao@cellcom
 * Date:     2005/11/24 12:40:25
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RADFWK_THREAD_H__
#define __RADFWK_THREAD_H__

#include <radfwk/RAD_Object.h>
#include <radfwk/RAD_Lock.h>

class RADFWK_API RAD_Thread : public RAD_Object
{
public:
    enum AliveState { ASUnused, ASAlive, ASStop, ASDead };

    void KillThread(int exit_code); // call hook and kill the primitive thread
    int StopAndWait(unsigned int delay); // join and reset this object
    void RequestStop(); // mark thread stopping flag
    bool StartThread(); // start until thread's execution
    bool IsStopped() const; // test thread stopping flag
    void Tick(); // tick internal loop counter to keep thread alive
    unsigned int LoopCount() const;

    unsigned int m_handle;
    unsigned int m_thr_id;
    string m_ThreadName;
    AliveState m_Alive;
    bool m_bShouldWatch;   // if watchdog should watch this thread

    static void Delay(unsigned int delay);
    static unsigned int CurrentThreadId();
    static unsigned int m_nRestarted; // times threads was killed and restarted
    static unsigned int m_nRelaunched;// times threads was exception recovered

protected:
    explicit RAD_Thread(const string& objname = "RAD_Thread");
    virtual ~RAD_Thread();

private:
    RAD_Thread(const RAD_Thread& );
    RAD_Thread& operator=(const RAD_Thread& other);

    static THREAD_CALLBACK ThreadProc(void* arg);

    virtual int MainLoop() = 0;
    virtual int StartThreadHook() { return 0; }
    virtual void KillThreadHook() {}

    RAD_Lock m_StartStopLock;
    unsigned int m_nCheckCtr; // how many times the thread was checked to be ok
    unsigned int m_nLoopCtr;
    bool m_bStop;          // if this thread should stop execution

    void Reset();
};

#endif // __RADFWK_THREAD_H__
