#ifndef __XSYSLIB_MESSAGEPOOL_H__
#define __XSYSLIB_MESSAGEPOOL_H__

#include "ObjectPool.h"
#include "SingletonObject.h"

// MessageBlockSize is the maximum number of bytes in a Message subclass.
//
const size_t MessageBlockSize = 1600;

class MessagePool : public ObjectPool
{
    friend class SingletonObject<MessagePool>;
protected:
    MessagePool(void) : ObjectPool(MessageBlockSize) { };
    ~MessagePool(void) { };
};

#endif // __XSYSLIB_MESSAGEPOOL_H__
