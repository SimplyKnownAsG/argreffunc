#pragma once

#include "arf/Arg.hpp"
#include "arf/ArgIterator.hpp"
#include "arf/FuncArg.hpp"
#include "arf/RefArg.hpp"

#include <functional>
#include <vector>

namespace arf {

    class Parser {
    private:
        std::vector<Arg*> args;

        std::vector<Arg*> positional_args;

        void _parse(ArgIterator& iterator, std::vector<Arg*>::iterator& positional);

        void _parse_long(ArgIterator& iterator);

        void _parse_short(ArgIterator& iterator);

        void _parse_positional(ArgIterator& iterator, std::vector<Arg*>::iterator& positional);

    public:
        template<typename T>
        Parser& add_positional(std::string name, T& refval) {
            auto arg = new RefArg<T>(name, refval);
            this->positional_args.push_back(arg);
            return *this;
        };

        template<typename T>
        Parser& add_positional(std::string name, std::function<void(T)> refval) {
            auto arg = new FuncArg<T>(name, refval);
            this->positional_args.push_back(arg);
            return *this;
        };

        template<typename T>
        Arg& add(std::string name, T& refval) {
            auto arg = new RefArg<T>(name, refval);
            this->args.push_back(arg);
            return *arg;
        };

        template<typename T>
        Parser& add(std::string name, std::function<void(T)> func) {
            auto arg = new FuncArg<T>(name, func);
            this->args.push_back(arg);
            return *this;
        };

        void parse(int argc, char* argv[]);

        void parse(std::vector<std::string> args);
    };
}
