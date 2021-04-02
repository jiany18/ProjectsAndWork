/* Copyright G. Hemingway @ 2019, All Rights Reserved */
#ifndef TREE_ITERATOR_IMPL_CPP
#define TREE_ITERATOR_IMPL_CPP

#include "Expression_Tree_Iterator_Impl.h"
#include "Component_Node.h"
#include "Expression_Tree.h"

// Constructor for Expression_Tree_Iterator_Impl that takes a tree
// to iterate over
Expression_Tree_Iterator_Impl::Expression_Tree_Iterator_Impl(const Expression_Tree& tree)
    : tree(tree)
{
}

// Construct an In_Order_Expression_Tree_Iterator_Impl. If end_iter is set to
// true,
// the iterator points to the end of the tree
In_Order_Expression_Tree_Iterator_Impl::In_Order_Expression_Tree_Iterator_Impl(
    const Expression_Tree& tree, bool end_iter)
    : Expression_Tree_Iterator_Impl(tree)
    , stack()
{
    // if the caller doesn't want an end iterator, insert the root tree
    // into the queue.
    if (!end_iter && !tree.is_null()) {
        stack.push(const_cast<Expression_Tree&>(tree));

        // start at the smallest element (left-most)
        while (!stack.top().left().is_null())
            stack.push(stack.top().left());
    }
}

// Returns the Node that the iterator is pointing to (non-const version)
Expression_Tree In_Order_Expression_Tree_Iterator_Impl::operator*()
{
    return stack.top();
}

// Returns the Node that the iterator is pointing to (const version)

const Expression_Tree In_Order_Expression_Tree_Iterator_Impl::operator*() const
{
    return stack.top();
}

// moves the iterator to the next node (pre-increment)

void In_Order_Expression_Tree_Iterator_Impl::operator++()
{
    // we know that at this point there is no left () of top ()
    // because we would have already visited it.

    if (!stack.empty()) {
        // if we have nodes greater than ourselves
        if (!stack.top().right().is_null()) {
            // push the right child node onto the stack
            // and pop the old parent (it's been visited now)
            auto val = stack.top();
            stack.pop();
            stack.push(val.right());

            // keep pushing until we get to the left most child
            while (!stack.top().left().is_null())
                stack.push(stack.top().left());
        } else
            stack.pop();
    }
}

// checks two iterators for equality

bool In_Order_Expression_Tree_Iterator_Impl::operator==(
    const Expression_Tree_Iterator_Impl& rhs) const
{
    auto in_order_rhs = dynamic_cast<const In_Order_Expression_Tree_Iterator_Impl*>(&rhs);

    // if the rhs was not a level_order iterator then we've already
    // discovered the relation is false.

    if (in_order_rhs) {
        // Check if the container we are iterating over has the same
        // root node and that the size of the queues are equal. The
        // latter doesn't truly determine equality. However, this is an
        // easy check for determining most inequalities and it allows us
        // to assume the queue at least has a front node (coupled with
        // the is_empty () function later).

        auto t1 = const_cast<Expression_Tree&>(tree);
        auto t2 = const_cast<Expression_Tree&>(in_order_rhs->tree);

        if (t1.get_root() == t2.get_root() && stack.size() == in_order_rhs->stack.size()) {
            // Check for both being is_empty (special condition).
            if (stack.empty() && in_order_rhs->stack.empty())
                return true;

            // check the front's node pointer. If the node pointers are
            // equal, then both iterators are pointing to the same
            // position in the tree.
            if (stack.top() == in_order_rhs->stack.top())
                return true;
        }
    }

    // either we were trying to compare a non-level order iterator or we
    // were comparing two level order iterators that were obviously at
    // different points in the tree (their queue sizes were different)

    return false;
}

// checks two iterators for inequality

bool In_Order_Expression_Tree_Iterator_Impl::operator!=(
    const Expression_Tree_Iterator_Impl& rhs) const
{
    return !(*this == rhs);
}

// Method for cloning an impl. Necessary for post increments (bridge)
// @see Expression_Tree_Iterator
Expression_Tree_Iterator_Impl* In_Order_Expression_Tree_Iterator_Impl::clone()
{
    return new In_Order_Expression_Tree_Iterator_Impl(*this);
}

// Construct an Pre_Order_Expression_Tree_Iterator. If end_iter is set to true,
// the iterator points to the end of the tree
Pre_Order_Expression_Tree_Iterator_Impl::Pre_Order_Expression_Tree_Iterator_Impl(
    const Expression_Tree& tree, bool end_iter)
    : Expression_Tree_Iterator_Impl(tree)
    , stack()
{
    // if the caller doesn't want an end iterator, insert the root tree
    // into the queue.
    if (!end_iter && !tree.is_null())
        stack.push(const_cast<Expression_Tree&>(tree));
}

// Returns the Node that the iterator is pointing to (non-const version)
Expression_Tree Pre_Order_Expression_Tree_Iterator_Impl::operator*()
{
    return stack.top();
}

// Returns the Node that the iterator is pointing to (const version)
const Expression_Tree Pre_Order_Expression_Tree_Iterator_Impl::operator*() const
{
    return stack.top();
}

// moves the iterator to the next node (pre-increment)
void Pre_Order_Expression_Tree_Iterator_Impl::operator++()
{
    // we know that at this point there is no left () of top ()
    // because we would have already visited it.

    if (!stack.empty()) {
        // we need to pop the node off the stack before pushing the
        // children, or else we'll revisit this node later

        Expression_Tree current = stack.top();
        stack.pop();

        // note the order here: right first, then left. Since this is
        // LIFO, this results in the left child being the first
        // evaluated, which fits into the Pre-order traversal strategy

        if (!current.right().is_null())
            stack.push(current.right());
        if (!current.left().is_null())
            stack.push(current.left());
    }
}

// checks two iterators for equality
bool Pre_Order_Expression_Tree_Iterator_Impl::operator==(
    const Expression_Tree_Iterator_Impl& rhs) const
{
    auto pre_order_rhs = dynamic_cast<const Pre_Order_Expression_Tree_Iterator_Impl*>(&rhs);

    // if the rhs was not a level_order iterator
    // then we've already discovered the relation is false

    if (pre_order_rhs) {
        // check if the container we are iterating over has the same
        // root node and that the size of the queues are equal. The
        // latter doesn't truly determine equality. However, this is an
        // easy check for determining most inequalities and it allows us
        // to assume the queue at least has a front node (coupled with
        // the is_empty () function later).

        auto t1 = const_cast<Expression_Tree&>(tree);
        auto t2 = const_cast<Expression_Tree&>(pre_order_rhs->tree);

        if (t1.get_root() == t2.get_root() && stack.size() == pre_order_rhs->stack.size()) {
            // check for both being is_empty (special condition)
            if (stack.empty() && pre_order_rhs->stack.empty())
                return true;

            // check the front's node pointer. If the node pointers
            // are equal, then both iterators are pointing to the same
            // position in the tree.

            if (stack.top() == pre_order_rhs->stack.top())
                return true;
        }
    }

    // either we were trying to compare a non-level order iterator or
    // we were comparing two level order iterators that were obviously
    // at different points in the tree (their queue sizes were different)

    return false;
}

// checks two iterators for inequality
bool Pre_Order_Expression_Tree_Iterator_Impl::operator!=(
    const Expression_Tree_Iterator_Impl& rhs) const
{
    return !(*this == rhs);
}

// Method for cloning an impl. Necessary for post increments (bridge)
// @see Expression_Tree_Iterator
Expression_Tree_Iterator_Impl* Pre_Order_Expression_Tree_Iterator_Impl::clone()
{
    return new Pre_Order_Expression_Tree_Iterator_Impl(*this);
}

// Construct an Post_Order_Expression_Tree_Iterator. If end_iter is set to
// true,
// the iterator points to the end of the tree
Post_Order_Expression_Tree_Iterator_Impl::Post_Order_Expression_Tree_Iterator_Impl(
    const Expression_Tree& tree, bool end_iter)
    : Expression_Tree_Iterator_Impl(tree)
    , stack()
{
    // if the caller doesn't want an end iterator, insert the root tree
    // into the queue.
    if (!end_iter && !tree.is_null()) {
        Expression_Tree current = const_cast<Expression_Tree&>(tree);
        stack.push(current);

        // the commented code does not work on unary operator nodes with no left
        // child, but
        // a right child - or at least, there is a certain depth that this will not
        // go down

        while (!current.is_null()) {
            if (!current.right().is_null())
                stack.push(current.right());
            if (!current.left().is_null()) {
                // if there was a left, then update current
                // this is the case for all non-negations
                stack.push(current.left());
                current = current.left();
            } else
                // if there was not a left, then current = current->right_
                // this handles cases of unary nodes, like negations
                current = current.right();
        }
    }
}

// Returns the Node that the iterator is pointing to (non-const version)
Expression_Tree Post_Order_Expression_Tree_Iterator_Impl::operator*()
{
    return stack.top();
}

// Returns the Node that the iterator is pointing to (const version)
const Expression_Tree Post_Order_Expression_Tree_Iterator_Impl::operator*() const
{
    return stack.top();
}

// moves the iterator to the next node (pre-increment)
void Post_Order_Expression_Tree_Iterator_Impl::operator++()
{
    // we know that at this point there is no left () of top ()
    // because we would have already visited it.

    if (!stack.empty()) {
        // we need to pop the node off the stack before pushing the
        // children, or else we'll revisit this node later

        Expression_Tree current = stack.top();
        stack.pop();

        // This is where stuff gets a little confusing.

        if (!stack.empty() && stack.top().left().get_root() != current.get_root()
            && stack.top().right().get_root() != current.get_root()) {
            current = stack.top();

            while (!current.is_null()) {
                if (!current.right().is_null())
                    stack.push(current.right());
                if (!current.left().is_null()) {
                    // if there was a left, then update current
                    // this is the case for all non-negations
                    stack.push(current.left());
                    current = current.left();
                } else {
                    // if there was not a left, then current = current->right_
                    // this handles cases of unary nodes, like negations
                    current = current.right();
                }
            }
        }
    }
}

// checks two iterators for equality

bool Post_Order_Expression_Tree_Iterator_Impl::operator==(
    const Expression_Tree_Iterator_Impl& rhs) const
{
    auto post_order_rhs = dynamic_cast<const Post_Order_Expression_Tree_Iterator_Impl*>(&rhs);

    // if the rhs was not a level_order iterator
    // then we've already discovered the relation is false

    if (post_order_rhs) {
        // check if the container we are iterating over has the same
        // root node and that the size of the queues are equal. The
        // latter doesn't truly determine equality. However, this is an
        // easy check for determining most inequalities and it allows us
        // to assume the queue at least has a front node (coupled with
        // the is_empty () function later).

        auto t1 = const_cast<Expression_Tree&>(tree);
        auto t2 = const_cast<Expression_Tree&>(post_order_rhs->tree);

        if (t1.get_root() == t2.get_root() && stack.size() == post_order_rhs->stack.size()) {
            // check for both being is_empty (special condition)
            if (stack.empty() && post_order_rhs->stack.empty())
                return true;

            // check the front's node pointer. If the node pointers are
            // equal, then both iterators are pointing to the same
            // position in the tree.

            if (stack.top() == post_order_rhs->stack.top())
                return true;
        }
    }

    // either we were trying to compare a non-level order iterator or
    // we were comparing two level order iterators that were obviously
    // at different points in the tree (their queue sizes were different)

    return false;
}

