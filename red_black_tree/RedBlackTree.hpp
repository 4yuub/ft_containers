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
            
                Node(T const &pValue, Node *pParent, bool pIsNull=false, bool pIsLeftChild=false) : value(pValue) {
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
                    parent = pParent;
                    isLeftChild = pIsLeftChild;
                    isNull = pIsNull;
                }

                ~Node() {
                    if (isNull)
                        return ;
                    if (left->isNull) {
                        _alloc.destroy(left);
                        _alloc.deallocate(left, 1);
                    }
                    if (right->isNull) {
                        _alloc.destroy(right);
                        _alloc.deallocate(right, 1);
                    }
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
};

#endif