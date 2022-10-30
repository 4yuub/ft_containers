#ifndef _SET_HPP_INCLUDED_
#define _SET_HPP_INCLUDED_
#include "common.hpp"
#include "functional.hpp"
#include "RedBlackTree.hpp"
#include "utility.hpp"
#include "RBT_Iterator.hpp"

namespace ft {
    template <class T, class Compare = less<T>, class Allocator = std::allocator<T> >
    class set {
        private:
            RedBlackTree<T, Compare> _tree;
            Compare                  _cmp;
            Allocator                _alloc;

            typedef RedBlackTree<T, Compare>                       tree_type;
        public:
            typedef T                                              key_type;
            typedef T                                              value_type;
            typedef Compare                                        key_compare;
            typedef Compare                                        value_compare;
            typedef Allocator                                      allocator_type;
            typedef typename allocator_type::reference             reference;
            typedef typename allocator_type::const_reference       const_reference;
            typedef typename allocator_type::pointer               pointer;
            typedef typename allocator_type::const_pointer         const_pointer;
            typedef RBT_Iterator<value_type, value_compare>        iterator;
            typedef const RBT_Iterator<value_type, value_compare>  const_iterator;
            typedef ft::reverse_iterator<const_iterator>           const_reverse_iterator;
            typedef ft::reverse_iterator<iterator>                 reverse_iterator;
            typedef iterator_traits<iterator>                      difference_type;
            typedef size_t                                         size_type;

            // constuctors
            explicit set(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
                : _tree(RedBlackTree<value_type, value_compare>()), _cmp(comp), _alloc(alloc) {}
            
            template <class InputIterator>
            set(InputIterator first, InputIterator last, \
                const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
                : _tree(RedBlackTree<value_type, value_compare>()), _cmp(comp), _alloc(alloc)
            {
                insert(first, last);
            }

            set(const set& x): _tree(RedBlackTree<value_type, value_compare>()), _cmp(x._cmp), _alloc(x._alloc)  {
                for (iterator it = x.begin(); it != x.end(); ++it) {
                    insert(*it);
                }
            }

            ~set() {}

            set& operator= (const set& x) {
                _tree.deleteTree();
                for (iterator it = x.begin(); it != x.end(); ++it) {
                    insert(*it);
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


            // modifiers
            pair<iterator, bool> insert(const value_type& val) {
                typename tree_type::Node *node;
                pair<iterator, bool>      ret;
                node = _tree.insertNode(val, &(ret.second));
                ret.first = iterator(node);
                return ret;
            }

            iterator insert(iterator position, const value_type& val) {
                (void) position; // my set don't need hint it's so smart
                return insert(val).first;
            }
            
            template <class InputIterator>
            void insert(InputIterator first, InputIterator last) {
                for (;first != last; ++first) {
                    insert(*first);
                }
            }

            void erase(iterator position) {
                _tree.deleteNode(position._ptr->value, position._ptr);
            }

            size_type erase(const key_type& k) {
                typename tree_type::Node *node;
                return _tree.deleteNode(k);
            }

            void erase(iterator first, iterator last) {
                if (first == last)
                    return ;
                iterator tmp(first);
                ++tmp;
                erase(first);
                erase(tmp, last);
            }

            void swap (set& x) {
                tree_type   tmp_tree(x._tree);
                Allocator   tmp_alloc(x._alloc);

                x._tree = _tree;
                x._alloc = _alloc;

                _tree = tmp_tree;
                _alloc = tmp_alloc;
            }

            void clear() {
                _tree.deleteTree();
            }

            // observers
            key_compare key_comp() const {
                return key_compare();
            }

            value_compare value_comp() const {
                return value_compare();
            }

            // operations
            iterator find(const value_type& k) {
                typename tree_type::Node *node;
                node = _tree.findNode(k);
                if (!node || node->isNull) {
                    return iterator(_tree.end());
                }
                return iterator(node);
            }

            const_iterator find(const value_type& k) const {
                typename tree_type::Node *node;
                node = _tree.findNode(k);
                if (!node || node->isNull) {
                    return const_iterator(_tree.end());
                }
                return const_iterator(node);

            }

            size_type count(const value_type& k) const {
                return find(k) != end();
            }

            iterator lower_bound(const value_type& k) const {
                typename tree_type::Node *node;
                node = _tree.findNode(k);
                if (!node)
                    return iterator(_tree.end());
                if (node->isNull) {
                    iterator it(node);
                    return ++it;
                }
                return iterator(node);
            }

            iterator upper_bound(const value_type& k) const {
                typename tree_type::Node *node;
                node = _tree.findNode(k);
                if (!node)
                    return iterator(_tree.end());
                iterator it(node);
                return ++it;
            }

            pair<iterator,iterator> equal_range(const value_type& k) const {
                return pair<iterator,iterator>(lower_bound(k), upper_bound(k));
            }

            // allocator
            allocator_type get_allocator() const {
                return allocator_type(_alloc);
            }
    };
} // namespace ft

#endif