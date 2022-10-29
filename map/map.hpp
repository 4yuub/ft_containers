#ifndef _MAP_HPP_INCLUDED_
#define _MAP_HPP_INCLUDED_
#include "common.hpp"
#include "functional.hpp"
#include "RedBlackTree.hpp"
#include "utility.hpp"
#include "RBT_Iterator.hpp"

namespace ft {
    template<class Key, class T, class Compare = less<Key>,
          class Allocator = std::allocator<pair<const Key, T> > >
    class map {
        private:
            class Comp {
                private:
                    Compare _cmp;
                public:
                    Comp() : _cmp(Compare()) {}
                    bool operator()(const pair<const Key, T> &lhs, const pair<const Key, T> &rhs) const {
                        return _cmp(lhs.first, rhs.first);
                    }
            };

            RedBlackTree<pair<const Key, T>, Comp> _tree;
            Compare                           _cmp;
            Allocator                         _alloc;

            typedef RedBlackTree<pair<const Key, T>, Comp>         tree_type;
        public:
            // member types
            typedef Key                                            key_type;
            typedef T                                              mapped_type;
            typedef pair<const key_type, mapped_type>              value_type;
            typedef Compare                                        key_compare;
            typedef Comp                                           value_compare;
            typedef Allocator                                      allocator_type;
            typedef typename allocator_type::reference             reference;
            typedef typename allocator_type::const_reference       const_reference;
            typedef typename allocator_type::pointer               pointer;
            typedef typename allocator_type::const_pointer         const_pointer;
            typedef RBT_Iterator<value_type, value_compare>        iterator;
            typedef RBT_Iterator<value_type, const value_compare>  const_iterator;
            typedef ft::reverse_iterator<const_iterator>               const_reverse_iterator;
            typedef ft::reverse_iterator<iterator>                     reverse_iterator;
            typedef iterator_traits<iterator>                      difference_type;
            typedef size_t                                         size_type;


            // constuctors
            explicit map(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
                : _tree(RedBlackTree<value_type, value_compare>()), _cmp(comp), _alloc(alloc) {}
            
            template <class InputIterator>
            map(InputIterator first, InputIterator last, \
                const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
                : _tree(RedBlackTree<value_type, value_compare>()), _cmp(comp), _alloc(alloc)
            {
                for (;first != last; ++first) {
                    _tree.insertNode(*first); // change this to insert
                }
            }

            map(const map& x): _tree(RedBlackTree<value_type, value_compare>()), _cmp(x._cmp), _alloc(x._alloc)  {
                for (iterator it = x.begin(); it != x.end(); ++it) {
                    _tree.insertNode(*it); // change this to insert
                }
            }

            ~map() {}

            map& operator= (const map& x) {
                _tree.deleteTree();
                for (iterator it = x.begin(); it != x.end(); ++it) {
                    _tree.insertNode(*it); // change this to insert
                }
            }

            // iterators
            iterator begin() {
                return iterator(_tree.size() == 0 ? _tree.end() : _tree.min());
            }
            
            const_iterator begin() const {
                return const_iterator(_tree.size() == 0 ? _tree.end() : _tree.min());
            }
            
            iterator end() {
                return iterator(_tree.end());
            }
            
            const_iterator end() const {
                return const_iterator(_tree.end());
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
            bool empty() const {
                return size() == 0;
            }

            size_type size() const {
                return _tree.size();
            }

            size_type max_size() const {
                return _tree.max_size();
            }

            mapped_type& operator[] (const key_type& k) {
                typename tree_type::Node *node;
                value_type to_find(k, mapped_type());
                node = _tree.insertNode(to_find);
                return node->value.second;
            }
    };
} // namespace ft

#endif