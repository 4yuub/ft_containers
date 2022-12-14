#ifndef _ITERATOR_HPP_INCLUDED_
# define _ITERATOR_HPP_INCLUDED_
# include "common.hpp"
# include "iterator_traits.hpp"

namespace ft {
    // iterator base class
    template <class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
    struct iterator {
        typedef T         value_type;
        typedef Distance  difference_type;
        typedef Pointer   pointer;
        typedef Reference reference;
        typedef Category  iterator_category;
    };

    // reverse iterator
    template <class Iterator>
    class reverse_iterator
        : public iterator<typename iterator_traits<Iterator>::iterator_category,
                          typename iterator_traits<Iterator>::value_type,
                          typename iterator_traits<Iterator>::difference_type,
                          typename iterator_traits<Iterator>::pointer,
                          typename iterator_traits<Iterator>::reference>
    {
        protected:
            Iterator current;
        public:
            typedef Iterator                                              iterator_type;
            typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
            typedef typename iterator_traits<Iterator>::value_type        value_type;
            typedef typename iterator_traits<Iterator>::difference_type   difference_type;
            typedef typename iterator_traits<Iterator>::pointer           pointer;
            typedef typename iterator_traits<Iterator>::reference         reference;

            // constuctors
            reverse_iterator() {} // default random value

            reverse_iterator(iterator_type x) {
                current = x;
            }
    
            template<class U>
            reverse_iterator(reverse_iterator<U> const &obj) {
                *this = obj;
            }

            // member function base
            iterator_type base() const {
                return current;
            }
            
            // operators
            template<class U>
            reverse_iterator<U> &operator=(reverse_iterator<U> const &rhs) {
                current = rhs.current;
            }

            reference operator*() {
                iterator_type tmp(current);
                return *(--tmp);
            }

            reverse_iterator operator+(difference_type n) const {
                return reverse_iterator(current - n);
            }
            
            reverse_iterator &operator++() {
                return current--, *this;
            } // pre increment

            reverse_iterator operator++(int) {
                reverse_iterator tmp(current);
                ++(*this);
                return tmp;
            } // post increment
            
            reverse_iterator &operator+=(difference_type n) {
                current -= n;
                return *this;
            }

            reverse_iterator operator-(difference_type n) const {
                return reverse_iterator(current + n);
            }

            reverse_iterator &operator--() {
                return current++, *this;
            } // pre decrement

            reverse_iterator operator--(int) {
                reverse_iterator tmp(current);
                --(*this);
                return tmp;
            } // post decrement

            reverse_iterator &operator-=(difference_type n) {
                current += n;
                return *this;
            }

            pointer operator->() const {
                return &(operator*());
            }

            reference operator[] (difference_type n) const {
                return base()[-n-1];
            }
    };
    // more operators for reverse iterator
    template <class Iterator>
    bool operator==(reverse_iterator<Iterator> const &lhs, reverse_iterator<Iterator> const &rhs) {
        return lhs.base() == rhs.base();
    }

    template <class Iterator>
    bool operator!=(reverse_iterator<Iterator> const &lhs, reverse_iterator<Iterator> const &rhs) {
        return lhs.base() != rhs.base();
    }

    template <class Iterator>
    bool operator<(reverse_iterator<Iterator> const &lhs, reverse_iterator<Iterator> const &rhs) {
        return lhs.base() > rhs.base();
    }

    template <class Iterator>
    bool operator<=(reverse_iterator<Iterator> const &lhs, reverse_iterator<Iterator> const &rhs) {
        return lhs.base() >= rhs.base();
    }

    template <class Iterator>
    bool operator>(reverse_iterator<Iterator> const &lhs, reverse_iterator<Iterator> const &rhs) {
        return lhs.base() < rhs.base();
    }

    template <class Iterator>
    bool operator>=(reverse_iterator<Iterator> const &lhs, reverse_iterator<Iterator> const &rhs) {
        return lhs.base() <= rhs.base();
    }

    template <class Iterator>
    reverse_iterator<Iterator> operator+(typename reverse_iterator<Iterator>::difference_type n, reverse_iterator<Iterator> const &rhs) {
        return reverse_iterator<Iterator>(rhs.base() - n);
    }

    template <class Iterator>
    typename reverse_iterator<Iterator>::difference_type operator-(reverse_iterator<Iterator> const &lhs, reverse_iterator<Iterator> const &rhs) {
        return rhs.base() - lhs.base();
    }

    // distance
    namespace detail {
        template <class It>
        typename ft::iterator_traits<It>::difference_type
        do_distance(It first, It last, input_iterator_tag) {
           typename ft::iterator_traits<It>::difference_type dist = 0;
           while (first != last) {
               first++;
               dist++;
           }
           return dist;
        }
        
        template <class It>
        typename ft::iterator_traits<It>::difference_type
        do_distance(It first, It last, random_access_iterator_tag) {
           return last - first;
        }

    } // namespace detail
    
    template <class It>
    typename iterator_traits<It>::difference_type
    distance(It first, It last) {
        return detail::do_distance(first, last, \
                                typename iterator_traits<It>::iterator_category());
    }
} // namespace ft

#endif