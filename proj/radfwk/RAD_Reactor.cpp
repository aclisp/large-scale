#include "RAD_Reactor.h"
#include "RAD_Thread.h"

#if defined(RAD_OS_WIN32)
  #include <winsock2.h>
#else
  #include <errno.h>
  #include <netdb.h>
  #include <unistd.h>
  #include <sys/socket.h>
  #include <sys/time.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>

  #define SOCKET_ERROR -1
  #define INVALID_SOCKET -1
#endif

typedef vector<RAD_Event_Handler*> hd_set;

static void handle_idle(const hd_set& idle);
static void handle_exception(const hd_set& ehs, const fd_set& efs);
static void handle_output(const hd_set& whs, const fd_set& wfs);
static void handle_input(const hd_set& rhs, const fd_set& rfs);

class RAD_Handler_List : public RAD_Thread
                   , public lrad_ring_entry
{
public:
    RAD_Mutex lock;
    size_t maxsize;
    size_t size;
    lrad_ring_entry data;

    RAD_Handler_List(size_t maxs)
        {
            m_ThreadName = "RAD_Reactor";
            maxsize = maxs;
            if(maxsize > FD_SETSIZE) {
                radlog(L_WARN|L_CONS, "[RAD_Handler_List ctor] "
                       "maxsize(=%u) exceeds FD_SETSIZE(=%u)",
                       maxsize, FD_SETSIZE);
                maxsize = FD_SETSIZE;
            }
            size = 0;
            lrad_ring_init_head(&data);
            if(!StartThread())
                radlog(L_ERROR|L_CONS, "[RAD_Handler_List ctor] "
                       "can not create thread for event looping");
        }

    ~RAD_Handler_List()
        {
            StopAndWait(2000);
            close();
        }

    void close()
        {
            while(size)
                remove(static_cast<RAD_Event_Handler*>(data.next),
                       RAD_Event_Handler::ALL_EVENTS_MASK|
                       RAD_Event_Handler::REMOVING);
        }

    /* This operation should be very fast. So if duplicate handler is
     * added, the event loop will check it out. [TODO]
     */
    int add(RAD_Event_Handler* handler, RAD_Event_Handler::Reactor_Mask mask)
        {
            int rc = -1;
            if(!rad_lock(lock))
                return rc;
            rad_assert(mask != RAD_Event_Handler::NULL_MASK);
            rad_assert((mask&RAD_Event_Handler::DONT_CALL) == 0);
            if(handler->owner() == NULL) {
                if(size < maxsize) {
                    rad_assert(handler->mask() == RAD_Event_Handler::NULL_MASK);
                    handler->owner(this);
                    handler->mask(mask);
                    lrad_ring_insert_tail(&data, handler);
                    ++ size;
                    rc = 0;
                }
            } else {
                handler->mask(handler->mask()|mask);
                rc = 1;
            }
            rad_unlock(lock);
            return rc;
        }

    /* This operation should also be very fast. That's why lrad_ring
     * is used. !!!ATTENTION!!! a handler should NOT be removed for a
     * second time.
     */
    int remove(RAD_Event_Handler* handler, RAD_Event_Handler::Reactor_Mask mask)
        {
            int rc = -1;
            if(!rad_lock(lock))
                return rc;
            rad_assert(handler->owner() == this);
            RAD_Event_Handler::Reactor_Mask m =
                handler->mask()&~(mask&~RAD_Event_Handler::DONT_CALL);
            if(m == RAD_Event_Handler::NULL_MASK) {
                rad_assert(size != 0);
                lrad_ring_remove_entry(handler);
                -- size;
                handler->owner(NULL);
                handler->mask(RAD_Event_Handler::NULL_MASK);
                rc = 0;
            } else {
                handler->mask(m);
                rc = 1;
            }
            if((mask&RAD_Event_Handler::DONT_CALL) == 0)
                handler->handle_close(mask);
            rad_unlock(lock);
            return rc;
        }

    virtual int MainLoop();
};

