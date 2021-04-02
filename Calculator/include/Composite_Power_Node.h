// Author: Yumeng Jiang
// VUnetid: jiany18
// Email: yumeng.jiang@vanderbilt.edu
// Class: CS3251
// Date: 11/20/2019
// Honor statement: I have neither given nor received any unauthorized aid on this assignment.
// Assignment Number: Project #7

#ifndef EXPRESSIONTREE_COMPOSITE_POWER_NODE_H
#define EXPRESSIONTREE_COMPOSITE_POWER_NODE_H

class Component_Node;

#include "Composite_Binary_Node.h"

class Visitor;

/**
 * @class Composite_Power_Node
 * @brief A composite node containing left and right
 *        children. The meaning of this node is left ^ right
 */
class Composite_Power_Node : public Composite_Binary_Node {
public:
    // Ctor
    Composite_Power_Node(Component_Node* left, Component_Node* right);

    // Dtor
    ~Composite_Power_Node() override = default;

    // Return the printable character stored in the node.
    int item() const override;

    // Define the accept() operation used for the Visitor pattern.
    void accept(Visitor& visitor) const override;
};

#endif // EXPRESSIONTREE_COMPOSITE_POWER_NODE_H
