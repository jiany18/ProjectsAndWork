/* Copyright G. Hemingway @ 2019, All Rights Reserved */
#include "Composite_Unary_Node.h"

// Ctor
Composite_Unary_Node::Composite_Unary_Node(Component_Node* right)
    : Component_Node()
    , rightChild(right)
{
}

// Return the right child pointer
Component_Node* Composite_Unary_Node::right() const
{
    return rightChild.get();
}
