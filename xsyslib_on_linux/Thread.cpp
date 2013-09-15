#include "Thread.h"
#include "Xsyslib.h"
#include "Trace.h"
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sched.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>


using namespace std;

const int  RunToCompletionTimeout = 20; // msecs
const int  DeathOfCriticalThread = 1;
const char *strUnknownException = "unknown exception";
const char *strStackFrame = "Stack Frame";
const char *strStackUnavailable = "unavailable";
const char *strTrapDuringRecovery = "Trap during recovery.";


void Thread::Lock(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();

    ThreadRegistry *reg = SingletonObject<ThreadRegistry>::Instance();

    // Acquire the run-to-completion lock. Record the locked thread and
    // the time when it started running. Start the run-to-completion timer
    // and unprotect memory if necessary.
    //
    if(reg->LockedThread == this)
        return;
    
    // The Mutex RunToCompletionLock was created signaled. That is it is 
    // not owned by any thread. The thread is using the wait function to 
    // request ownership. 
    //
    sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op  = -1;
    sem_b.sem_flg = SEM_UNDO;
    if(semop((int)reg->RunToCompletionLock, &sem_b, 1) < 0)
        trace.ErrorSys(errno, "semop");
    
    reg->LockedThread = this;
    startTime_ = GetTickCount();
    
    itimerval t;
    t.it_interval.tv_sec  = 0;
    t.it_interval.tv_usec = 0;
    t.it_value.tv_sec     = RunToCompletionTimeout / 1000;
    t.it_value.tv_usec    = (RunToCompletionTimeout % 1000) * 1000;
    if(setitimer(ITIMER_REAL, &t, NULL) < 0)
        trace.ErrorSys(errno, "setitimer");

    if(unprotectCount_ > 0)
        Unprotect();
}

void Thread::Unlock(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();

    ThreadRegistry *reg = SingletonObject<ThreadRegistry>::Instance();

    // Reprotect memory, cancel the run-to-completion timer, clear the 
    // locked thread, and release the run-to-completion lock.
    //
    if(reg->LockedThread != this)
        return;

    if(unprotectCount_ > 0)
        Protect();

    itimerval t;
    t.it_interval.tv_sec  = 0;
    t.it_interval.tv_usec = 0;
    t.it_value.tv_sec     = 0;
    t.it_value.tv_usec    = 0;
    if(setitimer(ITIMER_REAL, &t, NULL) < 0)
        trace.ErrorSys(errno, "setitimer");

    reg->LockedThread = NULL;

    sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op  = 1;
    sem_b.sem_flg = SEM_UNDO;
    if(semop((int)reg->RunToCompletionLock, &sem_b, 1) < 0)
        trace.ErrorSys(errno, "semop");
}

void Thread::Unprotect(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();
}

void Thread::Protect(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();
}

void Thread::MutexOn(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();

    // This must be nestable. If the thread is not already locked, 
    // schedule it out before it begins to run locked so that it can only
    // run for as long as the run-to-completion timeout.
    //
    Thread *thread = ThreadRegistry::RunningThread();
    if(thread == NULL)
        trace.ErrorQuit(0, 1, "null running thread");

    thread->mutexCount_++;
    if(thread->mutexCount_ == 1)
    {
        Pause(0);
    }
}

void Thread::MutexOff(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();

    // If the thread no longer needs to run locked, schedule it out so
    // that other threads can run first.
    // 
    Thread *thread = ThreadRegistry::RunningThread();
    if(thread == NULL)
        trace.ErrorQuit(0, 1, "null running thread");

    if(thread->mutexCount_ <= 0)
    {
        thread->mutexCount_ = 0;
        return;
    }
    thread->mutexCount_--;
    if(thread->mutexCount_ == 0)
    {
        Pause(0);
    }
}

int Thread::MsecsLeft(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();

    // Tell the thread how much longer it can run. This could also be 
    // implemented with getitimer, but this is inaccurate on many systems.
    // However, the implementation below is also inaccurate if the 
    // scheduler can preempt a locked thread to run an unlocked thread.
    // Should this occur, we will understate how much time is left.
    //
    Thread *thread = ThreadRegistry::RunningThread();
    if(thread == NULL)
        trace.ErrorQuit(0, 1, "null running thread");

    return (RunToCompletionTimeout - 
        (GetTickCount() - thread->startTime_));
}

