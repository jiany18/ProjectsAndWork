
#include "Count_Visitor.h"
#include "Composite_Add_Node.h"
#include "Composite_Divide_Node.h"
#include "Composite_Factorial_Node.h"
#include "Composite_Modulus_Node.h"
#include "Composite_Multiply_Node.h"
#include "Composite_Negate_Node.h"
#include "Composite_Power_Node.h"
#include "Composite_Subtract_Node.h"
#include "Leaf_Node.h"
#include <iostream>

void Count_Visitor::visit(const Leaf_Node& node)
{
}

void Count_Visitor::visit(const Composite_Negate_Node& node)
{
    count["-(Negation)"] += 1;
}

void Count_Visitor::visit(const Composite_Add_Node& node)
{
    count["+"] += 1;
}

void Count_Visitor::visit(const Composite_Subtract_Node& node)
{
    count["-(Subtraction)"] += 1;
}

void Count_Visitor::visit(const Composite_Divide_Node& node)
{
    count["/"] += 1;
}

void Count_Visitor::visit(const Composite_Multiply_Node& node)
{
    count["*"] += 1;
}

void Count_Visitor::visit(const Composite_Modulus_Node& node)
{
    count["%"] += 1;
}

void Count_Visitor::visit(const Composite_Power_Node& node)
{
    count["^"] += 1;
}

void Count_Visitor::visit(const Composite_Factorial_Node& node)
{
    count["!"] += 1;
}

void Count_Visitor::print()
{
    for (auto i = count.begin(); i != count.end(); ++i) {
        if (i->second != 0) {
            std::cout << i->first << ": " << i->second << std::endl;
        }
    }
}