#pragma once

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
        /**
         * @brief Long names for the Argument
         *
         * These are the long names for the argument. Long names will be prefixed with `--`.
         */
        std::vector<std::string> names;

        /**
         * @brief help message
         *
         * WHen printed out, this will be autoformatted to have 8 spaces at the start and be 80
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
         * @param help description of argument
         */
        Arg(std::string name, std::string help);

        /**
         * @brief Parses the Arg value, raises an Exception if the stream is not consumed.
         *
         * This calls the Arg::parse_hook method, and ensures that the stream was consumed.
         *
         * @param stream stream to be parsed
         */
        void parse(std::istringstream& stream);

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

        /**
         * @brief Add multiple aliases to the Arg in a sginle call.
         *
         * @tparam Alias resolves to std::string
         * @param alias a std::string alias or name (single or multi-character)
         * @param aliases pass as a parameter pack
         *
         * @return the Arg itself for chaining
         */
        template<typename... Alias>
        Arg& add_alias(std::string alias, Alias... aliases) {
            this->add_alias(alias);
            this->add_alias(aliases...);
            return *this;
        };

        /**
         * @brief Parse the Arg
         *
         * Must be overridden in subclass. A user of `argreffunc` can write their own subclass for
         * special parsing.
         *
         * @param stream a stream for parsing
         */
        virtual void parse_hook(std::istream& stream) = 0;

    private:
        friend class Parser;
        void print_help(std::ostream& stream) const;
    };
}
