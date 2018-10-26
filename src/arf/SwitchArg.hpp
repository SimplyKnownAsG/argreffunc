#pragma once

#include "arf/Arg.hpp"

#include <functional>

namespace arf {
    /**
     * @brief An Arg that passes the command line value to a user-specified function.
     *
     * A SwitchArg is created through the Parser::add or Parser::add_positional methods.
     */
    class SwitchArg : public Arg {
    private:
        std::function<void()> func;

    public:
        /**
         * @brief Create a new SwitchArg with a name and a function.
         *
         * @param name name of the argument
         * @param help help message
         * @param func function to be called with the argument value
         */
        SwitchArg(const std::string name, std::string help, std::function<void()> func);

        /**
         * @brief Default parse implementation
         *
         * @param stream stream to be parsed.
         */
        void parse_hook(ArgIterator& iterator) override;
    };
}
