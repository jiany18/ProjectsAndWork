// Author: Yumeng Jiang
// VUnetid: jiany18
// Email: yumeng.jiang@vanderbilt.edu
// Class: CS3251
// Date: 11/20/2019
// Honor statement: I have neither given nor received any unauthorized aid on this assignment.
// Assignment Number: Project #7

#ifndef TREE_CONTEXT_H
#define TREE_CONTEXT_H

#include <memory>
#include <string>

#include "Expression_Tree.h"
#include "Expression_Tree_State.h"
#include "Interpreter.h"
#include "LQueue.h"

/**
 * @class Expression_Tree_Context
 * @brief Interface for the State pattern that is used to ensure that
 *        commands are invoked according to the correct protocol.
 *
 *        Plays the role of the "context" class in the State pattern
 *        and delegates to the appropriate "state" subclass that
 *        performs the commands.
 */
class Expression_Tree_Context {
public:
    // Constructor.
    Expression_Tree_Context();

    // Set the desired format to the designated new_format.
    void format(const std::string& new_format);

    // Make an expression tree based on the designated expression
    // using the previously designated format.
    void make_tree(const std::string& expression);

    // Print the most recently created expression tree using the
    // designated format.
    void print(const std::string& format);

    // Evaluate the "yield" of the most recently created expression
    // tree using the designated format.
    void evaluate(const std::string& format);

    void make_set(const std::string& expression);

    // Set the value of the variable
    // is a string of the format "variable_name=variable_value"
    void set(const std::string& key_value_pair);

    void get(const std::string& var);

    void list();

    void history();

    // Return a pointer to the current Expression_Tree_State.
    Expression_Tree_State* state() const;

    // Set the current Expression_Tree_State to the designated @a
    // new_state pointer.
    void state(Expression_Tree_State* new_state);

    // Return a reference to the current Expression_Tree.
    Expression_Tree& tree();

    // Set the current Expression_Tree to the new_tree.
    void tree(const Expression_Tree& new_tree);

    // Returns whether or not a successful format call has been called
    bool formatted()
    {
        return isFormatted;
    }

    void addToCommands(const std::string& input);

    // Persistent interpreter context for variables. Our interpreter
    // will change values insilde of this, so I just stuck the variable
    // in the public section.
    Interpreter_Context int_context;

    bool hasSet()
    {
        return isSet;
    }

private:
    // Keep track of the current state that we're in.  We use an @a
    // std::unique_ptr to simplify memory management and avoid memory leaks.
    std::unique_ptr<Expression_Tree_State> treeState;
    // Current expression tree.
    Expression_Tree expTree;
    bool isFormatted;
    bool isSet;
    LQueue<std::string> commands;
};

#endif // TREE_CONTEXT
