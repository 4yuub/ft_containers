#ifndef _ITERATOR_HPP_INCLUDED_
# define _ITERATOR_HPP_INCLUDED_
# include "common.hpp"
# include "iterator_traits.hpp"

namespace ft {
    template <class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
    struct iterator {
        typedef T         value_type;
        typedef Distance  difference_type;
        typedef Pointer   pointer;
        typedef Reference reference;
        typedef Category  iterator_category;
    };
} // namespace ft

#endif