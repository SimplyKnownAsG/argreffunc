#pragma once

#include "arf/Arg.hpp"
#include <stdexcept>

namespace arf {
    /**
     * @brief An Arg that assigns the value to the supplied reference variable.
     *
     * A RefArg is created through the Parser::add or Parser::add_positional methods.
     *
     * @tparam T type of the argument.
     */
    template<typename T>
    class RefArg : public Arg {
    private:
        T& val;

    public:
        /**
         * @brief Create a new RefArg with a name and variable reference.
         *
         * @param name name of the argument
         * @param help help message
         * @param ref_variable variable reference
         */
        RefArg(const std::string name,
               bool positional,
               bool required,
               std::string help,
               T& ref_variable)
          : Arg(name, positional, required, help)
          , val(ref_variable){};

        /**
         * @brief Default parse implementation
         *
         * Cast the value to the correct type.
         *
         * @param stream stream to be parsed.
         */
        void parse_hook(ArgIterator& iterator) override {
            this->val = iterator.get_value<T>(this->name.name);
        };
    };
}
