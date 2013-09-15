#ifndef RADFWK_REACTOR_H
#define RADFWK_REACTOR_H

#include <radfwk/noncopyable.hpp>

class Event_Handler;

class Reactor : private boost::noncopyable
{
    class Detail;

public:
    Reactor();
    ~Reactor();

    int register_handler (Event_Handler* , Event_Handler::Reactor_Mask);
    int remove_handler   (Event_Handler* , Event_Handler::Reactor_Mask);

    size_t max_size(void) const;
    void max_size(size_t maxs);

    size_t fd_setsize(void) const;
    void fd_setsize(size_t s);

    size_t size(void) const;
    void resize(size_t newsize);

    void dump(void) const;

private:
    Detail* pImpl;
};

#endif /* RADFWK_REACTOR_H */

// Local Variables:
// mode:C++
// End:
