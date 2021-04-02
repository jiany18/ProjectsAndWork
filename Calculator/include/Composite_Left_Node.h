// Author: Yumeng Jiang
// VUnetid: jiany18
// Email: yumeng.jiang@vanderbilt.edu
// Class: CS3251
// Date: 11/20/2019
// Honor statement: I have neither given nor received any unauthorized aid on this assignment.
// Assignment Number: Project #7

#ifndef COMPOSITE_LEFT_NODE_H
#define COMPOSITE_LEFT_NODE_H

#include "Component_Node.h"
#include <memory>

/**
 * @class Composite_Left_Unary_Node
 * @brief Defines a left child (but not a right one)
 * @see Composite_Factorial_Node
 */
class Composite_Left_Node : public Component_Node {
public:
    // Ctor
    explicit Composite_Left_Node(Component_Node* left);

    // Return the right child.
    Component_Node* left() const override;

    // Dtor
    ~Composite_Left_Node() override = default;

private:
    // Right child
    std::unique_ptr<Component_Node> leftChild;
};

#endif // COMPOSITE_LEFT_NODE_H
