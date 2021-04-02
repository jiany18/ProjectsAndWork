
#ifndef COUNT_VISITOR_H
#define COUNT_VISITOR_H

#include "Visitor.h"
#include <map>
#include <string>

/**
 * @class Count_Visitor
 * @brief This class keeps track of the amount of each operator.
 */

class Count_Visitor : public Visitor {
public:
    // Visits a Leaf_Node
    void visit(const Leaf_Node& node) override;

    // Visit a Composite_Negate_Node
    void visit(const Composite_Negate_Node& node) override;

    // Visit a Composite_Add_Node
    void visit(const Composite_Add_Node& node) override;

    // Visit a Composite_Subtract_Node
    void visit(const Composite_Subtract_Node& node) override;

    // Visit a Composite_Divide_Node
    void visit(const Composite_Divide_Node& node) override;

    // visit Composite_Multiply_Node
    void visit(const Composite_Multiply_Node& node) override;

    // visit Composite_Modulus_Node
    void visit(const Composite_Modulus_Node& node) override;

    // visit Composite_Modulus_Node
    void visit(const Composite_Power_Node& node) override;

    // visit Composite_Modulus_Node
    void visit(const Composite_Factorial_Node& node) override;

    void print();

private:
    std::map<std::string, int> count;
};

#endif // COUNT_VISITOR_H
