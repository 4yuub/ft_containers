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
            typedef ft::reverse_iterator<const_iterator>           const_reverse_iterator;
            typedef ft::reverse_iterator<iterator>                 reverse_iterator;
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
                insert(first, last);
            }

            map(const map& x): _tree(RedBlackTree<value_type, value_compare>()), _cmp(x._cmp), _alloc(x._alloc)  {
                for (iterator it = x.begin(); it != x.end(); ++it) {
                    insert(*it);
                }
            }

            ~map() {}

            map& operator= (const map& x) {
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

            mapped_type& operator[] (const key_type& k) {
                typename tree_type::Node *node;
                value_type to_find(k, mapped_type());
                node = _tree.insertNode(to_find);
                return node->value.second;
            }

            mapped_type& at(const key_type& k) {
                typename tree_type::Node *node;
                value_type to_find(k, mapped_type());
                node = _tree.findNode(to_find);
                if (!node || node->isNull) {
                    throw std::out_of_range("map::at");
                }
                return node->value.second;
            }
            
            const mapped_type& at (const key_type& k) const {
                return at(k);
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
                (void) position; // my map don't need hint it's so smart
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
                value_type toDelete(k, mapped_type());
                return _tree.deleteNode(toDelete);
            }

            void erase(iterator first, iterator last) {
                if (first == last)
                    return ;
                iterator tmp(first);
                ++tmp;
                erase(first);
                erase(tmp, last);
            }

            void swap (map& x) {
                tree_type   tmp_tree(x._tree);
                Compare     tmp_cmp(x._cmp);
                Allocator   tmp_alloc(x._alloc);

                x._tree = _tree;
                x._cmp = _cmp;
                x._alloc = _alloc;

                _tree = tmp_tree;
                _cmp = tmp_cmp;
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
            iterator find(const key_type& k) {
                typename tree_type::Node *node;
                value_type toFind(k, mapped_type());
                node = _tree.findNode(toFind);
                if (!node || node->isNull) {
                    return iterator(_tree.end());
                }
                return iterator(node);
            }

            const_iterator find(const key_type& k) const {
                typename tree_type::Node *node;
                value_type toFind(k, mapped_type());
                node = _tree.findNode(toFind);
                if (!node || node->isNull) {
                    return const_iterator(_tree.end());
                }
                return const_iterator(node);

            }

            size_type count(const key_type& k) const {
                return find(k) != end();
            }

            iterator lower_bound(const key_type& k) {
                typename tree_type::Node *node;
                value_type toFind(k, mapped_type());
                node = _tree.findNode(toFind);
                if (!node)
                    return iterator(_tree.end());
                if (node->isNull) {
                    iterator it(node);
                    return ++it;
                }
                return iterator(node);
            }
            
            const_iterator lower_bound(const key_type& k) const {
                typename tree_type::Node *node;
                value_type toFind(k, mapped_type());
                node = _tree.findNode(toFind);
                if (!node)
                    return const_iterator(_tree.end());
                if (node->isNull) {
                    const_iterator it(node);
                    return ++it;
                }
                return const_iterator(node);
            }

            iterator upper_bound(const key_type& k) {
                typename tree_type::Node *node;
                value_type toFind(k, mapped_type());
                node = _tree.findNode(toFind);
                if (!node)
                    return iterator(_tree.end());
                iterator it(node);
                return ++it;
            }
            
            const_iterator upper_bound(const key_type& k) const {
                typename tree_type::Node *node;
                value_type toFind(k, mapped_type());
                node = _tree.findNode(toFind);
                if (!node)
                    return const_iterator(_tree.end());
                const_iterator it(node);
                return ++it;
            }

            pair<iterator,iterator> equal_range(const key_type& k) {
                return pair<iterator,iterator>(lower_bound(k), upper_bound(k));
            }

            pair<const_iterator,const_iterator> equal_range(const key_type& k) const {
                return pair<const_iterator,const_iterator>(lower_bound(k), upper_bound(k));
            }

            // allocator
            allocator_type get_allocator() const {
                return allocator_type(_alloc);
            }
    };
} // namespace ft

#endif