void Thread::Pause(int msecs)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.Parm("msecs", msecs)
         .EnterFunc();

    // If the thread has been interrupted, it has more work to do, so it
    // should only sleep for "zero time". If msecs is negative, the thread
    // wants to sleep "forever" (until interrupted).
    //
    Thread *thread = ThreadRegistry::RunningThread();
    if(thread == NULL)
        trace.ErrorQuit(0, 1, "null running thread");

    if(thread->interrupted_)
    {
        thread->interrupted_ = false;
        msecs = 0;
        trace.DebugOutput("thread->interrupted is true, set msecs to 0");
    }

    EnterBlockingOperation();
    if(msecs == 0)
    {
        if(sched_yield() < 0) // let others run
            trace.ErrorSys(errno, "sched_yield"); 
    }
    else if(msecs > 0)
    {
        timeval tv;
        tv.tv_sec = msecs/1000;
        tv.tv_usec = (msecs%1000)*1000;
        if(select(0, NULL, NULL, NULL, &tv) < 0)
            trace.ErrorSys(errno, "select");
    }
    else
    {
        pause();
    }
    ExitBlockingOperation();
}

void Thread::Interrupt(void)
{
    // If the thread is blocked, wake it up.
    //
    if(blocked_)
        kill(pid_, SIGALRM);
    else
        interrupted_ = true;
}

bool Thread::BlockingAllowed(void)
{
    return true; // can be overridden by subclasses
}

bool Thread::EnterBlockingOperation(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();

    ThreadRegistry *reg = SingletonObject<ThreadRegistry>::Instance();
    // When a thread could block, it must release the lock.
    //
    Thread *thread = ThreadRegistry::RunningThread();
    if(thread == NULL)
        trace.ErrorQuit(0, 1, "null running thread");

    if(!thread->BlockingAllowed())
        return false;
    if(reg->LockedThread == thread)
        thread->Unlock();
    thread->blocked_ = true;
    return true;
}

void Thread::ExitBlockingOperation(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();

    // When a previously locked thread resumes running after using a 
    // blocking operation, it becomes locked again.
    //
    Thread *thread = ThreadRegistry::RunningThread();
    if(thread == NULL)
        trace.ErrorQuit(0, 1, "null running thread");

    thread->blocked_ = false;
    if(thread->mutexCount_ > 0)
        thread->Lock();
}

void Thread::MemUnprotect(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();

    Thread *thread = ThreadRegistry::RunningThread();
    if(thread == NULL)
        trace.ErrorQuit(0, 1, "null running thread");

    if(thread->unprotectCount_ == 0)
    {
        MutexOn(); // update write-protected data in a critical region
        Unprotect();
    }
    thread->unprotectCount_++;
}

void Thread::MemProtect(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();

    Thread *thread = ThreadRegistry::RunningThread();
    if(thread == NULL)
        trace.ErrorQuit(0, 1, "null running thread");

    thread->unprotectCount_--;
    if(thread->unprotectCount_ == 0)
    {
        Protect();
        MutexOff(); // undo MutexOn in MemUnprotect
    }
    if(thread->unprotectCount_ < 0)
        thread->unprotectCount_ = 0;
}

bool Thread::Create(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();

    ThreadRegistry *reg = SingletonObject<ThreadRegistry>::Instance();

    if(reg->RootThread() == this)
        return false;

    // Create the thread and add it to the registry.
    //
    pid_t pid = fork();
    if(pid < 0)
    {
        trace.ErrorSys(errno, "fork");
        return false;
    } 
    else if(pid == 0) /* child */
    {
        Trace::NestedLevel = 1;
        trace.DebugOutput("child spawned");
        exit(EnterThread(this));
    }
    
    return true;
}

