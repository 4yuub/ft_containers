#include "iterator.hpp"
#include "RedBlackTree.hpp"

namespace ft {
    template<class T, class Comp>
    class RBT_Iterator : \
        public iterator<bidirectional_iterator_tag, typename RedBlackTree<T, Comp>::Node>
    {
        public:
            typedef T                                     value_type;
            typedef ptrdiff_t                             difference_type;
            typedef typename RedBlackTree<T, Comp>::Node* pointer;
            typedef typename RedBlackTree<T, Comp>::Node& reference;
            typedef bidirectional_iterator_tag            iterator_category;
        
        private:
            pointer _ptr;

        private:
            pointer _getSuccessor(pointer node) {
                pointer rightSubTree = node->right;
                while (!rightSubTree->isNull) {
                    rightSubTree = rightSubTree->left;
                }
                return rightSubTree->parent;
            }

            pointer _getPredecessor(pointer node) {
                pointer leftSubTree = node->left;
                while (!leftSubTree->isNull) {
                    leftSubTree = leftSubTree->right;
                }
                return leftSubTree->parent;
            }

            pointer _getRightParent(pointer node) {
                if (node->isLeftChild) {
                    return node->parent;
                }
                return _getRightParent(node->parent);
            }

            pointer _getLeftParent(pointer node) {
                if (!node->isLeftChild) {
                    return node->parent;
                }
                return _getLeftParent(node->parent);
            }

            pointer _getNext(pointer node) {
                if (!node->right->isNull) {
                    return _getSuccessor(node);
                }
                return _getRightParent(node);
            }

            pointer _getPrev(pointer node) {
                if (!node->left->isNull) {
                    return _getPredecessor(node);
                }
                return _getLeftParent(node);
            }

        public:
            RBT_Iterator() {} // default random value

            RBT_Iterator(pointer x) {
                _ptr = x;
            }

            RBT_Iterator(RBT_Iterator const &obj) {
                _ptr = obj._ptr;
            }

            // operators
            RBT_Iterator &operator=(RBT_Iterator const &rhs) {
                _ptr = rhs._ptr;
                return *this;
            }

            bool operator==(RBT_Iterator const &rhs) const {
                return _ptr == rhs._ptr;
            }

            bool operator!=(RBT_Iterator const &rhs) const {
                return _ptr != rhs._ptr;
            }

            value_type &operator*() {
                return _ptr->value;
            }
            
            value_type const &operator*() const {
                return _ptr->value;
            }

            value_type *operator->() const {
                return &(_ptr->value);
            }

            RBT_Iterator &operator++() {
                _ptr = _getNext(_ptr);
                return *this;
            } // pre increment

            RBT_Iterator operator++(int) {
                RBT_Iterator tmp(_ptr);
                ++(*this);
                return tmp;
            } // post increment

            RBT_Iterator &operator--() {
                _ptr = _getPrev(_ptr);
                return *this;
            } // pre decrement

            RBT_Iterator operator--(int) {
                RBT_Iterator tmp(_ptr);
                --(*this);
                return tmp;
            } // post decrement

    };
} // namespace ft
