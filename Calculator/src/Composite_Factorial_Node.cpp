
#include "Composite_Factorial_Node.h"
#include "Visitor.h"

// Ctor
Composite_Factorial_Node::Composite_Factorial_Node(Component_Node* left)
    : Composite_Left_Node(left)
{
}

int Composite_Factorial_Node::item() const
{
    return '!';
}

// accept a visitor
void Composite_Factorial_Node::accept(Visitor& visitor) const
{
    visitor.visit(*this);
}
