/* Copyright G. Hemingway @ 2019, All Rights Reserved */
#ifndef COMPONENT_NODE_H
#define COMPONENT_NODE_H

#include <stdexcept>
#include <string>

// Forward declaration.
class Visitor;

/**
 * @class Component_Node
 * @brief An abstract base class defines a simple abstract
 *        implementation of an expression tree node.
 *
 *        This class plays the role of the "Component" in the
 *        Composite pattern.  The methods in this class are not
 *        defined as pure virtual so that subclasses in the Composite
 *        pattern don't have to implement methods they don't care
 *        about.
 *
 * @see   See Composite_Unary_Node and Composite_Binary_Node for nodes
 *        with right only and left and right children, respectively.
 */
class Component_Node {
public:
    // Dtor
    virtual ~Component_Node() = default;

    // Return the item stored in the node
    virtual int item() const = 0;

    // Return the left child (returns nullptr if called directly).
    virtual Component_Node* left() const;

    // Return the right child (returns nullptr if called directly).
    virtual Component_Node* right() const;

    // Accept a visitor to perform some action on the node's item
    // completely arbitrary visitor template
    virtual void accept(Visitor& visitor) const = 0;
};

#endif // COMPONENT_NODE_H
