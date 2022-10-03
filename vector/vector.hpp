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

            void _reAlloc(typename Alloc::size_type new_capacity) {
                T *new_arr = _alloc.allocate(new_capacity);

                for (size_t i = 0; i < _size && i < new_capacity; i++) {
                    _alloc.construct(&new_arr[i],  _arr[i]);
                    _alloc.destroy(&_arr[i]);
                }
                _alloc.deallocate(_arr, _capacity);
                _arr = new_arr;
                _capacity = new_capacity;
            }

            void _moveRangeSTE(T* start, T* end, T* dest) { // start to end
                while (start < end) {
                    if (dest < end) {
                        *dest = *start;
                    }
                    else {
                        _alloc.construct(dest, *start);
                    }
                    dest++;
                    start++;
                }
            }
            
            void _moveRangeETS(T* start, T* end, T* dest) { // end to start
                T* i_end = end;
                int64_t diff = dest - start;
                dest = (--end) + diff;
                while (end >= start) {
                    if (dest < i_end) {
                        *dest = *end;
                    }
                    else {
                        _alloc.construct(dest, *end);
                    }
                    end--;
                    dest--;
                }
            }

            void _moveRange(T* start, T* end, T* dest) {
                if (start == dest)
                    return ;
                if (start > dest) {
                    _moveRangeSTE(start, end, dest);
                    return ;
                }
                _moveRangeETS(start, end, dest);
            }

        public:
            typedef T                                                   value_type;
            typedef Alloc                                               allocator_type;
            typedef typename allocator_type::reference                  reference;
            typedef typename allocator_type::const_reference            const_reference;
            typedef VectorIterator<value_type>                          iterator;
            typedef VectorIterator<const value_type>                    const_iterator;
            typedef ft::reverse_iterator<const_iterator>                const_reverse_iterator;
            typedef ft::reverse_iterator<iterator>                      reverse_iterator;
            typedef typename iterator_traits<iterator>::difference_type difference_type;
            typedef typename allocator_type::size_type                  size_type;

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
                for (size_t i = 0; i < _size; i++)
                    _alloc.construct(&_arr[i], obj._arr[i]);
            }

            // destructor
            ~vector() {
                if (_arr == NULL)
                    return ;
                for (size_t i = 0; i < _size; i++)
                    _alloc.destroy(&_arr[i]);
                _alloc.deallocate(_arr, _capacity);
            }

            // equal operator
            vector &operator=(const vector &rhs) {
                if (this == &rhs)
                    return *this;

                if (_size >= rhs._size) {
                    for (size_t i = 0; i < rhs._size; i++)
                        _arr[i] = rhs._arr[i];
                    for (size_t i = rhs._size; i < _size; i++)
                        _alloc.destroy(&_arr[i]);
                    return *this;
                }
                for (size_t i = 0; i < _size; i++)
                    _alloc.destroy(&_arr[i]);
                _alloc.deallocate(_arr, _capacity);
                _arr = _alloc.allocate(rhs._capacity);
                _capacity = rhs._capacity;
                _size = rhs._size;
                for (size_t i = 0; i < _size; i++)
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
                return reverse_iterator(end());
            }
            
            const_reverse_iterator rbegin() const {
                return const_reverse_iterator(end());
            }
            
            reverse_iterator rend() {
                return reverse_iterator(begin());
            }
            
            const_reverse_iterator rend() const {
                return const_reverse_iterator(begin());
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
                for (size_t i = n; i < _size; i++) // if n < size
                    _alloc.destroy(&_arr[i]);
                for (size_t i = _size; i < n; i++) // if n > size
                    _alloc.construct(&_arr[i], val);
                _size = n;
            }

            size_type capacity() const {
                return _capacity;
            }

            bool empty() const {
                return _size == 0;
            }

            void reserve(size_type n) {
                if (n <= _capacity)
                    return ;
                _reAlloc(n);
            }

            // element access
            reference operator[] (size_type n) {
                return _arr[n];
            }
            
            const_reference operator[] (size_type n) const {
                return _arr[n];
            }
            
            reference at(size_type n) {
                if (n >= _size) throw std::out_of_range("vector");
                return _arr[n];
            }
            
            const_reference at(size_type n) const {
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
                return _arr[_size - 1];
            }
            
            const_reference back() const {
                return _arr[_size - 1];
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
                size_t new_size = distance(first, last);
                if (new_size > _capacity)
                    _reAlloc(new_size);
                for (size_t i = new_size; i < _size; i++) // if new_size < size
                    _alloc.destroy(&_arr[i]);
                for (size_t i = 0; i < new_size; i++)
                    _alloc.construct(&_arr[i], *(first++));
                _size = new_size;
            }

            void assign(size_type n, const value_type& val) {
                if (n > _capacity)
                    _reAlloc(n);
                for (size_t i = n; i < _size; i++) // if n < size
                    _alloc.destroy(&_arr[i]);
                for (size_type i = 0; i < n; i++)
                    _alloc.construct(&_arr[i], val);
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
                _alloc.destroy(&_arr[_size]);
                _size--;
            }

            iterator insert(iterator position, const value_type& val) {
                int64_t pos = position - begin();
                if (_size >= _capacity)
                    _reAlloc(Max(_capacity * 2, 1)); // one if current _capacity is 0
                value_type* ptr = &_arr[pos];
                _moveRange(ptr, &_arr[_size], &ptr[1]);
                *ptr = val;
                _size++;
                return position;
            }

            void insert (iterator position, size_type n, const value_type& val) {
                int64_t pos = position - begin();
                if (_size + n > _capacity)
                    _reAlloc(Max(_capacity * 2, _size + n));
                value_type* ptr = &_arr[pos];
                _moveRange(ptr, &_arr[_size], &ptr[n]);
                for (size_type i = 0; i < n; i++) {
                    if (ptr < &_arr[_size])
                        *(ptr++) = val;
                    else
                        _alloc.construct(ptr++, val);
                }
                _size += n;
            }

            template <class InputIterator>
            void insert (iterator position, InputIterator first, InputIterator last) {
                int64_t pos = position - begin();
                difference_type n = distance(first, last);
                if (_size + n > _capacity)
                    _reAlloc(Max(_capacity * 2, _size + n));
                value_type* ptr = &_arr[pos];
                _moveRange(ptr, &_arr[_size], &ptr[n]);
                for (int64_t i = 0; i < n; i++) {
                    if (ptr < &_arr[_size])
                        *(ptr++) = *first;
                    else
                        _alloc.construct(ptr++, *first);
                    first++;
                }
                _size += n;
            }

            iterator erase(iterator position) {
                int64_t pos = position - begin();
                _alloc.destroy(&_arr[pos]);
                value_type* ptr = &_arr[pos];
                _moveRange(ptr, &_arr[_size], &ptr[-1]);
                _size--;
                return iterator(&_arr[pos]);
            }
            
            iterator erase(iterator first, iterator last) {
                int64_t pos = first - begin();
                value_type* ptr = &_arr[pos];
                int64_t diff = distance(first, last);
                for (int i = 0; first < last; i++) {
                    _alloc.destroy(&_arr[pos + i]);
                    first++;
                }
                _moveRange(ptr, &ptr[_size], &ptr[-diff]);
                _size -= diff;
                return iterator(&_arr[pos]);
            }

            void swap(vector& x) {
                value_type* tmp_arr = x._arr;
                size_t tmp_size = x._size;
                size_t tmp_capacity = x._capacity;
                allocator_type  tmp_alloc = x._alloc;

                x._arr = _arr;
                x._size = _size;
                x._capacity = _capacity;
                x._alloc = _alloc;

                _arr = tmp_arr;
                _size = tmp_size;
                _capacity = tmp_capacity;
                _alloc = tmp_alloc;
            }

            void clear() {
                erase(begin(), end());
            }

            allocator_type get_allocator() const {
                return _alloc;
            }
    };

    // relational operators
    template <class T, class Alloc>
    bool operator==(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
        return lhs.size() == rhs.size && equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template <class T, class Alloc>
    bool operator!=(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
        return !(lhs == rhs);
    }

    template <class T, class Alloc>
    bool operator<(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
        return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <class T, class Alloc>
    bool operator<=(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
        return !(rhs < lhs);
    }

    template <class T, class Alloc>
    bool operator>(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
        return rhs < lhs;
    }

    template <class T, class Alloc>
    bool operator>=(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs) {
        return !(lhs < rhs);
    }

    // swap
    template <class T, class Alloc>
    void swap(vector<T,Alloc>& x, vector<T,Alloc>& y) {
        x.swap(y);
    }
} // namespace ft

#endif