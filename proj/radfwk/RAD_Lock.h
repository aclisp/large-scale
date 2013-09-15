// -*- mode: c++ -*-

/* Author:   Huanghao@cellcom
 * Date:     2005/11/23 14:57:33
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RADFWK_LOCK_H__
#define __RADFWK_LOCK_H__

#include <radfwk/RAD_Object.h>

class RADFWK_API RAD_Lock : public RAD_Object
{
public:
    explicit RAD_Lock(bool can_broadcast = false);
    virtual ~RAD_Lock();

    bool TryAcquire(unsigned int delay, const char *file, unsigned int line);
    bool Acquire(unsigned int delay, const char *file, unsigned int line);
    bool Release(const char *file, unsigned int line);
    bool Wait(unsigned int delay, const char *file, unsigned int line);
    bool Signal(const char *file, unsigned int line);
    bool Broadcast(const char *file, unsigned int line);

    // @deprecated
    bool WaitEvent(unsigned int delay, const char *file, unsigned int line);
    void PulseEvent(const char *file, unsigned int line);

    void MakeRecursive(bool);
    int  IsHolding() const;
    int  IsWaiting() const;
    void Reset();
private:
    RAD_Lock(const RAD_Lock& );
    RAD_Lock& operator=(const RAD_Lock& other);
    struct Impl {
#ifdef RAD_OS_WIN32
        unsigned int mutex;
        unsigned int event;
#else
        pthread_mutex_t mutex;
        pthread_cond_t  event;
#endif
        char         state[4];
    } m_impl;
};

#endif // __RADFWK_LOCK_H__
