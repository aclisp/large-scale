#include "InvokerThread.h"
#include "Trace.h"
#include <sstream>

using namespace std;

InvokerThread::InvokerThread(void)
    : Thread(PayloadFaction)
    , recoverCounter_(5, 1)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();
}


InvokerThread::~InvokerThread(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();
}

void InvokerThread::Enter(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();
    
    MutexOn();
    while(true)
    {
        
    }
}

ThreadRc InvokerThread::Recover(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();
    
    Trace::NestedLevel--;
    
    trace.DebugOutput("MsecsLeft = %d", MsecsLeft());

    if(recoverCounter_.ThresholdExceeded())
        return DeleteThread;
    else
        return ReenterThread;
}
