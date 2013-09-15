// -*- mode: c++ -*-

/* Author:   Huanghao@cellcom
 * Date:     2005/11/26 23:14:56
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RADFWK_REACTOR_H__
#define __RADFWK_REACTOR_H__

#include <radfwk/RAD_Object.h>

class RAD_Reactor;
class RAD_Handler_List;

class RADFWK_API RAD_Event_Handler : public RAD_Object
                                   , public lrad_ring_entry
{
public:
    typedef int            HANDLE;
    typedef unsigned int   Reactor_Mask;
    typedef struct timeval timeval_t;

    enum {
        NULL_MASK       = 0,

        READ_MASK       = (1 << 0),
        WRITE_MASK      = (1 << 1),
        EXCEPT_MASK     = (1 << 2),
        ACCEPT_MASK     = (1 << 3),
        CONNECT_MASK    = (1 << 4),
        TIMER_MASK      = (1 << 5),
        ALL_EVENTS_MASK = READ_MASK    |
                          WRITE_MASK   |
                          EXCEPT_MASK  |
                          ACCEPT_MASK  |
                          CONNECT_MASK |
                          TIMER_MASK,
        RWE_MASK        = READ_MASK   |
                          WRITE_MASK  |
                          EXCEPT_MASK,
        DONT_CALL       = (1 << 6),
        REMOVING        = (1 << 7)
    };

    virtual ~RAD_Event_Handler();

    virtual HANDLE get_handle (void)   const = 0;
    virtual void   set_handle (HANDLE);

    virtual int handle_input     ();
    virtual int handle_output    ();
    virtual int handle_exception ();
    virtual int handle_idle      (const timeval_t& now);
    virtual int handle_timeout   (const timeval_t& now, void* act);
    virtual int handle_close     (Reactor_Mask close_mask);

    virtual void reactor(RAD_Reactor *reactor);
    virtual RAD_Reactor *reactor(void) const;

    virtual void owner(RAD_Handler_List *ol);
    virtual RAD_Handler_List *owner(void) const;

    virtual void mask(Reactor_Mask m);
    virtual Reactor_Mask mask(void) const;

    virtual string ToString() const;

protected:
    /* Force RAD_Event_Handler to be an abstract base class. */
    RAD_Event_Handler();

private:
    RAD_Reactor *reactor_;
    RAD_Handler_List* owner_;
    Reactor_Mask mask_;
};

class RADFWK_API RAD_Reactor : public RAD_Object
{
public:
    RAD_Reactor();
    virtual ~RAD_Reactor();

    int register_handler (RAD_Event_Handler* , RAD_Event_Handler::Reactor_Mask);
    int remove_handler   (RAD_Event_Handler* , RAD_Event_Handler::Reactor_Mask);

    size_t max_size(void) const;
    void max_size(size_t maxs);

    size_t fd_setsize(void) const;
    void fd_setsize(size_t s);

    size_t size(void) const;
    void resize(size_t newsize);

    void dump(void) const;

private:
    RAD_Reactor(const RAD_Reactor& );
    RAD_Reactor& operator=(const RAD_Reactor& other);

    mutable RAD_Mutex lock;
    size_t fd_setsize_;
    size_t maxsize_;
    size_t size_;
    lrad_ring_entry hg;
};

#endif // __RADFWK_REACTOR_H__
