#include "udpService.h"


udpService::udpService(void)
    : Thread(PayloadFaction)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();
}

udpService::~udpService(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();
}

void udpService::Enter(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();

    // allocate a socket and bind it to a port
}

ThreadRc udpService::Recover(void)
{
    Trace trace(__PRETTY_FUNCTION__);
    trace.EnterFunc();
}
