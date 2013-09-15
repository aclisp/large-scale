#ifndef __XSYSLIB_UDPSERVICE_H__
#define __XSYSLIB_UDPSERVICE_H__


#include "Thread.h"


class udpService : public Thread
{
public:
    udpService(void);
    ~udpService(void);

private:
    void Enter(void);
    ThreadRc Recover(void);


};

#endif // __XSYSLIB_UDPSERVICE_H__