Thread::Thread(ThreadFaction faction)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.Parm("faction", faction)
         .EnterFunc();

    faction_        = faction;
    pid_            = -1;
    tid_            = -1;
    entered_        = false;
    recovering_     = false;
    kill_           = false;
    startTime_      = 0;
    blocked_        = false;
    quit_           = false;
    interrupted_    = false;
    mutexCount_     = 0;
    unprotectCount_ = 0;
    trapCount_      = 0;

    ThreadRegistry *reg = SingletonObject<ThreadRegistry>::Instance();
    tid_ = reg->AddThread(*this);
}

Thread::Thread(ThreadFaction faction, RootThreadType)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.Parm("faction", faction)
         .EnterFunc();

    // The primordial thread calls this to get wrapped.
    //
    faction_        = faction;
    pid_            = getpid();
    tid_            = -1;
    entered_        = true;
    recovering_     = false;
    kill_           = false;
    startTime_      = GetTickCount();
    blocked_        = false;
    quit_           = false;
    interrupted_    = false;
    mutexCount_     = 0;
    unprotectCount_ = 0;
    trapCount_      = 0;

    ThreadRegistry *reg = SingletonObject<ThreadRegistry>::Instance();
    tid_ = reg->SetRootThread(*this);
}

Thread::~Thread(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();

    ThreadRegistry *reg = SingletonObject<ThreadRegistry>::Instance();

    bool isRoot = (reg->RootThread() == this);

    // End the child thread gracefully
    //     
    if(!isRoot && pid_ > 0)
    {
        trace.DebugOutput("end child %d gracefully", pid_);

        quit_ = true;
        if(kill(pid_, SIGALRM) < 0)
            trace.ErrorSys(errno, "kill");

        int status;
        if(waitpid(pid_, &status, 0) < 0)
            trace.ErrorSys(errno, "waitpid");

        if(WIFEXITED(status))
            trace.DebugOutput("child %d exit %d", pid_, WEXITSTATUS(status));
        else if(WIFSIGNALED(status))
            trace.DebugOutput("child %d exit at signal %d", pid_, WTERMSIG(status));
        else
            trace.DebugOutput("child %d exit without reason", pid_);
    }

    // If the thread was locked, make sure that things get cleaned up. 
    // When a thread is killed, the OS must release any lock that the
    // thread holds. If it didn't, we would have to allocate a new 
    // run-to-completion lock to prevent the system from hanging.
    //
    if(reg->LockedThread == this)
        reg->LockedThread = NULL;

    reg->RemoveThread(*this);
}

unsigned Thread::EnterThread(void *arg)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.Parm("arg", arg)
         .EnterFunc();

    // ARG actually references a Thread object. When the Thread constructor
    // creates a thread, the thread may start to run immediately, before 
    // the Thread object is fully constructed. This leads to trouble 
    // (invocation of a pure virtual Start function). Sleep for 2 seconds
    // to give the thread time to be constructed.
    //
    // When I port it to fork, I can not creates the thread in the constructor.
    // Because the vtable is not installed in child.
    //sleep(2);
    
    ((Thread*) arg)->Start();
    return 0;
}

void Thread::SignalHandler(int sig)
{
    ThreadRegistry *reg = SingletonObject<ThreadRegistry>::Instance();

    if(!reg->SafetyNetOn)
    {
        // Turning off the safety net in the lab allows the debugger to be 
        // entered. Restore the default handling for singal and reraise it.
        // 
        signal(sig, SIG_DFL);
        raise(sig);
        return;
    }
    if(reg->LongjmpOnSignal)
    {
        // This platform does not support throwing exceptions from a signal
        // handler. Use longjmp to return to Thread::Start. Exception resumes
        // at the point where setjmp was invoked.
        //
        Trace::DebugOutput_s("Thread::SignalHandler (pid:%d) (sig:%d)", getpid(), sig);
        Thread *thread = ThreadRegistry::RunningThread();
        if(thread == NULL)
            Trace::ErrorQuit_s(0, 1, "null running thread");
        thread->stackLog_ = new ostringstream;
        thread->CaptureStack(*thread->stackLog_, true);
        siglongjmp(thread->jmpBuffer_, sig);
    }
    else
    {
        // Turn the signal into a C++ exception. Thread::Start will catch it
        // and initiate recovery action.
        // 
        throw SignalException(sig);
    }
}

