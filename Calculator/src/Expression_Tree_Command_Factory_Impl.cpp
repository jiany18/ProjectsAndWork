/* Copyright G. Hemingway @ 2019, All Rights Reserved */
#ifndef COMMAND_FACTORY_IMPL_CPP
#define COMMAND_FACTORY_IMPL_CPP

#include "Expression_Tree_Command_Factory_Impl.h"
#include <sstream>

// Expression_Tree_Command_Factory_Impl Constructor.
Expression_Tree_Command_Factory_Impl::Expression_Tree_Command_Factory_Impl()
{
}

Expression_Tree_Command Concrete_Expression_Tree_Command_Factory_Impl::make_format_command(
    const std::string& param)
{
    return Expression_Tree_Command(new Format_Command(tree_context, param));
}

Expression_Tree_Command Concrete_Expression_Tree_Command_Factory_Impl::make_expr_command(
    const std::string& param)
{
    return Expression_Tree_Command(new Expr_Command(tree_context, param));
}

Expression_Tree_Command Concrete_Expression_Tree_Command_Factory_Impl::make_print_command(
    const std::string& param)
{
    return Expression_Tree_Command(new Print_Command(tree_context, param));
}

Expression_Tree_Command Concrete_Expression_Tree_Command_Factory_Impl::make_eval_command(
    const std::string& param)
{
    return Expression_Tree_Command(new Eval_Command(tree_context, param));
}

Expression_Tree_Command Concrete_Expression_Tree_Command_Factory_Impl::make_set_command(
    const std::string& param)
{
    return Expression_Tree_Command(new Set_Command(tree_context, param));
}

Expression_Tree_Command Concrete_Expression_Tree_Command_Factory_Impl::make_get_command(
    const std::string& param)
{
    return Expression_Tree_Command(new Get_Command(tree_context, param));
}

Expression_Tree_Command Concrete_Expression_Tree_Command_Factory_Impl::make_list_command(
    const std::string& param)
{
    return Expression_Tree_Command(new List_Command(tree_context));
}

Expression_Tree_Command Concrete_Expression_Tree_Command_Factory_Impl::make_history_command(
    const std::string& param)
{
    return Expression_Tree_Command(new History_Command(tree_context));
}

Expression_Tree_Command Concrete_Expression_Tree_Command_Factory_Impl::make_quit_command(
    const std::string&)
{
    return Expression_Tree_Command(new Quit_Command(tree_context));
}

Expression_Tree_Command Concrete_Expression_Tree_Command_Factory_Impl::make_macro_command(
    const std::string& expr_string)
{
    std::vector<Expression_Tree_Command> macro_commands;
    macro_commands.push_back(this->make_format_command("in-order"));
    macro_commands.push_back(this->make_expr_command(expr_string));
    macro_commands.push_back(this->make_eval_command("post-order"));

    return Expression_Tree_Command(new Macro_Command(tree_context, macro_commands));
}

// Constructor.
Concrete_Expression_Tree_Command_Factory_Impl::Concrete_Expression_Tree_Command_Factory_Impl(
    Expression_Tree_Context& tree_context)
    : command_map()
    , tree_context(tree_context)
{
    command_map["format"] = &Expression_Tree_Command_Factory_Impl::make_format_command;
    command_map["expr"] = &Expression_Tree_Command_Factory_Impl::make_expr_command;
    command_map["print"] = &Expression_Tree_Command_Factory_Impl::make_print_command;
    command_map["eval"] = &Expression_Tree_Command_Factory_Impl::make_eval_command;
    command_map["set"] = &Expression_Tree_Command_Factory_Impl::make_set_command;
    command_map["get"] = &Expression_Tree_Command_Factory_Impl::make_get_command;
    command_map["list"] = &Expression_Tree_Command_Factory_Impl::make_list_command;
    command_map["history"] = &Expression_Tree_Command_Factory_Impl::make_history_command;
    command_map["quit"] = &Expression_Tree_Command_Factory_Impl::make_quit_command;
}

Expression_Tree_Command Concrete_Expression_Tree_Command_Factory_Impl::make_command(
    const std::string& input)
{
    // separate the command from the parameters
    std::string::size_type space_pos = input.find(' ');
    std::string parameters = input.substr(space_pos + 1);
    std::string command_keyword = input.substr(0, space_pos);
    std::transform(command_keyword.begin(), command_keyword.end(), command_keyword.begin(),
        [](unsigned char c) { return std::tolower(c); });

    auto iter = command_map.find(command_keyword);
    if (iter == command_map.end())
        return Concrete_Expression_Tree_Command_Factory_Impl::make_quit_command(parameters);
    else {
        Concrete_Expression_Tree_Command_Factory_Impl::FACTORY_PTMF ptmf = iter->second;
        return (this->*ptmf)(parameters);
    }
}

#endif // COMMAND_FACTORY_IMPL_CPP
