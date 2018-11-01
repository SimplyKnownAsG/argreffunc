#pragma once

#include "arf/ArgIterator.hpp"
#include "arf/Name.hpp"

#include <sstream>
#include <string>
#include <vector>

#include <ostream>

namespace arf {
    /**
     * @brief An abstract command line argument.
     *
     * Actual command line arguments must be implemented as a FuncArg or RefArg.
     */
    class Arg {
    public:
        Name name;

        /**
         * @brief if true, the Arg must be assigned during Parser::parse
         */
        bool const required;

        /**
         * @brief help message
         *
         * When printed out, this will be auto-formatted to have 8 spaces at the start and be 80
         * characters wide.
         */
        std::string const help;

        /**
         * @brief Short (single character) aliases for the Argument
         *
         * These are the short (single character) aliases for the argument. Aliases are prefixed
         * with a single leading `-`, and may be stacked until an argument requires a value.
         */
        std::vector<std::string> aliases;

        /**
         * @brief Create a new Arg with the given name and help
         *
         * @param name name of the argument
         * @param positional if true, the Arg is not named when used
         * @param required if true, the Arg must be supplied, or parsing will result in an Exception
         * @param help description of argument
         */
        Arg(std::string name, bool positional, bool required, std::string help);

        /**
         * @brief Parses the Arg value, raises an Exception if the stream is not consumed.
         *
         * This calls the Arg::parse_hook method, and ensures that the stream was consumed.
         *
         * @param stream stream to be parsed
         */
        bool parse(ArgIterator& iterator);

        /**
         * @brief Parse the Arg
         *
         * Must be overridden in subclass. A user of `argreffunc` can write their own subclass for
         * special parsing.
         *
         * @param stream a stream for parsing
         */
        virtual void parse_hook(ArgIterator& iterator) = 0;

        /**
         * @brief write the help string to the supplied buffer
         *
         * @param stream stream to write
         */
        void print_help(std::ostream& stream) const;

        /**
         * @brief Add an alias to the Arg.
         *
         * Add an alias (single character) to the Arg, this can be used as a short alias.
         *
         * @param alias
         *
         * @return the Arg itself for chaining
         */
        Arg& add_alias(std::string alias);

        virtual ~Arg() = default;
    };
}
