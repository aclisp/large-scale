#include "RootThread.h"
#include "Trace.h"

RootThread::RootThread(void)
    : Thread(MaintenanceFaction, RootThreadType())
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();
}


RootThread::~RootThread(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();
}

void RootThread::Enter(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();
}

ThreadRc RootThread::Recover(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();

    return ReenterThread;
}
