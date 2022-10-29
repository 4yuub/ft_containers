#ifndef _FUNCTIONAL_HPP_INCLUDED_
#define _FUNCTIONAL_HPP_INCLUDED_

namespace ft {
    template <class T>
    struct less {
        bool operator()(const T &lhs, const T &rhs) const {
            return lhs < rhs;
        }
    };
} // namespace ft

#endif