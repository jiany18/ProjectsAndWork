/* Copyright G. Hemingway @ 2019, All Rights Reserved */
#ifndef EXPRESSION_TREE_CPP
#define EXPRESSION_TREE_CPP

#include <algorithm>
#include <cstdlib>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Component_Node.h"
#include "Expression_Tree.h"
#include "Expression_Tree_Iterator.h"
#include "Expression_Tree_Iterator_Impl.h"

/**
 * @class Expression_Tree_Iterator_Factory
 * @brief Implementation of a factory pattern that dynamically allocates
 *        the appropriate Expression_Tree_Iterator_Impl object.
 *
 *        This is a variant of the Abstract Factory pattern that
 *        has a set of related factory methods but which doesn't use
 *        inheritance.
 *
 * @see   Expression_Tree_Level_Order_Iterator_Impl,
 *        Expression_Tree_In_Order_Iterator_Impl,
 *        Expression_Tree_Pre_Order_Iterator_Impl, and
 *        Expression_Tree_Post_Order_Iterator_Impl.
 */
class Expression_Tree_Iterator_Factory {
public:
    // Constructor.
    Expression_Tree_Iterator_Factory();

    // Dynamically allocate a new Expression_Tree_Iterator_Impl
    // object based on the designated traversal_order and end_iter.
    Expression_Tree_Iterator_Impl* make_tree_iterator(
        Expression_Tree& tree, const std::string& traversal_order, bool end_iter);

private:
    // Dynamically allocate a new Expression_Tree_Level_Order_Iterator_Impl
    // object based on the designated end_iter.
    Expression_Tree_Iterator_Impl* make_in_order_tree_iterator(
        Expression_Tree& tree, bool end_iter);

    // Dynamically allocate a new Expression_Tree_Pre_Order_Iterator_Impl
    // object based on the designated end_iter.
    Expression_Tree_Iterator_Impl* make_pre_order_tree_iterator(
        Expression_Tree& tree, bool end_iter);

    // Dynamically allocate a new Expression_Tree_Post_Order_Iterator_Impl
    // object based on the designated end_iter.
    Expression_Tree_Iterator_Impl* make_post_order_tree_iterator(
        Expression_Tree& tree, bool end_iter);

    // Dynamically allocate a new Expression_Tree_Level_Order_Iterator_Impl
    // object based on the designated end_iter.
    Expression_Tree_Iterator_Impl* make_level_order_tree_iterator(
        Expression_Tree& tree, bool end_iter);

    typedef Expression_Tree_Iterator_Impl* (Expression_Tree_Iterator_Factory::*TRAVERSAL_PTMF)(
        Expression_Tree& tree, bool end_iter);

    typedef std::map<std::string, TRAVERSAL_PTMF> TRAVERSAL_MAP;

    TRAVERSAL_MAP traversal_map;
};

Expression_Tree_Iterator_Factory::Expression_Tree_Iterator_Factory()
{
    traversal_map["in-order"] = &Expression_Tree_Iterator_Factory::make_in_order_tree_iterator;
    traversal_map["pre-order"] = &Expression_Tree_Iterator_Factory::make_pre_order_tree_iterator;
    traversal_map["post-order"] = &Expression_Tree_Iterator_Factory::make_post_order_tree_iterator;
    traversal_map["level-order"]
        = &Expression_Tree_Iterator_Factory::make_level_order_tree_iterator;
}

Expression_Tree_Iterator_Impl* Expression_Tree_Iterator_Factory::make_level_order_tree_iterator(
    Expression_Tree& tree, bool end_iter)
{
    return new Level_Order_Expression_Tree_Iterator_Impl(tree, end_iter);
}

Expression_Tree_Iterator_Impl* Expression_Tree_Iterator_Factory::make_in_order_tree_iterator(
    Expression_Tree& tree, bool end_iter)
{
    return new In_Order_Expression_Tree_Iterator_Impl(tree, end_iter);
}

Expression_Tree_Iterator_Impl* Expression_Tree_Iterator_Factory::make_pre_order_tree_iterator(
    Expression_Tree& tree, bool end_iter)
{
    return new Pre_Order_Expression_Tree_Iterator_Impl(tree, end_iter);
}

