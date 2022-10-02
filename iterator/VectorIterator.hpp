#ifndef _VECTORITERATOR_HPP_INCLUDED_
#define _VECTORITERATOR_HPP_INCLUDED_
#include "common.hpp"
#include "iterator.hpp"

namespace ft {
    template <class T>
    class VectorIterator : public iterator<random_access_iterator_tag, T> {
        private:
            T *ptr;
        public:
            typedef T                          value_type;
            typedef ptrdiff_t                  difference_type;
            typedef T*                         pointer;
            typedef T&                         reference;
            typedef random_access_iterator_tag iterator_category;

            // constructors
            VectorIterator() {} // default random value

            VectorIterator(pointer x) {
                ptr = x;
            }

            VectorIterator(VectorIterator const &obj) {
                ptr = obj.ptr;
            }

            // operators
            VectorIterator &operator=(VectorIterator const &rhs) {
                ptr = rhs.ptr;
                return *this;
            }

            bool operator==(VectorIterator const &rhs) const {
                return ptr == rhs.ptr;
            }

            bool operator!=(VectorIterator const &rhs) const {
                return ptr != rhs.ptr;
            }

            reference operator*() {
                return *ptr;
            }
            
            const reference operator*() const {
                return *ptr;
            }

            const pointer operator->() const {
                return ptr;
            }

            VectorIterator &operator++() {
                return ptr++, *this;
            } // pre increment

            VectorIterator operator++(int) {
                VectorIterator tmp(ptr);
                ++(*this);
                return tmp;
            } // post increment
            
            VectorIterator &operator--() {
                return ptr--, *this;
            } // pre decrement

            VectorIterator operator--(int) {
                VectorIterator tmp(ptr);
                --(*this);
                return tmp;
            } // post decrement

            VectorIterator operator+(difference_type n) const {
                return VectorIterator(ptr + n);
            }

            template <class U>
            friend VectorIterator<U> operator+(
                typename VectorIterator<U>::difference_type n, VectorIterator<U> const &rhs);

            VectorIterator operator-(difference_type n) {
                return VectorIterator(ptr - n);
            }
            
            difference_type operator-(VectorIterator const &rhs) {
                return ptr - rhs.ptr;
            }

            bool operator<(VectorIterator const &rhs) const {
                return ptr < rhs.ptr;
            }

            bool operator>(VectorIterator const &rhs) const {
                return ptr > rhs.ptr;
            }

            bool operator<=(VectorIterator const &rhs) const {
                return ptr <= rhs.ptr;
            }

            bool operator>=(VectorIterator const &rhs) const {
                return ptr >= rhs.ptr;
            }
            
            VectorIterator &operator+=(difference_type n) {
                ptr += n;
                return *this;
            }

            VectorIterator &operator-=(difference_type n) {
                ptr -= n;
                return *this;
            }

            reference operator[] (difference_type n) {
                return ptr[n];
            }
            
            const reference operator[] (difference_type n) const {
                return ptr[n];
            }
    };

    template <class T>
    VectorIterator<T> operator+(
            typename VectorIterator<T>::difference_type n, VectorIterator<T> const &rhs)
    {
        return VectorIterator<T>(n + rhs.ptr);
    }
} // namespace ft

#endif