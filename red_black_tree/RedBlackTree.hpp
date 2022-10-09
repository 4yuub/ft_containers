#ifndef _REDBLACKTREE_HPP_INCLUDED_
#define _REDBLACKTREE_HPP_INCLUDED_
#include "common.hpp"

template <class T, class Comp, class Alloc> 
class RedBlackTree {
    public:
        class Node {
            private:
                Comp                 _comp;
                std::allocator<Node> _alloc;
            public:
                typedef int8_t      color_t;
                static const int8_t Red    = 0;
                static const int8_t Black  = 1;
                static const int8_t DBlack = 2;

                T              value;
                color_t        color;
                Node           *right;
                Node           *left;
                Node           *parent;
                bool           isLeftChild;
                bool           isNull;
            
                Node(T const &pValue, bool pIsNull=false) : value(pValue) {
                    _comp = Comp();
                    _alloc = std::allocator<Node>();
                    if (pIsNull) {
                        color = Black;
                        right = NULL;
                        left = NULL;
                    }
                    else {

                        color = Red;

                        right = _alloc.allocate(1);
                        *right = Node(pValue, this, true);

                        left = _alloc.allocate(1);
                        *left = Node(pValue, this, true);
                        left->isLeftChild = true;
                    }
                    parent = NULL;
                    isLeftChild = false;
                    isNull = pIsNull;
                }

                ~Node() {}

                void updateLeft(Node *pLeft) {
                    if (left->isNull) {
                        _alloc.destroy(left);
                        _alloc.deallocate(left, 1);
                    }
                    left = pLeft;
                    left->parent = this;
                    left->isLeftChild = true;
                }

                void updateRight(Node *pRight) {
                    if (right->isNull) {
                        _alloc.destroy(right);
                        _alloc.deallocate(right, 1);
                    }
                    right = pRight;
                    right->parent = this;
                    right->isLeftChild = false;
                }

                bool operator<(Node const &rhs) const {
                    return _comp(value, rhs.value);
                }

                bool operator<=(Node const &rhs) const {
                    return !(rhs < *this);
                }
                
                bool operator>(Node const &rhs) const {
                    return rhs < *this;
                }
                
                bool operator>=(Node const &rhs) const {
                    return !(*this < rhs);
                }
                
                bool operator==(Node const &rhs) const {
                    return !(*this < rhs) && !(rhs < *this);
                }
                
                bool operator!=(Node const &rhs) const {
                    return !(*this == rhs);
                }
        };
    
    private:
        Node                    *_root;
        Node                    *_end;
        std::allocator<Node>    _alloc;

    private:
        void _deleteTree(Node *pNode) {
            if (!pNode) return;
            if (pNode->isNull) {
                _alloc.destroy(pNode);
                _alloc.deallocate(pNode, 1);
                return;
            }
            _deleteTree(pNode->left);
            _deleteTree(pNode->right);
            _alloc.destroy(pNode);
            _alloc.deallocate(pNode, 1);
        }

    public:
        RedBlackTree() {
            _alloc = std::allocator<Node>();
            _end = _alloc.allocate(1);
            *_end = Node(T(), true);
            _root = NULL;
        }

        ~RedBlackTree() {
            if (_root) {
                _deleteTree(_root);
            }
            _alloc.destroy(_end);
            _alloc.deallocate(_end, 1);
        }

        void insert(T const &pValue) {
            Node *newNode = _alloc.allocate(1);
            *newNode = Node(pValue);
            if (!_root) {
                _root = newNode;
                _root->color = Node::Black;
                _root->isLeftChild = true;
                _end->updateLeft(_root);
                return;
            }
            Node *current = _root;
            while (true) {
                if (newNode == current) {
                    return;
                }
                if (newNode < current) {
                    if (current->left->isNull) {
                        current->updateLeft(newNode);
                        newNode->isLeftChild = true;
                        break;
                    }
                    current = current->left;
                }
                else {
                    if (current->right->isNull) {
                        current->updateRight(newNode);
                        break;
                    }
                    current = current->right;
                }
            }
            // _insertFixup(newNode);
        }
};

#endif