void Thread::RegisterForSignal(int sig)
{
    // This should also call sigaltstack, but the simple version is
    //
    struct sigaction act;
    act.sa_handler = SignalHandler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if(sig == SIGALRM)
    {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    }
    else
    {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif
    }
    if(sigaction(sig, &act, NULL) < 0)
    {
        ostringstream stream;
        stream << " *** sigaction for " 
               << SignalException::signalString(sig);
        perror(stream.str().c_str());
    }
}

void Thread::RegisterForSignals(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();

    // The signals to register for are system specific, but this list is 
    // typical. We do not handle SIGCHLD because the details are quite 
    // platform specific. Furthermore, if the safety net works properly,
    // a child should always be able to continue unless it is killed 
    // explicitly (by kill, in the destructor) or traps when recovering_
    // is set.
    //RegisterForSignal(SIGABRT);
    //RegisterForSignal(SIGBUS);
    //RegisterForSignal(SIGFPE);
    //RegisterForSignal(SIGILL);
    RegisterForSignal(SIGINT);
    RegisterForSignal(SIGQUIT);
    //RegisterForSignal(SIGSEGV);
    //RegisterForSignal(SIGSYS);
    //RegisterForSignal(SIGTERM);
    RegisterForSignal(SIGVTALRM);
    RegisterForSignal(SIGXCPU);
    RegisterForSignal(SIGALRM);
    //RegisterForSignal(SIGUSR1);
    //RegisterForSignal(SIGUSR2);

}

void Thread::LogTrap(std::exception *, std::ostringstream &)
{
    // Subclasses override this to add their own data to trap logs.
}

void Thread::CaptureStack(std::ostringstream &, bool )
{
    // Capture the function call chain in STACK. if FULL is set,
    // arguments and locals should also be captured. This has to 
    // invoke some black magic, platform-specific function.
}

bool Thread::HandleTrap(std::exception *ex, int sig, const std::string &stack)
{
    // Build a trap log.
    //
    ostringstream trapLog;
    trapLog << "EXCEPTION in ";
    trapLog << Object::StrObj(this, false) << endl;
    trapLog << "  type: ";
    if(ex != NULL)
        trapLog << ex->what();
    else if(sig >= 0)
        trapLog << SignalException::signalString(sig);
    else
        trapLog << strUnknownException;
    trapLog << endl;

    // If the thread trapped during recovery, it dies.
    //
    if(recovering_)
    {
        kill_ = true;
        trapLog << strTrapDuringRecovery << "  ";
    }
    LogTrap(ex, trapLog);
    trapLog << strStackFrame << endl;
    trapLog << stack << endl;
    // TODO: send trapLog_ to the log system;
    Trace::ErrorSys_s(0, "%s", trapLog.str().c_str());
    ThreadRegistry *reg = SingletonObject<ThreadRegistry>::Instance();
    return reg->SafetyNetOn;
}

void Thread::CauseTrap(void)
{
    // This is useful for testing trap handling.
    //
    int *p = (int*) 0xeeeeeeef;
    if(*p == 0) // dereferencing P causes a signal
        p++;
}

void Thread::CauseRestart(int code, int value)
{
    throw FatalException(code, value);
}

