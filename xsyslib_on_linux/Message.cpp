#include "Message.h"
#include "MessagePool.h"
#include "Trace.h"

ObjectPool *Message::Pool(void)
{
    return SingletonObject<MessagePool>::Instance();
}

void *Message::operator new (size_t size)
{
    ObjectPool *pool = SingletonObject<MessagePool>::Instance();
    void *p = pool->DeqBlock(size);
    if(p == NULL)
        Trace::ErrorSys_s(0, "Message::operator new(size:%u): not enough memory", size);
    return p;
}


void Message::operator delete (void *msg, size_t )
{
    ObjectPool *pool = SingletonObject<MessagePool>::Instance();
    pool->EnqBlock(*((PooledObject*) msg));
}

void Message::PostInitialize(void)
{
    PooledObject::PostInitialize();
}
