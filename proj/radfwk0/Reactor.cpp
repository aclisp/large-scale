#include "radutil.h"
#include "radsock.h"

#include "Event_Handler.h"
#include "Reactor.h"

#include <vector>

#if defined(RAD_OS_WIN32)
  #include <winsock2.h>
#endif

typedef std::vector<Event_Handler*> hd_set;

static THREAD_CALLBACK event_loop(void* arg);
static void handle_idle(const hd_set& idle);
static void handle_exception(const hd_set& ehs, const fd_set& efs);
static void handle_output(const hd_set& whs, const fd_set& wfs);
static void handle_input(const hd_set& rhs, const fd_set& rfs);

struct Handler_List : lrad_ring_entry
                    , rad_alloc_base<rad_track_alloc>
{
    rad_lock_t lock;
    size_t maxsize;
    size_t size;
    lrad_ring_entry data;
    pthread_t thread;
    bool shutdown;

    Handler_List(size_t maxs)
        {
            rad_lock_init(&lock);
            maxsize = maxs;
            if(maxsize > FD_SETSIZE) {
                radlog(L_WARN|L_CONS, "[Handler_List ctor] "
                       "maxsize(=%u) exceeds FD_SETSIZE(=%u)",
                       maxsize, FD_SETSIZE);
                maxsize = FD_SETSIZE;
            }
            size = 0;
            lrad_ring_init_head(&data);
            shutdown = false;
            if(pthread_create(&thread, NULL, event_loop, this) != 0) {
                radlog(L_ERROR|L_CONS, "[Handler_List ctor] "
                       "can not create thread for event looping");
            }
        }

    ~Handler_List()
        {
            shutdown = true;
            pthread_join(thread, NULL);
            close();
            rad_lock_destroy(&lock);
        }

    void close()
        {
            while(size)
                remove(static_cast<Event_Handler*>(data.next),
                       Event_Handler::ALL_EVENTS_MASK|Event_Handler::REMOVING);
        }

    /* This operation should be very fast. So if duplicate handler is
     * added, the event loop will check it out. [TODO]
     */
    int add(Event_Handler* handler, Event_Handler::Reactor_Mask mask)
        {
            int rc = -1;
            rad_lock_acquire(&lock);
            rad_assert(mask != Event_Handler::NULL_MASK);
            rad_assert((mask&Event_Handler::DONT_CALL) == 0);
            if(handler->owner() == NULL) {
                if(size < maxsize) {
                    rad_assert(handler->mask() == Event_Handler::NULL_MASK);
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
            rad_lock_release(&lock);
            return rc;
        }

    /* This operation should also be very fast. That's why lrad_ring
     * is used. !!!ATTENTION!!! a handler should NOT be removed for a
     * second time.
     */
    int remove(Event_Handler* handler, Event_Handler::Reactor_Mask mask)
        {
            int rc = -1;
            rad_lock_acquire(&lock);
            rad_assert(handler->owner() == this);
            Event_Handler::Reactor_Mask m =
                handler->mask()&~(mask&~Event_Handler::DONT_CALL);
            if(m == Event_Handler::NULL_MASK) {
                rad_assert(size != 0);
                lrad_ring_remove_entry(handler);
                -- size;
                handler->owner(NULL);
                handler->mask(Event_Handler::NULL_MASK);
                rc = 0;
            } else {
                handler->mask(m);
                rc = 1;
            }
            if((mask&Event_Handler::DONT_CALL) == 0)
                handler->handle_close(mask);
            rad_lock_release(&lock);
            return rc;
        }
};

class Reactor::Detail : private boost::noncopyable
{
public:
    rad_lock_t lock;
    size_t fd_setsize;
    size_t maxsize;
    size_t size;
    lrad_ring_entry hg;

    Detail()
        {
            rad_lock_init(&lock);
            fd_setsize = FD_SETSIZE;
            maxsize = 1;
            size = 1;
            lrad_ring_init_head(&hg);
            lrad_ring_insert_head(&hg, TRACK_NEW Handler_List(fd_setsize));
        }

    ~Detail()
        {
            Handler_List* hl;
            while((hl = static_cast<Handler_List*>(
                       lrad_ring_remove_head(&hg))) != &hg)
                delete hl;
            rad_lock_destroy(&lock);
        }

    int register_handler(Event_Handler* handler,
                         Event_Handler::Reactor_Mask mask)
        {
            int rc = -1;
            size_t i;
            Handler_List* hl;
            rad_lock_acquire(&lock);
            for(i = 0; i < size; ++ i) {
                hl = static_cast<Handler_List*>(lrad_ring_remove_head(&hg));
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
            rad_lock_release(&lock);
            return rc;
        }

    int remove_handler(Event_Handler* handler,
                       Event_Handler::Reactor_Mask mask)
        {
            int rc = -1;
            rad_lock_acquire(&lock);
            Handler_List* hl = handler->owner();
            rc = hl->remove(handler, mask);
            if(rc == 0) {
                lrad_ring_remove_entry(hl);
                lrad_ring_insert_head(&hg, hl);
            }
            rad_lock_release(&lock);
            return rc;
        }

    void resize(size_t newsize)
        {
            rad_lock_acquire(&lock);
            if(newsize > maxsize)
                maxsize = newsize;
            int delta = newsize - size;
            if(delta > 0) {
                for(int i = 0; i < delta; ++ i) {
                    lrad_ring_insert_head(&hg,
                                          TRACK_NEW Handler_List(fd_setsize));
                    ++ size;
                }
            } else if(delta < 0) {
                Handler_List* p = static_cast<Handler_List*>(hg.next);
                for(int i = 0; i < 0-delta && p != &hg; ++ i)
                    if(p->size == 0) {
                        Handler_List* q = p;
                        p = static_cast<Handler_List*>(p->next);
                        lrad_ring_remove_entry(q);
                        delete q;
                        -- size;
                    } else {
                        p = static_cast<Handler_List*>(p->next);
                    }
            }
            rad_lock_release(&lock);
        }
};

Reactor::Reactor()
{
    pImpl = new Detail;
    rad_assert(pImpl != NULL);
}

Reactor::~Reactor()
{
    delete pImpl;
}

int Reactor::register_handler(Event_Handler* handler,
                              Event_Handler::Reactor_Mask mask)
{
    return pImpl->register_handler(handler, mask);
}

int Reactor::remove_handler(Event_Handler* handler,
                            Event_Handler::Reactor_Mask mask)
{
    return pImpl->remove_handler(handler, mask);
}

size_t Reactor::max_size(void) const
{
    return pImpl->maxsize;
}

void Reactor::max_size(size_t maxs)
{
    pImpl->maxsize = maxs;
}

size_t Reactor::fd_setsize(void) const
{
    return pImpl->fd_setsize;
}

void Reactor::fd_setsize(size_t s)
{
    pImpl->fd_setsize = s;
}

size_t Reactor::size(void) const
{
    return pImpl->size;
}

void Reactor::resize(size_t newsize)
{
    pImpl->resize(newsize);
}

void Reactor::dump(void) const
{
    rad_lock_acquire(&pImpl->lock);
    DEBUG("[Reactor] %u/%u [FD_SET=%u]", size(), max_size(), fd_setsize());
    Handler_List* p = static_cast<Handler_List*>(pImpl->hg.next);
    for(size_t i = 0; p != &pImpl->hg;
        ++ i, p = static_cast<Handler_List*>(p->next)) {
        DEBUG("[Reactor] <%u> [thread_id=%4d] %u/%u", i,
              THREAD_ID(&p->thread), p->size, p->maxsize);
        rad_lock_acquire(&p->lock);
        Event_Handler* q = static_cast<Event_Handler*>(p->data.next);
        for(size_t j = 0; q != &p->data;
            ++j, q = static_cast<Event_Handler*>(q->next)) {
            rad_assert(q->owner() == p);
            DEBUG("[Reactor]   <%u> [mask=%03o] %s", j,
                  q->mask(), q->to_string());
        }
        rad_lock_release(&p->lock);
    }
    DEBUG("=================================================");
    rad_lock_release(&pImpl->lock);
}


THREAD_CALLBACK event_loop(void* arg)
{
    fd_set rfs, wfs, efs;
    hd_set rhs, whs, ehs, idle;
    Handler_List* hl = (Handler_List*)arg;
    Event_Handler* hd;
    Event_Handler::HANDLE fd, maxfd = 0;

    rhs.reserve(FD_SETSIZE);
    whs.reserve(FD_SETSIZE);
    ehs.reserve(FD_SETSIZE);
    idle.reserve(FD_SETSIZE);

    while(!hl->shutdown) {
        /* clear */
        FD_ZERO(&rfs);
        FD_ZERO(&wfs);
        FD_ZERO(&efs);

        rhs.clear();
        whs.clear();
        ehs.clear();
        idle.clear();

        /* handler list iteration */
        rad_lock_acquire(&hl->lock);
        hd = static_cast<Event_Handler*>(hl->data.next);
        for( ; hd != &hl->data; hd = static_cast<Event_Handler*>(hd->next)) {
            /* filter invalid mask? */
            Event_Handler::Reactor_Mask m = hd->mask();

            /* [TODO] The handler's mask could be NULL_MASK at
             * following scene:
             * For a Connector, it detects loss of connection in
             * handle_input. So it marks itself as unreadable as well
             * as unwritable (or do this after the first re-connect
             * attempt). Then only handle_idle would takes place and
             * that's where further re-connect attempts are made.
             */
            //rad_assert(m != Event_Handler::NULL_MASK);

            /* filter invalid socket? */
            fd = hd->get_handle();
            rad_assert(fd != INVALID_SOCKET);

            /* get maxfd */
            if(fd > maxfd)
                maxfd = fd;

            /* FD_SET by mask */
            if(m&Event_Handler::READ_MASK || m&Event_Handler::ACCEPT_MASK) {
                FD_SET(fd, &rfs);
                rhs.push_back(hd);
            }
            if(m&Event_Handler::WRITE_MASK || m&Event_Handler::CONNECT_MASK) {
                FD_SET(fd, &wfs);
                whs.push_back(hd);
            }
            if(m&Event_Handler::EXCEPT_MASK) {
                FD_SET(fd, &efs);
                ehs.push_back(hd);
            }
            idle.push_back(hd);
        }
        rad_lock_release(&hl->lock);

        /* select */
        if(rfs.fd_count || wfs.fd_count || efs.fd_count) {
            struct timeval wait = {1, 0};
            int cnt = select(maxfd+1, &rfs, &wfs, &efs, &wait);

            /* after select, thing may going be changed */
            if(hl->shutdown)
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
            Sleep(200);
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
            (*i)->handle_close(Event_Handler::NULL_MASK);
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
            (*i)->mask((*i)->mask()&~Event_Handler::EXCEPT_MASK);
            (*i)->handle_close(Event_Handler::EXCEPT_MASK);
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
                           Event_Handler::CONNECT_MASK|
                           Event_Handler::WRITE_MASK));
            (*i)->handle_close(Event_Handler::WRITE_MASK);
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
                           Event_Handler::ACCEPT_MASK|
                           Event_Handler::READ_MASK));
            (*i)->handle_close(Event_Handler::READ_MASK);
        }
    }
}
