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
                        *right = Node(pValue, true);

                        left = _alloc.allocate(1);
                        *left = Node(pValue, true);
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

        void _rotateLeft(Node *pNode) {
            Node *node = pNode;
            Node *parent = node->parent;
            Node *right = node->right;
            Node *rightLeft = right->left;

            right->parent = node->parent;
            right->updateLeft(node);
            if (node->isLeftChild) {
                parent->updateLeft(right);
                right->isLeftChild = true;
            } else {
                parent->updateRight(right);
                right->isLeftChild = false;
            }

            node->isLeftChild = true;
            node->parent = right;
            node->updateRight(rightLeft);
            
            rightLeft->isLeftChild = false;
            rightLeft->parent = node;
        }

        void _rotateRight(Node *pNode) {
            Node *node = pNode;
            Node *parent = node->parent;
            Node *left = node->left;
            Node *leftRight = left->right;

            left->parent = node->parent;
            left->updateRight(node);
            if (node->isLeftChild) {
                parent->updateLeft(left);
                left->isLeftChild = true;
            } else {
                parent->updateRight(left);
                left->isLeftChild = false;
            }

            node->isLeftChild = false;
            node->parent = left;
            node->updateLeft(leftRight);

            leftRight->isLeftChild = true;
            leftRight->parent = node;
        }

        Node *_getUncel(Node *pNode) {
            Node *node = pNode;
            Node *parent = node->parent;
            Node *grandParent = parent->parent;
            Node *uncle = NULL;
            if (parent->isLeftChild) {
                uncle = grandParent->right;
            } else {
                uncle = grandParent->left;
            }
            return uncle;
        }

        void _updateRoot(Node *pNode) {
            _root = pNode;
            _root->color = Node::Black;
            _root->isLeftChild = true;
            _root->parent = _end;
            _end->left = _root; 
        }

        void _insertFixup(Node *pNode) {
            if (pNode->color == Node::Black)
                return;
            if (pNode == _root) {
                _root->color = Node::Black;
                return;
            }
            if (pNode->color == Node::Red && pNode->parent->color == Node::Black)
                return;
        
            Node *node = pNode;
            Node *uncle = _getUncel(node);
            Node *parent = node->parent;
            Node *grandParent = parent->parent;

            if (uncle->color == Node::Red) {
                parent->color = Node::Black;
                uncle->color = Node::Black;
                parent->parent->color = Node::Red;
                _insertFixup(parent->parent);
                return;
            }

            if (node->isLeftChild && parent->isLeftChild) {
                _rotateRight(grandParent);
                grandParent->color = Node::Red;
                parent->color = Node::Black;
                if (grandParent == _root)
                    _updateRoot(parent);
                return;
            }

            if (!node->isLeftChild && !parent->isLeftChild) {
                _rotateLeft(grandParent);
                grandParent->color = Node::Red;
                parent->color = Node::Black;
                if (grandParent == _root)
                    _updateRoot(parent);
                return;
            }

            if (node->isLeftChild && !parent->isLeftChild) {
                _rotateRight(parent);
                _rotateLeft(grandParent);
                grandParent->color = Node::Red;
                node->color = Node::Black;
                if (grandParent == _root)
                    _updateRoot(node);
                return;
            }

            if (!node->isLeftChild && parent->isLeftChild) {
                _rotateLeft(parent);
                _rotateRight(grandParent);
                grandParent->color = Node::Red;
                node->color = Node::Black;
                if (grandParent == _root)
                    _updateRoot(node);
                return;
            }
        }

        void _printTree(const std::string &prefix, Node *node, bool is_right) {
            if (node->isNull) return;
            std::cout << prefix;
            if (!node->parent->isNull)
                std::cout << (is_right ? "├─ r:" : "└─ l:" );
            else
                std::cout << "root:";

            std::cout << (node->color == Node::Red ? "\033[0;31m" : "\033[0m") << node->value << "\033[0m" << std::endl; 
            _printTree( prefix + (is_right ? "│  " : "   "), node->right, true);
            _printTree( prefix + (is_right ? "│  " : "   "), node->left, false);
        }

    public:
        void printTree() {
            _printTree("", _root, false);
        }

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
                _updateRoot(newNode);
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
            _insertFixup(newNode);
        }
};

#endif