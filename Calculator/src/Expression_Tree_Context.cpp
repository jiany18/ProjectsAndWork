// Author: Yumeng Jiang
// VUnetid: jiany18
// Email: yumeng.jiang@vanderbilt.edu
// Class: CS3251
// Date: 11/20/2019
// Honor statement: I have neither given nor received any unauthorized aid on this assignment.
// Assignment Number: Project #7

In the Expression Tree application a user is in succinct mode.  Imagine the moment the user hits enter on the keyboard after typing in a valid expression.  In what order would we encounter the following patterns while executing from that moment until when the program prints out the correct answer: #include "Expression_Tree_Context.h"
#include <cstdlib>

Expression_Tree_Context::Expression_Tree_Context()
    : treeState(new Uninitialized_State)
    , isFormatted(false)
    , isSet(false)
{
}

void Expression_Tree_Context::format(const std::string& new_format)
{
    treeState->format(*this, new_format);
    // we only reach here if the format was successful, and once
    // this is set, we will always have at least one valid format
    // because we store the last valid command (which would have
    // a valid format, if this format function has ever been successfully ran
    isFormatted = true;
}

void Expression_Tree_Context::make_tree(const std::string& expression)
{
    treeState->make_tree(*this, expression);
}

void Expression_Tree_Context::print(const std::string& format)
{
    treeState->print(*this, format);
}

void Expression_Tree_Context::evaluate(const std::string& format)
{
    treeState->evaluate(*this, format);
}

void Expression_Tree_Context::make_set(const std::string& expression)
{
    treeState->set(*this, expression);
    isSet = true;
}

void Expression_Tree_Context::set(const std::string& key_value_pair)
{
    // this is quite enough function calls.
    // we'll just go ahead and parse all of this now.
    std::string input = key_value_pair;
    // used for keeping track of found characters
    std::string::size_type pos;
    // get rid of all spaces
    while ((pos = input.find(' ')) != std::string::npos)
        input.erase(pos, 1);

    // input should be key=value
    if ((pos = input.find('=')) != std::string::npos) {
        // if the position is not the first char (eg '=value')
        // and position is not the last char (eg 'key=')
        // then split the string and set the interpreter context
        // accordingly
        if (pos != 0 && pos < input.length() - 1) {
            std::string key = input.substr(0, pos);
            std::string value = input.substr(pos + 1);

            int_context.set(key, atoi(value.c_str()));
        } else
            throw std::domain_error("Must be in the form key=value");
    } else
        throw std::domain_error("Must have = sign present");
}

void Expression_Tree_Context::get(const std::string& val)
{
    try {
        if (int_context.get(val) == 0) {
            throw std::domain_error("Error: unknown variable \"" + val + "\"\n");
        } else
            std::cout << val << ": " << int_context.get(val) << std::endl;
    } catch (std::domain_error&) {
        std::cout << "Error: unknown variable \"" << val << "\"" << std::endl;
    }
}

void Expression_Tree_Context::list()
{
    int_context.print();
}

void Expression_Tree_Context::history()
{
    LQueue<std::string> tmp(commands);
    while (tmp.size() > 5) {
        tmp.dequeue();
    }

    int size = tmp.size();
    for (int i = 1; i <= size; i++) {
        std::cout << i << ") " << tmp.dequeue() << std::endl;
    }
}

void Expression_Tree_Context::addToCommands(const std::string& input)
{
    commands.enqueue(input);
}

Expression_Tree_State* Expression_Tree_Context::state() const
{
    return treeState.get();
}

void Expression_Tree_Context::state(Expression_Tree_State* state)
{
    treeState.reset(state);
}

Expression_Tree& Expression_Tree_Context::tree()
{
    return expTree;
}

void Expression_Tree_Context::tree(const Expression_Tree& tree)
{
    expTree = tree;
}
