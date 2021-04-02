/* Copyright G. Hemingway @ 2019, All Rights Reserved */
#ifndef COMPOSITE_UNARY_NODE_H
#define COMPOSITE_UNARY_NODE_H

#include "Component_Node.h"
#include <memory>

/**
 * @class Composite_Unary_Node
 * @brief Defines a right child (but not a left one) and thus is useful for
 * unary operations.
 * @see Composite_Binary_Node
 */
class Composite_Unary_Node : public Component_Node {
public:
    // Ctor
    explicit Composite_Unary_Node(Component_Node* right);

    // Return the right child.
    Component_Node* right() const override;

    // Dtor
    ~Composite_Unary_Node() override = default;

private:
    // Right child
    std::unique_ptr<Component_Node> rightChild;
};

#endif // COMPOSITE_UNARY_NODE_H
