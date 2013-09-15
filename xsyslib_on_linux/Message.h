#ifndef __XSYSLIB_MESSAGE_H__
#define __XSYSLIB_MESSAGE_H__

#include "PooledObject.h"

class Message : public PooledObject
{
public:
    virtual ~Message(void) { };
    virtual ObjectPool *Pool(void);
    virtual void PostInitialize(void);

    void *operator new (size_t size);
    void operator delete (void *msg, size_t );

protected:
    Message(void) { }; // this is an abstract class

private:
    
};

#endif // __XSYSLIB_MESSAGE_H__
