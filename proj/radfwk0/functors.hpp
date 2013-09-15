#ifndef RADFWK_FUNCTORS_HPP
#define RADFWK_FUNCTORS_HPP

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

#endif // RADFWK_FUNCTORS_HPP
