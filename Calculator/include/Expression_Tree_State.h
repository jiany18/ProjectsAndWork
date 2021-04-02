// Author: Yumeng Jiang
// VUnetid: jiany18
// Email: yumeng.jiang@vanderbilt.edu
// Class: CS3251
// Date: 11/20/2019
// Honor statement: I have neither given nor received any unauthorized aid on this assignment.
// Assignment Number: Project #7

#ifndef TREE_STATE_H
#define TREE_STATE_H

#include <iostream>
#include <map>
#include <string>

// Forward declaration.
class Expression_Tree_Context;

/**
 * @class Expression_Tree_State
 * @brief Implementation of the State pattern that is used to define
 *        the various states that affect how users operations are
 *        processed.
 *
 *        Plays the role of the "state" base class in the State
 *        pattern that is used as the basis for the subclasses that
 *        actually define the various user operation states.
 */
class Expression_Tree_State {
public:
    // Exception class for Invalid States exceptions
    class Invalid_State : public std::domain_error {
    public:
        explicit Invalid_State(const std::string& message)
            : std::domain_error(message)
        {
        }
    };

    virtual ~Expression_Tree_State() = default;

    // Set the desired format to the designated new_format, updating
    // the state of the @context accordingly.
    virtual void format(Expression_Tree_Context& context, const std::string& new_format) = 0;

    // Make an expression tree based on the designated expression
    // using the previously designated format, updating the state of
    // the @context accordingly.
    virtual void make_tree(Expression_Tree_Context& context, const std::string& expression) = 0;

    // Print the most recently created expression tree using the
    // designated format, updating the state of the @context
    // accordingly.
    virtual void print(Expression_Tree_Context& context, const std::string& format) = 0;

    // Evaluate the "yield" of the most recently created expression
    // tree using the designated format, updating the state of the
    // @context accordingly.
    virtual void evaluate(Expression_Tree_Context& context, const std::string& format) = 0;

    virtual void set(Expression_Tree_Context& context, const std::string& key_value_pair) = 0;

    virtual void print_valid_commands(Expression_Tree_Context& context) const = 0;

    // Print the operators and operands of the tree to the @os in
    // the designated traversal_order.
    static void print_tree(
        const Expression_Tree& tree, const std::string& traversal_order, std::ostream& os);

    // Evaluate and print the yield of the tree to the @os in the
    // designated traversal_order.
    static void evaluate_tree(
        const Expression_Tree& tree, const std::string& traversal_order, std::ostream& os);
};

/**
 * @class Uninitialized_State
 * @brief This is the initial state, which only allows format commands.
 */
class Uninitialized_State : public Expression_Tree_State {
public:
    // Sets the context's state to correspond to the designated @a
    // new_format, e.g., Pre_Order_Uninitialized_State, @a
    // Post_Order_Uninitialized_State, In_Order_Uninitialized_State,
    // or Level_Order_Uninitialized_State.
    void format(Expression_Tree_Context& context, const std::string& new_format) override;

    // Make an expression tree based on the designated expression
    // using the previously designated format, updating the state of
    // the @context accordingly.
    void make_tree(Expression_Tree_Context&, const std::string&) override;

    // Print the most recently created expression tree using the
    // designated format, updating the state of the @context
    // accordingly.
    void print(Expression_Tree_Context& context, const std::string& format) override;

    // Evaluate the "yield" of the most recently created expression
    // tree using the designated format, updating the state of the
    // @context accordingly.
    void evaluate(Expression_Tree_Context& context, const std::string& format) override;

    void set(Expression_Tree_Context& context, const std::string& key_value_pair) override;

    // Print the list of valid command if the user is in this state
    void print_valid_commands(Expression_Tree_Context& context) const override;

private:
    /**
     * @class Uninitialized_State_Factory
     * @brief Implementation of a factory pattern that dynamically
     *        allocates the appropriate Expression_Tree_State object.
     *
     *        This is a variant of the Abstract Factory pattern that has a
     *        set of related factory methods but which doesn't use
     *        inheritance.
     */
    class Uninitialized_State_Factory {
    public:
        // Constructor.
        Uninitialized_State_Factory();

        // Dynamically allocate a new Expression_Tree_State
        // object based on the designated traversal_order and end_iter.
        static Expression_Tree_State* make_uninitialized_state(const std::string& format);

    private:
        // Dynamically allocate a new Level_Order_Uninitialized_State
        // object based on the designated end_iter.
        static Expression_Tree_State* make_in_order_uninitialized_state();

        // Dynamically allocate a new Pre_Order_Uninitialized_State
        // object based on the designated end_iter.
        static Expression_Tree_State* make_pre_order_uninitialized_state();

        // Dynamically allocate a new Post_Order_Uninitialized_State
        // object based on the designated end_iter.
        static Expression_Tree_State* make_post_order_uninitialized_state();

        // Dynamically allocate a new Level_Order_Uninitialized_State
        // object based on the designated end_iter.
        static Expression_Tree_State* make_level_order_uninitialized_state();

        typedef Expression_Tree_State* (*UNINITIALIZED_STATE_PTF)();

        typedef std::map<std::string, UNINITIALIZED_STATE_PTF> UNINITIALIZED_STATE_MAP;

        static UNINITIALIZED_STATE_MAP uninitialized_state_map;
    };

    static Uninitialized_State_Factory uninitialized_state_factory;
};

/**
 * @class Pre_Order_Uninitialized_State
 * @brief Now that the user has provided a format command
 *        indicating "pre-order" input this state will interpret the
 *        subsequent expression accordingly.
 */
class Pre_Order_Uninitialized_State : public Uninitialized_State {
public:
    // Process the expression using a pre-order interpreter and
    // update the state of the context to the @a
    // Pre_Order_Initialized_State.
    void make_tree(Expression_Tree_Context& context, const std::string& expression) override;
};

/**
 * @class Pre_Order_Initialized_State
 * @brief Now that the user has provided a expr command this state
 *        will print or evaluate the subsequent expression
 *        accordingly.
 */
class Pre_Order_Initialized_State : public Pre_Order_Uninitialized_State {
public:
    // Print the current expression tree in the context using the
    // designed format.
    void print(Expression_Tree_Context& context, const std::string& format) override;

    // Evaluate the yield of the current expression tree in the @a
    // context using the designed format.
    void evaluate(Expression_Tree_Context& context, const std::string& format) override;
};

/**
 * @class Post_Order_Uninitialized_State
 * @brief Now that the user has provided a format command
 *        indicating "post-order" input this state will interpret the
 *        subsequent expression accordingly.
 */
class Post_Order_Uninitialized_State : public Uninitialized_State {
public:
    // Process the expression using a post-order interpreter and
    // update the state of the context to the @a
    // Post_Order_Initialized_State.
    void make_tree(Expression_Tree_Context& context, const std::string& expression) override;
};

/**
 * @class Post_Order_Initialized_State
 * @brief Now that the user has provided a expr command this state
 *        will print or evaluate the subsequent expression
 *        accordingly.
 */
class Post_Order_Initialized_State : public Post_Order_Uninitialized_State {
public:
    // Print the current expression tree in the context using the
    // designed format.
    void print(Expression_Tree_Context& context, const std::string& format) override;

    // Evaluate the yield of the current expression tree in the @a
    // context using the designed format.
    void evaluate(Expression_Tree_Context& context, const std::string& format) override;
};

/**
 * @class In_Order_Uninitialized_State
 * @brief Now that the user has provided a format command
 *        indicating "in-order" input this state will interpret the
 *        subsequent expression accordingly.
 */
class In_Order_Uninitialized_State : public Uninitialized_State {
public:
    // Process the expression using an in-order interpreter and
    // update the state of the context to the @a
    // In_Order_Initialized_State.
    void make_tree(Expression_Tree_Context& context, const std::string& expression) override;

    void set(Expression_Tree_Context& context, const std::string& key_value_pair) override;

    // Print the list of valid command if the user is in this state
    void print_valid_commands(Expression_Tree_Context& context) const override;
};

/**
 * @class In_Order_Initialized_State
 * @brief Now that the user has provided a expr command this state
 *        will print or evaluate the subsequent expression
 *        accordingly.
 */
class In_Order_Initialized_State : public In_Order_Uninitialized_State {
public:
    // Print the current expression tree in the context using the
    // designed format.
    void print(Expression_Tree_Context& context, const std::string& format) override;

    // Evaluate the yield of the current expression tree in the @a
    // context using the designed format.
    void evaluate(Expression_Tree_Context& context, const std::string& format) override;

    void set(Expression_Tree_Context& context, const std::string& key_value_pair) override;

    // Print the list of valid command if the user is in this state
    void print_valid_commands(Expression_Tree_Context& context) const override;
};

/**
 * @class Level_Order_Uninitialized_State
 * @brief Now that the user has provided a format command
 *        indicating "level-order" input this state will interpret the
 *        subsequent expression accordingly.
 */
class Level_Order_Uninitialized_State : public Uninitialized_State {
public:
    // Process the expression using a level-order interpreter and
    // update the state of the context to the @a
    // Level_Order_Initialized_State.
    void make_tree(Expression_Tree_Context& context, const std::string& expression) override;
};

/**
 * @class Level_Order_Initialized_State
 * @brief Now that the user has provided a expr command this state
 *        will print or evaluate the subsequent expression
 *        accordingly.
 */
class Level_Order_Initialized_State : public Level_Order_Uninitialized_State {
public:
    // Print the current expression tree in the context using the
    // designed format.
    void print(Expression_Tree_Context& context, const std::string& format) override;

    // Evaluate the yield of the current expression tree in the @a
    // context using the designed format.
    void evaluate(Expression_Tree_Context& context, const std::string& format) override;
};

#endif // TREE_STATE_H