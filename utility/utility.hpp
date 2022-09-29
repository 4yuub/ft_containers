#ifndef _PAIR_HPP_INCLUDED_
#define _PAIR_HPP_INCLUDED_

namespace ft {
    template <class T1, class T2>
    struct pair {
        typedef T1  first_type;
        typedef T2  second_type;
        first_type  first;
        second_type second;

        pair() {}

        pair(const first_type &a, const second_type &b) {
            first = a;
            second = b;
        }

        template <class U, class V>
        pair(const pair<U, V> &obj) {
            first = obj.first;
            second = obj.second;
        }

        pair &operator=(const pair& rhs) {
            first = rhs.first;
            second = rhs.second;
            return *this;
        }
    };
    
    template <class T1,class T2>
    pair<T1,T2> make_pair(T1 x, T2 y)
    {
        return pair<T1, T2>(x, y);
    }
} // namespace ft

#endif