#include "Evaluation_Visitor.h"
#include "Expression_Tree_Context.h"
#include "Expression_Tree_Iterator.h"
#include "Print_Visitor.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>

// Author: Yumeng Jiang
// VUnetid: jiany18
// Email: yumeng.jiang@vanderbilt.edu
// Class: CS3251
// Date: 11/20/2019
// Honor statement: I have neither given nor received any unauthorized aid on this assignment.
// Assignment Number: Project #7

/**
 * @class Accept_Visitor_Adapter
 * @brief This functor implements the Adapter pattern so the @a
 *        Component_Node's accept() method can be called with the
 *        appropriate visitor in the context of the std:for_each()
 *        algorithm.
 */
template <typename VISITOR> class Accept_Visitor_Adapter {
public:
    // Constructor.
    explicit Accept_Visitor_Adapter(VISITOR& v)
        : visitor(v)
    {
    }

    // Accept the visitor_ to visit the node.
    void operator()(const Expression_Tree& t)
    {
        t.accept(visitor);
    }

private:
    VISITOR& visitor;
};

// this method traverses the tree in with a given traversal strategy
void Expression_Tree_State::print_tree(
    const Expression_Tree& tree, const std::string& traversal_order, std::ostream& os)
{
    os << "traverse tree using strategy '" << traversal_order << "':" << std::endl;

    // create a print visitor
    Print_Visitor print_visitor;
    std::for_each(tree.begin(traversal_order), tree.end(traversal_order),
        Accept_Visitor_Adapter<Print_Visitor>(print_visitor));

    os << std::endl;
}

void Expression_Tree_State::evaluate_tree(
    const Expression_Tree& tree, const std::string& traversal_order, std::ostream&)
{
    Evaluation_Visitor evaluation_visitor;
    std::for_each(tree.begin(traversal_order), tree.end(traversal_order),
        Accept_Visitor_Adapter<Evaluation_Visitor>(evaluation_visitor));
    std::cout << evaluation_visitor.total() << std::endl;
}

// Static data member definitions.
Uninitialized_State::Uninitialized_State_Factory::UNINITIALIZED_STATE_MAP
    Uninitialized_State::Uninitialized_State_Factory::uninitialized_state_map;

Uninitialized_State::Uninitialized_State_Factory Uninitialized_State::uninitialized_state_factory;

Uninitialized_State::Uninitialized_State_Factory::Uninitialized_State_Factory()
{
    uninitialized_state_map["in-order"]
        = &Uninitialized_State::Uninitialized_State_Factory::make_in_order_uninitialized_state;
    uninitialized_state_map["pre-order"]
        = &Uninitialized_State::Uninitialized_State_Factory::make_pre_order_uninitialized_state;
    uninitialized_state_map["post-order"]
        = &Uninitialized_State::Uninitialized_State_Factory::make_post_order_uninitialized_state;
    uninitialized_state_map["level-order"]
        = &Uninitialized_State::Uninitialized_State_Factory::make_level_order_uninitialized_state;
}

Expression_Tree_State*
Uninitialized_State::Uninitialized_State_Factory::make_level_order_uninitialized_state()
{
    return new Level_Order_Uninitialized_State();
}

Expression_Tree_State*
Uninitialized_State::Uninitialized_State_Factory::make_in_order_uninitialized_state()
{
    return new In_Order_Uninitialized_State();
}

Expression_Tree_State*
Uninitialized_State::Uninitialized_State_Factory::make_pre_order_uninitialized_state()
{
    return new Pre_Order_Uninitialized_State();
}

Expression_Tree_State*
Uninitialized_State::Uninitialized_State_Factory::make_post_order_uninitialized_state()
{
    return new Post_Order_Uninitialized_State();
}

Expression_Tree_State* Uninitialized_State::Uninitialized_State_Factory::make_uninitialized_state(
    const std::string& format)
{
    auto iter = uninitialized_state_map.find(format);
    if (iter == uninitialized_state_map.end()) {
        // We don't understand the type. Convert the type to a string
        // and pass it back via an exception
        throw Expression_Tree::Invalid_Iterator(format);
    } else {
        Uninitialized_State::Uninitialized_State_Factory::UNINITIALIZED_STATE_PTF ptf
            = iter->second;
        return (*ptf)();
    }
}

void Uninitialized_State::format(Expression_Tree_Context& context, const std::string& new_format)
{
    // Call factory method to initialize the context state.
    context.state(Uninitialized_State_Factory::make_uninitialized_state(new_format));
}

void Uninitialized_State::make_tree(Expression_Tree_Context&, const std::string&)
{
    throw Expression_Tree_State::Invalid_State("Expr - Can't call that command yet.");
}

void Uninitialized_State::print(Expression_Tree_Context&, const std::string&)
{
    throw Expression_Tree_State::Invalid_State("Print - Can't call that command yet.");
}

