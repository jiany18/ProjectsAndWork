/* Copyright G. Hemingway @ 2019, All Rights Reserved */
#ifndef VISITOR_H
#define VISITOR_H

class Leaf_Node;
class Composite_Negate_Node;
class Composite_Add_Node;
class Composite_Subtract_Node;
class Composite_Divide_Node;
class Composite_Multiply_Node;
class Composite_Modulus_Node;
class Composite_Power_Node;
class Composite_Factorial_Node;

/**
 * @class Visitor
 * @brief Abstract base class for all visitors to all classes that derive
 *        from Component_Node.
 *
 *        This class plays the role of the "visitor" in the Visitor
 *        pattern.
 */
class Visitor {
public:
    // Visit a Leaf_Node.
    virtual void visit(const Leaf_Node& node) = 0;

    // Visit a Composite_Negate_Node.
    virtual void visit(const Composite_Negate_Node& node) = 0;

    // Visit a Composite_Add_Node.
    virtual void visit(const Composite_Add_Node& node) = 0;

    // Visit a Composite_Subtract_Node.
    virtual void visit(const Composite_Subtract_Node& node) = 0;

    // Visit a Composite_Divide_Node.
    virtual void visit(const Composite_Divide_Node& node) = 0;

    // Visit a Composite_Multiply_Node.
    virtual void visit(const Composite_Multiply_Node& node) = 0;

    // Visit a Composite_Modulus_Node.
    virtual void visit(const Composite_Modulus_Node& node) = 0;

    // Visit a Composite_Power_Node.
    virtual void visit(const Composite_Power_Node& node) = 0;

    // Visit a Composite_Power_Node.
    virtual void visit(const Composite_Factorial_Node& node) = 0;

    // No-op destructor to hold things together.
    virtual ~Visitor() = default;
};

#endif // VISITOR_H
