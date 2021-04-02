// Author: Yumeng Jiang
// VUnetid: jiany18
// Email: yumeng.jiang@vanderbilt.edu
// Class: CS3251
// Date: 11/20/2019
// Honor statement: I have neither given nor received any unauthorized aid on this assignment.
// Assignment Number: Project #7

#ifndef COMPOSITE_FACTORIAL_NODE_H
#define COMPOSITE_FACTORIAL_NODE_H

#include "Composite_Left_Node.h"
class Component_Node;
class Visitor;

/**
 * @class Composite_Multiply_Node
 * @brief get left / right
 */
class Composite_Factorial_Node : public Composite_Left_Node {
public:
    // Ctor
    Composite_Factorial_Node(Component_Node* right);

    // Dtor
    ~Composite_Factorial_Node() override = default;

    // Return the printable character stored in the node.
    int item() const override;

    // Define the accept() operation used for the Visitor pattern.
    void accept(Visitor& visitor) const override;
};

#endif // COMPOSITE_FACTORIAL_NODE_H
