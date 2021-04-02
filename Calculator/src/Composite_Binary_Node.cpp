/* Copyright G. Hemingway @ 2019, All Rights Reserved */
#include "Composite_Binary_Node.h"

// Ctor
Composite_Binary_Node::Composite_Binary_Node(Component_Node* left, Component_Node* right)
    : Composite_Unary_Node(right)
    , leftChild(left)
{
}

// Return the left child pointer
Component_Node* Composite_Binary_Node::left() const
{
    return leftChild.get();
}
