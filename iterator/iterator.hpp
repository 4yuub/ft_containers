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
} // namespace ft

#endif