RAD_Event_Handler::RAD_Event_Handler()
    : RAD_Object("RAD_Event_Handler")
{
    reactor_ = NULL;
    owner_ = NULL;
    mask_ = NULL_MASK;
}

RAD_Event_Handler::~RAD_Event_Handler()
{
    rad_assert(owner_ == NULL);
    rad_assert(mask_ == NULL_MASK);
}

void RAD_Event_Handler::set_handle(RAD_Event_Handler::HANDLE h)
{
}

int RAD_Event_Handler::handle_input()
{
    return 0;
}

int RAD_Event_Handler::handle_output()
{
    return 0;
}

int RAD_Event_Handler::handle_exception()
{
    return 0;
}

int RAD_Event_Handler::handle_idle(const timeval_t& now)
{
    return 0;
}

int RAD_Event_Handler::handle_timeout(const timeval_t& now, void* act)
{
    return 0;
}

int RAD_Event_Handler::handle_close(RAD_Event_Handler::Reactor_Mask close_mask)
{
    return 0;
}

void RAD_Event_Handler::reactor(RAD_Reactor *reactor)
{
    reactor_ = reactor;
}

RAD_Reactor *RAD_Event_Handler::reactor(void) const
{
    return reactor_;
}

void RAD_Event_Handler::owner(RAD_Handler_List *ol)
{
    owner_ = ol;
}

RAD_Handler_List *RAD_Event_Handler::owner(void) const
{
    return owner_;
}

void RAD_Event_Handler::mask(RAD_Event_Handler::Reactor_Mask m)
{
    mask_ = m;
}

RAD_Event_Handler::Reactor_Mask RAD_Event_Handler::mask(void) const
{
    return mask_;
}

string RAD_Event_Handler::ToString() const
{
    return "";
}

RAD_Reactor::RAD_Reactor()
    : RAD_Object("RAD_Reactor")
{
    fd_setsize_ = FD_SETSIZE;
    maxsize_ = 1;
    size_ = 1;
    lrad_ring_init_head(&hg);
    lrad_ring_insert_head(&hg, rad_new RAD_Handler_List(fd_setsize_));
}

RAD_Reactor::~RAD_Reactor()
{
    RAD_Handler_List* hl;
    while((hl = static_cast<RAD_Handler_List*>(
               lrad_ring_remove_head(&hg))) != &hg)
        delete hl;
}

int RAD_Reactor::register_handler(RAD_Event_Handler* handler,
                                  RAD_Event_Handler::Reactor_Mask mask)
{
    int rc = -1;
    size_t i;
    RAD_Handler_List* hl;
    if(!rad_lock(lock))
        return rc;
    for(i = 0; i < size_; ++ i) {
        hl = static_cast<RAD_Handler_List*>(lrad_ring_remove_head(&hg));
        rc = hl->add(handler, mask);
        if(rc == 0) {
            lrad_ring_insert_tail(&hg, hl);
            break;
        } else if(rc == 1) {
            lrad_ring_insert_head(&hg, hl);
            break;
        } else {
            lrad_ring_insert_tail(&hg, hl);
        }
    }
    rad_unlock(lock);
    return rc;
}

int RAD_Reactor::remove_handler(RAD_Event_Handler* handler,
                                RAD_Event_Handler::Reactor_Mask mask)
{
    int rc = -1;
    if(!rad_lock(lock))
        return rc;
    RAD_Handler_List* hl = handler->owner();
    rc = hl->remove(handler, mask);
    if(rc == 0) {
        lrad_ring_remove_entry(hl);
        lrad_ring_insert_head(&hg, hl);
    }
    rad_unlock(lock);
    return rc;
}

