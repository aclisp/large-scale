#ifndef __XSYSLIB_ROOTTHREAD_H__
#define __XSYSLIB_ROOTTHREAD_H__


#include "Thread.h"


class RootThread : public Thread
{
public:
    RootThread(void);
    ~RootThread(void);

private:
    void Enter(void);
    ThreadRc Recover(void);
};

#endif // __XSYSLIB_ROOTTHREAD_H__
