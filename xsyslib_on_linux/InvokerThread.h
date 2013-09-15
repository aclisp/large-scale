#ifndef __XSYSLIB_INVOKERTHREAD_H__
#define __XSYSLIB_INVOKERTHREAD_H__


#include "Thread.h"
#include "LeakyBucketCounter.h"


class InvokerThread : public Thread
{
public:
    InvokerThread(void);
    ~InvokerThread(void);

private:
    void Enter(void);
    ThreadRc Recover(void);

    LeakyBucketCounter recoverCounter_;
};

#endif // __XSYSLIB_INVOKERTHREAD_H__