// checks two iterators for inequality
bool Post_Order_Expression_Tree_Iterator_Impl::operator!=(
    const Expression_Tree_Iterator_Impl& rhs) const
{
    return !(*this == rhs);
}

// Method for cloning an impl. Necessary for post increments (bridge)
// @see Expression_Tree_Iterator
Expression_Tree_Iterator_Impl* Post_Order_Expression_Tree_Iterator_Impl::clone()
{
    return new Post_Order_Expression_Tree_Iterator_Impl(*this);
}

// Construct an Level_Order_Expression_Tree_Iterator. If end_iter is set to
// true, the iterator points to the end of the tree

Level_Order_Expression_Tree_Iterator_Impl::Level_Order_Expression_Tree_Iterator_Impl(
    const Expression_Tree& tree, bool end_iter)
    : Expression_Tree_Iterator_Impl(tree)
    , queue()
{
    // if the caller doesn't want an end iterator, insert the root tree
    // into the queue.
    if (!end_iter && !tree.is_null())
        queue.push(const_cast<Expression_Tree&>(tree));
}

// Returns the Node that the iterator is pointing to (non-const version)
Expression_Tree Level_Order_Expression_Tree_Iterator_Impl::operator*()
{
    return queue.front();
}

// Returns the Node that the iterator is pointing to (const version)
const Expression_Tree Level_Order_Expression_Tree_Iterator_Impl::operator*() const
{
    return queue.front();
}

// moves the iterator to the next node (pre-increment)
void Level_Order_Expression_Tree_Iterator_Impl::operator++()
{
    if (!queue.empty()) {
        // If the queue is not empty, dequeue an element
        Expression_Tree root = queue.front();
        queue.pop();

        if (!root.is_null()) {
            // If the element wasn't null, enqueue its children
            if (!root.left().is_null())
                queue.push(root.left());
            if (!root.right().is_null())
                queue.push(root.right());
        }
    }
}

// checks two iterators for equality
bool Level_Order_Expression_Tree_Iterator_Impl::operator==(
    const Expression_Tree_Iterator_Impl& rhs) const
{
    auto level_order_rhs = dynamic_cast<const Level_Order_Expression_Tree_Iterator_Impl*>(&rhs);

    // if the rhs was not a level_order iterator then we've already
    // discovered the relation is false.

    if (level_order_rhs) {
        // check if the container we are iterating over has the same
        // root node and that the size of the queues are equal. The
        // latter doesn't truly determine equality. However, this is an
        // easy check for determining most inequalities and it allows us
        // to assume the queue at least has a front node (coupled with
        // the is_empty () function later).

        auto t1 = const_cast<Expression_Tree&>(tree);
        auto t2 = const_cast<Expression_Tree&>(level_order_rhs->tree);

        if (t1.get_root() == t2.get_root() && queue.size() == level_order_rhs->queue.size()) {
            // check for both being is_empty (special condition)
            if (queue.empty() && level_order_rhs->queue.empty())
                return true;

            // check the front's node pointer. If the node pointers
            // are equal, then both iterators are pointing to the same
            // position in the tree.

            if (queue.front() == level_order_rhs->queue.front())
                return true;
        }
    }

    // either we were trying to compare a non-level order iterator or we
    // were comparing two level order iterators that were obviously at
    // different points in the tree (their queue sizes were different)

    return false;
}

// checks two iterators for inequality
bool Level_Order_Expression_Tree_Iterator_Impl::operator!=(
    const Expression_Tree_Iterator_Impl& rhs) const
{
    return !(*this == rhs);
}

// Method for cloning an impl. Necessary for post increments (bridge)
// @see Expression_Tree_Iterator
Expression_Tree_Iterator_Impl* Level_Order_Expression_Tree_Iterator_Impl::clone()
{
    return new Level_Order_Expression_Tree_Iterator_Impl(*this);
}

#endif // TREE_ITERATOR_IMPL_CPP
