/* Copyright G. Hemingway @ 2019, All Rights Reserved */
#ifndef OPTIONS_CPP
#define OPTIONS_CPP

#include "Options.h"
#include "getopt.h"
#include <iostream>

// Initialize the singleton.
Options* Options::inst = nullptr;

Options* Options::instance()
{
    // Create the options implementation if it hasn't already been done
    if (inst == nullptr)
        inst = new Options();
    return inst;
}

// Ctor
Options::Options()
    : isVerbose(false)
{
}

// Return exe name.
std::string Options::exe() const
{
    return execStr;
}

// Return path name.
std::string Options::path() const
{
    return pathStr;
}

bool Options::verbose() const
{
    return isVerbose;
}

// Parse the command line arguments.
bool Options::parse_args(int argc, char* argv[])
{
    // set exe_ to the first arg.
    execStr = parsing::getfilename(argv[0]);
    pathStr = parsing::getpath(argv[0]);
    char opts[] = "h?v";

    for (int c; (c = parsing::getopt(argc, argv, opts)) != EOF;)
        switch (c) {
        // check to see if the user asked for help
        case 'v':
            isVerbose = true;
            break;
        case 'h':
        case '?':
            print_usage();
            return false;
        default:
            break;
        }

    return true;
}

// Parse the command line arguments.
void Options::print_usage()
{
    std::cout << std::endl << "Help Invoked on " << pathStr + execStr << std::endl << std::endl;
    std::cout << "Usage: " << execStr << " [-h|-v]" << std::endl
              << std::endl
              << "  -h: invoke help" << std::endl
              << "  -v: enter verbose mode" << std::endl
              << std::endl;
}

#endif // OPTIONS_CPP
