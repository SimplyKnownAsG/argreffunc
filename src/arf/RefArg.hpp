#pragma once

#include "arf/Arg.hpp"

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
        RefArg(const std::string name, std::string help, T& ref_variable)
          : Arg(name, help)
          , val(ref_variable){};

        /**
         * @brief Default parse implementation
         *
         * This is a pretty naive parse, just `>>` to the reference variable. Error handling exists
         * elsewhere to ensure that the entire stream has been consumed.
         *
         * @param stream stream to be parsed.
         */
        void parse_hook(std::istream& stream) override {
            stream >> this->val;
        }
    };
}
