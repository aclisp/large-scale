#include "Class.h"
#include "PooledObject.h"
#include "ObjectPool.h"
#include <string.h>

Class::Class(size_t size)
{
    size_ = size;
    vptr_ = NULL;
    template_ = NULL;
    pool_ = NULL;
    quasi_ = false;
    singleton_ = NULL;
}

Class::~Class(void)
{
}

bool Class::SetTemplate(PooledObject &obj)
{
    // Register OBJ as this class's template. The class must already have 
    // called SetPool, OBJ's pool must match that pool, and OBJ must not be 
    // the quasi-singleton instance!
    // 
    ObjectPool *p = obj.Pool();
    if((p == NULL) || (p != pool_)) 
        return false;
    if(&obj == singleton_) 
        return false;
    template_ = &obj;
    return true;
}

bool Class::SetQuasiSingleton(PooledObject &obj)
{
    // Register OBJ as this class's quasi-singleton. The class must already
    // have called SetPool, OBJ's pool must match that pool, and OBJ  must 
    // not be the template instance!
    //
    ObjectPool *p = obj.Pool();
    if((p == NULL) || (p != pool_)) 
        return false;
    if(&obj == template_) 
        return false;
    singleton_ = &obj;
    quasi_ = true;
    return true;
}

PooledObject *Class::Create(void)
{
    // Create a new object and initialize it using the template. If the 
    // class has a quasi-singleton, use it for the object, else allocate 
    // a block from the class's object pool.
    //
    PooledObject *obj;
    if(template_ == NULL) 
        return NULL;
    if(quasi_)
        obj = GetQuasiSingleton();
    else
        obj = pool_->DeqBlock(size_);
    if(obj != NULL)
    {
        memcpy(obj, template_, size_);
        obj->PostInitialize();
    }
    return obj;
}

PooledObject *Class::GetQuasiSingleton(void)
{
    // If the quasi-singleton is available, return it, else allocate a new
    // block from the class's pool.
    // 
    PooledObject *p = singleton_;
    if(p != NULL)
    {
        singleton_ = NULL;
        return p;
    }
    return pool_->DeqBlock(size_);
}

void Class::FreeQuasiSingleton(PooledObject &obj)
{
    // If no quasi-singleton exists, this object becomes the singleton. 
    // If a quasi-singleton already exists, return this object to its
    // pool.
    //
    if(singleton_ == NULL)
        singleton_ = &obj;
    else
        pool_->EnqBlock(obj);
}

