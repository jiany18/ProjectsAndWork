/* Copyright G. Hemingway @ 2019, All Rights Reserved */
#ifndef OPTIONS_H
#define OPTIONS_H

// This header defines "size_t"
#include <cstdlib>
#include <stdexcept>
#include <string>

/**
 * @class Options
 * @brief Defines a singleton class to handle command-line option
 *        processing.
 *
 *        This class implements the Singleton pattern since there's a
 *        need for just one set of command-line options.
 */
class Options {
public:
    // Method to return the one and only instance.
    static Options* instance();

    // Destructor
    ~Options() = default;

    // This returns the exe name, passed in through the command line.
    std::string exe() const;

    // This returns the path, passed in through the command line.
    std::string path() const;

    // Run the program in verbose mode.
    bool verbose() const;

    // Parse command-line arguments and set the appropriate values as
    // follows:
    // 't' - Traversal strategy, i.e., 'P' for pre-order, 'O' for
    // post-order, 'I' for in-order, and 'L' for level-order.
    // 'q' - Type of queue, i.e., either 'L' for LQeuue or 'A' for AQueue.
    bool parse_args(int argc, char* argv[]);

    // Print out usage and default values.
    void print_usage();

private:
    // Make the constructor private for a singleton.
    Options();

    // Values for parameters passed in on the command line.
    std::string execStr;
    std::string pathStr;
    // Are we running in verbose mode or not?
    bool isVerbose;

    // Pointer to the singleton Options instance.
    static Options* inst;
};

#endif // OPTIONS_H
