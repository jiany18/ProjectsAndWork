// Author: Yumeng Jiang
// VUnetid: jiany18
// Email: yumeng.jiang@vanderbilt.edu
// Class: CS3251
// Date: 11/20/2019
// Honor statement: I have neither given nor received any unauthorized aid on this assignment.
// Assignment Number: Project #7
#include "Composite_Power_Node.h"
#include "Visitor.h"

// Ctor
Composite_Power_Node::Composite_Power_Node(Component_Node* left, Component_Node* right)
    : Composite_Binary_Node(left, right)
{
}

int Composite_Power_Node::item() const
{
    return '^';
}

// accept a visitor
void Composite_Power_Node::accept(Visitor& visitor) const
{
    visitor.visit(*this);
}
