#ifndef _VECTOR_HPP_INCLUDED_
#define _VECTOR_HPP_INCLUDED_
#define Max(x, y) ((x > y) ? (x) : (y))
#include "common.hpp"
#include "iterator.hpp"
#include "VectorIterator.hpp"

namespace ft {
    template <class T, class Alloc = std::allocator<T> >
    class vector {
        private:
            T*     _arr;
            size_t _size;
            size_t _capacity;
            Alloc  _alloc;

            void _reAlloc(size_type new_capacity) {
                T *new_arr = _alloc.allocate(new_capacity);

                for (int i = 0; i < _size && i < new_capacity; i++) {
                    _alloc.construct(&new_arr[i],  _arr[i]);
                    _alloc.destroy(&_arr[i]);
                }
                _alloc.deallocate(_arr, _capacity);
                _arr = new_arr;
                _capacity = new_capacity;
            }

        public:
            typedef T                                          value_type;
            typedef Alloc                                      allocator_type;
            typedef typename allocator_type::reference         reference;
            typedef typename allocator_type::const_reference   const_reference;
            typedef VectorIterator<value_type>                 iterator;
            typedef VectorIterator<const value_type>           const_iterator;
            typedef reverse_iterator<iterator>                 reverse_iterator;
            typedef reverse_iterator<const_iterator>           const_reverse_iterator;
            typedef iterator_traits<iterator>::difference_type difference_type;
            typedef typename allocator_type::size_type         size_type;

            // constructors
            vector(const allocator_type& alloc = allocator_type()) {
                _alloc = alloc;
                _arr = NULL;
                _size = 0;
                _capacity = 0;
            }

            vector(size_type n, const value_type &val = value_type(), \
                        const allocator_type &alloc = allocator_type())
            {
                _alloc = alloc;
                _arr = _alloc.allocate(n);
                _size = n;
                _capacity = n;
                for (size_t i = 0; i < n; i++)
                    _alloc.construct(&_arr[i], val);
            }

            template <class InputIterator>
            vector(InputIterator begin, InputIterator end, \
                        const allocator_type& alloc = allocator_type())
            {
                difference_type n = distance(begin, end);
                _alloc = alloc;
                _arr = _alloc.allocate(n);
                _size = n;
                _capacity = n;
                for (size_t i = 0; begin != end; begin++, i++)
                    _alloc.construct(&_arr[i], *begin);
            }

            vector(const vector& obj) {
                _alloc = obj._alloc;
                _capacity = obj._capacity;
                _arr = _alloc.allocate(_capacity);
                _size = obj._size;
                for (int i = 0; i < _size; i++)
                    _alloc.construct(&_arr[i], obj._arr[i]);
            }

            // destructor
            ~vector() {
                if (_arr == NULL)
                    return ;
                for (int i = 0; i < _size; i++)
                    _alloc.destroy(&_arr[i]);
                _alloc.deallocate(_arr, _capacity);
            }

            // equal operator
            vector &operator=(const vector &rhs) {
                if (this == &rhs)
                    return *this;

                if (_size >= rhs._size) {
                    for (int i = 0; i < rhs._size; i++)
                        _arr[i] = rhs._arr[i];
                    for (int i = rhs._size; i < _size; i++)
                        _alloc.destroy(&arr[i]);
                    return *this;
                }
                for (int i = 0; i < _size; i++)
                    _alloc.destroy(&_arr[i]);
                _alloc.deallocate(_arr, _capacity);
                _arr = _alloc.allocate(rhs._capacity);
                _capacity = rhs._capacity;
                _size = rhs._size;
                for (int i = 0; i < _size; i++)
                    _alloc.construct(&_arr[i], rhs._arr[i]);
                return *this;
            }

            // member functions

            // iterators
            iterator begin() {
                return iterator(_arr);
            }
            
            const_iterator begin() const {
                return const_iterator(_arr);
            }

            iterator end() {
                return iterator(_arr == NULL ? _arr : _arr + _size);
            }

            const_iterator end() const {
                return const_iterator(_arr == NULL ? _arr : _arr + _size);
            }

            reverse_iterator rbegin() {
                return reverse_iterator(begin());
            }
            
            const_reverse_iterator rbegin() const {
                return const_reverse_iterator(begin());
            }
            
            reverse_iterator rend() {
                return reverse_iterator(end());
            }
            
            const_reverse_iterator rend() const {
                return const_reverse_iterator(end());
            }

            // capacity
            size_type size() const {
                return _size;
            }

            size_type max_size() const {
                return _alloc.max_size();
            }

            void resize(size_type n, value_type val = value_type()) {
                if (n > _capacity) {
                    _reAlloc(Max(_capacity * 2, n));
                }
                for (int i = n; i < _size; i++) // if n < size
                    _alloc.destroy(&arr[i]);
                for (int i = _size; i < n; i++) // if n > size
                    _alloc.construct(&arr[i], val);
                _size = n;
            }

            size_type capacity() const {
                return _capacity;
            }

            bool empty() const {
                return _size == 0;
            }

            void resever(size_type n) {
                if (n <= _capacity)
                    return ;
                reAlloc(n);
            }

            // element access
            reference operator[] (size_type n) {
                return _arr[n];
            }
            
            const_reference operator[] (size_type n) const {
                return _arr[n];
            }
            
            reference operator at (size_type n) {
                if (n >= _size) throw std::out_of_range("vector");
                return _arr[n];
            }
            
            const_reference operator at (size_type n) const {
                if (n >= _size) throw std::out_of_range("vector");
                return _arr[n];
            }

            reference front() {
                return _arr[0];
            }
            
            const_reference front() const {
                return _arr[0];
            }

            reference back() {
                return _arr[size - 1];
            }
            
            const_reference back() const {
                return _arr[size - 1];
            }

            value_type* data() {
                return _arr;
            }
            
            const value_type* data() const {
                return _arr;
            }

            // modifiers
            template <class InputIterator> 
            void assign(InputIterator first, InputIterator last) {
                difference_type new_size = distance(first, last);
                if (new_size > _capacity)
                    _reAlloc(new_size);
                for (int i = new_size; i < _size; i++) // if new_size < size
                    _alloc.destroy(&arr[i]);
                for (int i = 0; i < new_size; i++)
                    _alloc.construct(&arr[i], *(first++));
                _size = new_size;
            }

            void assign(size_type n, const value_type& val) {
                if (n > _capacity)
                    _reAlloc(n);
                for (int i = n; i < _size; i++) // if n < size
                    _alloc.destroy(&arr[i]);
                for (int i = 0; i < n; i++)
                    _alloc.construct(&arr[i], val);
                _size = n;
            }

            void push_back(const value_type& val) {
                if (_size >= _capacity)
                    _reAlloc(Max(_capacity * 2, 1)); // one if current _capacity is 0
                _alloc.construct(&_arr[_size], val);
                _size++;
            }

            void pop_back() {
                if (empty())
                    return ;
                _alloc.destory(&_arr[_size]);
                _size--;
            }
    };
} // namespace ft

#endif