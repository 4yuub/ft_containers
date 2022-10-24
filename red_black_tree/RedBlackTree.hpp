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
            
                Node(std::allocator<Node>& pAlloc, T const &pValue, bool pIsNull=false) : _alloc(pAlloc), value(pValue) {
                    _comp = Comp();
                    parent = NULL;
                    if (pIsNull) {
                        value = T();
                        color = Black;
                        right = NULL;
                        left = NULL;
                    }
                    else {

                        color = Red;

                        right = _alloc.allocate(1);
                        _alloc.construct(right, Node(_alloc, pValue, true));
                        right->parent = this;

                        left = _alloc.allocate(1);
                        _alloc.construct(left, Node(_alloc, pValue, true));
                        left->isLeftChild = true;
                        left->parent = this;
                    }
                    isLeftChild = false;
                    isNull = pIsNull;
                }

                Node(Node const &src) : _alloc(src._alloc), value(src.value) {
                    _comp = src._comp;
                    color = src.color;
                    right = src.right;
                    left = src.left;
                    parent = src.parent;
                    isLeftChild = src.isLeftChild;
                    isNull = src.isNull;
                    if (!isNull) {
                        left->parent = this;
                        right->parent = this;
                    }
                }

                ~Node() {}

                void updateLeft(Node *pLeft, bool pDelete=false) {
                    if (isNull) return;
                    if (left->isNull && pDelete) {
                        _alloc.destroy(left);
                        _alloc.deallocate(left, 1);
                    }
                    left = pLeft;
                    left->parent = this;
                    left->isLeftChild = true;
                }

                void updateRight(Node *pRight, bool pDelete=false) {
                    if (isNull) return;
                    if (right->isNull && pDelete) {
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
        size_t                  _size;
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
            bool isLeftChild = node->isLeftChild;

            // right->parent = parent;
            right->updateLeft(node);
            if (isLeftChild) {
                parent->updateLeft(right);
                right->isLeftChild = true;
            } else {
                parent->updateRight(right);
                right->isLeftChild = false;
            }

            node->isLeftChild = true;
            // node->parent = right;
            node->updateRight(rightLeft);

            rightLeft->isLeftChild = false;
            // rightLeft->parent = node;
        }

        void _rotateRight(Node *pNode) {
            Node *node = pNode;
            Node *parent = node->parent;
            Node *left = node->left;
            Node *leftRight = left->right;
            bool isLeftChild = node->isLeftChild;

            // left->parent = node->parent;
            left->updateRight(node);
            if (isLeftChild) {
                parent->updateLeft(left);
                left->isLeftChild = true;
            } else {
                parent->updateRight(left);
                left->isLeftChild = false;
            }

            node->isLeftChild = false;
            // node->parent = left;
            node->updateLeft(leftRight);

            leftRight->isLeftChild = true;
            // leftRight->parent = node;
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
            if (pNode->isNull) {
                _alloc.destroy(pNode);
                _alloc.deallocate(pNode, 1);
                _root = NULL;
                return;
            }
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
            if (!node || node->isNull) return;
            std::cout << prefix;
            if (!node->parent->isNull)
                std::cout << (is_right ? "├─ r:" : "└─ l:" );
            else
                std::cout << "root:";

            std::cout << (node->color == Node::Red ? "\033[0;31m" : "\033[0m") << node->value << "\033[0m" << std::endl; 
            _printTree( prefix + (is_right ? "│  " : "   "), node->right, true);
            _printTree( prefix + (is_right ? "│  " : "   "), node->left, false);
        }

        Node *_getPredecessor(Node *pNode) {
            Node *leftSubtree = pNode->left;
            while (!leftSubtree->isNull) {
                leftSubtree = leftSubtree->right;
            }
            return leftSubtree->parent;
        }
    
        Node *_getSibling(Node *pNode) {
            if (_root == pNode) return NULL;
            return pNode->isLeftChild ? \
                pNode->parent->right : pNode->parent->left;
        }

        Node *_getFarNephew(Node *pNode) {
            Node *sibling = _getSibling(pNode);
            if (!sibling) return NULL;
            return pNode->isLeftChild ? sibling->right : sibling->left;
        }

        Node *_getNearNephew(Node *pNode) {
            Node *sibling = _getSibling(pNode);
            if (!sibling) return NULL;
            return pNode->isLeftChild ? sibling->left : sibling->right;
        }

        template<class X>
        void _swap(X &a, X &b) {
            X tmp(a);
            a = b;
            b = tmp;
        }

        void _deleteFixup(Node *pNode, typename Node::color_t pColor) {
            if (!pNode || pColor == Node::Red) return ; //!Not srue
            pNode->color += pColor;
            if (pNode->color != Node::DBlack)
                return; // nothing to fix
            Node *node = pNode;
            Node *parent = node->parent;
            Node *sibling = _getSibling(node);
            Node *farNephew = _getFarNephew(node);
            Node *nearNephew = _getNearNephew(node);

            while (true) {
                if (node->color != Node::DBlack) break;
                if (node == _root) {
                    node->color = Node::Black;
                    break;
                }

                if (parent->color == Node::Black && sibling->color == Node::Red
                    && farNephew->color == Node::Black && nearNephew->color == Node::Black)
                {
                    _swap(parent->color, sibling->color);
                    if (node->isLeftChild)
                    {
                        if (_root == parent)
                            _updateRoot(parent->right);
                        _rotateLeft(parent);
                    }
                    else {
                        if (_root == parent)
                            _updateRoot(parent->left);
                        _rotateRight(parent);
                    }
                }

                else if (sibling->color == Node::Black && farNephew->color == Node::Black
                    && nearNephew->color == Node::Black)
                {
                    sibling->color = Node::Red;
                    node->color -= Node::Black;
                    parent->color += Node::Black;
                    node = parent;
                }

                else if (parent->color == Node::Black && sibling->color == Node::Black
                    && farNephew->color == Node::Black && nearNephew->color == Node::Red)
                {
                    _swap(sibling->color, nearNephew->color);
                    if (node->isLeftChild)
                        _rotateRight(sibling);
                    else
                        _rotateLeft(sibling);
                }

                else if (sibling->color == Node::Black && farNephew->color == Node::Red)
                {
                    if (node->isLeftChild)
                    {
                        if (_root == parent)
                            _updateRoot(parent->right);
                        _rotateLeft(parent);
                    }
                    else {
                        if (_root == parent)
                            _updateRoot(parent->left);
                        _rotateRight(parent);
                    }
                    sibling->color = parent->color;
                    parent->color = Node::Black;
                    farNephew->color = Node::Black;
                    node->color -= Node::Black;
                }
                parent = node->parent;
                sibling = _getSibling(node);
                farNephew = _getFarNephew(node);
                nearNephew = _getNearNephew(node);
            }
        }

    public:
        void printTree() {
            _printTree("", _root, false);
        }

        RedBlackTree() {
            _alloc = std::allocator<Node>();
            _end = _alloc.allocate(1);
            _alloc.construct(_end, Node(_alloc, T(), true));
            _root = NULL;
            _size = 0;
        }

        ~RedBlackTree() {
            if (_root) {
                _deleteTree(_root);
            }
            _alloc.destroy(_end);
            _alloc.deallocate(_end, 1);
        }

        void insertNode(T const &pValue) {
            Node *newNode = _alloc.allocate(1);
            _alloc.construct(newNode, Node(_alloc, pValue));
            if (!_root) {
                _updateRoot(newNode);
                _size++;
                return;
            }
            Node *current = _root;
            while (true) {
                if (*newNode == *current) {
                    newNode->updateLeft(newNode, true); // free left
                    newNode->updateRight(newNode, true); // free right
                    _alloc.destroy(newNode);
                    _alloc.deallocate(newNode, 1);
                    return;
                }
                if (*newNode < *current) {
                    if (current->left->isNull) {
                        current->updateLeft(newNode, true);
                        newNode->isLeftChild = true;
                        break;
                    }
                    current = current->left;
                }
                else {
                    if (current->right->isNull) {
                        current->updateRight(newNode, true);
                        break;
                    }
                    current = current->right;
                }
            }
            _insertFixup(newNode);
            _size++;
        }

        Node *findNode(T const &pValue) {
            if (!_root)
                return _root;
            Node *current = _root;
            while (true) {
                if (current->isNull || pValue == current->value)
                    return current;
                if (pValue < current->value)
                    current = current->left;
                else
                    current = current->right;
            }
        }

        void deleteNode(T const &pValue) {
            Node *node = findNode(pValue);
            if (!node || node->isNull)
                return;
            Node *parent = node->parent;
            Node *left = node->left;
            Node *right = node->right;
            typename Node::color_t \
                original_color = node->color;

            if (left->isNull && right->isNull) {
                if (node->isLeftChild) {
                    parent->updateLeft(left);
                } else {
                    parent->updateRight(left);
                }
                if (_root == node) {
                    _updateRoot(left);
                    _size--;
                    return;
                }
                _alloc.destroy(node);
                _alloc.deallocate(node, 1);
                _alloc.destroy(right);
                _alloc.deallocate(right, 1);
                _deleteFixup(left, original_color);
                _size--;
                return;
            }

            if (left->isNull) {
                if (node->isLeftChild) {
                    parent->updateLeft(right);
                } else {
                    parent->updateRight(right);
                }
                if (_root == node) {
                    _updateRoot(right);
                    _size--;
                    return;
                }
                _alloc.destroy(node);
                _alloc.deallocate(node, 1);
                _alloc.destroy(left);
                _alloc.deallocate(left, 1);
                _deleteFixup(right, original_color);
                _size--;
                return;
            }

            if (right->isNull) {
                if (node->isLeftChild) {
                    parent->updateLeft(left);
                } else {
                    parent->updateRight(left);
                }
                if (_root == node) {
                    _updateRoot(left);
                    _size--;
                    return;
                }
                _alloc.destroy(node);
                _alloc.deallocate(node, 1);
                _alloc.destroy(right);
                _alloc.deallocate(right, 1);
                _deleteFixup(left, original_color);
                _size--;
                return;
            }

            Node *predecessor = _getPredecessor(node);
            node->value = predecessor->value; // copy don't work with const
            if (predecessor->isLeftChild) {
                predecessor->parent->updateLeft(predecessor->left);
            } else {
                predecessor->parent->updateRight(predecessor->left);
            }
            Node *newChild = predecessor->left;
            original_color = predecessor->color;
            _alloc.destroy(predecessor->right);
            _alloc.deallocate(predecessor->right, 1);
            _alloc.destroy(predecessor);
            _alloc.deallocate(predecessor, 1);
            _deleteFixup(newChild, original_color);
            _size--;
        }

        size_t size() {
            return _size;
        }
};

#endif