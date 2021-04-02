
#ifndef PRINT_VISITOR_H
#define PRINT_VISITOR_H

#include "Visitor.h"

/**
 * @class Print_Visitor
 * @brief This class serves as a visitor for printing the contents of
 *        nodes to std::cout.
 */

class Print_Visitor : public Visitor {
public:
    // Visits a Leaf_Node and prints it contents to std::cout.
    void visit(const Leaf_Node& node) override;

    // Visit a Composite_Negate_Node and prints its contents to std::cout.
    void visit(const Composite_Negate_Node& node) override;

    // Visit a Composite_Add_Node and prints its contents to std::cout.
    void visit(const Composite_Add_Node& node) override;

    // Visit a Composite_Subtract_Node and prints its contents to @a
    // std::cout.
    void visit(const Composite_Subtract_Node& node) override;

    // Visit a Composite_Divide_Node and prints its contents to std::cout.
    void visit(const Composite_Divide_Node& node) override;

    // visit function - prints Composite_Multiply_Node contents to std::cout
    void visit(const Composite_Multiply_Node& node) override;

    // visit function - prints Composite_Modulus_Node contents to std::cout
    void visit(const Composite_Modulus_Node& node) override;

    // visit function - prints Composite_Modulus_Node contents to std::cout
    void visit(const Composite_Power_Node& node) override;

    // visit function - prints Composite_Modulus_Node contents to std::cout
    void visit(const Composite_Factorial_Node& node) override;
};

#endif // PRINT_VISITOR_H