void Thread::Start(void)
{
    pid_ = getpid();
    RegisterForSignals();

    for(trapCount_ = 0; !Quit(); ++trapCount_)
    {
        try
        {
            int      sig;
            ThreadRc trc = DeleteThread;

            // Save the thread's identifier. Mark this location so that the
            // signal handler can longjmp to it. Create the safety net for
            // signals.
            //
            sig = sigsetjmp(jmpBuffer_, 1);
            
            // If SIG is zero, we just invoked setjmp. If it is non-zero, 
            // we got here because the signal handler invoked longjmp.
            //
            if(sig != 0)
            {
                Trace::DebugOutput_s("Thread::Start longjmp to trap");
                
                string stack = stackLog_->str();
                delete stackLog_;
                
                if(HandleTrap(NULL, sig, stack))
                    continue;
                return;
            }

            Trace::DebugOutput_s("Thread::Start (pid:%d) (trapCount:%d)", pid_, trapCount_);
            if(trapCount_ > 0)
            {
                // The thread is being reentered after a trap. Tell it to 
                // recover and then continue as directed.
                //
                if(!recovering_)
                {
                    recovering_ = true;
                    trc = Recover();
                    recovering_ = false;
                }

                if((kill_) && (trc == ReenterThread))
                    trc = DeleteThread;

                switch(trc)
                {
                case DeleteThread:
                	return;
                case ReenterThread:
                    break;
                default:
                    CauseRestart(DeathOfCriticalThread, pid_);
                }

                // If the thread was running locked, unlock it and force
                // it to yield. Its Enter function will relock it.
                // 
                if(mutexCount_ > 0)
                {
                    mutexCount_ = 0;
                    Pause(0);
                }
            }
            
            // Record the time when the thread started to run and invoke
            // its entry function.
            //
            startTime_ = GetTickCount();
            entered_ = true;
            Enter();
        }
        catch (FatalException &ex)
        {
            ostringstream trapLog;
            trapLog << "FATAL ERROR in " << endl;
            trapLog << Object::StrObj(this, false);
            trapLog << endl;
            trapLog << "  code:  " << ex.code() << endl;
            trapLog << "  value: " << ex.value() << endl;
            trapLog << strStackFrame << endl;
            trapLog << ex.stack() << endl;
            // TODO: save trapLog in the flight recorder;
            // TODO: interrupt the primordial thread to cause a restart;
        }
        // We can recover from other exceptions.
        //
        catch (ExceptionWithStack &ex)
        {
            if(HandleTrap(&ex, -1, ex.stack()))
                continue;
            throw(ex);
        }
        catch (exception &ex)
        {
            if(HandleTrap(&ex, -1, strStackUnavailable))
                continue;
            throw(ex);
        }
        catch(...)
        {
            if(HandleTrap(NULL, -1, strStackUnavailable))
                continue;
            throw;
        }
    }
}

