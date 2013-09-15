#ifndef __XSYSLIB_POOLEDOBJECT_H__
#define __XSYSLIB_POOLEDOBJECT_H__

#include "Object.h"

class ObjectPool;

class PooledObject : public Object
{
    friend class ObjectPool;
public:
    virtual ~PooledObject(void) { };
    virtual ObjectPool *Pool(void) = 0; // subclass must identify pool
    virtual void PostInitialize() { }; // used with object template

protected:
    PooledObject(void) { link_ = NULL; }; // not on free queue

private:
    PooledObject *link_; // link when on free queue
};

#endif // __XSYSLIB_POOLEDOBJECT_H__
