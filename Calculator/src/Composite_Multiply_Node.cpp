/* Copyright G. Hemingway @ 2019, All Rights Reserved */
#include "Composite_Multiply_Node.h"
#include "Visitor.h"

// Ctor
Composite_Multiply_Node::Composite_Multiply_Node(Component_Node* left, Component_Node* right)
    : Composite_Binary_Node(left, right)
{
}

int Composite_Multiply_Node::item() const
{
    return '*';
}

// accept a visitor
void Composite_Multiply_Node::accept(Visitor& visitor) const
{
    visitor.visit(*this);
}