ThreadRegistry::ThreadRegistry(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();

    for(int i = 0; i <= MaxThreadId; ++i)
        threads_[i] = NULL;
    root_ = NilThreadId;
    systemStartTime_ = GetTickCount();

    key_t sem_key = ftok(PathName, SemaphoreId);
    if(sem_key < 0)
        trace.ErrorQuit(errno, 1, "ftok");

    int sem_id = semget(sem_key, 1, IPC_CREAT | 0666);
    if(sem_id < 0)
        trace.ErrorQuit(errno, 1, "semget");


    union semun 
    {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;
    arg.val = 1;
    if(semctl(sem_id, 0, SETVAL, arg) < 0)
        trace.ErrorQuit(errno, 1, "semctl");


    RunToCompletionLock = (void *)sem_id;
    LockedThread = NULL;
    SafetyNetOn = true;
    LongjmpOnSignal = true;
}

ThreadRegistry::~ThreadRegistry(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();

    // Delete threads in reverse order of their creation.
    //
    for(int i = MaxThreadId; i >= 0; --i)
        delete threads_[i];

    if(semctl((int)RunToCompletionLock, 0, IPC_RMID) < 0)
        trace.ErrorSys(errno, "semctl");
}

ThreadId ThreadRegistry::AddThread(Thread &thread)
{
    for(int i = 0; i <= MaxThreadId; ++i)
    {
        if(threads_[i] == NULL)
        {
            threads_[i] = &thread;
            return i;
        }
    }
    return NilThreadId;
}

void ThreadRegistry::RemoveThread(Thread &thread)
{
    ThreadId tid = thread.tid_;
    if((tid < 0) || (tid > MaxThreadId))
        return;
    if(threads_[tid] != &thread)
        return;
    threads_[tid] = NULL;
}

ThreadId ThreadRegistry::SetRootThread(Thread &root)
{
    // ROOT has been wrapped. If there is no root thread, assume that
    // it is the root thread, else assume it is a child thread.
    //
    if(root_ == NilThreadId)
    {
        root_ = AddThread(root);
        return root_;
    }
    return AddThread(root);
}

Thread *ThreadRegistry::RootThread(void)
{
    if(root_ == NilThreadId)
        return NULL;
    return threads_[root_];
}

Thread *ThreadRegistry::RunningThread(void)
{
    ThreadRegistry *reg = SingletonObject<ThreadRegistry>::Instance();

    // Most threads run to completion, so the running thread is usually
    // the locked thread. If it isn't, we have to search for it the hard
    // way.
    //
    int pid = getpid();
    Thread *thread = reg->LockedThread;
    if((thread != NULL) && (thread->pid_ == pid))
        return thread;

    for(ThreadId tid = 0; tid < MaxThreadId; ++tid)
    {
        thread = reg->threads_[tid];
        if((thread != NULL) && (thread->pid_ == pid))
            return thread;
    }

    return NULL;
}

ExceptionWithStack::ExceptionWithStack()
{
    ostringstream stack;
    Thread::CaptureStack(stack, true);
    stack_ = stack.str();
}

FatalException::FatalException(int code, int value)
{
    code_ = code;
    value_ = value;
}

const char *SigAbrtStr = "program aborted";
const char *SigBusStr  = "bus error";
const char *SigFpeStr  = "floating point error";
const char *SigIllStr  = "illegal instruction";
const char *SigIntStr  = "program interrupted";
const char *SigQuitStr = "program quit";
const char *SigSegvStr = "segmentation violation";
const char *SigSysStr  = "invalid system call";
const char *SigTermStr = "program terminated";
const char *SigVtAlrmStr = "virtual alarm timeout";
const char *SigXcpuStr = "program ran too long";
const char *SigAlrmStr = "alarm timeout";
const char *SigUsr1Str = "user-defined signal 1";
const char *SigUsr2Str = "user-defined signal 2";
const char *SigUnknStr = "unknown signal";

SignalException::SignalException(int sig)
{
    signal_ = sig;
}

const string SignalException::reason(void) const
{
    return signalString(signal_);
}

const string SignalException::signalString(int sig)
{
    switch(sig)
    {
    case SIGABRT: return SigAbrtStr;
    case SIGBUS: return SigBusStr;
    case SIGFPE: return SigFpeStr;
    case SIGILL: return SigIllStr;
    case SIGINT: return SigIntStr;
    case SIGQUIT: return SigQuitStr;
    case SIGSEGV: return SigSegvStr;
    case SIGSYS: return SigSysStr;
    case SIGTERM: return SigTermStr;
    case SIGVTALRM: return SigVtAlrmStr;
    case SIGXCPU: return SigXcpuStr;
    case SIGALRM: return SigAlrmStr;
    case SIGUSR1: return SigUsr1Str;
    case SIGUSR2: return SigUsr2Str;
    }
    return SigUnknStr;
}

void Thread::Display(std::ostream &stream, int indent, bool verbose)
{
    Object::Display (stream, indent, verbose);
    indent = indent + 2;
    stream << Spaces(indent) << "threadID  : " << pid_ << endl;
    stream << Spaces(indent) << "startTime : " << startTime_ << endl;
}

void ThreadRegistry::Display(std::ostream &stream, int indent, bool verbose)
{
    Object::Display (stream, indent, verbose);
    indent = indent + 2;

    if(LockedThread != NULL)
        stream << Spaces(indent) << "LockedThread    : " << LockedThread->pid_ << endl;
    else
        stream << Spaces(indent) << "LockedThread    : " << endl;
    stream << Spaces(indent) << "SafetyNetOn     : " << SafetyNetOn << endl;
    stream << Spaces(indent) << "LongjmpOnSignal : " << LongjmpOnSignal << endl;
    stream << Spaces(indent) << "SystemStartTime : " << systemStartTime_ << endl;
    stream << Spaces(indent) << "Threads         : " << endl;
    for(int i = 0; i <= MaxThreadId; ++i)
        if(threads_[i] != NULL)
            threads_[i]->Display(stream, indent, verbose);
    stream << Spaces(indent) << "Root Thread     : " << endl;
    if(root_ != NilThreadId)
        threads_[root_]->Display(stream, indent, verbose);
}

void *Thread::operator new (size_t size)
{
    void *p = Alloc(size);
    if(p == NULL)
        Trace::ErrorSys_s(0, "Thread::operator new(size:%u): not enough memory", size);
    return p;
}

void Thread::operator delete (void * , size_t )
{
    // do nothing
}