void Uninitialized_State::evaluate(Expression_Tree_Context&, const std::string&)
{
    throw Expression_Tree_State::Invalid_State("Eval - Can't call that command yet.");
}

void Uninitialized_State::print_valid_commands(Expression_Tree_Context& context) const
{
    std::cout << "1a. format [in-order]\n";
    std::cout << "1b. set [variable=value]\n";
    std::cout << "2. expr [expression]\n";
    std::cout << "3a. eval [post-order]\n";
    std::cout << "3b. print [in-order | pre-order | post-order | level-order]\n";
    std::cout << "0. quit\n";
    std::cout.flush();
}

void Uninitialized_State::set(Expression_Tree_Context& context, const std::string& key_value_pair)
{
    context.set(key_value_pair);
    context.state(new Uninitialized_State);
}

void Pre_Order_Uninitialized_State::make_tree(Expression_Tree_Context&, const std::string&)
{
    throw Expression_Tree_State::Invalid_State(
        "Pre_Order_Uninitialized_State::make_tree - not implemented");
}

void Pre_Order_Initialized_State::print(Expression_Tree_Context& context, const std::string& format)
{
    Expression_Tree_State::print_tree(context.tree(), format, std::cout);
}

void Pre_Order_Initialized_State::evaluate(
    Expression_Tree_Context& context, const std::string& param)
{
    Expression_Tree_State::evaluate_tree(context.tree(), param, std::cout);
}

void Post_Order_Uninitialized_State::make_tree(
    Expression_Tree_Context& tree_context, const std::string& expr)
{
    throw Expression_Tree_State::Invalid_State(
        "Post_Order_Uninitialized_State::make_tree - not implemented");
}

void Post_Order_Initialized_State::print(
    Expression_Tree_Context& context, const std::string& format)
{
    Expression_Tree_State::print_tree(context.tree(), format, std::cout);
}

void Post_Order_Initialized_State::evaluate(Expression_Tree_Context& context, const std::string&)
{
    Expression_Tree_State::evaluate_tree(context.tree(), "param", std::cout);
}

void Level_Order_Uninitialized_State::make_tree(Expression_Tree_Context&, const std::string&)
{
    throw Expression_Tree_State::Invalid_State(
        "Level_Order_Uninitialized_State::make_tree - not implemented");
}

void Level_Order_Initialized_State::print(
    Expression_Tree_Context& context, const std::string& format)
{
    Expression_Tree_State::print_tree(context.tree(), format, std::cout);
}

void Level_Order_Initialized_State::evaluate(
    Expression_Tree_Context& context, const std::string& param)
{
    Expression_Tree_State::evaluate_tree(context.tree(), param, std::cout);
}

void In_Order_Uninitialized_State::make_tree(
    Expression_Tree_Context& tree_context, const std::string& expr)
{
    // Interpreter_Context context;
    Interpreter interpreter;
    tree_context.tree(interpreter.interpret(tree_context.int_context, expr));
    tree_context.state(new In_Order_Initialized_State);
}

void In_Order_Uninitialized_State::print_valid_commands(Expression_Tree_Context& context) const
{
    std::cout << "\n";
    std::cout << "1. expr [expression]\n";
    std::cout << "2a. eval [post-order]\n";
    std::cout << "2b. print [in-order | pre-order | post-order | level-order]\n";
    std::cout << "0a. format [in-order]\n";
    std::cout << "0b. set [variable=value]\n";
    if (context.hasSet()) {
        std::cout << "0b-1. get [variable]\n";
        std::cout << "0b-2. list\n";
    }
    std::cout << "0c. history\n";
    std::cout << "0d. quit\n";
    std::cout.flush();
}

void In_Order_Uninitialized_State::set(
    Expression_Tree_Context& context, const std::string& key_value_pair)
{
    context.set(key_value_pair);
    context.state(new In_Order_Uninitialized_State);
}

void In_Order_Initialized_State::print(Expression_Tree_Context& context, const std::string& format)
{
    print_tree(context.tree(), format, std::cout);
}

void In_Order_Initialized_State::evaluate(
    Expression_Tree_Context& context, const std::string& param)
{
    Expression_Tree_State::evaluate_tree(context.tree(), param, std::cout);
}

void In_Order_Initialized_State::print_valid_commands(Expression_Tree_Context& context) const
{
    std::cout << "\n";
    std::cout << "1a. eval [post-order]\n";
    std::cout << "1b. print [in-order | pre-order | post-order | level-order]\n";
    std::cout << "0a. format [in-order]\n";
    std::cout << "0b. set [variable=value]\n";
    if (context.hasSet()) {
        std::cout << "0b-1. get [variable]\n";
        std::cout << "0b-2. list\n";
    }
    std::cout << "0c. history\n";
    std::cout << "0d. quit\n";
    std::cout.flush();
}

void In_Order_Initialized_State::set(
    Expression_Tree_Context& context, const std::string& key_value_pair)
{
    context.set(key_value_pair);
    context.state(new In_Order_Initialized_State);
}