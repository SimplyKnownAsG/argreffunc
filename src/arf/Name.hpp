#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace arf {
    class ArgIterator;

    /**
     * @brief names of an Arg
     *
     * The Name of an Arg contains the multiple names and aliases. It is used to determine if a Arg
     * matches
     */
    class Name {
    public:
        /**
         * @brief The first supplied name
         */
        std::string const name;

        /**
         * @brief Long names for the Arg
         *
         * These are the long names for the Arg. Long names will be prefixed with `--`.
         */
        std::vector<std::string> names;

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
         */
        Name(std::string name);

        /**
         * @brief Returns true if the arg matches, otherwise false
         *
         * @param iterator current ArgIterator
         */
        /* bool matches(ArgIterator& iterator); */

        /**
         * @brief Add an alias to the Arg.
         *
         * Add an alias (single character) to the Arg.
         *
         * @param alias name or alias
         *
         * @return the Name itself for chaining
         */
        Name& add_alias(std::string alias);

        /**
         * @brief Add multiple aliases to the Name in a single call.
         *
         * @tparam Alias resolves to std::string
         * @param alias a std::string alias or name (single or multi-character)
         * @param aliases pass as a parameter pack
         *
         * @return the Arg itself for chaining
         */
        template<typename... Alias>
        Name& add_alias(std::string alias, Alias... aliases) {
            this->add_alias(alias);
            this->add_alias(aliases...);
            return *this;
        };

        void print_help(std::ostream& stream,
                        bool is_positional,
                        bool is_required,
                        bool is_switch) const;
    };
}
