#ifndef RADFWK_EVENT_HANDLER_H
#define RADFWK_EVENT_HANDLER_H

#include <radutil/radheaptrack.h>
#include <radfwk/radalloc.hpp>

class Reactor;
struct Handler_List;

class Event_Handler : public lrad_ring_entry
                    , public rad_alloc_base<rad_track_alloc>
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

    virtual ~Event_Handler();

    virtual HANDLE get_handle (void)   const = 0;
    virtual void   set_handle (HANDLE);

    virtual int handle_input     ();
    virtual int handle_output    ();
    virtual int handle_exception ();
    virtual int handle_idle      (const timeval_t& now);
    virtual int handle_timeout   (const timeval_t& now, void* act);
    virtual int handle_close     (Reactor_Mask close_mask);

    virtual void reactor(Reactor *reactor);
    virtual Reactor *reactor(void) const;

    virtual void owner(Handler_List *ol);
    virtual Handler_List *owner(void) const;

    virtual void mask(Reactor_Mask m);
    virtual Reactor_Mask mask(void) const;

    virtual const char* to_string() const;

protected:
    /* Force Event_Handler to be an abstract base class. */
    Event_Handler();

private:
    Reactor *reactor_;
    Handler_List* owner_;
    Reactor_Mask mask_;
};

#endif /* RADFWK_EVENT_HANDLER_H */

// Local Variables:
// mode:C++
// End:
