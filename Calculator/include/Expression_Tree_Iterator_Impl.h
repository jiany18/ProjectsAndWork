/* Copyright G. Hemingway @ 2019, All Rights Reserved */
#ifndef TREE_ITERATOR_IMPL_H
#define TREE_ITERATOR_IMPL_H

#include "Expression_Tree.h"
#include <cstdlib>
#include <queue>
#include <stack>
#include <stdexcept>

// Solve circular include problem

template <typename T> class Refcounter;

class Component_Node;

class Expression_Tree_Iterator;

/**
 * @class Expression_Tree_Iterator_Impl
 * @brief Implementation of the Iterator pattern that is used to define
 *        the various iterations algorithms that can be performed to
 *        traverse the expression tree.
 *
 *        Plays the role of the "implementor" base class in the Bridge
 *        pattern that is used as the basis for the subclasses that
 *        actually define the various iteration algorithms.
 *
 * @see   Expression_Tree_Level_Order_Iterator_Impl,
 * Expression_Tree_In_Order_Iterator_Impl,
 *        Expression_Tree_Pre_Order_Iterator_Impl,
 * Expression_Tree_Post_Order_Iterator_Impl,
 */
class Expression_Tree_Iterator_Impl {
    friend class Expression_Tree_Iterator;

public:
    // Construct an Expression_Tree_Iterator_Impl to iterate over a tree.
    explicit Expression_Tree_Iterator_Impl(const Expression_Tree& tree);

    // Dtor.
    virtual ~Expression_Tree_Iterator_Impl() = default;

    // Dereference operator returns a reference to the item contained
    // at the current position.
    virtual Expression_Tree operator*() = 0;

    // Returns a const reference to the item contained at the current
    // position.
    virtual const Expression_Tree operator*() const = 0;

    // Increment operator (used for both pre- and post-increment).
    virtual void operator++() = 0;

    // Equality operator.
    virtual bool operator==(const Expression_Tree_Iterator_Impl& rhs) const = 0;

    // Nonequality operator.
    virtual bool operator!=(const Expression_Tree_Iterator_Impl& rhs) const = 0;

    // Method for cloning an impl. Necessary for post increments.
    virtual Expression_Tree_Iterator_Impl* clone() = 0;

    // = Necessary traits
    typedef std::forward_iterator_tag iterator_category;
    typedef int value_type;
    typedef int* pointer;
    typedef int& reference;
    typedef int difference_type;

protected:
    // The tree we are iterating over.
    const Expression_Tree& tree;
};

/**
 * @class In_Order_Expression_Tree_Iterator_Impl
 * @brief Iterates through an Expression_Tree in in-order.
 *
 *        Plays the role of the "implementor" class in the Bridge
 *        pattern that defines the in-order iteration algorithm.
 */
class In_Order_Expression_Tree_Iterator_Impl : public Expression_Tree_Iterator_Impl {
    friend class Expression_Tree_Iterator;

    friend class Refcounter<Expression_Tree_Iterator_Impl>;

public:
    // Construct an In_Order_Expression_Tree_Iterator_Impl. If end_iter
    // is set to true, the iterator points to the end of the
    // tree. Otherwise, the iterator starts with a free tree.
    explicit In_Order_Expression_Tree_Iterator_Impl(
        const Expression_Tree& tree, bool end_iter = false);

    // Dtor.
    virtual ~In_Order_Expression_Tree_Iterator_Impl() = default;

    // Dereference operator returns a reference to the item contained
    // at the current position.
    virtual Expression_Tree operator*();

    // Returns a const reference to the item contained at the current
    // position.
    virtual const Expression_Tree operator*() const;

    // Increment operator (used for both pre- and post-increment).
    virtual void operator++();

    // Equality operator.
    virtual bool operator==(const Expression_Tree_Iterator_Impl& rhs) const;

    // Nonequality operator.
    virtual bool operator!=(const Expression_Tree_Iterator_Impl& lhs) const;

    // Method for cloning an impl. Necessary for post increments.
    virtual Expression_Tree_Iterator_Impl* clone();

    // = Necessary traits
    typedef std::forward_iterator_tag iterator_category;
    typedef int value_type;
    typedef int* pointer;
    typedef int& reference;
    typedef int difference_type;

private:
    // Our current position in the iteration.
    std::stack<Expression_Tree> stack;
};

/**
 * @class Pre_Order_Expression_Tree_Iterator_Impl
 * @brief Iterates through an Expression_Tree in level-order.
 *
 *        Plays the role of the "implementor" class in the Bridge
 *        pattern that defines the level-order iteration algorithm.
 */
class Pre_Order_Expression_Tree_Iterator_Impl : public Expression_Tree_Iterator_Impl {
    friend class Expression_Tree_Iterator;

    friend class Refcounter<Expression_Tree_Iterator_Impl>;

public:
    // Construct an Level_Order_Expression_Tree_Iterator. If end_iter
    // is set to true, the iterator points to the end of the
    // tree. Otherwise, the iterator starts with a free tree.
    Pre_Order_Expression_Tree_Iterator_Impl(const Expression_Tree& tree, bool end_iter = false);

    // Dtor.
    virtual ~Pre_Order_Expression_Tree_Iterator_Impl() = default;

    // Dereference operator returns a reference to the item contained
    // at the current position.
    virtual Expression_Tree operator*();

    // Returns a const reference to the item contained at the current
    // position.
    virtual const Expression_Tree operator*() const;

    // Increment operator (used for both pre- and post-increment).
    virtual void operator++();

    // Equality operator.
    virtual bool operator==(const Expression_Tree_Iterator_Impl& rhs) const;

    // Nonequality operator.
    virtual bool operator!=(const Expression_Tree_Iterator_Impl& lhs) const;

    // Method for cloning an impl. Necessary for post increments.
    virtual Expression_Tree_Iterator_Impl* clone();

    // = Necessary traits
    typedef std::forward_iterator_tag iterator_category;
    typedef int value_type;
    typedef int* pointer;
    typedef int& reference;
    typedef int difference_type;

private:
    // Our current position in the iteration.
    std::stack<Expression_Tree> stack;
};

/**
 * @class Post_Order_Expression_Tree_Iterator_Impl
 * @brief Iterates through an Expression_Tree in post-order.
 *
 *        Plays the role of the "implementor" class in the Bridge
 *        pattern that defines the post-order iteration algorithm.
 */
class Post_Order_Expression_Tree_Iterator_Impl : public Expression_Tree_Iterator_Impl {
    friend class Expression_Tree_Iterator;

    friend class Refcounter<Expression_Tree_Iterator_Impl>;

public:
    // Construct an Post_Order_Expression_Tree_Iterator_Impl. If end_iter is set
    // to true, the iterator points to the end of the tree. Otherwise,
    // the iterator starts with a free tree.
    Post_Order_Expression_Tree_Iterator_Impl(const Expression_Tree& tree, bool end_iter = false);

    // Dtor.
    virtual ~Post_Order_Expression_Tree_Iterator_Impl() = default;

    // Dereference operator returns a reference to the item contained
    // at the current position.
    virtual Expression_Tree operator*();

    // Returns a const reference to the item contained at the current
    // position.
    virtual const Expression_Tree operator*() const;

    // Increment operator (used for both pre- and post-increment).
    virtual void operator++();

    // Equality operator.
    virtual bool operator==(const Expression_Tree_Iterator_Impl& rhs) const;

    // Nonequality operator.
    virtual bool operator!=(const Expression_Tree_Iterator_Impl& lhs) const;

    // Method for cloning an impl. Necessary for post increments.
    virtual Expression_Tree_Iterator_Impl* clone();

    // = Necessary traits
    typedef std::forward_iterator_tag iterator_category;
    typedef int value_type;
    typedef int* pointer;
    typedef int& reference;
    typedef int difference_type;

private:
    // Our current position in the iteration.
    std::stack<Expression_Tree> stack;
};

/**
 * @class Level_Order_Expression_Tree_Iterator_Impl
 * @brief Iterates through an Expression_Tree in level-order.
 *
 *        Plays the role of the "implementor" class in the Bridge
 *        pattern that defines the post-order iteration algorithm.
 */
class Level_Order_Expression_Tree_Iterator_Impl : public Expression_Tree_Iterator_Impl {
    friend class Expression_Tree_Iterator;

    friend class Refcounter<Expression_Tree_Iterator_Impl>;

public:
    // Construct an Level_Order_Expression_Tree_Iterator. If end_iter is set to
    // true, the iterator points to the end of the tree. Otherwise, the
    // iterator starts with a free tree.
    Level_Order_Expression_Tree_Iterator_Impl(const Expression_Tree& tree, bool end_iter = false);

    // Dtor.
    virtual ~Level_Order_Expression_Tree_Iterator_Impl() = default;

    // Dereference operator returns a reference to the item contained
    // at the current position.
    virtual Expression_Tree operator*();

    // Returns a const reference to the item contained at the current
    // position.
    virtual const Expression_Tree operator*() const;

    // Increment operator (used for both pre- and post-increment).
    virtual void operator++();

    // Equality operator.
    virtual bool operator==(const Expression_Tree_Iterator_Impl& rhs) const;

    // Nonequality operator.
    virtual bool operator!=(const Expression_Tree_Iterator_Impl& lhs) const;

    // Method for cloning an impl. Necessary for post increments.
    virtual Expression_Tree_Iterator_Impl* clone();

    // = Necessary traits
    typedef std::forward_iterator_tag iterator_category;
    typedef int value_type;
    typedef int* pointer;
    typedef int& reference;
    typedef int difference_type;

private:
    // Our current position in the iteration.
    // @@ Consider zapping this.
    std::queue<Expression_Tree> queue;
};

#endif // TREE_ITERATOR_IMPL_H
