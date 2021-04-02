/* Copyright G. Hemingway @ 2019, All Rights Reserved */
#ifndef EXPRESSION_TREE_COMMAND_CPP
#define EXPRESSION_TREE_COMMAND_CPP

#include "Expression_Tree_Command.h"

Expression_Tree_Command::Expression_Tree_Command(Expression_Tree_Command_Impl* impl)
    : command_impl(impl)
{
}

Expression_Tree_Command::Expression_Tree_Command(const Expression_Tree_Command& rhs)
    : command_impl(rhs.command_impl)
{
}

Expression_Tree_Command& Expression_Tree_Command::operator=(const Expression_Tree_Command& rhs)
{
    // check for self assignment first
    if (this != &rhs)
        // we just make use of the Refcounter functionality here
        command_impl = rhs.command_impl;
    return *this;
}

bool Expression_Tree_Command::execute()
{
    return command_impl->execute();
}

#endif // EXPRESSION_TREE_COMMAND_CPP
