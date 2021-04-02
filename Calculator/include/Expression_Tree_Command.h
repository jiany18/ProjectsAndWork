/* Copyright G. Hemingway @ 2019, All Rights Reserved */
#ifndef EXPRESSION_TREE_COMMAND_H
#define EXPRESSION_TREE_COMMAND_H

#include "Expression_Tree_Command_Impl.h"
#include "Refcounter.h"

/**
 * @class Expression_Tree_Command
 * @brief Interface for the Command pattern to define a command that
 *        performs an operation on the expression tree when executed.
 *
 *        Plays the role of the "abstraction" class in the Bridge
 *        pattern and delegates to the appropriate "implementor"
 *        class that performs the command.
 */
class Expression_Tree_Command {
public:
    // Constructor that assumes ownership of an Expression_Tree_Command_Impl
    // *.
    explicit Expression_Tree_Command(Expression_Tree_Command_Impl* = nullptr);

    // Copy ctor.
    Expression_Tree_Command(const Expression_Tree_Command&);

    // Assignment operator
    Expression_Tree_Command& operator=(const Expression_Tree_Command&);

    // Dtor.
    ~Expression_Tree_Command() = default;

    // Runs the command.
    bool execute();

private:
    // Pointer to actual implementation, i.e., the "bridge", which is
    // reference counted to automate memory management.
    Refcounter<Expression_Tree_Command_Impl> command_impl;
};

#endif // EXPRESSION_TREE_COMMAND_H
