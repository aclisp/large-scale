#include "radutil.h"

#include "Event_Handler.h"

Event_Handler::Event_Handler()
{
    reactor_ = NULL;
    owner_ = NULL;
    mask_ = NULL_MASK;
}

Event_Handler::~Event_Handler()
{
    rad_assert(owner_ == NULL);
    rad_assert(mask_ == NULL_MASK);
}

void Event_Handler::set_handle(Event_Handler::HANDLE h)
{
}

int Event_Handler::handle_input()
{
    return 0;
}

int Event_Handler::handle_output()
{
    return 0;
}

int Event_Handler::handle_exception()
{
    return 0;
}

int Event_Handler::handle_idle(const timeval_t& now)
{
    return 0;
}

int Event_Handler::handle_timeout(const timeval_t& now, void* act)
{
    return 0;
}

int Event_Handler::handle_close(Event_Handler::Reactor_Mask close_mask)
{
    return 0;
}

void Event_Handler::reactor(Reactor *reactor)
{
    reactor_ = reactor;
}

Reactor *Event_Handler::reactor(void) const
{
    return reactor_;
}

void Event_Handler::owner(Handler_List *ol)
{
    owner_ = ol;
}

Handler_List *Event_Handler::owner(void) const
{
    return owner_;
}

void Event_Handler::mask(Event_Handler::Reactor_Mask m)
{
    mask_ = m;
}

Event_Handler::Reactor_Mask Event_Handler::mask(void) const
{
    return mask_;
}

const char* Event_Handler::to_string() const
{
    return "";
}
