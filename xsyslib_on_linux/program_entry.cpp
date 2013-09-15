#include "Xsyslib.h"
#include "Thread.h"
#include "RootThread.h"
#include "InvokerThread.h"
#include "Trace.h"
#include "MessagePool.h"
#include "TlvMessage.h"
#include "udpService.h"


using namespace std;


int main(int , char *[])
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();

    InitializeSharedMemory();
    SingletonObject<ThreadRegistry>::Initialize();
    trace.DebugOutput("ThreadRegistry size is %u", sizeof(ThreadRegistry));
    SingletonObject<MessagePool>::Initialize();
    trace.DebugOutput("MessagePool size is %u", sizeof(MessagePool));
    MessagePool *pool = SingletonObject<MessagePool>::Instance();
    pool->SetSegments(1);
    pool->Create();
    trace.DebugOutput("RootThread size is %u", sizeof(RootThread));
    trace.DebugOutput("InvokerThread size is %u", sizeof(InvokerThread));
    RootThread *rootThread = new RootThread;
    if(rootThread == NULL)
        return 1;
    signal(SIGALRM, SIG_IGN);
    rootThread->MutexOn();

    udpService *udp = new udpService;
    if(udp == NULL)
        return 1;
    InvokerThread *invokerThread = new InvokerThread;
    if(invokerThread == NULL)
        return 1;

    udp->Create();
    invokerThread->Create();
    
    rootThread->Pause(1000);

    trace.DebugOutput("main resumes");

    /*
    TlvMessage *msg = new TlvMessage;
    TlvParmLayout *parm;
    ParmIterator pit;

    parm = msg->FirstParm(pit);
    if(parm == NULL)
        trace.DebugOutput("msg->FirstParm is null");

    parm = msg->AddParm(1001, 0);
    if(parm == NULL)
        trace.DebugOutput("msg->AddParm 1001 failed");
    parm = msg->AddParm(1002, 6);
    if(parm == NULL)
        trace.DebugOutput("msg->AddParm 1002 failed");
    parm = msg->AddParm(1003, 9);
    if(parm == NULL)
        trace.DebugOutput("msg->AddParm 1003 failed");
    parm = msg->AddParm(1004, 17);
    if(parm == NULL)
        trace.DebugOutput("msg->AddParm 1004 failed");
    DebugDumpBinary(msg, 120, msg);

    for(parm = msg->FirstParm(pit); parm != NULL; parm = msg->NextParm(pit))
    {
        DebugDumpBinary(parm, sizeof(TlvParmHeader) + parm->header.length, parm);
    }

    trace.DebugOutput("msg->FindParm");
    parm = msg->FindParm(1003);
    if(parm != NULL)
        DebugDumpBinary(parm, sizeof(TlvParmHeader) + parm->header.length, parm);
    */

    pool->Display(cout, 0, true);
    DisplaySharedMemory();
    SingletonObject<ThreadRegistry>::Instance()->Display(cout, 0, true);

    SingletonObject<MessagePool>::Destroy();
    SingletonObject<ThreadRegistry>::Destroy();
    DestorySharedMemory();
    return 0;
}
