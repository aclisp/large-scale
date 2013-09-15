#ifndef __XSYSLIB_SINGLETONOBJECT_H__
#define __XSYSLIB_SINGLETONOBJECT_H__

#include "Xsyslib.h"
#include "Trace.h"
#include <stdlib.h>
#include <stdio.h>

template<class T> 
class SingletonObject
{
public: 
    static void Initialize(void);
    static void Destroy(void);

    inline static T *Instance(void)
    {
        return instance_;
    };

private:
    SingletonObject(void);
    ~SingletonObject(void);
    SingletonObject(const SingletonObject<T>& );
    SingletonObject<T>& operator=(const SingletonObject<T>& );

    static T *instance_; // the singleton instance
};

template<class T>
void SingletonObject<T>::Initialize(void)
{
    void *p = Alloc(sizeof(T));
    if(p == NULL)
        Trace::ErrorQuit_s(0, 1, "Initialize singleton: not enough memory");

    if(instance_ == NULL) 
        instance_ = new(p) T;
}

template<class T>
void SingletonObject<T>::Destroy(void)
{
    if(instance_ != NULL)
    {
        instance_->T::~T();
        instance_ = NULL;
    }
}

// For initializing each singleton's instance_
//
template<class T> 
T *SingletonObject<T>::instance_ = NULL;

#endif // __XSYSLIB_SINGLETONOBJECT_H__