Expression_Tree_Iterator_Impl* Expression_Tree_Iterator_Factory::make_post_order_tree_iterator(
    Expression_Tree& tree, bool end_iter)
{
    return new Post_Order_Expression_Tree_Iterator_Impl(tree, end_iter);
}

Expression_Tree_Iterator_Impl* Expression_Tree_Iterator_Factory::make_tree_iterator(
    Expression_Tree& tree, const std::string& traversal_order, bool end_iter)
{
    auto iter = traversal_map.find(traversal_order);
    if (iter == traversal_map.end()) {
        // We don't understand the type. Convert the type to a string
        // and pass it back via an exception

        throw Expression_Tree::Invalid_Iterator(traversal_order);
    } else {
        Expression_Tree_Iterator_Factory::TRAVERSAL_PTMF ptmf = iter->second;
        return (this->*ptmf)(tree, end_iter);
    }
}

// Define a single instance of a factory that's local to this class.
static Expression_Tree_Iterator_Factory tree_iterator_factory;

// Default ctor
Expression_Tree::Expression_Tree()
    : root(nullptr)
{
}

// Ctor take an underlying NODE*.
Expression_Tree::Expression_Tree(Component_Node* inRoot, bool increase_count)
    : root(inRoot, increase_count)
{
}

// Copy ctor
Expression_Tree::Expression_Tree(const Expression_Tree& t)
    : root(t.root)
{
}

// Assignment operator
Expression_Tree& Expression_Tree::operator=(const Expression_Tree& t)
{
    // Refcounter class takes care of the internal decrements and
    // increments.
    if (this != &t)
        root = t.root;
    return *this;
}

bool Expression_Tree::operator==(const Expression_Tree& rhs) const
{
    return root.get_ptr() == rhs.root.get_ptr();
}

// Check if the tree is empty.
bool Expression_Tree::is_null() const
{
    return root.get_ptr() == nullptr;
}

// return root pointer
Component_Node* Expression_Tree::get_root()
{
    return root.get_ptr();
}

// Return the stored item.
int Expression_Tree::item() const
{
    return root->item();
}

// Return the left branch.
Expression_Tree Expression_Tree::left()
{
    return Expression_Tree(root->left(), true);
}

// Return the left branch.
Expression_Tree Expression_Tree::right()
{
    return Expression_Tree(root->right(), true);
}

// Return a begin iterator of a specified type.
Expression_Tree::iterator Expression_Tree::begin(const std::string& traversal_order)
{
    /*
    if (traversal_order == "Pre-order")
      return Express_Tree_Iterator (new Pre_Order_Tree_Iterator_Impl (*this,
    false));
    else if (traversal_order == "Level-order")
      return Express_Tree_Iterator (new Level_Order_Tree_Iterator_Impl (*this,
    false));
    */
    return Expression_Tree::iterator(
        tree_iterator_factory.make_tree_iterator(*this, traversal_order, false));
}

// Return an end iterator of a specified type.
Expression_Tree::iterator Expression_Tree::end(const std::string& traversal_order)
{
    return Expression_Tree::iterator(
        tree_iterator_factory.make_tree_iterator(*this, traversal_order, true));
}

// Return a begin iterator of a specified type.
Expression_Tree_Const_Iterator Expression_Tree::begin(const std::string& traversal_order) const
{
    Expression_Tree* non_const_this = const_cast<Expression_Tree*>(this);
    return Expression_Tree::const_iterator(
        tree_iterator_factory.make_tree_iterator(*non_const_this, traversal_order, false));
}

// Return an end iterator of a specified type.
Expression_Tree_Const_Iterator Expression_Tree::end(const std::string& traversal_order) const
{
    Expression_Tree* non_const_this = const_cast<Expression_Tree*>(this);
    return Expression_Tree::const_iterator(
        tree_iterator_factory.make_tree_iterator(*non_const_this, traversal_order, true));
}

// Accept a visitor to perform some action on the Expression_Tree.
void Expression_Tree::accept(Visitor& visitor) const
{
    root->accept(visitor);
}

#endif // EXPRESSION_TREE_CPP
