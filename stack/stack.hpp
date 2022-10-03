#ifndef _STACK_HPP_INCLUDED_
#define _STACK_HPP_INCLUDED_
#include "vector.hpp"

namespace ft {
    template <class T, class Container = vector<T> >
    class stack {
        protected:
            Container c;
        public:
            typedef Container                          container_type;
            typedef T                                  value_type;
            typedef typename container_type::size_type size_type;

            explicit stack(const container_type& ctnr = container_type()) : c(ctnr) {}
            
            bool empty() {
                return c.empty();
            }

            size_type size() {
                return c.size();
            }

            value_type& top() {
                return c.back();
            }
            
            const value_type& top() const {
                return c.back();
            }

            void push(const value_type& val) {
                return c.push_back(val);
            }

            void pop() {
                c.pop_back();
            }
    };
    // relational operators
    template <class T, class Container>
    bool operator==(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
        return lhs.c == rhs.c;
    }

    template <class T, class Container>
    bool operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
        return lhs.c != rhs.c;
    }

    template <class T, class Container>
    bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
        return lhs.c < rhs.c;
    }

    template <class T, class Container>
    bool operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
        return lhs.c <= rhs.c;
    }

    template <class T, class Container>
    bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
        return lhs.c > rhs.c;
    }

    template <class T, class Container>
    bool operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
        return lhs.c >= rhs.c;
    }    
} // namespace ft

#endif