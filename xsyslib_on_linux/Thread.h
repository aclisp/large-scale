#ifndef __XSYSLIB_THREAD_H__
#define __XSYSLIB_THREAD_H__

#include "Object.h"
#include "SingletonObject.h"
#include <setjmp.h>


typedef int ThreadId; // identifier assigned by ThreadRegistry
typedef struct RootThreadType {} RootThreadType;

const ThreadId NilThreadId = -1;
const ThreadId MaxThreadId = 255;

enum ThreadFaction // for proportional scheduling
{
    IdleFaction,
    AuditFaction,
    BackgroundFaction,
    ProvisioningFaction,
    MaintenanceFaction,
    PayloadFaction,
    PriorityFaction
};

enum ThreadRc // what to do after a thread traps
{
    DeleteThread,  // exit the thread
    ReenterThread, // reinvoke the thread's Enter function
    ForceRestart   // cause a restart
};


class Thread : public Object
{
    friend class ThreadRegistry;
    friend class Watchdog;
public:
    virtual ~Thread(void);

    void *operator new (size_t );
    void operator delete (void * , size_t );
    
    /*static*/ void MutexOn(void);
    /*static*/ void MutexOff(void);
    /*static*/ int  MsecsLeft(void);
    /*static*/ void Pause(int msecs);    
    /*static*/ bool EnterBlockingOperation(void);
    /*static*/ void ExitBlockingOperation(void);
    /*static*/ void MemUnprotect(void);
    /*static*/ void MemProtect(void);
    /*static*/ void CauseTrap(void);
    /*static*/ void CauseRestart(int code, int value);
    static void CaptureStack(std::ostringstream &stack, bool full);

    bool          Create(void);
    void          Interrupt(void);
    ThreadFaction Faction(void) const { return faction_; };
    ThreadId      Tid(void) const { return tid_; };
    bool          Blocked(void) const { return blocked_; };
    void          Display(std::ostream &stream, int indent, bool verbose);

protected:
    Thread(ThreadFaction faction);
    //
    // The second constructor wraps the primordial thread. Subclasses provide
    // Enter and Recover.
    //
    Thread(ThreadFaction faction, RootThreadType);
    
    virtual void     Enter(void) = 0;
    virtual ThreadRc Recover(void) = 0;
    virtual bool     BlockingAllowed(void);
    virtual void     LogTrap(std::exception *ex, std::ostringstream &log);

    bool Quit(void) const { return quit_; };

private:
    Thread(const Thread& );
    Thread& operator=(const Thread& );
    // EnterThread is the true entry function.
    // SignalHandler catches signals. 
    // Unprotect and Reprotect support write-protected memory. 
    // Lock prevents a thread from being scheduled out until it calls Unlock.
    // 
    static unsigned EnterThread(void *arg);
    static void SignalHandler(int sig);
    void RegisterForSignal(int sig);
    void RegisterForSignals(void);
    /*static*/ void Unprotect(void);
    /*static*/ void Protect(void);
    void Lock(void);
    void Unlock(void);
    bool HandleTrap(std::exception *ex, int sig, const std::string &stack);
    void Start(void); // provides safety net

    ThreadFaction faction_; // scheduler faction
    int pid_;               // id from getpid()
    ThreadId tid_;          // assign by ThreadRegistry
    bool entered_;          // true if execution has started
    bool recovering_;       // true if recovering from a trap
    bool kill_;             // true if to be killed
    unsigned int startTime_;// time when starting to run locked
    bool blocked_;          // true if blocked
    bool quit_;             // true if destructor called
    bool interrupted_;      // true if interrupted
    void *interruptHandle_; // It is a WIN32 Event HANDLE
    void *threadHandle_;    // It is a WIN32 Thread HANDLE
    int mutexCount_;        // >0 if running locked
    int unprotectCount_;    // >0 if writing to protected memory
    int trapCount_;         // number of traps
    sigjmp_buf jmpBuffer_;     // for setjmp/longjmp
    std::ostringstream *stackLog_;
};


class ThreadRegistry : public Object
{
    friend class SingletonObject<ThreadRegistry>;
    friend class Thread;
    friend class Watchdog;
public:
    ThreadId AddThread(Thread &thread);
    ThreadId SetRootThread(Thread &thread);
    Thread   *RootThread(void);
    void     RemoveThread(Thread &thread);
    void     Display(std::ostream &stream, int indent, bool verbose);

    static Thread *RunningThread(void);

protected:
    ThreadRegistry(void);
    ~ThreadRegistry(void);

private:
    ThreadRegistry(const ThreadRegistry& );
    ThreadRegistry& operator=(const ThreadRegistry& );

    // These are original Thread static member variables 
    //
    Thread *LockedThread;        // thread running locked
    void   *RunToCompletionLock; // global lock
    bool   SafetyNetOn;          // enables signal handler
    bool   LongjmpOnSignal;      // controls signal handler

    Thread *threads_[MaxThreadId + 1]; // registry of threads
    ThreadId root_; // primorial thread
    unsigned int systemStartTime_;
};


class ExceptionWithStack : public std::exception
{
public:
    virtual ~ExceptionWithStack() throw () {};
    std::string stack(void) const { return stack_; };
protected:
    ExceptionWithStack(void);
private:
    std::string stack_; // contains stack trace
};

class FatalException : public ExceptionWithStack
{
public:
    FatalException(int code, int value);
    ~FatalException() throw () { };
    int code(void)  const { return code_; };
    int value(void) const { return value_; };
    const char *what(void) const throw () { return "fatal error"; };
private:
    int code_;  // from Thread::CauseRestart argument
    int value_; // from Thread::CauseRestart argument
};

class SignalException : public ExceptionWithStack
{
public:
    SignalException(int sig);
    ~SignalException() throw () { };
    int signal(void) const { return signal_; };
    const std::string reason(void) const;
    const char *what(void) const throw () { return "signal caught"; };
    static const std::string signalString(int sig);
private:
    int signal_; // the actual signal (e.g. SIGSEGV)
};

#endif // __XSYSLIB_THREAD_H__
