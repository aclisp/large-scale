#ifndef RADFWK_SHIFT_ARRAY_HPP
#define RADFWK_SHIFT_ARRAY_HPP

#include <radfwk/noncopyable.hpp>

template <class T>
class shift_array : boost::noncopyable {
public:
    shift_array(size_t max)
        {
            entry.fr = entry.to = 0;
            rad_lock_init(&lock);
            maxsize = max;
            data = new T[max];
        }
    ~shift_array()
        {
            rad_lock_destroy(&lock);
            delete[] data;
        }

    bool push_back(const T& d)
        {
            bool ok;
            rad_lock_acquire(&lock);
            if(size() == maxsize) {
                ok = false;
            } else {
                data[
                    lrad_shift_get_push_back_index(
                        &entry.fr,
                        &entry.to,
                        maxsize)
                    ] = d;
                ok = true;
            }
            rad_lock_release(&lock);
            return ok;
        }
    bool push_front(const T& d)
        {
            bool ok;
            rad_lock_acquire(&lock);
            if(size() == maxsize) {
                ok = false;
            } else {
                data[
                    lrad_shift_get_push_front_index(
                        &entry.fr,
                        &entry.to,
                        maxsize)
                    ] = d;
                ok = true;
            }
            rad_lock_release(&lock);
            return ok;
        }
    bool pop_back(T* d)
        {
            rad_lock_acquire(&lock);
            unsigned int n =
                lrad_shift_get_pop_back_index(
                    &entry.fr,
                    &entry.to,
                    maxsize);
            rad_lock_release(&lock);
            if(n == LRAD_SHIFT_TOINFI) {
                return false;
            } else {
                if(d) *d = data[n];
                return true;
            }
        }
    bool pop_front(T* d)
        {
            rad_lock_acquire(&lock);
            unsigned int n =
                lrad_shift_get_pop_front_index(
                    &entry.fr,
                    &entry.to,
                    maxsize);
            rad_lock_release(&lock);
            if(n == LRAD_SHIFT_TOINFI) {
                return false;
            } else {
                if(d) *d = data[n];
                return true;
            }
        }

    size_t max_size() const
        { return maxsize; }
    size_t size() const
        { return LRAD_SHIFT_LENGTH(entry.fr, entry.to, maxsize); }

    unsigned int incr(unsigned int& i) const
        { return LRAD_SHIFT_INCIDX(i, maxsize); }
    unsigned int decr(unsigned int& i) const
        { return LRAD_SHIFT_DECIDX(i, maxsize); }

    unsigned int from() const
        { return entry.fr; }
    unsigned int to() const
        { return entry.to; }

    const T& operator[](unsigned int i) const
        { return data[i]; }
    T& operator[](unsigned int i)
        { return data[i]; }
private:
    lrad_shift_entry entry;
    rad_lock_t lock;
    size_t maxsize;
    T* data;
};

#endif /* RADFWK_SHIFT_ARRAY_HPP */

// Local Variables:
// mode:C++
// End:
