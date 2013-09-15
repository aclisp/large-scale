#ifndef RADFWK_LISTMAP_HPP
#define RADFWK_LISTMAP_HPP

#include <utility>
#include <list>

template <class Key, class T>
class ListMap
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

#endif // RADFWK_LISTMAP_HPP
