#ifndef _ALGORITH_HPP_INCLUDED_
#define _ALGORITH_HPP_INCLUDED_
#include "common.hpp"

namespace ft {
    // equal
    template <class InputIterator1, class InputIterator2>
    bool equal(InputIterator1 begin1, InputIterator1 end1, InputIterator2 begin2) {
        while (begin1 != end1) {
            if (!(*begin1 == *begin2))
                return false;
            begin1++, begin2++;
        }
        return true;
    }
    
    template <class InputIterator1, class InputIterator2, class BinaryPredicate>
    bool equal(InputIterator1 begin1, InputIterator1 end1, InputIterator2 begin2, BinaryPredicate pred) {
        while (begin1 != end1) {
            if (!pred(*begin1, *begin2))
                return false;
            begin1++, begin2++;
        }
        return true;
    }

    // lexicographical compare
    template<class InputIterator1, class InputIterator2>
    bool lexicographical_compare(InputIterator1 begin1, InputIterator1 end1, 
                                 InputIterator2 begin2, InputIterator2 end2)
    {
        while (begin1 != end1) {
            if (begin2 == end2 || *begin2 < *begin1)
                return false;
            if (*begin1 < *begin2)
                return true;
            begin1++, begin2++;
        }
        return begin2 != end2;
    }
    
    template<class InputIterator1, class InputIterator2, class Compare>
    bool lexicographical_compare(InputIterator1 begin1, InputIterator1 end1, 
                                 InputIterator2 begin2, InputIterator2 end2,
                                 Compare comp)
    {
        while (begin1 != end1) {
            if (begin2 == end2 || comp(*begin2, *begin1))
                return false;
            if (comp(*begin1, *begin2))
                return true;
            begin1++, begin2++;
        }
        return begin2 != end2;
    }
} // namespace ft

#endif