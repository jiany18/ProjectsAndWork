// Author: Yumeng Jiang
// VUnetid: jiany18
// Email: yumeng.jiang@vanderbilt.edu
// Class: CS3251
// Date: 11/20/2019
// Honor statement: I have neither given nor received any unauthorized aid on this assignment.
// Assignment Number: Project #7

#include "Composite_Left_Node.h"

// Ctor
Composite_Left_Node::Composite_Left_Node(Component_Node* left)
    : Component_Node()
    , leftChild(left)
{
}

// Return the right child pointer
Component_Node* Composite_Left_Node::left() const
{
    return leftChild.get();
}
