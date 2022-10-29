#ifndef _REDBLACKTREE_HPP_INCLUDED_
#define _REDBLACKTREE_HPP_INCLUDED_
#include "common.hpp"

namespace ft {
    template <class T, class Comp> 
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
                            color = Black;
                            right = NULL;
                            left = NULL;
                        }
                        else {

                            color = Red;

                            right = _alloc.allocate(1);
                            _alloc.construct(right, Node(_alloc, T(), true));
                            right->parent = this;

                            left = _alloc.allocate(1);
                            _alloc.construct(left, Node(_alloc, T(), true));
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
            Comp                    _cmp;
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

                right->updateLeft(node);
                if (isLeftChild) {
                    parent->updateLeft(right);
                    right->isLeftChild = true;
                } else {
                    parent->updateRight(right);
                    right->isLeftChild = false;
                }

                node->isLeftChild = true;
                node->updateRight(rightLeft);

                rightLeft->isLeftChild = false;
            }

            void _rotateRight(Node *pNode) {
                Node *node = pNode;
                Node *parent = node->parent;
                Node *left = node->left;
                Node *leftRight = left->right;
                bool isLeftChild = node->isLeftChild;

                left->updateRight(node);
                if (isLeftChild) {
                    parent->updateLeft(left);
                    left->isLeftChild = true;
                } else {
                    parent->updateRight(left);
                    left->isLeftChild = false;
                }

                node->isLeftChild = false;
                node->updateLeft(leftRight);

                leftRight->isLeftChild = true;
            }

            Node *_getUncel(Node *pNode) const {
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
#ifdef DEBUG
            void _printTree(const std::string &prefix, Node *node, bool is_right) const {
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
#endif
            Node *_getPredecessor(Node *pNode) const {
                Node *leftSubtree = pNode->left;
                while (!leftSubtree->isNull) {
                    leftSubtree = leftSubtree->right;
                }
                return leftSubtree->parent;
            }
        
            Node *_getSibling(Node *pNode) const {
                if (_root == pNode) return NULL;
                return pNode->isLeftChild ? \
                    pNode->parent->right : pNode->parent->left;
            }

            Node *_getFarNephew(Node *pNode) const {
                Node *sibling = _getSibling(pNode);
                if (!sibling) return NULL;
                return pNode->isLeftChild ? sibling->right : sibling->left;
            }

            Node *_getNearNephew(Node *pNode) const {
                Node *sibling = _getSibling(pNode);
                if (!sibling) return NULL;
                return pNode->isLeftChild ? sibling->left : sibling->right;
            }

            template<class X>
            void _swap(X &a, X &b) const {
                X tmp(a);
                a = b;
                b = tmp;
            }

            void _deleteFixup(Node *pNode, typename Node::color_t pColor) {
                pNode->color += pColor;
                if (pNode->color != Node::DBlack)
                    return; // nothing to fix
                Node *node = pNode;
                Node *parent = node->parent;
                Node *sibling = _getSibling(node);
                Node *farNephew = _getFarNephew(node);
                Node *nearNephew = _getNearNephew(node);

                if (node == _root) {
                    node->color = Node::Black;
                    return;
                }

                if (sibling->color == Node::Red)
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

                else if (sibling->color == Node::Black
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
                _deleteFixup(node, 0);
            }

            void _changeValue(Node *pPos, Node *pNewNode) {
                Node *parent = pPos->parent;
                Node *newNode = _alloc.allocate(1);
                _alloc.construct(newNode, Node(_alloc, pNewNode->value));
                newNode->color = pPos->color;
                newNode->updateLeft(pPos->left, true);
                newNode->updateRight(pPos->right, true);
                if (_root == pPos) {
                    _updateRoot(newNode);
                }
                else if (pPos->isLeftChild) {
                    parent->updateLeft(newNode);
                }
                else {
                    parent->updateRight(newNode);
                }
                _alloc.destroy(pPos);
                _alloc.deallocate(pPos, 1);
            }
#ifdef DEBUG
            size_t _getBlackHeight(Node *node) const {
                if (!node || node->isNull) return 0;
                size_t leftHeight = _getBlackHeight(node->left);
                return leftHeight + (node->color == Node::Black);
            }

            bool _testRedBlack(Node *node) const {
                if (!node || node->isNull) return true;
                if (node->color == Node::Red) {
                    if (node->left->color == Node::Red || node->right->color == Node::Red) {
                        return false;
                    }
                }
                int leftBlackHeight = _getBlackHeight(node->left);
                int rightBlackHeight = _getBlackHeight(node->right);
                if (leftBlackHeight != rightBlackHeight) {
                    return false;
                }
                return _testRedBlack(node->left) && _testRedBlack(node->right);
            }
#endif
        public:
#ifdef DEBUG
            void printTree() const {
                _printTree("", _root, false);
            }
#endif
            RedBlackTree() {
                _alloc = std::allocator<Node>();
                _cmp = Comp();
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

            void deleteTree() {
               _deleteTree(_root);
               _size = 0;
               _root = NULL;
               _end->left = NULL; 
            }

            void insertNode(T const &pValue) {
                Node *nodePos = findNode(pValue);
                if (nodePos && !nodePos->isNull)
                    return ;
                Node *newNode = _alloc.allocate(1);
                _alloc.construct(newNode, Node(_alloc, pValue));
                if (!nodePos) {
                    _updateRoot(newNode);
                    _size++;
                    return;
                }
                Node *parent = nodePos->parent;
                if (nodePos->isLeftChild) {
                    parent->updateLeft(newNode, true);
                }
                else {
                    parent->updateRight(newNode, true);
                }
                _insertFixup(newNode);
                _size++;
            }

            Node *findNode(T const &pValue) const {
                if (!_root)
                    return _root;
                Node *current = _root;
                while (true) {
                    if (current->isNull
                        || (!_cmp(pValue, current->value) 
                        && !_cmp(current->value, pValue)))
                        return current;
                    if (_cmp(pValue, current->value))
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
                    }
                    _alloc.destroy(node);
                    _alloc.deallocate(node, 1);
                    _alloc.destroy(right);
                    _alloc.deallocate(right, 1);
                    if (_root) // because we replace root with NULL (0x0) if left->isNull
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
                _changeValue(node, predecessor);
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

            size_t size() const {
                return _size;
            }
#ifdef DEBUG
            bool testRedBlack() const {
                return _testRedBlack(_root);
            }
#endif
            Node *root() const {
                return _root;
            }

            Node *end() const {
                return _end;
            }

            Node *min() const {
                Node *current = _root;
                while (current && !current->isNull) {
                    current = current->left;
                }
                return current ? current->parent : _end;
            }

            Node *max() const {
                Node *current = _root;
                while (current && !current->isNull) {
                    current = current->right;
                }
                return current ? current->parent : _end;
            }
    };
} // namespace ft

#endif