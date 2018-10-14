#pragma once

#include "arf/Arg.hpp"

#include <functional>

namespace arf {
    /**
     * @brief An Arg that passes the command line value to a user-specified function.
     *
     * A FuncArg is created through the Parser::add or Parser::add_positional methods.
     *
     * @tparam T argument type of the user-specified function
     */
    template<typename T>
    class FuncArg : public Arg {
    private:
        std::function<void(T)> func;

    public:
        /**
         * @brief Create a new FuncArg with a name and a function.
         *
         * @param name name of the argument
         * @param func function to be called with the argument value
         */
        FuncArg(const std::string name, std::function<void(T)> func)
          : Arg(name)
          , func(func){};

        /**
         * @brief Default parse implementation
         *
         * This is a pretty naive parse, just `>>` to a temporary variable, and then call the
         * function. Error handling exists elsewhere to ensure that the entire stream has been
         * consumed.
         *
         * @param stream stream to be parsed.
         */
        void parse_hook(std::istream& stream) override {
            T value;
            stream >> value;
            this->func(value);
        };
    };
}
