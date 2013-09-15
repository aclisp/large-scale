#ifndef __XSYSLIB_CLASS_H__
#define __XSYSLIB_CLASS_H__

#include "Object.h"

class PooledObject;
class ObjectPool;

class Class : public Object
{
    friend class Object;
public:
    virtual ~Class(void);
    // A subclass overrides SetAttrs to call the protected Set... functions defined below.
    virtual void SetAttrs() = 0;
    PooledObject *Create(void);
    PooledObject *GetQuasiSingleton(void);
    void         FreeQuasiSingleton(PooledObject &obj);

protected:
    Class(size_t size);
    void SetPool(ObjectPool &pool) { pool_ = &pool; };
    void SetVptr(Object &obj) { vptr_ = obj.GetVptr(); };
    bool SetTemplate(PooledObject &obj);
    bool SetQuasiSingleton(PooledObject &obj);

private:
    Class(const Class& );
    Class& operator=(const Class& );

    ClassVptr    vptr_;
    PooledObject *template_;
    size_t       size_;
    ObjectPool   *pool_;
    bool         quasi_;
    PooledObject *singleton_;
};

#endif // __XSYSLIB_CLASS_H__
