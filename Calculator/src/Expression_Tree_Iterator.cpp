/* Copyright G. Hemingway @ 2019, All Rights Reserved */
#ifndef TREE_ITERATOR_CPP
#define TREE_ITERATOR_CPP

#include <algorithm>
#include <cstdlib>
#include <sstream>

#include "Expression_Tree.h"
#include "Expression_Tree_Iterator.h"
#include "Refcounter.h"

// Copy ctor - needed for reference counting.

Expression_Tree_Iterator::Expression_Tree_Iterator(const Expression_Tree_Iterator& ts)
    : impl(ts.impl)
{
}

// Copy ctor for impl *.

Expression_Tree_Iterator::Expression_Tree_Iterator(Expression_Tree_Iterator_Impl* impl)
    : impl(impl)
{
}

// Dereference operator returns a reference to the item contained at
// the current position.

Expression_Tree Expression_Tree_Iterator::operator*()
{
    // return impl_->operator* ();
    return *(*impl);
}

// Returns a const reference to the item contained at the current position

const Expression_Tree Expression_Tree_Iterator::operator*() const
{
    // return impl_->operator* ();
    return *(*impl);
}

// Pre-increment operator

Expression_Tree_Iterator& Expression_Tree_Iterator::operator++()
{
    // return impl_->operator++ ();
    ++(*impl);
    return *this;
}

// Post-increment operator

Expression_Tree_Iterator Expression_Tree_Iterator::operator++(int)
{
    Expression_Tree_Iterator temp(impl->clone());
    // return impl_->operator++ ();
    ++(*impl);
    return temp;
}

// Equality operator

bool Expression_Tree_Iterator::operator==(const Expression_Tree_Iterator& rhs)
{
    // return impl_->operator () == rhs.impl_->operator ();
    return *impl == *rhs.impl;
}

// Nonequality operator

bool Expression_Tree_Iterator::operator!=(const Expression_Tree_Iterator& rhs)
{
    return !(*this == rhs);
}

// Copy ctor - needed for reference counting.

Expression_Tree_Const_Iterator::Expression_Tree_Const_Iterator(
    const Expression_Tree_Const_Iterator& ts)
    : impl(ts.impl)
{
}

// Copy ctor for impl *.

Expression_Tree_Const_Iterator::Expression_Tree_Const_Iterator(Expression_Tree_Iterator_Impl* impl)
    : impl(impl)
{
}

// Returns a const reference to the item contained at the current position

const Expression_Tree Expression_Tree_Const_Iterator::operator*() const
{
    return *(*impl);
}

// Preincrement operator

Expression_Tree_Const_Iterator& Expression_Tree_Const_Iterator::operator++()
{
    ++(*impl);
    return *this;
}

// Preincrement operator

Expression_Tree_Const_Iterator Expression_Tree_Const_Iterator::operator++(int)
{
    Expression_Tree_Const_Iterator temp(impl.get_ptr()->clone());
    ++(*impl);
    return *this;
}

// Equality operator

bool Expression_Tree_Const_Iterator::operator==(const Expression_Tree_Const_Iterator& rhs)
{
    return impl->operator==(*rhs.impl);
}

// Nonequality operator

bool Expression_Tree_Const_Iterator::operator!=(const Expression_Tree_Const_Iterator& rhs)
{
    return !(*this == rhs);
}

#endif // TREE_ITERATOR_CPP
