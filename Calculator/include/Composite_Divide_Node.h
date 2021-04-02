/* Copyright G. Hemingway @ 2019, All Rights Reserved */
#ifndef COMPOSITE_DIVIDE_NODE_H
#define COMPOSITE_DIVIDE_NODE_H

#include "Composite_Binary_Node.h"

class Component_Node;

class Visitor;

/**
 * @class Composite_Divide_Node
 * @brief A composite node containing left and right
 *        children. The meaning of this node is left / right.
 */
class Composite_Divide_Node : public Composite_Binary_Node {
public:
    // Ctor
    Composite_Divide_Node(Component_Node* left, Component_Node* right);

    // Dtor
    ~Composite_Divide_Node() override = default;

    // Return the printable character stored in the node.
    int item() const override;

    // Define the accept() operation used for the Visitor pattern.
    void accept(Visitor& visitor) const override;
};

#endif // COMPOSITE_DIVIDE_NODE_H
