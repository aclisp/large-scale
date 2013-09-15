// -*- mode: c++ -*-

/* Author:   Huanghao@cellcom
 * Date:     2005/11/24 14:21:24
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RADFWK_UTIL_H__
#define __RADFWK_UTIL_H__

#include <radfwk/RAD_Object.h>

template <class Key, class T>
class list_map
{
public:
    typedef std::pair<const Key, T> value_type;
private:
    typedef std::list<value_type> rep_type;
public:
    typedef typename rep_type::iterator iterator;
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::size_type size_type;

    iterator begin() { return t.begin(); }
    const_iterator begin() const { return t.begin(); }
    iterator end() { return t.end(); }
    const_iterator end() const {return t.end(); }
    size_type size() const { return t.size(); }
    void clear() { t.clear(); }

    iterator find(const Key& keyval) {
        iterator b=begin(), e=end();
        for(iterator i=b; i!=e; ++i) if(i->first == keyval) return i;
        return e;
    }
    const_iterator find(const Key& keyval) const {
        const_iterator b=begin(), e=end();
        for(const_iterator i=b; i!=e; ++i) if(i->first == keyval) return i;
        return e;
    }

    void insert(const value_type& val) { t.push_back(val); }
    iterator erase(iterator where) { return t.erase(where); }
    void erase(const Key& keyval) {
        iterator b=begin(), e=end();
        for(iterator i=b; i!=e; ) i->first == keyval ? i = erase(i) : ++i;
    }

private:
    rep_type t;
};

// STL algorithm functors

template <class T>
struct delete_pointer {

    // For STL Sequence Containers which contains pointers of type T
    // created by operator new, use
    //     std::for_each(SeqContainer.begin(), SeqContainer.end(),
    //                   delete_pointer<T>());
    // to delete all of them just before clear/dtor Container

    void operator()(T* pointer) const {
        delete pointer;
    }

    // For STL Associative Containers which contains pointers of type T
    // created by operator new, use
    //     std::for_each(AssocContainer.begin(), AssocContainer.end(),
    //                   delete_pointer<AssocContainer::value_type>());
    // to delete all of them just before clear/dtor Container

    void operator()(T& pair) const {
        delete pair.second;
        // pair.second = NULL;
    }

};

class RAD_Container : public RAD_Object {
public:
    explicit RAD_Container(const string& name) : RAD_Object(name) {}
    virtual ~RAD_Container() {}
    virtual const char* name() const { return "Unnamed"; }
    virtual size_t size() const = 0;
    virtual size_t max_size() const = 0;
    virtual bool acquire_lock() const = 0;
    virtual void release_lock() const = 0;
};

template <class T>
class RAD_ShiftArray : public RAD_Container {
public:
    explicit RAD_ShiftArray(size_t max)
        : RAD_Container("RAD_ShiftArray")
        {
            entry.fr = entry.to = 0;
            strname = "RAD_ShiftArray";
            maxsize = max;
            data = new T[max];
        }
    virtual ~RAD_ShiftArray()
        {
            delete[] data;
        }

    bool push_back(const T& d, bool overwrite = true)
        {
            bool ok;
            if(!rad_lock(lock))
                return false;
            if(size() == maxsize) {
                if(overwrite)
                data[
                    lrad_shift_get_push_back_index(
                        &entry.fr,
                        &entry.to,
                        maxsize)
                    ] = d;
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
            rad_unlock(lock);
            return ok;
        }
    bool push_front(const T& d, bool overwrite = true)
        {
            bool ok;
            if(!rad_lock(lock))
                return false;
            if(size() == maxsize) {
                if(overwrite)
                data[
                    lrad_shift_get_push_front_index(
                        &entry.fr,
                        &entry.to,
                        maxsize)
                    ] = d;
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
            rad_unlock(lock);
            return ok;
        }
    bool pop_back(T* d)
        {
            if(!rad_lock(lock))
                return false;
            unsigned int n =
                lrad_shift_get_pop_back_index(
                    &entry.fr,
                    &entry.to,
                    maxsize);
            rad_unlock(lock);
            if(n == LRAD_SHIFT_TOINFI) {
                return false;
            } else {
                if(d) *d = data[n];
                return true;
            }
        }
    bool pop_front(T* d)
        {
            if(!rad_lock(lock))
                return false;
            unsigned int n =
                lrad_shift_get_pop_front_index(
                    &entry.fr,
                    &entry.to,
                    maxsize);
            rad_unlock(lock);
            if(n == LRAD_SHIFT_TOINFI) {
                return false;
            } else {
                if(d) *d = data[n];
                return true;
            }
        }
    void clear()
        {
            if(!rad_lock(lock))
                return;
            entry.fr = entry.to = 0;
            rad_unlock(lock);
        }

    void name(const char* s)
        { strname = s; }
    const char* name() const
        { return strname.c_str(); }
    size_t size() const
        { return LRAD_SHIFT_LENGTH(entry.fr, entry.to, maxsize); }
    size_t max_size() const
        { return maxsize; }

    bool acquire_lock() const
        { return rad_lock(lock); }
    void release_lock() const
        { rad_unlock(lock); }

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
    RAD_ShiftArray(const RAD_ShiftArray& );
    RAD_ShiftArray& operator=(const RAD_ShiftArray& other);

    lrad_shift_entry entry;
    mutable RAD_Mutex lock;
    string strname;
    size_t maxsize;
    T* data;
};

template <class T>
class RAD_LinkArray : public RAD_Container {
public:
    struct Item : lrad_link_entry {
        Item() { next = prev = 0; }
        T data;
    };

    explicit RAD_LinkArray(size_t max)
        : RAD_Container("RAD_LinkArray")
        {
            strname = "RAD_LinkArray";
            maxsize = max;
            cursize = 0;
            array = new Item[max+2];
            array[1].next = array[1].prev = 1;
        }
    virtual ~RAD_LinkArray()
        {
            delete[] array;
        }

    bool push_back(const T& d)
        {
            bool ok = false;
            if(!rad_lock(lock))
                return false;
            Item* item = alloc_item();
            if(item) {
                item->data = d;
                append_item(item);
                ok = true;
            }
            rad_unlock(lock);
            return ok;
        }
    bool push_front(const T& d)
        {
            bool ok = false;
            if(!rad_lock(lock))
                return false;
            Item* item = alloc_item();
            if(item) {
                item->data = d;
                prepend_item(item);
                ok = true;
            }
            rad_unlock(lock);
            return ok;
        }
    bool pop_back(T* d)
        {
            bool ok = false;
            if(!rad_lock(lock))
                return false;
            if(cursize > 0) {
                Item* item = static_cast<Item*>(
                    lrad_link_remove_tail((char*)array, sizeof(Item)));
                -- cursize;
                if(d) *d = item->data;
                ok = true;
            }
            rad_unlock(lock);
            return ok;
        }
    bool pop_front(T* d)
        {
            bool ok = false;
            if(!rad_lock(lock))
                return false;
            if(cursize > 0) {
                Item* item = static_cast<Item*>(
                    lrad_link_remove_head((char*)array, sizeof(Item)));
                -- cursize;
                if(d) *d = item->data;
                ok = true;
            }
            rad_unlock(lock);
            return ok;
        }
    void clear()
        {
            if(!rad_lock(lock))
                return;
            while(cursize)
                remove_item( first_item() );
            rad_unlock(lock);
        }

    void append_item(const Item* item)
        {
            if(item == NULL)
                return;
            lrad_link_insert_tail(item-array, (char*)array, sizeof(Item));
            ++ cursize;
        }
    void prepend_item(const Item* item)
        {
            if(item == NULL)
                return;
            lrad_link_insert_head(item-array, (char*)array, sizeof(Item));
            ++ cursize;
        }
    const Item* remove_item(const Item* item)
        {
            if(item == NULL)
                return NULL;
            if(cursize > 0) {
                const Item* next = &array[item->next];
                lrad_link_remove_entry(const_cast<Item*>(item),
                                       (char*)array, sizeof(Item));
                -- cursize;
                return next;
            }
            return NULL;
        }
    Item* alloc_item()
        {
            unsigned short dim = maxsize+2;
            for(unsigned short i = 2; i < dim; ++i) {
                if(array[i].next == 0)
                    return &array[i];
            }
            return NULL;
        }

    void name(const char* s)
        { strname = s; }
    const char* name() const
        { return strname.c_str(); }
    size_t size() const
        { return cursize; }
    size_t max_size() const
        { return maxsize; }

    bool acquire_lock() const
        { return rad_lock(lock); }
    void release_lock() const
        { rad_unlock(lock); }

    const Item* first_item() const
        { return &array[array[1].next]; }
    const Item* last_item() const
        { return &array[array[1].prev]; }
    const Item* next_item(const Item* item) const
        { return &array[item->next]; }
    const Item* prev_item(const Item* item) const
        { return &array[item->prev]; }
    const Item* knob_addr() const
        { return &array[1]; }
    const Item* array_addr() const
        { return array; }
    unsigned int item_index(const Item* item) const
        { return item-array; }
    const Item* item_addr(unsigned int index) const
        { return &array[index]; }

private:
    RAD_LinkArray(const RAD_LinkArray& );
    RAD_LinkArray& operator=(const RAD_LinkArray& other);

    mutable RAD_Mutex lock;
    string strname;
    size_t maxsize;
    size_t cursize;
    Item* array;
};

class RADFWK_API RAD_Util
{
private:
    RAD_Util();
    ~RAD_Util();
    RAD_Util(const RAD_Util& );
    const RAD_Util& operator=(const RAD_Util& );
public:
    static string GetLastErrDesc(const string& func);
};

#endif // __RADFWK_UTIL_H__