void RAD_Reactor::resize(size_t newsize)
{
    if(!rad_lock(lock))
        return;
    if(newsize > maxsize_)
        maxsize_ = newsize;
    int delta = newsize - size_;
    if(delta > 0) {
        for(int i = 0; i < delta; ++ i) {
            lrad_ring_insert_head(&hg,
                                  rad_new RAD_Handler_List(fd_setsize_));
            ++ size_;
        }
    } else if(delta < 0) {
        RAD_Handler_List* p = static_cast<RAD_Handler_List*>(hg.next);
        for(int i = 0; i < 0-delta && p != &hg; ++ i)
            if(p->size == 0) {
                RAD_Handler_List* q = p;
                p = static_cast<RAD_Handler_List*>(p->next);
                lrad_ring_remove_entry(q);
                delete q;
                -- size_;
            } else {
                p = static_cast<RAD_Handler_List*>(p->next);
            }
    }
    rad_unlock(lock);
}

size_t RAD_Reactor::max_size(void) const
{
    return maxsize_;
}

void RAD_Reactor::max_size(size_t maxs)
{
    maxsize_ = maxs;
}

size_t RAD_Reactor::fd_setsize(void) const
{
    return fd_setsize_;
}

void RAD_Reactor::fd_setsize(size_t s)
{
    fd_setsize_ = s;
}

size_t RAD_Reactor::size(void) const
{
    return size_;
}

void RAD_Reactor::dump(void) const
{
    if(!rad_lock(lock))
        return;
    DEBUG("[Reactor] %u/%u [FD_SET=%u]", size(), max_size(), fd_setsize());
    RAD_Handler_List* p = static_cast<RAD_Handler_List*>(hg.next);
    for(size_t i = 0; p != &hg;
        ++ i, p = static_cast<RAD_Handler_List*>(p->next)) {
        DEBUG("[Reactor] <%u> [thread_id=%4d] %u/%u", i,
              p->m_thr_id, p->size, p->maxsize);
        if(!rad_lock(p->lock))
            continue;
        RAD_Event_Handler* q = static_cast<RAD_Event_Handler*>(p->data.next);
        for(size_t j = 0; q != &p->data;
            ++j, q = static_cast<RAD_Event_Handler*>(q->next)) {
            rad_assert(q->owner() == p);
            DEBUG("[Reactor]   <%u> [mask=%03o] %s", j,
                  q->mask(), q->ToString().c_str());
        }
        rad_unlock(p->lock);
    }
    DEBUG("=================================================");
    rad_unlock(lock);
}

int RAD_Handler_List::MainLoop()
{
    fd_set rfs, wfs, efs;
    hd_set rhs, whs, ehs, idle;
    RAD_Event_Handler* hd;
    RAD_Event_Handler::HANDLE fd, maxfd = 0;

    rhs.reserve(FD_SETSIZE);
    whs.reserve(FD_SETSIZE);
    ehs.reserve(FD_SETSIZE);
    idle.reserve(FD_SETSIZE);

    while(!IsStopped()) {
        /* clear */
        FD_ZERO(&rfs);
        FD_ZERO(&wfs);
        FD_ZERO(&efs);

        rhs.clear();
        whs.clear();
        ehs.clear();
        idle.clear();

        /* handler list iteration */
        if(!rad_lock(lock))
            continue;
        hd = static_cast<RAD_Event_Handler*>(data.next);
        for( ; hd != &data; hd = static_cast<RAD_Event_Handler*>(hd->next)) {
            /* filter invalid mask? */
            RAD_Event_Handler::Reactor_Mask m = hd->mask();

            /* [TODO] The handler's mask could be NULL_MASK at
             * following scene:
             * For a Connector, it detects loss of connection in
             * handle_input. So it marks itself as unreadable as well
             * as unwritable (or do this after the first re-connect
             * attempt). Then only handle_idle would takes place and
             * that's where further re-connect attempts are made.
             */
            //rad_assert(m != RAD_Event_Handler::NULL_MASK);

            /* filter invalid socket? */
            fd = hd->get_handle();
            rad_assert(fd != INVALID_SOCKET);

            /* get maxfd */
            if(fd > maxfd)
                maxfd = fd;

            /* FD_SET by mask */
            if(m&RAD_Event_Handler::READ_MASK ||
               m&RAD_Event_Handler::ACCEPT_MASK) {
                FD_SET(fd, &rfs);
                rhs.push_back(hd);
            }
            if(m&RAD_Event_Handler::WRITE_MASK ||
               m&RAD_Event_Handler::CONNECT_MASK) {
                FD_SET(fd, &wfs);
                whs.push_back(hd);
            }
            if(m&RAD_Event_Handler::EXCEPT_MASK) {
                FD_SET(fd, &efs);
                ehs.push_back(hd);
            }
            idle.push_back(hd);
        }
        rad_unlock(lock);

        /* select */
        if(rhs.size() || whs.size() || ehs.size()) {
            struct timeval wait = {0, 1000};
            int cnt = select(maxfd+1, &rfs, &wfs, &efs, &wait);

            /* after select, thing may going be changed */
            if(IsStopped())
                break;

            /* check select results */
            if(cnt == SOCKET_ERROR) {
                log_socket_error(L_ERROR|L_CONS,
                                 "[Reactor] [event_loop] select()");
            } else if(cnt == 0) {
                handle_idle(idle);
            } else {
                handle_exception(ehs, efs);
                handle_output(whs, wfs);
                handle_input(rhs, rfs);
            }
        } else {
            handle_idle(idle);
            Delay(1);
        } /* end select */
    } /* end while */

    return 0;
}

void handle_idle(const hd_set& idle)
{
    struct timeval now;
    gettimeofday(&now, 0);
    hd_set::const_iterator b = idle.begin();
    hd_set::const_iterator e = idle.end();
    for(hd_set::const_iterator i = b; i != e; ++ i) {
        if((*i)->owner() == NULL)
            continue;
        if((*i)->handle_idle(now) == -1)
            (*i)->handle_close(RAD_Event_Handler::NULL_MASK);
    }
}

void handle_exception(const hd_set& ehs, const fd_set& efs)
{
    hd_set::const_iterator b = ehs.begin();
    hd_set::const_iterator e = ehs.end();
    for(hd_set::const_iterator i = b; i != e; ++ i) {
        if((*i)->owner() == NULL)
            continue;
        if(FD_ISSET((*i)->get_handle(), &efs)
           && (*i)->handle_exception() == -1) {
            (*i)->mask((*i)->mask()&~RAD_Event_Handler::EXCEPT_MASK);
            (*i)->handle_close(RAD_Event_Handler::EXCEPT_MASK);
        }
    }
}

void handle_output(const hd_set& whs, const fd_set& wfs)
{
    hd_set::const_iterator b = whs.begin();
    hd_set::const_iterator e = whs.end();
    for(hd_set::const_iterator i = b; i != e; ++ i) {
        if((*i)->owner() == NULL)
            continue;
        if(FD_ISSET((*i)->get_handle(), &wfs)
           && (*i)->handle_output() == -1) {
            (*i)->mask((*i)->mask()&~(
                           RAD_Event_Handler::CONNECT_MASK|
                           RAD_Event_Handler::WRITE_MASK));
            (*i)->handle_close(RAD_Event_Handler::WRITE_MASK);
        }
    }
}

void handle_input(const hd_set& rhs, const fd_set& rfs)
{
    hd_set::const_iterator b = rhs.begin();
    hd_set::const_iterator e = rhs.end();
    for(hd_set::const_iterator i = b; i != e; ++ i) {
        if((*i)->owner() == NULL)
            continue;
        if(FD_ISSET((*i)->get_handle(), &rfs)
           && (*i)->handle_input() == -1) {
            (*i)->mask((*i)->mask()&~(
                           RAD_Event_Handler::ACCEPT_MASK|
                           RAD_Event_Handler::READ_MASK));
            (*i)->handle_close(RAD_Event_Handler::READ_MASK);
        }
    }
}
