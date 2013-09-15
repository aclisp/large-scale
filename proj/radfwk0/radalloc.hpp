#ifndef RADFWK_ALLOC_HPP
#define RADFWK_ALLOC_HPP

/* Predefined allocation components */

class rad_dummy_alloc {};

class rad_track_alloc {
public:
    static void * allocate(size_t size, const char *file, unsigned int line);
    static void deallocate(void *p);
};

/* Template base class */

template <class Alloc>
class rad_alloc_base {
public:
    static void * operator new(size_t size,
                               const char *file,
                               unsigned int line) {
        return Alloc::allocate(size, file, line);
    }
    static void operator delete(void *p) {
        if(p) Alloc::deallocate(p);
    }
    static void operator delete(void *p, const char *, unsigned int) {
        if(p) Alloc::deallocate(p);
    }
};

/* Template Specialization */

template <>
class rad_alloc_base<rad_dummy_alloc> {};

/* Convenience defines */

#define TRACK_NEW new(__FILE__, __LINE__)

#endif /* RADFWK_ALLOC_HPP */

// Local Variables:
// mode:C++
// End:
